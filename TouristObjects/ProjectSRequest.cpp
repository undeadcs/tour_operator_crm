
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectSRequest( ) {
		return new udProjectSRequest;
	} // udPBase GetProjectSRequest

	CProjectSRequest::CProjectSRequest( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);
		this->get( "name"	)->ini( wstring( L"" )	);
	}

	CProjectSRequest::~CProjectSRequest( ) {
	}

	void CProjectSRequest::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_srequest" );
		objConfig.SetPrefix( "srequest_" );
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
