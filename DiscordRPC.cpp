// Supported with union (c) 2020 Union team
// Union SOURCE file
#include <sstream>
#include <ctime>

namespace GOTHIC_ENGINE {
	
	GDiscordRPC GDiscordRPC::oInstance;

	void GDiscordRPC::Initialize()
	{
		DiscordEventHandlers handlers;
		memset( &handlers, 0, sizeof( handlers ) );
		Discord_Initialize( "796368615212843018", &handlers, 0, NULL );
		tStartTimestamp = std::time( nullptr );

		DetectLanguage();
	}

	void GDiscordRPC::SwitchLanguage()
	{
		iLang == 0 ? iLang = 1 : iLang = 0;
	}

	void GDiscordRPC::DetectLanguage()
	{
		if ( MAKELANGID( LANG_RUSSIAN, SUBLANG_DEFAULT ) == GetSystemDefaultLangID() )
			iLang = 1;
		else
			iLang = 0;
	}

	void GDiscordRPC::ConvertString( char * from, char * to)
	{
		int size = 128;
		wchar_t *wsValid = new wchar_t[ size ];

		MultiByteToWideChar( CP_ACP, 0, from, -1, wsValid, size );
		WideCharToMultiByte( CP_UTF8, NULL, wsValid, -1, to, size, NULL, NULL );
	}


	void GDiscordRPC::Update()
	{
		DiscordRichPresence discordPresence;
		memset( &discordPresence, 0, sizeof( discordPresence ) );

		int day, hour, min;

		if ( gameMan->IsGameRunning() )
		{
			if ( ogame->GetGameWorld() )
			{
				zSTRING wName = ogame->GetGameWorld()->GetWorldFilename();
				ogame->GetTime( day, hour, min );

				zSTRING guildName = player->GetGuildName();
				int level = player->level;

				char timeBuffer[ 128 ];
				if ( iLang == 0 )
				{
					sprintf( timeBuffer, "Day %d, %02d:%02d", day, hour, min );
				}
				else
				{
					sprintf( timeBuffer, "День %d, %02d:%02d", day, hour, min );
					ConvertString( timeBuffer, timeBuffer );
				}

				char infoBuffer[ 128 ];

				if ( iLang == 0 )
				{
					sprintf( infoBuffer, "%s, %d lvl.", guildName.ToChar(), level );
				}
				else
				{
					sprintf( infoBuffer, "%s, %d ур.", guildName.ToChar(), level );
				}

				char *sValidGuildName = new char[ 128 ];
				ConvertString( infoBuffer, sValidGuildName );
				discordPresence.details = sValidGuildName;

				if ( wName.Search("NEWWORLD.ZEN", 1U) != -1 )
				{
					if ( iLang == 0 )
					{
						discordPresence.state = "Khorinis";
						discordPresence.largeImageText = "Khorinis";
					}
					else
					{
						char locationName[ 128 ] = "Хоринис";
						ConvertString( locationName, locationName );
						discordPresence.state = locationName;
						discordPresence.largeImageText = locationName;
					}
					
					if ( hour >= 8 && hour < 18 )
						discordPresence.largeImageKey = "khorinis_day";
					else if (hour >= 18 && hour < 22 )
						discordPresence.largeImageKey = "khorinis_evening";
					else if ( hour >= 4 && hour < 8 )
						discordPresence.largeImageKey = "khorinis_morning";
					else
						discordPresence.largeImageKey = "khorinis_night";
				}
				else if ( wName.Search( "ADDONWORLD.ZEN", 1U ) != -1 )
				{
					if ( iLang == 0 )
					{
						discordPresence.state = "Jharkendar";
						discordPresence.largeImageText = "Jharkendar";
					}
					else
					{
						char locationName[ 128 ] = "Яркендар";
						ConvertString( locationName, locationName );
						discordPresence.state = locationName;
						discordPresence.largeImageText = locationName;
					}

					if ( hour >= 8 && hour < 18 )
						discordPresence.largeImageKey = "jharkendar_day";
					else if ( hour >= 18 && hour < 22 )
						discordPresence.largeImageKey = "jharkendar_evening";
					else if ( hour >= 4 && hour < 8 )
						discordPresence.largeImageKey = "jharkendar_morning";
					else
						discordPresence.largeImageKey = "jharkendar_night";
				}
				else if ( wName.Search( "OLDWORLD.ZEN", 1U ) != -1 )
				{
					if ( iLang == 0 )
					{
						discordPresence.state = "Valley of Mines";
						discordPresence.largeImageText = "Valley of Mines";
					}
					else
					{
						char locationName[ 128 ] = "Долина Рудников";
						ConvertString( locationName, locationName );
						discordPresence.state = locationName;
						discordPresence.largeImageText = locationName;
					}

					if ( hour >= 8 && hour < 18 )
						discordPresence.largeImageKey = "valley_day";
					else if ( hour >= 18 && hour < 22 )
						discordPresence.largeImageKey = "valley_evening";
					else if ( hour >= 4 && hour < 8 )
						discordPresence.largeImageKey = "valley_morning";
					else
						discordPresence.largeImageKey = "valley_night";
				}
				else if ( wName.Search( "DRAGONISLAND.ZEN", 1U ) != -1 )
				{
					if ( iLang == 0 )
					{
						discordPresence.state = "Irdorath";
						discordPresence.largeImageText = "Irdorath";
					}
					else
					{
						char locationName[ 128 ] = "Ирдорат";
						ConvertString( locationName, locationName );
						discordPresence.state = locationName;
						discordPresence.largeImageText = locationName;
					}

					discordPresence.largeImageKey = "irdorath";
				}
				else
				{
					if ( iLang == 0 )
					{
						discordPresence.state = "In Game";
						discordPresence.largeImageText = "Exploring";
					}
					else
					{
						char gameState[ 128 ] = "В Игре";
						char largeImageText[ 128 ] = "Исследование";
						ConvertString( gameState, gameState );
						ConvertString( largeImageText, largeImageText );

						discordPresence.state = gameState;
						discordPresence.largeImageText =  largeImageText;
					}

					discordPresence.largeImageKey = "menu";
				}

				discordPresence.smallImageKey = "info";
				discordPresence.smallImageText = timeBuffer;
			}
		}
		else
		{
			if ( iLang == 0 )
			{
				discordPresence.largeImageText = "Menu";
				discordPresence.state = "Menu";
			}
			else
			{
				char gameState[ 128 ] = "Меню";
				char largeImageText[ 128 ] = "Меню";
				ConvertString( gameState, gameState );
				ConvertString( largeImageText, largeImageText );

				discordPresence.state = gameState;
				discordPresence.largeImageText = largeImageText;
			}

			discordPresence.details = "";
			discordPresence.largeImageKey = "menu";
			discordPresence.smallImageKey = "";
			discordPresence.smallImageText = "";
		}

		discordPresence.startTimestamp = tStartTimestamp;

		Discord_UpdatePresence( &discordPresence );
	}
}