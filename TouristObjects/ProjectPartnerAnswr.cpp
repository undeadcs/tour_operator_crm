
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectPartnerAnswr( ) {
		return new udProjectPartnerAnswr;
	} // udPBase GetProjectPartnerAnswr

	CProjectPartnerAnswr::CProjectPartnerAnswr( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);	// ID
		this->get( "created"	)->ini( udDatetime( 0, true )	);	// Дата создания
		this->get( "partner"	)->ini( ( udUInt )	0	);	// партнер
		this->get( "text"		)->ini( wstring( L"" )	);	// текст
		this->get( "attach"		)->ini( ( udUInt )	0	);	// прикрепленные файлы
		this->get( "user"		)->ini( ( udUInt )	0	);	// ID пользователя, создавшего ответ
		this->get( "preq"		)->ini( ( udUInt )	0	);	// ID запроса
	}

	CProjectPartnerAnswr::~CProjectPartnerAnswr( ) {
	}

	void CProjectPartnerAnswr::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_partneranswr" );
		objConfig.SetPrefix( "partneranswr_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"					);
		objConfig.SetAttrTitle( "created",	"Дата ответа"			);
		objConfig.SetAttrTitle( "partner",	"Партнер"				);
		objConfig.SetAttrTitle( "text",		"Текст"					);
		objConfig.SetAttrTitle( "attach",	"Прикрепленные файлы"	);
		objConfig.SetAttrTitle( "user",		"Пользователь"			);
		objConfig.SetAttrTitle( "preq",		"Запрос партнеру"		);
	} // void GetConfig

} // namespace UdSDK
