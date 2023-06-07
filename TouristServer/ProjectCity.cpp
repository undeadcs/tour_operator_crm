
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectCity2( ) {
		udPBase ret = GetProjectCity( );
		ret->get( "country_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectCity2

	CHCity::CHCity( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_city";
		m_szUpdIndex	= "id";
		m_szName		= "city";
		m_fnGetObject	= GetProjectCity;

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_country", "country", "id", GetProjectCountry );

		m_pObjFilter->get( "name" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "country" )->ini( ( udUInt ) 0 );
	}

	CHCity::CHCity( const CHCity& objCopy ) {
	}

	CHCity::~CHCity( ) {
	}

	CHCity& CHCity::operator =( const CHCity& objCopy ) {
		return *this;
	}

	udInt CHCity::FindObj( udPBase obj, const string& attr, bool apply ) {
		if ( apply ) {
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
			arrString.push_back( "(" + config.GetAttrIndex( "name" ) + "=" + obj->GetAttrValue( "name", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
			arrString.push_back( "(" + config.GetAttrIndex( "country" ) + "=" + obj->GetAttrValue( "country", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
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

	/*void CHCity::AfterLoadList( udArrBase& arr ) {
		if ( arr.empty( ) ) {
			return;
		}
		//LoadListDataByAttr( arr, g_pApp->GetHandler( ), GetProjectCountry, "ud_country", "country", "name", "country_" );
	} // void AfterLoadList*/

	bool CHCity::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_city" );
		option.SetClass( GetProjectCity2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "country" ) {
			szOrder = "`city_country_` ";
		} else {
			szOrder = "`city_" + orderby + "` ";
		}
		szOrder += szOrderType;
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		option.AddLeftJoin(
			"name", "city_country_", "ud_country",
			"`ud_country`.`country_id`=`ud_city`.`city_country`", GetProjectCountry
		);
		option.SetIgnoreAttr( "country_" );
		handler->GetUdObject( arr, option, &err );
		return true;
	} // bool CustomLoadObjects

	string CHCity::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectCity;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		wstring str;
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		str = m_pObjFilter->get( "name" )->toWString( );
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

			string add = config.GetAttrIndex( "name" ) + " LIKE '";

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
		id = m_pObjFilter->get( "country" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "country" ) + "=" + toString( id ) );
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

	void CHCity::SchemeCenter( udInt opId, udPSocket sock ) {
		if ( opId > opidGet ) {
			if ( opId == opidGet2 ) {
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				//////////////////////////////////////////////////////////////////////////////////////
				// набор ID городов
				udType ids_num( ( udUInt ) 0 );
				ids_num.read( sock );
				if ( ids_num.toUInt( ) > 0 ) {
					map< udUInt, udUInt > ids;
					udUInt cnt1 = ids_num.toUInt( );
					for( udUInt i = 0; i < cnt1; ++i ) {
						if ( ids_num.read( sock ) == 1 ) {
							ids[ ids_num.toUInt( ) ] = 1;
						}
					}
					//
					udPBase obj = m_fnGetObject( );
					udBaseConfig config;
					obj->GetConfig( config );
					udDELETE( obj );
					string szWhere = "";
					if ( !ids.empty( ) ) {
						udArrString arrString;
						for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
							arrString.push_back( toString( i->first ) );
						}
						szWhere = config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrString ) + ")";
					}
					config.Clear( );
					//////////////////////////////////////////////////////////////////////////////////////
					udPHandler handler = g_pApp->GetHandler( );
					udHandlerOption option;
					udArrError err;
					udArrBase arr;
					udBinaryData data;

					option.SetTable( m_szTableName );
					option.SetClass( GetProjectCity2 );
					option.SetWhere( szWhere );
					option.SetIgnoreAttr( "country_" );
					option.AddLeftJoin(
						"name", "city_country_", "ud_country",
						"`ud_country`.`country_id`=`ud_city`.`city_country`", GetProjectCountry
					);
					udLongLong cnt = handler->CountUdObject( option );
					handler->GetUdObject( arr, option, &err );
					if ( !err.empty( ) ) {
						WriteErrors( g_pApp->GetLog( ), err );
						data.add( ( char ) 0 );
					} else {
						data.add( ( char ) 1 );
						data.add( ( udUInt ) cnt );
						this->AfterLoadList( arr );
						WriteInstances( arr, data );
					}
					//////////////////////////////////////////////////////////////////////////////////////
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->Send( data );
					sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
					char code = 0;
					sock->ReadByte( &code );
					//////////////////////////////////////////////////////////////////////////////////////
					ClearArrBase( arr );
				} else {
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 0 );
				}
			} else {
				udStateHandlerSimple::SchemeCenter( opId, sock );
			}
		} else {
			udStateHandlerSimple::SchemeCenter( opId, sock );
		}
	} // void SchemeCenter

} // namespace UdSDK
