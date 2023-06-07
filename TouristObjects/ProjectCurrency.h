/**
	Tourist application
	Набор классов объектов

	валюта
*/
#ifndef TOURIST_APPLICATION_OBJECTS_CURRENCY_H
#define TOURIST_APPLICATION_OBJECTS_CURRENCY_H

namespace UdSDK {

	class CProjectCurrency : public udBase {

	public:
		CProjectCurrency( );

		virtual ~CProjectCurrency( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectCurrency

} // namespace UdSDK

#endif
