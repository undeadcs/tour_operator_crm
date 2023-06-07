
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTransferPoint( ) {
		return new udProjectTransferPoint;
	} // udPBase GetProjectCity

	CProjectTransferPoint::CProjectTransferPoint( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);
		this->get( "name"		)->ini( wstring( L"" )	);
		this->get( "namelatin"	)->ini( wstring( L"" )	);
	}

	CProjectTransferPoint::~CProjectTransferPoint( ) {
	}

	void CProjectTransferPoint::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_transferpoint" );
		objConfig.SetPrefix( "transferpoint_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",			"ID"			);
		objConfig.SetAttrTitle( "name",			"Наименование"	);
		objConfig.SetAttrTitle( "namelatin",	"Наименование латиницей"	);
	} // void GetConfig

} // namespace UdSDK
