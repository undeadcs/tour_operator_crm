
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectClDoc2( ) {
		udPBase ret = GetProjectClDoc( );
		ret->get( "client_" )->ini( wstring( L"" ) );
		ret->get( "type_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectClDoc2

	CHClDoc::CHClDoc( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_cldoc";
		m_szUpdIndex	= "id";
		m_szName		= "cldoc";
		m_fnGetObject	= GetProjectClDoc;

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_client", "client", "id", GetProjectClient );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_doctype", "type", "id", GetProjectDocType );

		m_pObjFilter->get( "client" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "type" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "series" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "number" )->ini( wstring( L"" ) );
	}

	CHClDoc::CHClDoc( const CHClDoc& objCopy ) {
	}

	CHClDoc::~CHClDoc( ) {
	}

	CHClDoc& CHClDoc::operator =( const CHClDoc& objCopy ) {
		return *this;
	}

	udInt CHClDoc::FindObj( udPBase obj, const string& attr, bool apply ) {
		if ( apply ) { // при добавлении клонов среди запросов пока не может быть
			/*
			0 - клонов не найдено и все прошло хорошо
			1 - клон найден и он только 1
			2 - клон найден и их больше 1
			3 - произошли ошибки при поиске клонов
			*/
			//////////////////////////////////////////////////////////////////////////////////////////
			udPHandler		handler = g_pApp->GetHandler( );
			udBaseConfig	config;
			udHandlerOption	option;
			udArrBase		arr;
			udArrError		err;
			string			szWhere = "";
			udInt			ret		= 0;
			//////////////////////////////////////////////////////////////////////////////////////////
			obj->GetConfig( config );

			udArrString arrString;
			arrString.push_back( "(" + config.GetAttrIndex( "client" ) + "=" + obj->GetAttrValue( "client", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
			arrString.push_back( "(" + config.GetAttrIndex( "type" ) + "=" + obj->GetAttrValue( "type", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
			arrString.push_back( "(" + config.GetAttrIndex( "series" ) + "=" + obj->GetAttrValue( "series", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
			arrString.push_back( "(" + config.GetAttrIndex( "number" ) + "=" + obj->GetAttrValue( "number", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
			arrString.push_back( "(" + config.GetAttrIndex( "issuedate" ) + "=" + obj->GetAttrValue( "issuedate", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
			arrString.push_back( "(" + config.GetAttrIndex( "issuedby" ) + "=" + obj->GetAttrValue( "issuedby", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
			arrString.push_back( "(" + config.GetAttrIndex( "birthplace" ) + "=" + obj->GetAttrValue( "birthplace", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
			arrString.push_back( "(" + config.GetAttrIndex( "enddate" ) + "=" + obj->GetAttrValue( "enddate", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
			arrString.push_back( "(" + config.GetAttrIndex( "fio" ) + "=" + obj->GetAttrValue( "fio", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
			szWhere = Join( " AND ", arrString );
			config.Clear( );
			//////////////////////////////////////////////////////////////////////////////////////////
			option.SetTable( m_szTableName );
			option.SetClass( m_fnGetObject );
			option.SetWhere( szWhere );
			handler->GetUdObject( arr, option, &err );
			option.Clear( );

			if ( !err.empty( ) ) {
				udPLog __log = g_pApp->GetLog( );
				for( udArrError::iterator i = err.begin( ); i != err.end( ); ++i ) {
					__log->Write( i->GetText( ) + "\r\n" );
				}
				err.clear( );
				ret = 3;
			} else if ( arr.empty( ) ) { // объект не найден
				ret = 0;
			} else { // объект найден
				arr[ 0 ]->apply( obj );
				if ( arr.size( ) > 1 ) {
					ret = 2;
				} else {
					ret = 1;
				}
			}
			ClearArrBase( arr );
			err.clear( );
			//////////////////////////////////////////////////////////////////////////////////////////
			return ret;
		}
		// при обновлении пожалуйста, ищем по ID и пох
		return udStateHandlerSimple::FindObj( obj, attr, apply );
	} // bool FindObj

	void CHClDoc::AfterLoadList( udArrBase& arr ) {
		if ( arr.empty( ) ) {
			return;
		}
		/*udPHandler handler = g_pApp->GetHandler( );
		LoadListDataByAttr( arr, handler, GetProjectClient, "ud_client", "client", "fio", "client_" );
		LoadListDataByAttr( arr, handler, GetProjectDocType, "ud_doctype", "type", "name", "type_" );*/
	} // void AfterLoadList

	bool CHClDoc::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPLog __log = g_pApp->GetLog( );
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		string szOrder = "", szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC";

		option.SetTable( m_szTableName );
		option.SetClass( GetProjectClDoc2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "client" ) {
			szOrder = "`cldoc_client_` ";
		} else if ( orderby == "type" ) {
			szOrder = "`cldoc_type_` ";
		} else {
			szOrder = "`cldoc_" + orderby + "` ";
		}
		szOrder += szOrderType;
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		option.AddLeftJoin(
			"fio", "cldoc_client_", "ud_client",
			"`ud_client`.`client_id`=`ud_cldoc`.`cldoc_client`", GetProjectClient
		);
		option.AddLeftJoin(
			"name", "cldoc_type_", "ud_doctype",
			"`ud_doctype`.`doctype_id`=`ud_cldoc`.`cldoc_type`", GetProjectDocType
		);
		option.SetIgnoreAttr( "client_" );
		option.SetIgnoreAttr( "type_" );
		handler->GetUdObject( arr, option, &err );
		return true;
	} // bool CustomLoadObjects

	string CHClDoc::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectClDoc;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		wstring str = L"";
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "client" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "client" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "type" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "type" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		str = m_pObjFilter->get( "series" )->toWString( );
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

			string add = config.GetAttrIndex( "series" ) + " LIKE '";

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
		str = m_pObjFilter->get( "number" )->toWString( );
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

			string add = config.GetAttrIndex( "number" ) + " LIKE '";

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
