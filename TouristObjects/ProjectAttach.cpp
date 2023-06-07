
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectAttach( ) {
		return new udProjectAttach;
	} // udPBase GetProjectAttach

	CProjectAttach::CProjectAttach( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);	// ID
		this->get( "obj"		)->ini( ( udUInt )	0	);	// ID объекта, к которому привязан файл
		this->get( "type"		)->ini( ( udInt )	0	);	// Тип
		this->get( "name"		)->ini( wstring( L"" )	);	// Наименование
		this->get( "path"		)->ini( wstring( L"" )	);	// Путь
		this->get( "tmp_path"	)->ini( wstring( L"" )	);	// временный путь
		this->get( "orig"		)->ini( wstring( L"" )	);	// Оригинальный путь
	}

	CProjectAttach::~CProjectAttach( ) {
	}

	void CProjectAttach::GetConfig( udBaseConfig& config ) {
		udBase::GetConfig( config );
		// общие настройки
		config.SetSQLTable( "ud_attach" );
		config.SetPrefix( "attach_" );
		config.SetSelectBy( "id" );
		config.SetUpdateBy( "id" );
		config.SetDeleteBy( "id" );
		// настройки атрибутов
		config.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		config.SetAttrTitle( "id",			"ID"				);
		config.SetAttrTitle( "type",		"Тип"				);
		config.SetAttrTitle( "name",		"Имя файла"			);
		config.SetAttrTitle( "path",		"Путь"				);
		config.SetAttrTitle( "tmp_path",	"Временный путь"	);
		config.SetAttrTitle( "orig",		"Оригинальный путь"	);
	} // void GetConfig

} // namespace UdSDK
