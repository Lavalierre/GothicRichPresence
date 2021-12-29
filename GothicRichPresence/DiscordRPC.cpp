// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

  GDiscordRPC GDiscordRPC::oInstance;

  void GDiscordRPC::Initialize()
  {
    language = GetSysPackLanguage().Lower();

    ParseConfig();
    ParseWorlds();
    ParseStrings();

    // Default app keys
#if ENGINE >= Engine_G2
    string AppId = "925674889350889482";
#else
    string AppId = "925673141609574401";
#endif

    if ( config["publickeyoverride"].is_string() )
      AppId = config["publickeyoverride"].get<std::string>().c_str();

    // Init RPC with given app key (default or from the config)
    DiscordEventHandlers handlers;
    memset( &handlers, 0, sizeof( handlers ) );
    Discord_Initialize( AppId, &handlers, false, NULL );
    tStartTimestamp = std::time( nullptr );
    Update();
  }

  string GDiscordRPC::GetSysPackLanguage()
  {
    string lang;
    Union.GetSysPackOption().Read( lang, "CORE", "Language" );
    return (lang.Length()) ? lang : "eng";
  }

  void GDiscordRPC::ParseConfig()
  {
    zoptions->ChangeDir( DIR_SYSTEM );
    zFILE_VDFS* originFile = new zFILE_VDFS( configFileName );

    if ( !originFile->Exists() ) {
      delete originFile;
      return;
    }

    originFile->Open( false );

    zSTRING line, buffer;
    do {
      originFile->Read( line );
      buffer += line;
    } while ( !originFile->Eof() );

    config = nlohmann::json::parse( buffer.ToChar() );
  }

  void GDiscordRPC::ParseWorlds()
  {
    if ( !config["worlds"].is_array() )
      return;

    if ( config["worlds"].size() < 1 )
      return;

    for ( size_t i = 0; i < config["worlds"].size() - 1; i++ ) {
      if ( !config["worlds"][i].is_object() )
        continue;

      if ( !config["worlds"][i]["zen"].is_string() )
        continue;

      if ( !config["worlds"][i]["image"].is_string() )
        continue;

      if ( !config["worlds"][i]["name"].is_object() )
        continue;

      if ( !config["worlds"][i]["name"][language.ToChar()].is_string() )
        continue;

      WorldInfo world;
      world.zen = A config["worlds"][i]["zen"].get<std::string>().c_str();
      world.image = A config["worlds"][i]["image"].get<std::string>().c_str();
      world.name = A config["worlds"][i]["name"][language.ToChar()].get<std::string>().c_str();
      vWorlds.push_back( world );
    }
  }

  void GDiscordRPC::ParseStrings()
  {
#define GETRPCSTRING(x) ( ( config["strings"][x].is_object() && config["strings"][x][language.ToChar()].is_string() ) ? A config["strings"][x][language.ToChar()].get<std::string>().c_str() : "" )

    // Modification title when playing from gothic starter
    if ( zgameoptions && !Union.GetGameIni().Compare( "gothicgame.ini" ) )
      strings.title = A zgameoptions->ReadString( "Info", "Title", "Unknown Title" );

    if ( !config["strings"].is_object() )
      return;

    strings.day = GETRPCSTRING( "day" );
    strings.level = GETRPCSTRING( "level" );
    strings.chapter = GETRPCSTRING( "chapter" );
    strings.unknownworld = GETRPCSTRING( "unknownworld" );

#undef GETRPCSTRING
  }

  void GDiscordRPC::Update()
  {
    DiscordRichPresence discordPresence;
    memset( &discordPresence, 0, sizeof( discordPresence ) );

    if ( strings.title.Length() )
      discordPresence.details = strings.title;
    discordPresence.startTimestamp = tStartTimestamp;
    discordPresence.largeImageKey = images.menu;

    if ( ogame && ogame->GetGameWorld() && player ) {
      // Ingame time
      if ( strings.day.Length() ) {
        int day, hour, min;
        ogame->GetTime( day, hour, min );
        string minutes = (min > 9) ? Z min : "0" + Z min;
        string smallImageText = string::Combine( "%s %u - %u:%s", strings.day, day, hour, minutes );
        discordPresence.smallImageText = smallImageText;
        discordPresence.smallImageKey = (hour >= 6 && hour < 20) ? images.day : images.night;
      }

      // Hero guild and level
      if ( strings.level.Length() && strings.chapter.Length() ) {
        string state = string::Combine( "%z - %s %u", player->GetGuildName(), strings.level, player->level );

        // Adding current chapter info if kapitel variable is present
        if ( auto sym = parser->GetSymbol( "kapitel" ) )
          state = string::Combine( "%s - %s %u", state, strings.chapter, sym->single_intdata );

        discordPresence.state = state;
      }

      // Location name and image
      bool foundWorld = false;
      for ( WorldInfo world : vWorlds ) {
        // Must be exact the same names to avoid mistaking for example world.zen with newworld.zen
        if ( ogame->GetGameWorld()->GetWorldName().Compare( Z world.zen.Upper() ) ) {
          discordPresence.largeImageKey = world.image;
          discordPresence.largeImageText = world.name;
          foundWorld = true;
          break;
        }
      }

      // Unknown location
      if ( !foundWorld && strings.unknownworld.Length() ) {
        discordPresence.largeImageKey = images.unknown;
        discordPresence.largeImageText = strings.unknownworld;
      }
    }

    Discord_UpdatePresence( &discordPresence );
  }
}