
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectDocType( ) {
		return new udProjectDocType;
	} // udPBase GetProjectDocType

	CProjectDocType::CProjectDocType( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);
		this->get( "name"		)->ini( wstring( L"" )	);
	}

	CProjectDocType::~CProjectDocType( ) {
	}

	void CProjectDocType::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_doctype" );
		objConfig.SetPrefix( "doctype_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",	"ID"			);
		objConfig.SetAttrTitle( "name",	"Наименование"	);
	} // void GetConfig

} // namespace UdSDK
