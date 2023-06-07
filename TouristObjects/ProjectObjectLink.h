/**
	Tourist application
	Набор классов объектов

	связь объектов
*/
#ifndef TOURIST_APPLICATION_OBJECTS_OBJECT_LINK_H
#define TOURIST_APPLICATION_OBJECTS_OBJECT_LINK_H

namespace UdSDK {

	class CProjectObjectLink : public udBase {
	public:
		enum {
			objlnkRequest,			// обращение
			objlnkOrder,			// заявка
			objlnkTour,				// тур
			objlnkTplTour,			// шаблон тура
			objlnkPartnerReq,		// запрос партнеру
		};

	public:
		CProjectObjectLink( );

		virtual ~CProjectObjectLink( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectObjectLink

} // namespace UdSDK

#endif
