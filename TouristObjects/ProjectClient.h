/**
	Tourist application
	Набор классов объектов

	клиент физ лицо
*/
#ifndef TOURIST_APPLICATION_OBJECTS_CLIENT_H
#define TOURIST_APPLICATION_OBJECTS_CLIENT_H

namespace UdSDK {

	class CProjectClient : public udBase {
	public:
		enum {
			SEX_WHOIS,	// хз, мутант!
			SEX_MAN,
			SEX_WOMAN
		};

	public:
		CProjectClient( );

		virtual ~CProjectClient( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectClient

} // namespace UdSDK

#endif
