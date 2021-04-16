// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

	bool is_utf8( const std::string &string )
	{
		int c, i, ix, n, j;
		for ( i = 0, ix = string.length(); i < ix; i++ )
		{
			c = ( unsigned char )string[ i ];
			if ( 0x00 <= c && c <= 0x7f ) n = 0;
			else if ( ( c & 0xE0 ) == 0xC0 ) n = 1;
			else if ( c == 0xed && i < ( ix - 1 ) && ( ( unsigned char )string[ i + 1 ] & 0xa0 ) == 0xa0 ) return false;
			else if ( ( c & 0xF0 ) == 0xE0 ) n = 2;
			else if ( ( c & 0xF8 ) == 0xF0 ) n = 3;
			else return false;
			for ( j = 0; j < n && i < ix; j++ ) {
				if ( ( ++i == ix ) || ( ( ( unsigned char )string[ i ] & 0xC0 ) != 0x80 ) )
					return false;
			}
		}
		return true;
	}

	std::string cp1251_to_utf8( const char *str ) {
		std::string res;
		int result_u, result_c;
		result_u = MultiByteToWideChar( 1251, 0, str, -1, 0, 0 );
		if ( !result_u ) { return 0; }
		wchar_t *ures = new wchar_t[ result_u ];
		if ( !MultiByteToWideChar( 1251, 0, str, -1, ures, result_u ) ) {
			delete[] ures;
			return 0;
		}
		result_c = WideCharToMultiByte( 65001, 0, ures, -1, 0, 0, 0, 0 );
		if ( !result_c ) {
			delete[] ures;
			return 0;
		}
		char *cres = new char[ result_c ];
		if ( !WideCharToMultiByte( 65001, 0, ures, -1, cres, result_c, 0, 0 ) ) {
			delete[] cres;
			return 0;
		}
		delete[] ures;
		res.append( cres );
		delete[] cres;
		return res;
	}

	/*
		ConvertString is needed for UTF-8/ANSI coversion
		Union is using ANSI encoding by default, so any text that typed directly in the CPP files will be messed up in the RPC
		So, when you're done with text operations and ready to send it to RPC, use ConvertString before
		However, you don't need to use it if you send UTF-8 string (which is all the text readed from json)
		If you not sure if it's UTF-8 text or not, just use is_utf8 function
	*/

	void ConvertString( char *inStr, char *outputStr)
	{
		if ( is_utf8( inStr ) )
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> wconv;
			std::wstring wstr = wconv.from_bytes( inStr );
			
			std::vector<char> buf( wstr.size() );
			std::use_facet<std::ctype<wchar_t>>( std::locale( std::locale(".1251") ) ).narrow( wstr.data(), wstr.data() + wstr.size(), '?', buf.data() );

			memset( outputStr, 0, sizeof outputStr );
			strcat( outputStr, std::string( buf.data(), buf.size() ).c_str() );
		}
		else
		{
			std::string u8string = cp1251_to_utf8( inStr );
			memset( outputStr, 0, sizeof outputStr );
			strcat( outputStr, u8string.c_str() );
		}
	}

	static inline void ltrim( std::string &s ) {
		s.erase( s.begin(), std::find_if( s.begin(), s.end(), []( unsigned char ch ) {
			return !std::isspace( ch );
			} ) );
	}

	static inline void rtrim( std::string &s ) {
		s.erase( std::find_if( s.rbegin(), s.rend(), []( unsigned char ch ) {
			return !std::isspace( ch );
			} ).base(), s.end() );
	}

	static inline void trim( std::string &s ) {
		ltrim( s );
		rtrim( s );
	}

	static inline std::string ltrim_copy( std::string s ) {
		ltrim( s );
		return s;
	}

	static inline std::string rtrim_copy( std::string s ) {
		rtrim( s );
		return s;
	}

	static inline std::string trim_copy( std::string s ) {
		trim( s );
		return s;
	}

	std::vector<std::string> ExplodeString( std::string str, const char delimiter, bool remove_spaces = false )
	{
		std::vector<std::string> resultArray;
		std::stringstream data( str );

		std::string line;
		while ( std::getline( data, line, delimiter ) )
		{
			if ( remove_spaces )
				trim( line );

			resultArray.push_back( line );
		}

		return resultArray;
	}

	LangTags GetLang( std::string tag )
	{
		std::transform( tag.begin(), tag.end(), tag.begin(), ::toupper );

		if ( tag == "EN" )
			return LangTags::EN;
		else if ( tag == "RU" )
			return LangTags::RU;
		else if ( tag == "PL" )
			return LangTags::PL;
		else
			return LangTags::NONE;
	}

	std::string GetLang( LangTags tag )
	{
		switch ( tag )
		{
		case EN:
			return "EN";
			break;
		case RU:
			return "RU";
			break;
		case PL:
			return "PL";
			break;
		default:
			return "";
			break;
		}
	}

	LangTags ReadAliasTag( std::string &alias )
	{
		trim( alias );

		// Look for tag
		int index = alias.find( "{" );
		int index2 = alias.find( "}" );
		if ( index == std::string::npos || index2 == std::string::npos ) return LangTags::NONE;

		// Getting tag
		std::string tag = alias.substr( index + 1, index2 - 1);
		alias = alias.substr( index2 + 1 );		// remove tag from the string, we don't need it anymore
		trim( alias );

		LangTags aliasLanguage = GetLang( tag );
		return aliasLanguage;
		
	}

}