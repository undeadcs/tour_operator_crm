/**
	Tourist application
	Набор классов объектов

	валютная пара
*/
#ifndef TOURIST_APPLICATION_OBJECTS_CURRENCYPAIR_H
#define TOURIST_APPLICATION_OBJECTS_CURRENCYPAIR_H

namespace UdSDK {
	
	class CProjectCurrencyPair : public udBase {

	public:
		CProjectCurrencyPair( );

		virtual ~CProjectCurrencyPair( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectCurrencyPair

} // namespace UdSDK

#endif
