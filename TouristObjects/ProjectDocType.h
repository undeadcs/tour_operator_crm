/**
	Tourist application
	Набор классов объектов

	тип документа клиента
*/
#ifndef TOURIST_APPLICATION_OBJECTS_DOCTYPE_H
#define TOURIST_APPLICATION_OBJECTS_DOCTYPE_H

namespace UdSDK {

	class CProjectDocType : public udBase {

	public:
		CProjectDocType( );

		virtual ~CProjectDocType( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectDocType

} // namespace UdSDK

#endif
