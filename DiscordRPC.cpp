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
				ogame->GetTime( day, hour, min );

				zSTRING guildName = player->GetGuildName();
				int level = player->level;

				char timeBuffer[ 128 ];
				
				if (iLang == 1) 
					sprintf(timeBuffer, "День %d, %02d:%02d", day, hour, min);
				else if (iLang == 2) 
					sprintf(timeBuffer, "Dzieс %d, %02d:%02d", day, hour, min);
				else 
					sprintf(timeBuffer, "Day %d, %02d:%02d", day, hour, min);

				ConvertString(timeBuffer, timeBuffer);


				char infoBuffer[ 128 ];

				if (iLang == 1)
					sprintf(infoBuffer, "%s, %d ур.", guildName.ToChar(), level);
				else if (iLang == 2)
					sprintf(infoBuffer, "%s, %d poziom", guildName.ToChar(), level);
				else
					sprintf(infoBuffer, "%s, %d lvl.", guildName.ToChar(), level);

				char *sValidGuildName = new char[ 128 ];
				ConvertString( infoBuffer, sValidGuildName );
				discordPresence.state = sValidGuildName;

				if ( wName.Search("NEWWORLD.ZEN", 1U) != -1 )
				{
					char locationName[128];
					if (iLang == 1)
						sprintf(locationName, "Хоринис");
					else if (iLang == 2)
						sprintf(locationName, "Khorinis");
					else
						sprintf(locationName, "Khorinis");

					ConvertString(locationName, locationName);
					discordPresence.largeImageText = locationName;
					
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
					char locationName[128];
					if (iLang == 1)
						sprintf(locationName, "Яркендар");
					else if (iLang == 2)
						sprintf(locationName, "Jarkendar");
					else
						sprintf(locationName, "Jharkendar");

					ConvertString(locationName, locationName);
					discordPresence.largeImageText = locationName;

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
					char locationName[128];
					if (iLang == 1)
						sprintf(locationName, "Долина Рудников");
					else if (iLang == 2)
						sprintf(locationName, "Gуrnicza Dolina");
					else
						sprintf(locationName, "Valley of Mines");

					ConvertString(locationName, locationName);
					discordPresence.largeImageText = locationName;

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
					char locationName[128];
					if (iLang == 1)
						sprintf(locationName, "Ирдорат");
					else if (iLang == 2)
						sprintf(locationName, "Irdorath");
					else
						sprintf(locationName, "Irdorath");

					ConvertString(locationName, locationName);
					discordPresence.largeImageText = locationName;

					discordPresence.largeImageKey = "irdorath";
				}
				else
				{
					char locationName[128];
					if (iLang == 1)
						sprintf(locationName, "Исследование");
					else if (iLang == 2)
						sprintf(locationName, "Nieznana Kraina");
					else
						sprintf(locationName, "Unknown Lands");

					ConvertString(locationName, locationName);
					discordPresence.largeImageText = locationName;

					discordPresence.largeImageKey = "menu"; // Could be changed to some terrain image
				}

				discordPresence.smallImageKey = "info";
				discordPresence.smallImageText = timeBuffer;
			}
		}
		else
		{
			char gameState[128];
			if (iLang == 1)
				sprintf(gameState, "Меню");
			else if (iLang == 2)
				sprintf(gameState, "Menu");
			else
				sprintf(gameState, "Menu");

			ConvertString(gameState, gameState);
			discordPresence.state = gameState;

			discordPresence.largeImageKey = "menu";
			discordPresence.smallImageKey = "";
			discordPresence.smallImageText = "";
		}

		if (zgameoptions)
		{
			string gameTitle = A zgameoptions->ReadString("Info", "Title", "Gothic II");
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