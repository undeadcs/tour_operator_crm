/**
	Tourist application
	Набор классов объектов

	цена услуги
*/
#ifndef TOURIST_APPLICATION_OBJECTS_PRICETABLEITEM_H
#define TOURIST_APPLICATION_OBJECTS_PRICETABLEITEM_H

namespace UdSDK {

	class CProjectPriceTableItem : public udBase {

	public:
		CProjectPriceTableItem( );

		virtual ~CProjectPriceTableItem( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectPriceTableItem

} // namespace UdSDK

#endif
