
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectCity( ) {
		return new udProjectCity;
	} // udPBase GetProjectCity

	CProjectCity::CProjectCity( ) {
		this->get( "id"			)->ini(	( udUInt )	0	);
		this->get( "name"		)->ini( wstring( L"" )	);
		this->get( "country"	)->ini( ( udUInt )	0	);
	}

	CProjectCity::~CProjectCity( ) {
	}

	void CProjectCity::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_city" );
		objConfig.SetPrefix( "city_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id",		udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"			);
		objConfig.SetAttrTitle( "name",		"Наименование"	);
		objConfig.SetAttrTitle( "country",	"Страна"		);
	} // void GetConfig

} // namespace UdSDK
