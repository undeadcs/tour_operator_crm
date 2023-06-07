
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectGroupPeriodList( ) {
		udPBase ret = GetProjectGroupPeriod( );
		ret->get( "client_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectGroupPeriodList

	CHGroupPeriod::CHGroupPeriod( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_groupperiod";
		m_szUpdIndex	= "id";
		m_szName		= "group";
		m_fnGetObject	= GetProjectGroupPeriod;

		this->AddManagedTable( "ud_groupperiod", "id", GetProjectGroupPeriod );

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_client", "client", "id", GetProjectClient );

		m_pObjFilter->get( "group" )->ini( ( udUInt ) 0 );
	} // CHGroupPeriod

	CHGroupPeriod::CHGroupPeriod( const CHGroupPeriod& objCopy ) : udStateHandlerSimple( ) {
	} // CHGroupPeriod

	CHGroupPeriod::~CHGroupPeriod( ) {
	} // ~CHGroupPeriod

	CHGroupPeriod& CHGroupPeriod::operator =( const CHGroupPeriod& objCopy ) {
		return *this;
	} // CHGroupPeriod& operator =

	bool CHGroupPeriod::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_groupperiod" );
		option.SetClass( GetProjectGroupPeriodList );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "client" ) {
			szOrder = "`groupperiod_client_` ";
		} else {
			szOrder = "`groupperiod_" + orderby + "` ";
		}
		if ( !szOrder.empty( ) ) {
			szOrder += szOrderType;
		}
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		option.AddLeftJoin(
			"fio", "groupperiod_client_", "ud_client",
			"`ud_client`.`client_id`=`ud_groupperiod`.`groupperiod_client`", GetProjectClient
		);
		option.SetIgnoreAttr( "client_" );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
		}
		return true;
	} // bool CustomLoadObjects

	string CHGroupPeriod::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectGroupPeriod;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "group" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "group" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !arrString.empty( ) ) {
			ret = Join( " AND ", arrString );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		return ret;
	} // string GetWhereCondition

} // namespace UdSDK
