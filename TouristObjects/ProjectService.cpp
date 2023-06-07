
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectService( ) {
		return new udProjectService;
	} // udPBase GetProjectService

	CProjectService::CProjectService( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID сущности
		this->get( "name"		)->ini( ( udUInt )	0	);	// ID названия услуги
		this->get( "type"		)->ini( ( udUInt )	0	);	// ID типа услуги
		this->get( "country"	)->ini( ( udUInt )	0	);	// ID страны
		this->get( "city"		)->ini( ( udUInt )	0	);	// ID города
		this->get( "partner"	)->ini( ( udUInt )	0	);	// ID партнера
		this->get( "munit"		)->ini( ( udUInt )	0	);	// ID единицы измерения
	}

	CProjectService::~CProjectService( ) {
	}

	void CProjectService::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_service" );
		objConfig.SetPrefix( "service_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"				);
		objConfig.SetAttrTitle( "name",		"Наименование"		);
		objConfig.SetAttrTitle( "type",		"Тип"				);
		objConfig.SetAttrTitle( "country",	"Страна"			);
		objConfig.SetAttrTitle( "city",		"Город"				);
		objConfig.SetAttrTitle( "partner",	"Партнер"			);
		objConfig.SetAttrTitle( "munit",	"Единица измерения"	);
		objConfig.SetAttrTitle( "lperiod",	"Период действия"	);
		objConfig.SetAttrTitle( "table",	"Таблица цен"		);
	} // void GetConfig





	udPBase GetProjectServicePeriod( ) {
		return new udProjectServicePeriod;
	} // udPBase GetProjectServicePeriod

	CProjectServicePeriod::CProjectServicePeriod( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID сущности
		this->get( "service"	)->ini( ( udUInt )	0	);	// ID названия услуги
		this->get( "date1"		)->ini( udDatetime( 0, true ) );	// дата с
		this->get( "date2"		)->ini( udDatetime( 0, true ) );	// дата по
		this->get( "index"		)->ini( ( udUInt )	0	);	// индекс
	}

	CProjectServicePeriod::~CProjectServicePeriod( ) {
	}

	void CProjectServicePeriod::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_serviceperiod" );
		objConfig.SetPrefix( "serviceperiod_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"		);
		objConfig.SetAttrTitle( "service",	"Услуга"	);
		objConfig.SetAttrTitle( "date1",	"Дата с"	);
		objConfig.SetAttrTitle( "date2",	"Дата по"	);
		objConfig.SetAttrTitle( "index",	"Индекс"	);
	} // void GetConfig

} // namespace UdSDK
