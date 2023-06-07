
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectPriceTableItem( ) {
		return new udProjectPriceTableItem;
	} // udPBase GetProjectPriceTableItem

	CProjectPriceTableItem::CProjectPriceTableItem( ) {
		this->get( "id"			)->ini(	( udUInt )	0		);	// ID
		this->get( "service"	)->ini( ( udUInt )	0		);	// ID услуги
		this->get( "period"		)->ini( ( udUInt )	0		);	// Индекс периода
		this->get( "num"		)->ini( ( udUInt )	0		);	// Количество людей
		this->get( "amount"		)->ini( ( udFloat )	0.0f	);	// Сумма
		this->get( "currency"	)->ini( ( udUInt )	0		);	// ID валюты
		this->get( "amount_recommend"	)->ini( ( udFloat )	0.0f	);	// Сумма "рекомендованная"
		this->get( "currency_recommend"	)->ini( ( udUInt )	0		);	// Сумма "рекомендованная"
	}

	CProjectPriceTableItem::~CProjectPriceTableItem( ) {
	}

	void CProjectPriceTableItem::GetConfig( udBaseConfig& objConfig ) {
		udBase::GetConfig( objConfig );
		// общие настройки
		objConfig.SetSQLTable( "ud_pricetableitem" );
		objConfig.SetPrefix( "pricetableitem_" );
		objConfig.SetSelectBy( "id" );
		objConfig.SetUpdateBy( "id" );
		objConfig.SetDeleteBy( "id" );
		// настройки атрибутов
		objConfig.SetAttrType( "id", udBaseConfig::UDSDK_TYPE_INTKEY );
		// заголовки атрибутов
		objConfig.SetAttrTitle( "id",		"ID"				);
		objConfig.SetAttrTitle( "service",	"Услуга"			);
		objConfig.SetAttrTitle( "period",	"Период"			);
		objConfig.SetAttrTitle( "num",		"Количество людей"	);
		objConfig.SetAttrTitle( "amount",	"Сумма"				);
		objConfig.SetAttrTitle( "currency",	"Валюта"			);
	} // void GetConfig

} // namespace UdSDK
