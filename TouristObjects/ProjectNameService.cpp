
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectNameService( ) {
		return new udProjectNameService;
	} // udPBase GetProjectNameService

	CProjectNameService::CProjectNameService( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);
		this->get( "name"	)->ini( wstring( L"" )	);
	}

	CProjectNameService::~CProjectNameService( ) {
	}

	void CProjectNameService::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_nameservice" );
		objConfig.SetPrefix( "nameservice_" );
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