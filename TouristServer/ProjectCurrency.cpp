
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHCurrency::CHCurrency( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_currency";
		m_szUpdIndex	= "id";
		m_szName		= "currency";
		m_fnGetObject	= GetProjectCurrency;

		m_pObjFilter->get( "name" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "code" )->ini( wstring( L"" ) );
	}

	CHCurrency::CHCurrency( const CHCurrency& objCopy ) {
	}

	CHCurrency::~CHCurrency( ) {
	}

	CHCurrency& CHCurrency::operator =( const CHCurrency& objCopy ) {
		return *this;
	}

	string CHCurrency::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectCurrency;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		wstring str = L"";
		string attrs[ 2 ] = { "name", "code" };
		//////////////////////////////////////////////////////////////////////////////////////////
		for( int i = 0; i < 2; ++i ) {
			str = m_pObjFilter->get( attrs[ i ] )->toWString( );
			if ( !str.empty( ) ) {
				string szStr = ConvertString( str, CP_UTF8 );
				if ( szStr.size( ) ) {
					udDWord	dwLen	= szStr.size( ) * 2;
					udPChar szTmp	= new udChar[ dwLen + 1 ];

					memset( szTmp, 0, dwLen + 1 );
					UdBaseEscapeString( szTmp, szStr.c_str( ), szStr.size( ) );

					szStr = szTmp;

					delete [ ] szTmp;
				}

				string add = config.GetAttrIndex( attrs[ i ] ) + " LIKE '";

				int pos = 0;
				for( string::iterator i = szStr.begin( ); i != szStr.end( ); ++i, ++pos ) {
					if ( *i == '*' ) {
						add += '%';
					} else if ( *i == '%' ) {
					} else {
						add += *i;
					}
				}

				add += "'";
				arrString.push_back( add );
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !arrString.empty( ) ) {
			ret = Join( " AND ", arrString );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		//g_pApp->GetWndServer( )->UpdateText( L"ret='" + toWString( ret ) + L"'" );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ret;
	} // string GetWhereCondition

} // namespace UdSDK
