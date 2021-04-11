// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	
	class GDiscordRPC
	{
	public:

		GDiscordRPC( const GDiscordRPC & ) = delete;
		GDiscordRPC &operator=( GDiscordRPC & ) = delete;
		static GDiscordRPC &Instance() { return oInstance; }

		void Initialize();
		void Update();

		void DetectLanguage();
		void SwitchLanguage();

		void ConvertString( char *, char * );

	private:

		GDiscordRPC() = default;
		static GDiscordRPC oInstance;
		time_t tStartTimestamp{ 0 };
		int iLang{ 0 }; // 0 - English, 1 - Russian
	};

}