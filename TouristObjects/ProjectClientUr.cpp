
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectClientUr( ) {
		return new udProjectClientUr;
	} // udPBase GetProjectClientUr

	CProjectClientUr::CProjectClientUr( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);	// ID
		this->get( "name"		)->ini( wstring( L"" )	);	// Наименование
		this->get( "dfio"		)->ini( wstring( L"" )	);	// ФИО руководителя
		this->get( "cfio"		)->ini( wstring( L"" )	);	// ФИО контактного лица
		this->get( "lemail"		)->ini( wstring( L"" )	);	// список e-mail
		this->get( "lphone"		)->ini( wstring( L"" )	);	// список телефонов
		this->get( "lfax"		)->ini( wstring( L"" )	);	// список факсов
		this->get( "laddr"		)->ini( wstring( L"" )	);	// список адресов
		this->get( "comment"	)->ini( wstring( L"" )	);	// комментарий
	}

	CProjectClientUr::~CProjectClientUr( ) {
	}

	void CProjectClientUr::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_clientur" );
		objConfig.SetPrefix( "clientur_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY	);
		objConfig.SetAttrType( "comment",	udBaseConfig::UDSDK_TYPE_TEXT	);
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"					);
		objConfig.SetAttrTitle( "name",		"Наименование"			);
		objConfig.SetAttrTitle( "dfio",		"ФИО руководителя"		);
		objConfig.SetAttrTitle( "cfio",		"ФИО контактного лица"	);
		objConfig.SetAttrTitle( "lemail",	"Список E-mail"			);
		objConfig.SetAttrTitle( "lphone",	"Список телефонов"		);
		objConfig.SetAttrTitle( "lfax",		"Список факсов"			);
		objConfig.SetAttrTitle( "laddr",	"Список адресов"		);
		objConfig.SetAttrTitle( "comment",	"Комментарий"			);
		objConfig.SetAttrTitle( "persacc",	"Лицевой счет"			);
	} // void GetConfig

} // namepsace UdSDK
