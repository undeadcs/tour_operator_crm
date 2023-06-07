
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectPartnerReq2( ) {
		udPBase ret = GetProjectPartnerReq( );
		ret->get( "partner_" )->ini( wstring( L"" ) );
		ret->get( "manager_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectPartnerReq2

	CHPartnerReq::CHPartnerReq( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_partnerreq";
		m_szUpdIndex	= "id";
		m_szName		= "partnerreq";
		m_fnGetObject	= GetProjectPartnerReq;

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_partner", "partner", "id", GetProjectPartner );

		m_pObjFilter->get( "partner" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "date1" )->ini( datetime( ) );
		m_pObjFilter->get( "date2" )->ini( datetime( ) );
		m_pObjFilter->get( "manager" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "order" )->ini( ( udUInt ) 0 );
	}

	CHPartnerReq::CHPartnerReq( const CHPartnerReq& objCopy ) {
	}

	CHPartnerReq::~CHPartnerReq( ) {
	}

	CHPartnerReq& CHPartnerReq::operator =( const CHPartnerReq& objCopy ) {
		return *this;
	}

	udInt CHPartnerReq::FindObj( udPBase obj, const string& attr, bool apply ) {
		if ( apply ) { // при добавлении клонов среди запросов пока не может быть
			return 0;
		}
		// при обновлении пожалуйста, ищем по ID и пох
		return udStateHandlerSimple::FindObj( obj, attr, apply );
	} // bool FindObj

	bool CHPartnerReq::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_partnerreq" );
		option.SetClass( GetProjectPartnerReq2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "partner" ) {
			szOrder = "`partnerreq_partner_` ";
		} else if ( orderby == "manager" ) {
			szOrder = "`partnerreq_manager_` ";
		} else {
			szOrder = "`partnerreq_" + orderby + "` ";
		}
		if ( !szOrder.empty( ) ) {
			szOrder += szOrderType;
		}
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		option.AddLeftJoin(
			"name", "partnerreq_partner_", "ud_partner",
			"`ud_partner`.`partner_id`=`ud_partnerreq`.`partnerreq_partner`", GetProjectPartner
		);
		option.AddLeftJoin(
			"fio", "partnerreq_manager_", "ud_user",
			"`ud_user`.`user_id`=`ud_partnerreq`.`partnerreq_manager`", GetProjectUser
		);
		option.SetSqlCalcFoundRows( true );
		//////////////////////////////////////////////////////////////////////////////////////////
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "order" )->toUInt( );
		if ( id > 0 ) {
			option.AddLeftJoin( "",
				"CROSS JOIN `ud_objlnk` ON `ud_objlnk`.`objlnk_id_src`=" + toString( id ) + " AND "
				"`ud_objlnk`.`objlnk_id_dst`=`ud_partnerreq`.`partnerreq_id` AND "
				"`ud_objlnk`.`objlnk_type_src`=" + toString( udProjectObjectLink::objlnkOrder ) + " AND "
				"`ud_objlnk`.`objlnk_type_dst`=" + toString( udProjectObjectLink::objlnkPartnerReq )
			);
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		option.SetIgnoreAttr( "partner_" );
		option.SetIgnoreAttr( "manager_" );
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

	string CHPartnerReq::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectPartnerReq;
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
		id = m_pObjFilter->get( "manager" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "manager" ) + "=" + toString( id ) );
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

	void CHPartnerReq::ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj ) {
		/*char code = 0;
		sock->ReadByte( &code );
		if ( code == 80 ) { // связь с обращением
			udUInt id = 0, type = 0;
			udType num( ( udUInt ) 0 );
			if ( num.read( sock ) != 1 ) {
				return;
			}
			type = num.toUInt( );
			if ( num.read( sock ) != 1 ) {
				return;
			}
			id = num.toUInt( );
			udPBase add = new udProjectObjectLink;
			add->get( "id_src" )->ini( id );
			add->get( "id_dst" )->ini( obj->get( "id" )->toUInt( ) );
			add->get( "type_src" )->ini( type );
			add->get( "type_dst" )->ini( udProjectObjectLink::objlnkPartnerReq );
			datetime date;
			date.load( );
			add->get( "created" )->ini( date );
			udHandlerOption option;
			option.SetTable( "ud_objlnk" );
			udArrError err;
			g_pApp->GetHandler( )->AddUdObject( add, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( g_pApp->GetLog( ), err );
			}
			//*
			string log = "\nobject link\n";
			add->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			//* /
			udDELETE( add );
		}*/
	} // void ExtraDataRecieve

} // namespace UdSDK
