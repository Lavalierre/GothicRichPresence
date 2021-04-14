// Supported with union (c) 2020 Union team
// Union SOURCE file
#include <sstream>
#include <ctime>

namespace GOTHIC_ENGINE {
	
	GDiscordRPC GDiscordRPC::oInstance;
	TEngineVersion engineVersion = Union.GetEngineVersion();

	void GDiscordRPC::Initialize()
	{
		DiscordEventHandlers handlers;
		memset( &handlers, 0, sizeof( handlers ) );
		
#if defined __G1 || defined __G1A
		if (engineVersion == Engine_G1 || engineVersion == Engine_G1A)
			Discord_Initialize("831520290261172266", &handlers, 0, NULL);
#endif

#if defined __G2 || defined __G2A
		if (engineVersion == Engine_G2 || engineVersion == Engine_G2A)
			Discord_Initialize("796368615212843018", &handlers, 0, NULL);
#endif

		tStartTimestamp = std::time( nullptr );

		DetectLanguage();
	}

	void GDiscordRPC::SwitchLanguage()
	{
		iLang <= 2 ? iLang++ : iLang = 0;
	}

	void GDiscordRPC::DetectLanguage()
	{
		if (MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT) == GetSystemDefaultLangID())
			iLang = 1;
		else if (MAKELANGID(LANG_POLISH, SUBLANG_DEFAULT) == GetSystemDefaultLangID())
			iLang = 2;
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
				int hasTimeVersions = FALSE;
				int kapitel = 0;
				ogame->GetTime( day, hour, min );

				zSTRING guildName = player->GetGuildName();
				int level = player->level;

				if (parser->GetSymbol("kapitel"))
					kapitel = parser->GetSymbol("kapitel")->single_intdata;

				char timeBuffer[ 128 ];
				
				if (iLang == 1) 
					sprintf(timeBuffer, "Äåíü %d - %02d:%02d", day, hour, min);
				else if (iLang == 2) 
					sprintf(timeBuffer, "Dzieñ %d - %02d:%02d", day, hour, min);
				else 
					sprintf(timeBuffer, "Day %d - %02d:%02d", day, hour, min);

				ConvertString(timeBuffer, timeBuffer);

				char infoBuffer[ 128 ];

				if (iLang == 1)
					sprintf(infoBuffer, "%s - %d óð.", guildName.ToChar(), level);
				else if (iLang == 2)
					sprintf(infoBuffer, "%s - %d Poziom", guildName.ToChar(), level);
				else
					sprintf(infoBuffer, "%s - %d Level", guildName.ToChar(), level);

				if (kapitel) {
					if (iLang == 1)
						sprintf(infoBuffer, "%s - %d Ãëàâà", infoBuffer, kapitel);
					else if (iLang == 2)
						sprintf(infoBuffer, "%s - %d Rozdzia³", infoBuffer, kapitel);
					else
						sprintf(infoBuffer, "%s - %d Chapter", infoBuffer, kapitel);
				}

				char *sValidGuildName = new char[ 128 ];
				ConvertString( infoBuffer, sValidGuildName );
				discordPresence.state = sValidGuildName;

				char locationName[128];
				char imageKey[128];
				string imageStr, locationStr;

#if defined __G1 || defined __G1A
				if (engineVersion == Engine_G1 || engineVersion == Engine_G1A)

