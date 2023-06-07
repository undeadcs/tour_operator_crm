
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectPartnerReq( ) {
		return new udProjectPartnerReq;
	} // udPBase GetProjectPartnerReq

	CProjectPartnerReq::CProjectPartnerReq( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);	// ID
		this->get( "partner"	)->ini( ( udUInt )	0	);	// ID партнера
		this->get( "comment"	)->ini( wstring( L"" )	);	// комментарий
		this->get( "manager"	)->ini( ( udUInt )	0	);	// менеджер
		this->get( "created"	)->ini( udDatetime( 0, true ) );	// дата отправки
		this->get( "text"		)->ini( wstring( L"" )	);	// текст
	}

	CProjectPartnerReq::~CProjectPartnerReq( ) {
	}

	void CProjectPartnerReq::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_partnerreq" );
		objConfig.SetPrefix( "partnerreq_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY	);
		objConfig.SetAttrType( "comment",	udBaseConfig::UDSDK_TYPE_TEXT	);
		objConfig.SetAttrType( "text",		udBaseConfig::UDSDK_TYPE_TEXT	);
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"			);
		objConfig.SetAttrTitle( "partner",	"Партнер"		);
		objConfig.SetAttrTitle( "comment",	"Комментарий"	);
		objConfig.SetAttrTitle( "manager",	"Менеджер"		);
		objConfig.SetAttrTitle( "created",	"Дата отправки"	);
		objConfig.SetAttrTitle( "text",		"Текст"			);
	} // void GetConfig

} // namespace UdSDK
