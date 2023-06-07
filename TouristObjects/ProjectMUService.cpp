
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectMUService( ) {
		return new udProjectMUService;
	} // udPBase GetProjectMUService

	CProjectMUService::CProjectMUService( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);
		this->get( "name"	)->ini( wstring( L"" )	);
	}

	CProjectMUService::~CProjectMUService( ) {
	}

	void CProjectMUService::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_muservice" );
		objConfig.SetPrefix( "muservice_" );
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
