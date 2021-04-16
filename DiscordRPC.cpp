// Supported with union (c) 2020 Union team
// Union SOURCE file
#include <sstream>
#include <ctime>
#include <codecvt>

namespace GOTHIC_ENGINE {

	using json = nlohmann::json;
	GDiscordRPC GDiscordRPC::oInstance;
	TEngineVersion engineVersion = Union.GetEngineVersion();

	void GDiscordRPC::Initialize()
	{
		// Default app keys

#if defined __G1 || defined __G1A
		sAppPublicKey = "831894054089523280";
#endif

#if defined __G2 || defined __G2A
		sAppPublicKey = "831893873164288031";
#endif

		// Parsing config before actual initializing

		ParseConfig();

		// Init RPC with given app key (default or from the config)

		DiscordEventHandlers handlers;
		memset( &handlers, 0, sizeof( handlers ) );
		Discord_Initialize( sAppPublicKey, &handlers, 0, NULL );
		tStartTimestamp = std::time( nullptr );

		// Register default worlds, if .json config is not parsed
		if ( vWorlds.size() == 0 )
		{
#if defined __G1 || defined __G1A
			RegisterWorld( "WORLD", "colony_day", { "Penal Colony", "Колония", "Kolonia Karna", } );
			RegisterWorld( "OLDMINE", "oldmine", { "Old Mine", "Старая Шахта", "Stara Kopalnia" } );
			RegisterWorld( "FREEMINE", "freemine", { "Free Mine", "Свободная Шахта", "Wolna Kopalnia" } );
			RegisterWorld( "ORCGRAVEYARD", "graveyard", { "Orc Graveyard", "Кладбище Орков", "Cmentarzysko Orkуw" } );
			RegisterWorld( "ORCTEMPEL", "orctempel", { "Sleeper Temple", "Храм Спящего", "Њwi№tynia Њni№cego" } );
#endif

#if defined __G2 || defined __G2A
			RegisterWorld( "NEWWORLD", "khorinis", { "Khorinis", "Хоринис", "Khorinis" } );
			RegisterWorld( "OLDWORLD", "valley", { "Valley of Mines", "Долина Рудников", "Gуrnicza Dolina" } );
			RegisterWorld( "ADDONWORLD", "jharkendar", { "Jharkendar", "Яркендар", "Jarkendar" } );
			RegisterWorld( "DRAGONISLAND", "irdorath", { "Irdorath", "Ирдорат", "Irdorath" } );
#endif
		}

		DetectLanguage();
	}

	void GDiscordRPC::DetectLanguage()
	{
		// Do not detect sys language if it's already set through config
		if ( iLang != LangTags::NONE ) return;

		if ( MAKELANGID( LANG_RUSSIAN, SUBLANG_DEFAULT ) == GetSystemDefaultLangID() )
			iLang = LangTags::RU;
		else if ( MAKELANGID( LANG_POLISH, SUBLANG_DEFAULT ) == GetSystemDefaultLangID() )
			iLang = LangTags::PL;
		else
			iLang = LangTags::EN;

	}

	void GDiscordRPC::RegisterWorld( std::string zen, std::string image, std::initializer_list<std::string> names )
	{
		for ( auto it = vWorlds.begin(); it != vWorlds.end(); it++ )
		{
			if ( ( *it ).zenName == zen )
				vWorlds.erase( it );
		}

		std::vector<std::string> vNames;
		for ( auto it = names.begin(); it != names.end(); it++ )
			vNames.push_back( *it );

		WorldInfo newWorld;
		newWorld.zenName = zen;
		newWorld.sImage = image;
		newWorld.vAliases = vNames;
		vWorlds.push_back( newWorld );
	}

	void GDiscordRPC::RegisterWorld( std::string zen, std::string image, std::vector<std::string> vNames )
	{
		for ( auto it = vWorlds.begin(); it != vWorlds.end(); it++ )
		{
			if ( ( *it ).zenName == zen )
				vWorlds.erase( it );
		}

		std::transform( zen.begin(), zen.end(), zen.begin(), ::toupper );

		WorldInfo newWorld;
		newWorld.zenName = zen;
		newWorld.sImage = image;
		newWorld.vAliases = vNames;
		vWorlds.push_back( newWorld );
	}

