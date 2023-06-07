
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectUser2( ) {
		udPBase ret = GetProjectUser( );
		ret->get( "rank_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectUser2

	CHUser::CHUser( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_user";
		m_szUpdIndex	= "id";
		m_szName		= "user";
		m_fnGetObject	= GetProjectUser;

		m_pObjFilter->get( "login" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "email" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "fio" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "rank" )->ini( ( udInt ) udProjectUser::USERRANK_INVALID );
	}

	CHUser::CHUser( const CHUser& objCopy ) {
	}

	CHUser::~CHUser( ) {
	}

	CHUser& CHUser::operator =( const CHUser& objCopy ) {
		return *this;
	}

	udInt CHUser::UpdObj( udPBase obj ) {
		udArrBase		arr;
		udHandlerOption	option;
		udArrError		err;
		udPHandler		handler = g_pApp->GetHandler( );

		if ( obj->get( "password" )->toWString( ).empty( ) ) { // игнорим пароль
			option.SetIgnoreAttr( "password" );
		} else { // тут шифруем новый пароль
		}
		arr.push_back( obj );
		option.SetTable( m_szTableName );
		option.SetIndexAttr( m_szUpdIndex );
		handler->UpdUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			udPLog log = g_pApp->GetLog( );
			for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
				log->Write( i->GetText( ) + "\n" );
			}
			err.clear( );
			return 0;
		}
		return 1;
	} // udInt UpdObj

	udInt CHUser::FindObj( udPBase obj, const string& attr, bool apply ) {
		if ( apply ) {
			return udStateHandlerSimple::FindObj( obj, "login", apply );
		}
		// при обновлении пожалуйста, ищем по ID и пох
		return udStateHandlerSimple::FindObj( obj, attr, apply );
	} // udInt FindObj

	bool CHUser::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_user" );
		//option.SetClass( GetProjectUser2 );
		option.SetClass( GetProjectUser );
		option.SetLimit( pager->GetSQLLimit( ) );
		szOrder = "`user_" + orderby + "` ";
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		//option.SetIgnoreAttr( "rank_" );
		handler->GetUdObject( arr, option, &err );
		if ( err.empty( ) ) {
			/*map< udUInt, wstring > mapRank;
			mapRank[ udProjectUser::USERRANK_INVALID		] = L"";
			mapRank[ udProjectUser::USERRANK_ADMINISTRATOR	] = L"Администратор";
			mapRank[ udProjectUser::USERRANK_MANAGER		] = L"Менеджер";
			mapRank[ udProjectUser::USERRANK_CACHIER		] = L"Кассир";
			mapRank[ udProjectUser::USERRANK_ACCOUNTANT		] = L"Бухгалтер";
			mapRank[ udProjectUser::USERRANK_CHIEF			] = L"Руководитель";
			mapRank[ udProjectUser::USERRANK_SUPERADMIN		] = L"Суперадминистратор";
			udUInt	rank	= 0;
			udPBase	obj		= NULL;

			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				rank = obj->get( "rank" )->toUInt( );
				obj->get( "rank_" )->ini( mapRank[ rank ] );
			}*/
		} else {
			WriteErrors( g_pApp->GetLog( ), err );
		}
		return true;
	} // bool CustomLoadObjects

	string CHUser::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectUser;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		wstring str = L"";
		udInt rank = 0;
		string attrs[ 3 ] = { "login", "email", "fio" };
		//////////////////////////////////////////////////////////////////////////////////////////
		for( int i = 0; i < 3; ++i ) {
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
		rank = m_pObjFilter->get( "rank" )->toInt( );
		if ( rank > 0 ) {
			arrString.push_back( config.GetAttrIndex( "rank" ) + "=" + toString( rank ) );
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
