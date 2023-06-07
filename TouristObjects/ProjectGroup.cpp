
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectGroup( ) {
		return new udProjectGroup;
	} // udPBase GetProjectGroup

	CProjectGroup::CProjectGroup( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID сущности
		this->get( "name"		)->ini( wstring( L"" )	);	// название
		this->get( "comment"	)->ini( wstring( L"" )	);	// комментарий
		this->get( "direct"		)->ini( ( udUInt )	0	);	// ID направления
	}

	CProjectGroup::~CProjectGroup( ) {
	}

	void CProjectGroup::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_group" );
		objConfig.SetPrefix( "group_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY	);
		objConfig.SetAttrType( "comment",	udBaseConfig::UDSDK_TYPE_TEXT	);
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"				);
		objConfig.SetAttrTitle( "name",		"Название"			);
		objConfig.SetAttrTitle( "direct",	"Направление"		);
		objConfig.SetAttrTitle( "comment",	"Комментарий"		);
	} // void GetConfig



	udPBase GetProjectGroupTplTour( ) {
		return new udProjectGroupTplTour;
	} // udPBase GetProjectGroupTplTour

	CProjectGroupTplTour::CProjectGroupTplTour( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID
		this->get( "group"		)->ini( ( udUInt )	0	);	// ID группы
		this->get( "tpltour"	)->ini( ( udUInt )	0	);	// ID шаблона тура
	}

	CProjectGroupTplTour::~CProjectGroupTplTour( ) {
	}

	void CProjectGroupTplTour::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_grouptpltour" );
		config.SetPrefix( "grouptpltour_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",		"ID"			);
		config.SetAttrTitle( "group",	"Группа"		);
		config.SetAttrTitle( "tpltour",	"Шаблон тура"	);
	} // void GetConfig




	udPBase GetProjectGroupTour( ) {
		return new udProjectGroupTour;
	}

	CProjectGroupTour::CProjectGroupTour( ) {
		this->get( "id"		)->ini( ( udUInt )	0	);	// ID
		this->get( "group"	)->ini( ( udUInt )	0	);	// ID группы
		this->get( "tour"	)->ini( ( udUInt )	0	);	// ID тура
		this->get( "period"	)->ini( ( udUInt )	0	);	// индекс периода
	}

	CProjectGroupTour::~CProjectGroupTour( ) {
	}

	void CProjectGroupTour::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_grouptour" );
		config.SetPrefix( "grouptour_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",		"ID"			);
		config.SetAttrTitle( "group",	"Группа"		);
		config.SetAttrTitle( "tour",	"Тур"			);
		config.SetAttrTitle( "period",	"Период"		);
	} // void GetConfig





	udPBase GetProjectGroupPeriod( ) {
		return new udProjectGroupPeriod;
	}

	CProjectGroupPeriod::CProjectGroupPeriod( ) {
		this->get( "id"		)->ini( ( udUInt )	0	);	// ID
		this->get( "group"	)->ini( ( udUInt )	0	);	// ID группы
		this->get( "date1"	)->ini( udDatetime( 0, true )	);	// дата начала
		this->get( "date2"	)->ini( udDatetime( 0, true )	);	// дата окончания
		this->get( "min"	)->ini( ( udUInt )	0	);	// минимум мест
		this->get( "max"	)->ini( ( udUInt )	0	);	// максимум мест
		this->get( "index"	)->ini( ( udUInt )	0	);	// индекс
		this->get( "client"	)->ini( ( udUInt )	0	);	// ID клиента
	}

	CProjectGroupPeriod::~CProjectGroupPeriod( ) {
	}

	void CProjectGroupPeriod::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_groupperiod" );
		config.SetPrefix( "groupperiod_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",		"ID"				);
		config.SetAttrTitle( "group",	"Группа"			);
		config.SetAttrTitle( "date1",	"Дата начала"		);
		config.SetAttrTitle( "date2",	"Дата окончания"	);
		config.SetAttrTitle( "index",	"Индекс"			);
		config.SetAttrTitle( "client",	"Руководитель"		);
	} // void GetConfig

} // namespace UdSDK
