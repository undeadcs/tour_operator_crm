/**
	Tourist application
	Набор классов объектов

	пункт перехода
*/
#ifndef TOURIST_APPLICATION_OBJECTS_TRANSFER_POINT_H
#define TOURIST_APPLICATION_OBJECTS_TRANSFER_POINT_H

namespace UdSDK {

	class CProjectTransferPoint : public udBase {

	public:
		CProjectTransferPoint( );

		virtual ~CProjectTransferPoint( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectTransferPoint

} // namespace UdSDK

#endif
