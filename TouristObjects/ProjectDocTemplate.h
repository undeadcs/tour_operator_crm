/**
	Tourist application
	Набор классов объектов

	шаблон документа
*/
#ifndef TOURIST_APPLICATION_OBJECTS_DOC_TEMPLATE_H
#define TOURIST_APPLICATION_OBJECTS_DOC_TEMPLATE_H

namespace UdSDK {

	class CProjectDocTemplate : public udBase {
	public:
		enum {
			tplClassic,		// общий шаблон
			tplContract,	// договор
			tplPermit,		// путевка
			tplContract2,	// групповой договор
		};

	private:

	public:
		CProjectDocTemplate( );

		virtual ~CProjectDocTemplate( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectDocTemplate

} // namespace UdSDK

#endif
