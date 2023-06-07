
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectCurrencyPair( ) {
		return new udProjectCurrencyPair;
	} // udPBase GetProjectCurrencyPair

	CProjectCurrencyPair::CProjectCurrencyPair( ) {
		this->get( "id"			)->ini(	( udUInt )	0		);	// ID
		this->get( "currency1"	)->ini( ( udUInt )	0		);	// ID ведущей валюты
		this->get( "currency2"	)->ini( ( udUInt )	0		);	// ID ведомой валюты
		this->get( "course"		)->ini( ( udFloat ) 0.0f	);	// Курс
		this->get( "set"		)->ini( udDatetime( 0, true )	);	// Дата установки курса
		this->get( "internal"	)->ini( ( udFloat ) 0.0f	);	// Внутренний курс
	}

	CProjectCurrencyPair::~CProjectCurrencyPair( ) {
	}

	void CProjectCurrencyPair::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_currencypair" );
		objConfig.SetPrefix( "currencypair_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",			"ID"				);
		objConfig.SetAttrTitle( "currency1",	"Ведущая валюта"	);
		objConfig.SetAttrTitle( "currency2",	"Ведомая валюта"	);
		objConfig.SetAttrTitle( "course",		"Курс ЦБ"			);
		objConfig.SetAttrTitle( "set",			"Дата установки"	);
		objConfig.SetAttrTitle( "internal",		"Внутренний курс"	);
	} // void GetConfig

} // namespace UdSDK
