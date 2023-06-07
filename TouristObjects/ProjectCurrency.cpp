
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectCurrency( ) {
		return new udProjectCurrency;
	} // udPBase GetProjectCurrency

	CProjectCurrency::CProjectCurrency( ) {
		this->get( "id"		)->ini(	( udUInt )	0	);
		this->get( "name"	)->ini( wstring( L"" )	);
		this->get( "code"	)->ini( wstring( L"" )	);
	}

	CProjectCurrency::~CProjectCurrency( ) {
	}

	void CProjectCurrency::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_currency" );
		objConfig.SetPrefix( "currency_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",	"ID"			);
		objConfig.SetAttrTitle( "name",	"Наименование"	);
		objConfig.SetAttrTitle( "code",	"Код"			);
	} // void GetConfig

} // namespace UdSDK
