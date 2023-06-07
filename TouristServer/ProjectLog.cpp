
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectLog2( ) {
		udPBase ret = GetProjectLog( );
		ret->get( "user_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectLog2

	CHLog::CHLog( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_log";
		m_szUpdIndex	= "id";
		m_szName		= "log";
		m_fnGetObject	= GetProjectLog;

		m_pObjFilter->get( "date1" )->ini( datetime( ) );
		m_pObjFilter->get( "date2" )->ini( datetime( ) );
		m_pObjFilter->get( "user" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "text" )->ini( wstring( L"" ) );
	}

	CHLog::CHLog( const CHLog& objCopy ) {
	}

	CHLog::~CHLog( ) {
	}

	CHLog& CHLog::operator =( const CHLog& objCopy ) {
		return *this;
	}

	bool CHLog::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_log" );
		option.SetClass( GetProjectLog2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "user" ) {
			szOrder = "`log_user_` ";
		} else {
			szOrder = "`log_" + orderby + "` ";
		}
		if ( !szOrder.empty( ) ) {
			szOrder += szOrderType;
		}
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		option.AddLeftJoin(
			"fio", "log_user_", "ud_user",
			"`ud_user`.`user_id`=`ud_log`.`log_user`", GetProjectUser
		);
		option.SetIgnoreAttr( "user_" );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
		}
		return true;
	} // bool CustomLoadObjects

	string CHLog::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectLog;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		datetime date;
		wstring str = L"";
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date1" )->toTime( );
		if ( date.get( ) > 0 ) {
			arrString.push_back( config.GetAttrIndex( "created" ) + ">='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date2" )->toTime( );
		if ( date.get( ) > 0 ) {
			date.inc( 1, datetime::datetimeDay );
			arrString.push_back( config.GetAttrIndex( "created" ) + "<='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "user" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "user" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		str = m_pObjFilter->get( "text" )->toWString( );
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

			string add = config.GetAttrIndex( "action" ) + " LIKE '";

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
