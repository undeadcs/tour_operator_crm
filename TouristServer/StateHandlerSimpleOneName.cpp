
#include "includes.h"

namespace UdSDK {

	CStateHandlerSimpleOneName::CStateHandlerSimpleOneName( ) : udStateHandlerSimple( ) {
		m_pObjFilter->get( "name" )->ini( wstring( L"" ) );
	}

	CStateHandlerSimpleOneName::~CStateHandlerSimpleOneName( ) {
	}

	string CStateHandlerSimpleOneName::GetWhereCondition( ) {
		wstring name = m_pObjFilter->get( "name" )->toWString( );
		if ( name.empty( ) ) {
			return "";
		}
		string ret = "";
		udPBase obj = m_fnGetObject( );
		udBaseConfig config;
		obj->GetConfig( config );

		string szStr = ConvertString( name, CP_UTF8 );
		if ( szStr.size( ) ) {
			udDWord	dwLen	= szStr.size( ) * 2;
			udPChar szTmp	= new udChar[ dwLen + 1 ];

			memset( szTmp, 0, dwLen + 1 );
			UdBaseEscapeString( szTmp, szStr.c_str( ), szStr.size( ) );

			szStr = szTmp;

			delete [ ] szTmp;
		}

		ret = config.GetAttrIndex( "name" ) + " LIKE '";

		int pos = 0;
		for( string::iterator i = szStr.begin( ); i != szStr.end( ); ++i, ++pos ) {
			if ( *i == '*' ) {
				ret += '%';
			} else if ( *i == '%' ) {
			} else {
				ret += *i;
			}
		}

		ret += "'";
		udDELETE( obj );
		return ret;
	} // string GetWhereCondition

} // namespace UdSDK