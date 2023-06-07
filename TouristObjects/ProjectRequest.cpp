
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectRequest( ) {
		return new udProjectRequest;
	} // udPBase GetProjectRequest

	CProjectRequest::CProjectRequest( ) : udBase( ) {
		this->get( "id"			)->ini( ( udUInt )	0	);	// ID
		this->get( "created"	)->ini( udDatetime( 0, true ) );	// Дата создания
		this->get( "status"		)->ini( ( udUInt )	0	);	// Статус
		this->get( "state"		)->ini( ( udUInt )	0	);	// ID состояния обращения
		this->get( "type"		)->ini( ( udUInt )	0	);	// Тип
		this->get( "fio"		)->ini( wstring( L"" )	);	// ФИО абонента
		this->get( "contact"	)->ini( wstring( L"" )	);	// Контактные данные
		this->get( "direct"		)->ini( ( udUInt )	0	);	// Направление
		this->get( "manager"	)->ini( ( udUInt )	0	);	// Менеджер
		this->get( "manager2"	)->ini( ( udUInt )	0	);	// Ответственный менеджер
		this->get( "result"		)->ini( ( udUInt )	0	);	// Результат
		this->get( "remind"		)->ini( udDatetime( 0, true ) );	// Напомнить
		this->get( "comment"	)->ini( wstring( L"" )	);	// Комментарий
		this->get( "remindc"	)->ini( wstring( L"" )	);	// Сообщение напоминания
	}

	CProjectRequest::~CProjectRequest( ) {
	}

	void CProjectRequest::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_request" );
		objConfig.SetPrefix( "request_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY	);
		objConfig.SetAttrType( "contact",	udBaseConfig::UDSDK_TYPE_TEXT	);
		objConfig.SetAttrType( "comment",	udBaseConfig::UDSDK_TYPE_TEXT	);
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"				);
		objConfig.SetAttrTitle( "created",	"Дата добавления"	);
		objConfig.SetAttrTitle( "type",		"Тип"				);
		objConfig.SetAttrTitle( "fio",		"ФИО абонента"		);
		objConfig.SetAttrTitle( "contact",	"Контактные данные"	);
		objConfig.SetAttrTitle( "direct",	"Направление"		);
		objConfig.SetAttrTitle( "manager",	"Менеджер"			);
		objConfig.SetAttrTitle( "manager2",	"Менеджер2"			);
		objConfig.SetAttrTitle( "result",	"Результат"			);
		objConfig.SetAttrTitle( "remind",	"Напомнить"			);
		objConfig.SetAttrTitle( "comment",	"Комментарий"		);
		objConfig.SetAttrTitle( "status",	"Статус"			);
		objConfig.SetAttrTitle( "state",	"Состояние"			);
		objConfig.SetAttrTitle( "remindc",	"Сообщение"			);
	} // void GetConfig

} // namespace UdSDK