					if (wName.HasWord("WORLD.ZEN"))
					{
						imageStr = "colony";
						hasTimeVersions = TRUE;

						if (iLang == 1)
							locationStr = "Äîëèíà Ðóäíèêîâ";
						else if (iLang == 2)
							locationStr = "Kolonia Karna";
						else
							locationStr = "Penal Colony";

					}
					else if (wName.HasWord("OLDMINE.ZEN"))
					{
						imageStr = "oldmine";
						hasTimeVersions = FALSE;

						if (iLang == 1)
							locationStr = "Ñòàðàÿ øàõòà";
						else if (iLang == 2)
							locationStr = "Stara Kopalnia";
						else
							locationStr = "Old Mine";

					}
					else if (wName.HasWord("FREEMINE.ZEN"))
					{
						imageStr = "freemine";
						hasTimeVersions = FALSE;

						if (iLang == 1)
							locationStr = "Íîâàÿ øàõòà";
						else if (iLang == 2)
							locationStr = "Wolna Kopalnia";
						else
							locationStr = "Free Mine";

					}
					else if (wName.HasWord("ORCGRAVEYARD.ZEN"))
					{
						imageStr = "graveyard";
						hasTimeVersions = FALSE;

						if (iLang == 1)
							locationStr = "Êëàäáèùå îðêîâ";
						else if (iLang == 2)
							locationStr = "Cmentarzysko Orków";
						else
							locationStr = "Orc Cemetery";

					}
					else if (wName.HasWord("ORCTEMPEL.ZEN"))
					{
						imageStr = "orctempel";
						hasTimeVersions = FALSE;

						if (iLang == 1)
							locationStr = "Õðàì Ñïÿùåãî";
						else if (iLang == 2)
							locationStr = "Œwi¹tynia Œni¹cego";
						else
							locationStr = "Sleeper Temple";

					}
#endif

#if defined __G2 || defined __G2A
				if (engineVersion == Engine_G2 || engineVersion == Engine_G2A)
					if (wName.HasWord("NEWWORLD.ZEN"))
					{
						imageStr = "khorinis";
						hasTimeVersions = TRUE;

						if (iLang == 1)
							locationStr = "Õîðèíèñ";
						else if (iLang == 2)
							locationStr = "Khorinis";
						else
							locationStr = "Khorinis";

					}
					else if (wName.HasWord("ADDONWORLD.ZEN"))
					{
						imageStr = "jharkendar";
						hasTimeVersions = TRUE;

						if (iLang == 1)
							locationStr = "ßðêåíäàð";
						else if (iLang == 2)
							locationStr = "Jarkendar";
						else
							locationStr = "Jharkendar";

					}
					else if (wName.HasWord("OLDWORLD.ZEN"))
					{
						imageStr = "valley";
						hasTimeVersions = TRUE;

						if (iLang == 1)
							locationStr = "Äîëèíà Ðóäíèêîâ";
						else if (iLang == 2)
							locationStr = "Górnicza Dolina";
						else
							locationStr = "Valley of Mines";

					}
					else if (wName.HasWord("DRAGONISLAND.ZEN"))
					{
						imageStr = "irdorath";
						hasTimeVersions = FALSE;

						if (iLang == 1)
							locationStr = "Èðäîðàò";
						else if (iLang == 2)
							locationStr = "Irdorath";
						else
							locationStr = "Irdorath";

					}
#endif
				if (imageStr == "" || locationStr == "") {
					imageStr = "unknown";
					hasTimeVersions = FALSE;

					if (iLang == 1)
						locationStr = "Èññëåäîâàíèå";
					else if (iLang == 2)
						locationStr = "Nieznana Kraina";
					else
						locationStr = "Unknown Lands";
				}

				if (hasTimeVersions) {
					if (hour >= 4 && hour < 8)
						imageStr += "_morning";
					else if (hour >= 8 && hour < 18)
						imageStr += "_day";
					else if (hour >= 18 && hour < 22)
						imageStr += "_evening";
					else
						imageStr += "_night";
				}

				sprintf(imageKey, imageStr);
				ConvertString(imageKey, imageKey);
				discordPresence.largeImageKey = imageKey;

				sprintf(locationName, locationStr);
				ConvertString(locationName, locationName);
				discordPresence.largeImageText = locationName;

				// Ingame day and time small image display
				if (hour >= 8 && hour < 20)
					discordPresence.smallImageKey = "day";
				else if (hour >= 20 || hour < 8)
					discordPresence.smallImageKey = "night";
				else
					discordPresence.smallImageKey = "info";

				discordPresence.smallImageText = timeBuffer;
			}
		}
		else
		{
			char gameState[128];
			if (iLang == 1)
				sprintf(gameState, "Ìåíþ");
			else if (iLang == 2)
				sprintf(gameState, "Menu");
			else
				sprintf(gameState, "Menu");

			ConvertString(gameState, gameState);
			discordPresence.state = gameState;

			// "menu" image will change automatically when discord app id changes on Initialize
			discordPresence.largeImageKey = "menu"; 
			discordPresence.smallImageKey = "";
			discordPresence.smallImageText = "";
		}

		if (zgameoptions)
		{
			string gameTitle = A zgameoptions->ReadString("Info", "Title", "Unknown Title");
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