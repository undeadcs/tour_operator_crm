
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectRoute( ) {
		return new udProjectRoute;
	}

	CProjectRoute::CProjectRoute( ) {
		this->get( "id"			)->ini( ( udUInt ) 0	);
		this->get( "name"		)->ini( wstring( L"" )	);
		this->get( "namelatin"	)->ini( wstring( L"" )	);
	}

	CProjectRoute::~CProjectRoute( ) {
	}

	void CProjectRoute::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_route" );
		config.SetPrefix( "route_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",			"ID"			);
		config.SetAttrTitle( "name",		"Наименование"	);
		config.SetAttrTitle( "namelatin",	"Наименование латиницей"	);
	} // void GetConfig



	udPBase GetProjectTourRoute( ) {
		return new udProjectTourRoute;
	}

	CProjectTourRoute::CProjectTourRoute( ) {
		this->get( "id" )->ini( ( udUInt ) 0 );
		this->get( "tour" )->ini( ( udUInt ) 0 );
		this->get( "city" )->ini( ( udUInt ) 0 );
	}

	CProjectTourRoute::~CProjectTourRoute( ) {
	}

	void CProjectTourRoute::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_tourroute" );
		config.SetPrefix( "tourroute_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",		"ID"	);
		config.SetAttrTitle( "tour",	"Тур"	);
		config.SetAttrTitle( "city",	"Город"	);
	} // void GetConfig



	udPBase GetProjectTplTourRoute( ) {
		return new udProjectTplTourRoute;
	}

	CProjectTplTourRoute::CProjectTplTourRoute( ) {
		this->get( "id" )->ini( ( udUInt ) 0 );
		this->get( "tpltour" )->ini( ( udUInt ) 0 );
		this->get( "city" )->ini( ( udUInt ) 0 );
	}

	CProjectTplTourRoute::~CProjectTplTourRoute( ) {
	}

	void CProjectTplTourRoute::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_tpltourroute" );
		config.SetPrefix( "tpltourroute_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",		"ID"			);
		config.SetAttrTitle( "tpltour",	"Шаблон тура"	);
		config.SetAttrTitle( "city",	"Город"			);
	} // void GetConfig




	udPBase GetProjectOrderRoute( ) {
		return new udProjectOrderRoute;
	}

	CProjectOrderRoute::CProjectOrderRoute( ) {
		this->get( "id" )->ini( ( udUInt ) 0 );
		this->get( "order" )->ini( ( udUInt ) 0 );
		this->get( "name" )->ini( wstring( L"" ) );
	}

	CProjectOrderRoute::~CProjectOrderRoute( ) {
	}

	void CProjectOrderRoute::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_orderroute" );
		config.SetPrefix( "orderroute_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",		"ID"		);
		config.SetAttrTitle( "order",	"Заявка"	);
		config.SetAttrTitle( "name",	"Пункт"		);
	} // void GetConfig

} // namespace UdSDK
