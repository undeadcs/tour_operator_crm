
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTService( ) {
		return new udProjectTService;
	} // udPBase GetProjectTService

	CProjectTService::CProjectTService( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);
		this->get( "name"	)->ini( wstring( L"" )	);
	}

	CProjectTService::~CProjectTService( ) {
	}

	void CProjectTService::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_tservice" );
		objConfig.SetPrefix( "tservice_" );
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
