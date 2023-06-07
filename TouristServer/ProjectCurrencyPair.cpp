
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectCurrencyPair2( ) {
		udPBase ret = GetProjectCurrencyPair( );
		ret->get( "pair" )->ini( wstring( L"" ) );
		//ret->get( "set_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectCurrencyPair2

	CHCurrencyPair::CHCurrencyPair( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_currencypair";
		m_szUpdIndex	= "id";
		m_szName		= "currencypair";
		m_fnGetObject	= GetProjectCurrencyPair;

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_currency", "currency1", "id", GetProjectCurrency );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_currency", "currency2", "id", GetProjectCurrency );

		m_pObjFilter->get( "date1" )->ini( datetime( ) );
		m_pObjFilter->get( "date2" )->ini( datetime( ) );
		m_pObjFilter->get( "course1" )->ini( ( udFloat ) 0.0f );
		m_pObjFilter->get( "course2" )->ini( ( udFloat ) 0.0f );
		m_pObjFilter->get( "cb1" )->ini( ( udFloat ) 0.0f );
		m_pObjFilter->get( "cb2" )->ini( ( udFloat ) 0.0f );
	}

	CHCurrencyPair::CHCurrencyPair( const CHCurrencyPair& objCopy ) {
	}

	CHCurrencyPair::~CHCurrencyPair( ) {
	}

	CHCurrencyPair& CHCurrencyPair::operator =( const CHCurrencyPair& objCopy ) {
		return *this;
	}

	udInt CHCurrencyPair::FindObj( udPBase obj, const string& attr, bool apply ) {
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
			arrString.push_back( "(" + config.GetAttrIndex( "currency1" ) + "=" + obj->GetAttrValue( "currency1", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
			arrString.push_back( "(" + config.GetAttrIndex( "currency2" ) + "=" + obj->GetAttrValue( "currency2", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
			arrString.push_back( "(" + config.GetAttrIndex( "course" ) + "=" + obj->GetAttrValue( "course", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
			arrString.push_back( "(" + config.GetAttrIndex( "set" ) + "=" + obj->GetAttrValue( "set", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
			arrString.push_back( "(" + config.GetAttrIndex( "internal" ) + "=" + obj->GetAttrValue( "internal", config, udBase::UDSDK_CONFIG_DATABASE )->toString( ) + ")" );
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

	bool CHCurrencyPair::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_currencypair" );
		option.SetClass( GetProjectCurrencyPair2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "pair" ) {
			szOrder = "`currencypair_pair` ";
		} else {
			szOrder = "`currencypair_" + orderby + "` ";
		}
		if ( !szOrder.empty( ) ) {
			szOrder += szOrderType;
		}
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		option.AddLeftJoin( "CONCAT(cpl.`currency_name`,' / ', cpr.`currency_name`) AS `currencypair_pair`", "" );
		option.AddLeftJoin( "", "LEFT JOIN `ud_currency` cpl ON cpl.`currency_id`=`ud_currencypair`.`currencypair_currency1`" );
		option.AddLeftJoin( "", "LEFT JOIN `ud_currency` cpr ON cpr.`currency_id`=`ud_currencypair`.`currencypair_currency2`" );
		option.SetIgnoreAttr( "pair" );
		//option.SetIgnoreAttr( "set_" );
		handler->GetUdObject( arr, option, &err );
		if ( err.empty( ) ) {
		} else {
			WriteErrors( g_pApp->GetLog( ), err );
		}
		return true;
	} // bool CustomLoadObjects

	string CHCurrencyPair::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectCurrencyPair;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		datetime date;
		udFloat val = 0.0f;
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date1" )->toTime( );
		if ( date.get( ) > 0 ) {
			arrString.push_back( config.GetAttrIndex( "set" ) + ">='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date2" )->toTime( );
		if ( date.get( ) > 0 ) {
			date.inc( 1, datetime::datetimeDay );
			arrString.push_back( config.GetAttrIndex( "set" ) + "<='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		val = m_pObjFilter->get( "course1" )->toFloat( );
		if ( val > 0.0f ) {
			arrString.push_back( config.GetAttrIndex( "internal" ) + ">=" + toString( val ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		val = m_pObjFilter->get( "course2" )->toFloat( );
		if ( val > 0.0f ) {
			arrString.push_back( config.GetAttrIndex( "internal" ) + "<=" + toString( val ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		val = m_pObjFilter->get( "cb1" )->toFloat( );
		if ( val > 0.0f ) {
			arrString.push_back( config.GetAttrIndex( "course" ) + ">=" + toString( val ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		val = m_pObjFilter->get( "cb2" )->toFloat( );
		if ( val > 0.0f ) {
			arrString.push_back( config.GetAttrIndex( "course" ) + "<=" + toString( val ) );
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
