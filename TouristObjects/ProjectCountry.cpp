
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectCountry( ) {
		return new udProjectCountry;
	} // udPBase GetProjectCountry

	CProjectCountry::CProjectCountry( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);
		this->get( "name"	)->ini( wstring( L"" )	);
	}

	CProjectCountry::~CProjectCountry( ) {
	}

	void CProjectCountry::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_country" );
		objConfig.SetPrefix( "country_" );
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
