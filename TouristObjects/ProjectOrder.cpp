
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectOrder( ) {
		return new udProjectOrder;
	} // udPBase GetProjectOrder

	CProjectOrder::CProjectOrder( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID сущности
		this->get( "created"	)->ini( udDatetime( 0, true ) );	// дата создания
		this->get( "manager"	)->ini( ( udUInt )	0	);	// ID менеджера (прием.)
		this->get( "manager2"	)->ini( ( udUInt )	0	);	// ID менеджера (отв.)
		this->get( "status"		)->ini( ( udUInt )	0	);	// Статус заявки
		this->get( "state"		)->ini( ( udUInt )	0	);	// ID состояния заявки
		this->get( "amount"		)->ini( ( udUInt )	0	);	// количество человек
		this->get( "employer"	)->ini( ( udUInt )	0	);	// ID заказчика
		this->get( "emptype"	)->ini( ( udInt )	0	);	// тип заказчика
		this->get( "document"	)->ini( ( udUInt )	0	);	// ID документа заказчика по умолчанию
		this->get( "date1"		)->ini( udDatetime( 0, true ) );	// желаемая дата отъезда
		this->get( "date2"		)->ini( udDatetime( 0, true ) );	// желаемая дата приезда
		this->get( "direct"		)->ini( ( udUInt )	0	);	// направление
		this->get( "comment"	)->ini( wstring( L"" )	);	// комментарий
	}

	CProjectOrder::~CProjectOrder( ) {
	}

	void CProjectOrder::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_order" );
		objConfig.SetPrefix( "order_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY	);
		objConfig.SetAttrType( "comment",	udBaseConfig::UDSDK_TYPE_TEXT	);
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"						);
		objConfig.SetAttrTitle( "created",	"Дата добавления"			);
		objConfig.SetAttrTitle( "status",	"Статус"					);
		objConfig.SetAttrTitle( "manager",	"Менеджер-приемник"			);
		objConfig.SetAttrTitle( "manager2",	"Ответственный менеджер"	);
		objConfig.SetAttrTitle( "amount",	"Количество человек"		);
		objConfig.SetAttrTitle( "employer",	"Заказчик"					);
		objConfig.SetAttrTitle( "date1",	"Дата отъезда"				);
		objConfig.SetAttrTitle( "date2",	"Дата приезда"				);
		objConfig.SetAttrTitle( "direct",	"Направление"				);
		objConfig.SetAttrTitle( "comment",	"Комментарий"				);
		objConfig.SetAttrTitle( "state",	"Состояние"					);
		objConfig.SetAttrTitle( "emptype",	"Тип закказчика"			);
	} // void GetConfig




	udPBase GetProjectOrderService( ) {
		return new udProjectOrderService;
	}

	CProjectOrderService::CProjectOrderService( ) {
		this->get( "id" )->ini( ( udUInt ) 0 );
		this->get( "order" )->ini( ( udUInt ) 0 );
		this->get( "date1" )->ini( udDatetime( 0, true ) );
		this->get( "date2" )->ini( udDatetime( 0, true ) );
		this->get( "type" )->ini( ( udUInt ) 0 );
		this->get( "city" )->ini( ( udUInt ) 0 );
		this->get( "comment" )->ini( wstring( L"" ) );
	}

	CProjectOrderService::~CProjectOrderService( ) {
	}

	void CProjectOrderService::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_orderservice" );
		config.SetPrefix( "orderservice_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",		"ID"			);
		config.SetAttrTitle( "order",	"Заявка"		);
		config.SetAttrTitle( "date1",	"Дата с"		);
		config.SetAttrTitle( "date2",	"Дата по"		);
		config.SetAttrTitle( "type",	"Тип"			);
		config.SetAttrTitle( "city",	"Город"			);
		config.SetAttrTitle( "comment",	"Комментарий"	);
	} // void GetConfig

} // namespace UdSDK
