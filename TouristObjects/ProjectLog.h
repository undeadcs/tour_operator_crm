/**
	Tourist application
	Набор классов объектов

	лог
*/
#ifndef TOURIST_APPLICATION_OBJECTS_LOG_H
#define TOURIST_APPLICATION_OBJECTS_LOG_H

namespace UdSDK {

	class CProjectLog : public udBase {

	public:
		CProjectLog( );

		virtual ~CProjectLog( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectLog

} // namespace UdSDK

#endif
