
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectHostSide( ) {
		return new udProjectHostSide;
	} // udPBase GetProjectHostSide

	CProjectHostSide::CProjectHostSide( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);
		this->get( "name"		)->ini( wstring( L"" )	);
		this->get( "namelatin"	)->ini( wstring( L"" )	);
	}

	CProjectHostSide::~CProjectHostSide( ) {
	}

	void CProjectHostSide::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_hostside" );
		objConfig.SetPrefix( "hostside_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",			"ID"			);
		objConfig.SetAttrTitle( "name",			"Наименование"	);
		objConfig.SetAttrTitle( "namelatin",	"Наименование латиницей"	);
	} // void GetConfig

} // namespace UdSDK
