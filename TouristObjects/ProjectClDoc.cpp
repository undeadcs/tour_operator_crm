
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectClDoc( ) {
		return new udProjectClDoc;
	} // udPBase GetProjectClDoc

	CProjectClDoc::CProjectClDoc( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);	// ID
		this->get( "client"		)->ini( ( udUInt )	0	);	// ID клиента
		this->get( "type"		)->ini( ( udUInt )	0	);	// ID типа документа клиента
		this->get( "series"		)->ini( wstring( L"" )	);	// Серия
		this->get( "number"		)->ini( wstring( L"" )	);	// Номер
		this->get( "issuedate"	)->ini( udDatetime( 0, true )	);	// Дата выдачи
		this->get( "issuedby"	)->ini( wstring( L"" )	);	// Кем выдан
		this->get( "birthplace"	)->ini( wstring( L"" )	);	// Место рождения
		this->get( "enddate"	)->ini( udDatetime( 0, true )	);	// Дата окончания
		this->get( "fio"		)->ini( wstring( L"" )	);	// ФИО
		this->get( "fiolatin"	)->ini( wstring( L"" )	);	// ФИО латиницей
		this->get( "is_default"	)->ini( ( udUInt )	0	);	// документ по умолчанию
	}

	CProjectClDoc::~CProjectClDoc( ) {
	}

	void CProjectClDoc::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_cldoc" );
		objConfig.SetPrefix( "cldoc_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",			"ID"				);
		objConfig.SetAttrTitle( "client",		"Клиент"			);
		objConfig.SetAttrTitle( "type",			"Тип"				);
		objConfig.SetAttrTitle( "series",		"Серия"				);
		objConfig.SetAttrTitle( "number",		"Номер"				);
		objConfig.SetAttrTitle( "issuedate",	"Дата выдачи"		);
		objConfig.SetAttrTitle( "issuedby",		"Кем выдан"			);
		objConfig.SetAttrTitle( "birthplace",	"Место рождения"	);
		objConfig.SetAttrTitle( "enddate",		"Дата окончания"	);
		objConfig.SetAttrTitle( "fio",			"ФИО"				);
		objConfig.SetAttrTitle( "fiolatin",		"ФИО латиницей"		);
	} // void GetConfig

} // namespace UdSDK