	void GDiscordRPC::ParseConfig()
	{
		COption &gameOptions = Union.GetGameOption();

		string appKey, rpcFileName, language;

		gameOptions.Read( language, "GothicRichPresence", "Lang" );
		gameOptions.Read( appKey, "GothicRichPresence", "AppKey" );
		gameOptions.Read( rpcFileName, "GothicRichPresence", "ConfigFile" );

		!appKey.IsEmpty() ? sAppPublicKey = appKey : false;
		!rpcFileName.IsEmpty() ? sRPCFile = rpcFileName : false;

		std::string sLanguage = language.ToChar();
		std::transform( sLanguage.begin(), sLanguage.end(), sLanguage.begin(), ::toupper );

		if ( !language.IsEmpty() )
		{
			if ( sLanguage == "RU" )
				iLang = LangTags::RU;
			else if ( sLanguage == "PL" )
				iLang = LangTags::PL;
			else
				iLang = LangTags::EN;
		}

		if ( !sRPCFile.IsEmpty() )
			ParseRPCFile();
	}

	void GDiscordRPC::ParseRPCFile()
	{
  
		// Reading file
		zoptions->ChangeDir( DIR_SYSTEM );
		zFILE_VDFS *originFile = zNEW( zFILE_VDFS )( sRPCFile );

		if ( !originFile->Exists() )
		{
			delete originFile;
			return;
		}

		originFile->Open( false );

		zSTRING line, buffer;
		do
		{
			originFile->Read( line );
			buffer += line;
		} while ( !originFile->Eof() );

		// JSON parsing
		auto jsonFile = json::parse( buffer.ToChar() );

		std::string worldList;
		if ( !jsonFile[ "worldList" ].is_string() ) return;
		worldList = jsonFile[ "worldList" ].get<std::string>();
		std::vector<std::string> zenNames = ExplodeString( worldList, ',', true );

		std::string sAliases, sImage;
		for ( std::string zen : zenNames )
		{
			if ( jsonFile[ zen ].is_object() )
			{
				if ( !jsonFile[ zen ][ "aliases" ].is_string() || !jsonFile[ zen ][ "image" ].is_string() )
					continue;
				sAliases = jsonFile[ zen ][ "aliases" ].get<std::string>();
				sImage = jsonFile[ zen ][ "image" ].get<std::string>();

				// Splitting aliases by ',' delimiter and then checking language tag in {}
				std::vector<std::string> vAliases = ExplodeString( sAliases, ',' );
				std::vector<std::string> vActualAliases( MAX_LANGUAGES );
				for ( std::string alias : vAliases )
				{

					LangTags lang = ReadAliasTag( alias );

					if (lang != LangTags::NONE )
						vActualAliases[ int( lang ) ] = alias;
				}
				RegisterWorld( zen, sImage, vActualAliases );
			}
		}
	}

