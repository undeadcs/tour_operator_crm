
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTRequest( ) {
		return new udProjectTRequest;
	} // udPBase GetProjectTRequest

	CProjectTRequest::CProjectTRequest( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);
		this->get( "name"	)->ini( wstring( L"" )	);
	}

	CProjectTRequest::~CProjectTRequest( ) {
	}

	void CProjectTRequest::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_trequest" );
		objConfig.SetPrefix( "trequest_" );
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
