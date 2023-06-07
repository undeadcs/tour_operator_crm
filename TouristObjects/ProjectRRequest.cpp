
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectRRequest( ) {
		return new udProjectRRequest;
	} // udPBase GetProjectRRequest

	CProjectRRequest::CProjectRRequest( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);
		this->get( "name"	)->ini( wstring( L"" )	);
	}

	CProjectRRequest::~CProjectRRequest( ) {
	}

	void CProjectRRequest::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_rrequest" );
		objConfig.SetPrefix( "rrequest_" );
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