	void GDiscordRPC::Update()
	{
		DiscordRichPresence discordPresence;
		memset( &discordPresence, 0, sizeof( discordPresence ) );

		if ( gameMan->IsGameRunning() )
		{
			if ( ogame->GetGameWorld() )
			{
				zSTRING wName		= ogame->GetGameWorld()->GetWorldName();
				zSTRING guildName	= player->GetGuildName();
				int level			= player->level;
				int kapitel			= 0;
				int day, hour, min;

				ogame->GetTime( day, hour, min );									// Parsing current time

				if ( parser->GetSymbol( "kapitel" ) )								// Parsing current chapter
					kapitel = parser->GetSymbol( "kapitel" )->single_intdata;

				// ** Main RPC variables ** //
				char timeBuffer[ 128 ];
				char infoBuffer[ 128 ];
				char imageKey[ 128 ];
				char locationName[ 128 ];
				char chapterInfo[ 128 ];

				// *** TIME INFO *** //

				switch ( iLang )
				{
				case EN:
					sprintf( timeBuffer, "Day %d - %02d:%02d", day, hour, min );
					break;
				case RU:
					sprintf( timeBuffer, "День %d - %02d:%02d", day, hour, min );
					break;
				case PL:
					sprintf( timeBuffer, "Dzieс %d - %02d:%02d", day, hour, min );
					ConvertString( timeBuffer, timeBuffer, std::locale( ".1250" ) );
					break;
				}

				if ( !is_utf8( timeBuffer ) )
					ConvertString( timeBuffer, timeBuffer );

				// *** CHARACTER INFO *** //

				switch ( iLang )
				{
				case EN:
					sprintf( infoBuffer, "%s - %d Level", guildName.ToChar(), level );
					break;
				case RU:
					sprintf( infoBuffer, "%s - %d ур.", guildName.ToChar(), level );
					break;
				case PL:
					sprintf( infoBuffer, "%s - %d Poziom", guildName.ToChar(), level );
					ConvertString( infoBuffer, infoBuffer, std::locale( ".1250" ) );
					break;
				}

				if ( !is_utf8(infoBuffer) )
					ConvertString( infoBuffer, infoBuffer );

				// *** LOCATION & CHAPTER INFO *** //

				for ( WorldInfo world : vWorlds )
				{
					// Must be exact the same names to avoid mistaking for example world.zen with newworld.zen
					if ( wName.Compare( world.zenName.c_str() ) )
					{
						sprintf( imageKey, world.sImage.c_str() );
						sprintf( locationName, world.vAliases[ iLang ].c_str() );

						// In case if it's default worlds
						if ( !is_utf8( locationName ) ) {
							if ( iLang == LangTags::PL )
								ConvertString( locationName, locationName, std::locale( ".1250" ) );
							else
								ConvertString( locationName, locationName );
						}  
					}
				}
				if ( locationName == NULL ) {

					sprintf( imageKey, "menu" );

					switch ( iLang )
					{
					case EN:
						sprintf( locationName, "Unknown Lands" );
						break;
					case RU:
						sprintf( locationName, "Неизвестные Земли" );
						break;
					case PL:
						sprintf( locationName, "Nieznana Kraina" );
						ConvertString( locationName, locationName, std::locale( ".1250" ) );
						break;
					}
					if ( !is_utf8( locationName ) )
						ConvertString( locationName, locationName );
				}

				switch ( iLang )
				{
				case EN:
					sprintf( chapterInfo, " - Chapter %d", kapitel );
					break;
				case RU:
					sprintf( chapterInfo, " - Глава %d", kapitel );
					break;
				case PL:
					sprintf( chapterInfo, " - Rozdziaі %d", kapitel );
					ConvertString( chapterInfo, chapterInfo, std::locale( ".1250" ) );
					break;
				}

				if ( !is_utf8( chapterInfo ) )
					ConvertString( chapterInfo, chapterInfo );

				strcat( locationName, chapterInfo );

				// *** SUMMARISING INFO *** //

				discordPresence.state = infoBuffer;
				discordPresence.largeImageKey = imageKey;
				discordPresence.largeImageText = locationName;
				discordPresence.smallImageKey = "info";
				discordPresence.smallImageText = timeBuffer;
			}
		}
		else
		{
			char gameState[ 128 ];
			switch ( iLang )
			{
			case EN:
				sprintf( gameState, "Menu" );
				break;
			case RU:
				sprintf( gameState, "Меню" );
				break;
			case PL:
				sprintf( gameState, "Menu" );
				ConvertString( gameState, gameState, std::locale( ".1250" ) );
				break;
			}

			if ( !is_utf8( gameState ) )
				ConvertString( gameState, gameState );

			discordPresence.state = gameState;
			discordPresence.largeImageKey = "menu";
			discordPresence.smallImageKey = "";
			discordPresence.smallImageText = "";
		}

		if ( zgameoptions )
		{
			string gameTitle = A zgameoptions->ReadString( "Info", "Title", "Unknown Title" );
			discordPresence.details = gameTitle;
		}
		else
		{
			discordPresence.details = "";
		}

		discordPresence.startTimestamp = tStartTimestamp;

		Discord_UpdatePresence( &discordPresence );
	}
}