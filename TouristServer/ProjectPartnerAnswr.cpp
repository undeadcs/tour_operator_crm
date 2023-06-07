
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectPartnerAnswr2( ) {
		udPBase ret = GetProjectPartnerAnswr( );
		ret->get( "partner_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectPartnerAnswr2

	CHPartnerAnswr::CHPartnerAnswr( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_partneranswr";
		m_szUpdIndex	= "id";
		m_szName		= "partneranswr";
		m_fnGetObject	= GetProjectPartnerAnswr;

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_partner", "partner", "id", GetProjectPartner );

		m_pObjFilter->get( "partner" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "date1" )->ini( datetime( ) );
		m_pObjFilter->get( "date2" )->ini( datetime( ) );
	}

	CHPartnerAnswr::CHPartnerAnswr( const CHPartnerAnswr& objCopy ) {
	}

	CHPartnerAnswr::~CHPartnerAnswr( ) {
	}

	CHPartnerAnswr& CHPartnerAnswr::operator =( const CHPartnerAnswr& objCopy ) {
		return *this;
	}

	udInt CHPartnerAnswr::FindObj( udPBase obj, const string& attr, bool apply ) {
		if ( apply ) { // при добавлении клонов среди запросов пока не может быть
			return 0;
		}
		// при обновлении пожалуйста, ищем по ID и пох
		return udStateHandlerSimple::FindObj( obj, attr, apply );
	} // bool FindObj

	bool CHPartnerAnswr::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_partneranswr" );
		option.SetClass( GetProjectPartnerAnswr2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "partner" ) {
			szOrder = "`partneranswr_partner_` ";
		} else {
			szOrder = "`partneranswr_" + orderby + "` ";
		}
		if ( !szOrder.empty( ) ) {
			szOrder += szOrderType;
		}
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		option.AddLeftJoin(
			"name", "partneranswr_partner_", "ud_partner",
			"`ud_partner`.`partner_id`=`ud_partneranswr`.`partneranswr_partner`", GetProjectPartner
		);
		option.SetIgnoreAttr( "partner_" );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
		}
		return true;
	} // bool CustomLoadObjects

	string CHPartnerAnswr::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectPartnerAnswr;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		datetime date;
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "partner" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "partner" ) + "=" + toString( id ) );
		}
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
		if ( !arrString.empty( ) ) {
			ret = Join( " AND ", arrString );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		//g_pApp->GetWndServer( )->UpdateText( L"ret='" + toWString( ret ) + L"'" );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ret;
	} // string GetWhereCondition

} // namespace UdSDK
