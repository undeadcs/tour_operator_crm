/**
	Tourist application
	Набор классов объектов

	приложения
*/
#ifndef TOURIST_APPLICATION_OBJECTS_ATTACH_H
#define TOURIST_APPLICATION_OBJECTS_ATTACH_H

namespace UdSDK {

	class CProjectAttach : public udBase {
	public:
		enum {
			ATTACH_ORDER,
			ATTACH_TOUR,
			ATTACH_GROUP,
			ATTACH_CLIENT,
			ATTACH_PARTNER_ANSWER,
			ATTACH_TPLTOUR,
		};

	public:
		CProjectAttach( );

		virtual ~CProjectAttach( );

		virtual void	GetConfig( udBaseConfig& config );

	}; // class CProjectAttach

} // namespace UdSDK

#endif
