
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectRequest2( ) {
		udPBase ret = GetProjectRequest( );
		ret->get( "type_" )->ini( wstring( L"" ) );
		ret->get( "direct_" )->ini( wstring( L"" ) );
		ret->get( "manager2_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectRequest2

	CHRequest::CHRequest( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_request";
		m_szUpdIndex	= "id";
		m_szName		= "request";
		m_fnGetObject	= GetProjectRequest;

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_srequest", "state", "id", GetProjectSRequest );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_trequest", "type", "id", GetProjectTRequest );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_direct", "direct", "id", GetProjectDirect );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_user", "manager2", "id", GetProjectUser );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_rrequest", "result", "id", GetProjectRRequest );

		m_pObjFilter->get( "date1" )->ini( datetime( ) );
		m_pObjFilter->get( "date2" )->ini( datetime( ) );
		m_pObjFilter->get( "type" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "fio" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "direct" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "manager" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "order" )->ini( ( udUInt ) 0 );
	}

	CHRequest::CHRequest( const CHRequest& objCopy ) {
	}

	CHRequest::~CHRequest( ) {
	}

	CHRequest& CHRequest::operator =( const CHRequest& objCopy ) {
		return *this;
	}

	udInt CHRequest::FindObj( udPBase obj, const string& attr, bool apply ) {
		if ( apply ) {
			return 0;
		}
		// при обновлении пожалуйста, ищем по ID и пох
		return udStateHandlerSimple::FindObj( obj, attr, apply );
	} // bool FindObj

	bool CHRequest::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_request" );
		option.SetClass( GetProjectRequest2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "type" ) {
			szOrder = "`request_type_` ";
		} else if ( orderby == "direct" ) {
			szOrder = "`request_direct_` ";
		} else if ( orderby == "manager2" ) {
			szOrder = "`request_manager2_` ";
		} else {
			szOrder = "`request_" + orderby + "` ";
		}
		if ( !szOrder.empty( ) ) {
			szOrder += szOrderType;
		}
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		option.AddLeftJoin(
			"name", "request_type_", "ud_trequest",
			"`ud_trequest`.`trequest_id`=`ud_request`.`request_type`", GetProjectTRequest
		);
		option.AddLeftJoin(
			"name", "request_direct_", "ud_direct",
			"`ud_direct`.`direct_id`=`ud_request`.`request_direct`", GetProjectDirect
		);
		option.AddLeftJoin(
			"fio", "request_manager2_", "ud_user",
			"`ud_user`.`user_id`=`ud_request`.`request_manager2`", GetProjectUser
		);
		option.SetSqlCalcFoundRows( true );
		//////////////////////////////////////////////////////////////////////////////////////////
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "order" )->toUInt( );
		if ( id > 0 ) {
			option.AddLeftJoin( "",
				"CROSS JOIN `ud_objlnk` ON `ud_objlnk`.`objlnk_id_src`=`ud_request`.`request_id` AND "
				"`ud_objlnk`.`objlnk_id_dst`=" + toString( id ) + " AND "
				"`ud_objlnk`.`objlnk_type_src`=" + toString( udProjectObjectLink::objlnkRequest ) + " AND "
				"`ud_objlnk`.`objlnk_type_dst`=" + toString( udProjectObjectLink::objlnkOrder )
			);
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		option.SetIgnoreAttr( "type_" );
		option.SetIgnoreAttr( "direct_" );
		option.SetIgnoreAttr( "manager2_" );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udPDatabase db = g_pApp->GetDatabase( );
		udArrMorph arr1;
		if ( db->GetAssoc( "SELECT FOUND_ROWS( ) cnt", arr1 ) ) {
			cnt = arr1[ 0 ]->get( "cnt" )->toLLong( );
			ClearArrMorph( arr1 );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		return true;
	} // bool CustomLoadObjects

	string CHRequest::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectRequest;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		wstring str = L"";
		udUInt id = 0;
		datetime date;
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
		id = m_pObjFilter->get( "type" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "type" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		str = m_pObjFilter->get( "fio" )->toWString( );
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

			string add = config.GetAttrIndex( "fio" ) + " LIKE '";

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
		id = m_pObjFilter->get( "direct" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "direct" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "manager" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "manager2" ) + "=" + toString( id ) );
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
