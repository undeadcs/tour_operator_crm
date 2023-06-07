
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectClient( ) {
		return new udProjectClient;
	} // udPBase GetProjectClient

	CProjectClient::CProjectClient( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);	// ID
		this->get( "fio"		)->ini( wstring( L"" )	);	// ФИО
		this->get( "fiolatin"	)->ini( wstring( L"" )	);	// ФИО латиницей
		this->get( "born"		)->ini( udDatetime( 0, true ).toWString( "%Y-%m-%d" )	);	// Дата рождения
		this->get( "bornplace"		)->ini( wstring( L"" )	);	// место рождения
		this->get( "bornplacelatin"	)->ini( wstring( L"" )	);	// место рождения латиницей
		this->get( "laddr"		)->ini( wstring( L"" )	);	// список адресов
		this->get( "sex"		)->ini( ( udUInt )	0	);	// Пол
		this->get( "lemail"		)->ini( wstring( L"" )	);	// список e-mail
		this->get( "lphone"		)->ini( wstring( L"" )	);	// список телефонов
		this->get( "laddr"		)->ini( wstring( L"" )	);	// список адресов
		this->get( "doc"		)->ini( ( udUInt ) 0 );		// документ по умолчанию
	}

	CProjectClient::~CProjectClient( ) {
	}

	void CProjectClient::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_client" );
		objConfig.SetPrefix( "client_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"				);
		objConfig.SetAttrTitle( "fio",		"ФИО"				);
		objConfig.SetAttrTitle( "fiolatin",	"ФИО латиницей"		);
		objConfig.SetAttrTitle( "born",		"Дата рождения"		);
		objConfig.SetAttrTitle( "bornplace",		"Место рождения"			);
		objConfig.SetAttrTitle( "bornplacelatin",	"Место рождения латиницей"	);
		objConfig.SetAttrTitle( "sex",		"Пол"				);
		objConfig.SetAttrTitle( "ldocs",	"Список документов"	);
		objConfig.SetAttrTitle( "app",		"Приложения"		);
		//objConfig.SetAttrTitle( "persacc",	"Лицевой счет"		);
		objConfig.SetAttrTitle( "lemail",	"Список e-mail"	);
		objConfig.SetAttrTitle( "laddr",	"Список адресов"	);
		objConfig.SetAttrTitle( "lphone",	"Список телефонов"	);
	} // void GetConfig

} // namespace UdSDK
