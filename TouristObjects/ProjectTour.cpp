
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTour( ) {
		return new udProjectTour;
	} // udPBase GetProjectTour

	CProjectTour::CProjectTour( ) {
		this->get( "id"			)->ini( ( udUInt )	0		);	// ID сущности
		this->get( "created"	)->ini( udDatetime( 0, true ) );	// дата создания
		this->get( "name"		)->ini( wstring( L"" )		);	// название
		this->get( "date1"		)->ini( udDatetime( 0, true ) );	// дата начала
		this->get( "date2"		)->ini( udDatetime( 0, true ) );	// дата завершения
		this->get( "cost"		)->ini( ( udFloat )	0.0f	);	// себестоимость
		this->get( "price"		)->ini( ( udFloat )	0.0f	);	// итоговая цена
		this->get( "company"	)->ini( ( udUInt )	0		);	// компания продавец
		this->get( "status"		)->ini( ( udUInt )	0		);	// ID состояния тура
		this->get( "employer"	)->ini( ( udUInt )	0		);	// ID заказчика
		this->get( "emptype"	)->ini( ( udUInt )	0		);	// тип заказчика
		this->get( "document"	)->ini( ( udUInt )	0		);	// ID документа заказчика по умолчанию
		this->get( "manager"	)->ini( ( udUInt )	0		);	// ID пользователя, создавшего тур
		this->get( "manager2"	)->ini( ( udUInt )	0		);	// ID пользователя, ответственного за тур
		this->get( "direct"		)->ini( ( udUInt )	0		);	// ID направления
		this->get( "comment"	)->ini( wstring( L"" )		);	// комментарий
		this->get( "state"		)->ini( ( udUInt )	0		);	// состояние
	}

	CProjectTour::~CProjectTour( ) {
	}

	void CProjectTour::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_tour" );
		objConfig.SetPrefix( "tour_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY	);
		objConfig.SetAttrType( "comment",	udBaseConfig::UDSDK_TYPE_TEXT	);
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"						);
		objConfig.SetAttrTitle( "created",	"Дата создания"				);
		objConfig.SetAttrTitle( "name",		"Название"					);
		objConfig.SetAttrTitle( "date1",	"Дата начала"				);
		objConfig.SetAttrTitle( "date2",	"Дата завершения"			);
		objConfig.SetAttrTitle( "cost",		"Себестоимость"				);
		objConfig.SetAttrTitle( "price",	"Итоговая цена"				);
		objConfig.SetAttrTitle( "company",	"Компания продавец"			);
		objConfig.SetAttrTitle( "status",	"Статус"					);
		objConfig.SetAttrTitle( "employer",	"Заказчик"					);
		objConfig.SetAttrTitle( "manager",	"Менеджер"					);
		objConfig.SetAttrTitle( "direct",	"Направление"				);
		objConfig.SetAttrTitle( "comment",	"Комментарий"				);
	} // void GetConfig




	udPBase GetProjectTourService( ) {
		return new udProjectTourService;
	}

	CProjectTourService::CProjectTourService( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID
		this->get( "tour"		)->ini( ( udUInt )	0	);	// ID тура
		this->get( "service"	)->ini( ( udUInt )	0	);	// ID услуги
		this->get( "num"		)->ini( ( udUInt )	0	);	// количество
		this->get( "date1"		)->ini( udDatetime( 0, true ) );	// дата с
		this->get( "date2"		)->ini( udDatetime( 0, true ) );	// дата по
		this->get( "ready"		)->ini( ( udUInt )	0	);	// готова/не готова
	}

	CProjectTourService::~CProjectTourService( ) {
	}

	void CProjectTourService::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_tourservice" );
		config.SetPrefix( "tourservice_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",		"ID"			);
		config.SetAttrTitle( "tour",	"Тур"			);
		config.SetAttrTitle( "service",	"Услуга"		);
		config.SetAttrTitle( "num",		"Количество"	);
		config.SetAttrTitle( "date1",	"Дата \"с\""	);
		config.SetAttrTitle( "date2",	"Дата \"по\""	);
		config.SetAttrTitle( "ready",	"Готовность"	);
	} // void GetConfig




	udPBase GetProjectTourClient( ) {
		return new udProjectTourClient;
	}

	CProjectTourClient::CProjectTourClient( ) {
		this->get( "id"		)->ini( ( udUInt )	0	);	// ID
		this->get( "tour"	)->ini( ( udUInt )	0	);	// ID тура
		this->get( "client"	)->ini( ( udUInt )	0	);	// ID клиента
	}

	CProjectTourClient::~CProjectTourClient( ) {
	}

	void CProjectTourClient::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_tourclient" );
		config.SetPrefix( "tourclient_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",		"ID"		);
		config.SetAttrTitle( "tour",	"Тур"		);
		config.SetAttrTitle( "client",	"Клиент"	);
	} // void GetConfig




	udPBase GetProjectTourTour( ) {
		return new udProjectTourTour;
	}

	CProjectTourTour::CProjectTourTour( ) {
		this->get( "id"		)->ini( ( udUInt )	0	);	// ID
		this->get( "tour1"	)->ini( ( udUInt )	0	);	// ID тура слева (у которого связь)
		this->get( "tour2"	)->ini( ( udUInt )	0	);	// ID тура справа (с которым связь)
	}

	CProjectTourTour::~CProjectTourTour( ) {
	}

	void CProjectTourTour::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_tourtour" );
		config.SetPrefix( "tourtour_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",		"ID"	);
		config.SetAttrTitle( "tour1",	"Тур1"	);
		config.SetAttrTitle( "tour2",	"Тур2"	);
	} // void GetConfig




	udPBase GetProjectTourPrice( ) {
		return new udProjectTourPrice;
	} // udPBase GetProjectTourPrice

	CProjectTourPrice::CProjectTourPrice( ) {
		this->get( "id"		)->ini( ( udUInt )	0	);	// ID
		this->get( "amount"	)->ini( ( udUInt )	0	);	// сумма
	}

	CProjectTourPrice::~CProjectTourPrice( ) {
	}

	void CProjectTourPrice::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_tourprice" );
		config.SetPrefix( "tourprice_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",		"ID"	);
		config.SetAttrTitle( "amount",	"Сумма"	);
	} // void GetConfig

} // namespace UdSDK
