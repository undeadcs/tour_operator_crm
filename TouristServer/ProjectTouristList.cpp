
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTouristList2( ) {
		udPBase ret = GetProjectTouristList( );
		ret->get( "leader_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTouristList2

	static void TouristListFilterClient( udArrBase& arr ) {
		if ( arr.empty( ) ) {
			return;
		}
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "fio" )->ini( wstring( L"" ) );
		}
		LoadListDataByAttr( arr, g_pApp->GetHandler( ), GetProjectClient, "ud_client", "client", "fio", "fio" );
	} // void TouristListFilterClient

	CHTouristList::CHTouristList( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_touristlist";
		m_szUpdIndex	= "id";
		m_szName		= "touristlist";
		m_fnGetObject	= GetProjectTouristList;

		this->AddManagedTable( "ud_touristlistlink", "id", GetProjectTouristListLink );

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_transferpoint", "exit", "id", GetProjectTransferPoint );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_transferpoint", "enter", "id", GetProjectTransferPoint );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_hostside", "host", "id", GetProjectHostSide );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_route", "route", "id", GetProjectRoute );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_client", "leader", "id", GetProjectClient );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_touristlistlink", "list", "id", GetProjectTouristListLink, "`touristlistlink_id` ASC", "", TouristListFilterClient );

		m_pObjFilter->get( "date1" )->ini( datetime( ) );
		m_pObjFilter->get( "date2" )->ini( datetime( 0, true ) );
		m_pObjFilter->get( "leader" )->ini( ( udUInt ) 0 );
	}

	CHTouristList::CHTouristList( const CHTouristList& objCopy ) {
	}

	CHTouristList::~CHTouristList( ) {
	}

	CHTouristList& CHTouristList::operator =( const CHTouristList& objCopy ) {
		return *this;
	}

	bool CHTouristList::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_touristlist" );
		option.SetClass( GetProjectTouristList2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "leader" ) {
			szOrder = "`touristlist_leader_` ";
		} else {
			szOrder = "`touristlist_" + orderby + "` ";
		}
		szOrder += szOrderType;
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		option.AddLeftJoin(
			"fio", "touristlist_leader_", "ud_client",
			"`ud_client`.`client_id`=`ud_touristlist`.`touristlist_leader`", GetProjectClient
		);
		option.SetIgnoreAttr( "leader_" );
		handler->GetUdObject( arr, option, &err );
		return true;
	} // bool CustomLoadObjects

	string CHTouristList::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectTouristList;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		datetime date;
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date1" )->toTime( );
		if ( date.get( ) > 0 ) {
			arrString.push_back( config.GetAttrIndex( "date1" ) + ">='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date2" )->toTime( );
		if ( date.get( ) > 0 ) {
			date.inc( 1, datetime::datetimeDay );
			arrString.push_back( config.GetAttrIndex( "date1" ) + "<='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "leader" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "leader" ) + "=" + toString( id ) );
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

	udInt CHTouristList::FindObj( udPBase obj, const string& attr, bool apply ) {
		if ( apply ) {
			return 0;
		}
		// при обновлении пожалуйста, ищем по ID и пох
		return udStateHandlerSimple::FindObj( obj, attr, apply );
	} // bool FindObj

} // namespace UdSDK
