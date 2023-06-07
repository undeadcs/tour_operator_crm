
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectCompany( ) {
		return new udProjectCompany;
	} // udPBase GetProjectCompany

	CProjectCompany::CProjectCompany( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);	// ID
		this->get( "name"	)->ini( wstring( L"" )	);	// Название
		this->get( "email"	)->ini( wstring( L"" )	);	// E-mail
		this->get( "inn"	)->ini( wstring( L"" )	);	// ИНН
		this->get( "uaddr"	)->ini( wstring( L"" )	);	// Юридический адрес
		this->get( "paddr"	)->ini( wstring( L"" )	);	// Почтовый адрес
	}

	CProjectCompany::~CProjectCompany( ) {
	}

	void CProjectCompany::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_company" );
		objConfig.SetPrefix( "company_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",	udBaseConfig::UDSDK_TYPE_INTKEY	);
		objConfig.SetAttrType( "uaddr",	udBaseConfig::UDSDK_TYPE_TEXT	);
		objConfig.SetAttrType( "paddr",	udBaseConfig::UDSDK_TYPE_TEXT	);
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"				);
		objConfig.SetAttrTitle( "name",		"Наименование"		);
		objConfig.SetAttrTitle( "email",	"E-mail"			);
		objConfig.SetAttrTitle( "inn",		"ИНН"				);
		objConfig.SetAttrTitle( "uaddr",	"Юридический адрес"	);
		objConfig.SetAttrTitle( "paddr",	"Почтовый адрес"	);
	} // void GetConfig

} // namespace UdSDK
