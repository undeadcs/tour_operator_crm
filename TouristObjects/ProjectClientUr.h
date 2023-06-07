/**
	Tourist application
	Набор классов объектов

	клиент юр лицо
*/
#ifndef TOURIST_APPLICATION_OBJECTS_CLIENTUR_H
#define TOURIST_APPLICATION_OBJECTS_CLIENTUR_H

namespace UdSDK {

	class CProjectClientUr : public udBase {

	public:
		CProjectClientUr( );

		virtual ~CProjectClientUr( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectClientUr

} // namespace UdSDK

#endif