
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectPartner( ) {
		return new udProjectPartner;
	} // udPBase GetProjectPartner

	CProjectPartner::CProjectPartner( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);	// ID
		this->get( "name"		)->ini( wstring( L"" )	);	// Наименование
		this->get( "dfio"		)->ini( wstring( L"" )	);	// ФИО директора
		this->get( "cfio"		)->ini( wstring( L"" )	);	// ФИО контактного лица
		this->get( "email"		)->ini( wstring( L"" )	);	// E-mail
		this->get( "phone"		)->ini( wstring( L"" )	);	// Телефон
		this->get( "fax"		)->ini( wstring( L"" )	);	// Факс
		this->get( "comment"	)->ini( wstring( L"" )	);	// Комментарий
	}

	CProjectPartner::~CProjectPartner( ) {
	}

	void CProjectPartner::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_partner" );
		objConfig.SetPrefix( "partner_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY	);
		objConfig.SetAttrType( "comment",	udBaseConfig::UDSDK_TYPE_TEXT	);
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"					);
		objConfig.SetAttrTitle( "name",		"ФИО директора"			);
		objConfig.SetAttrTitle( "dfio",		"ФИО контактного лица"	);
		objConfig.SetAttrTitle( "cfio",		"E-mail"				);
		objConfig.SetAttrTitle( "email",	"Телефон"				);
		objConfig.SetAttrTitle( "phone",	"Факс"					);
		objConfig.SetAttrTitle( "fax",		"Комментарий"			);
		objConfig.SetAttrTitle( "comment",	"Лицевой счет"			);
	} // void GetConfig

} // namespace UdSDK