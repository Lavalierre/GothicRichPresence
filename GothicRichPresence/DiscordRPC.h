// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	

	struct WorldInfo
	{
		std::string zenName;
		std::string sImage;
		std::vector<std::string> vAliases;
	};

	class GDiscordRPC
	{
	public:

		GDiscordRPC( const GDiscordRPC & ) = delete;
		GDiscordRPC &operator=( GDiscordRPC & ) = delete;
		static GDiscordRPC &Instance() { return oInstance; }

		void Initialize();
		void Update();

		void DetectLanguage();

		void RegisterWorld( std::string, std::string, std::initializer_list<std::string> );
		void RegisterWorld( std::string, std::string, std::vector<std::string> );
		void ParseConfig();
		void ParseRPCFile();

	private:

		GDiscordRPC() = default;
		static GDiscordRPC oInstance;
		time_t tStartTimestamp{ 0 };
		string sAppPublicKey;										// application public key
		string sRPCFile;											// file name of rpc config
		LangTags iLang{ LangTags::NONE };
		std::vector< WorldInfo > vWorlds;
	};

}