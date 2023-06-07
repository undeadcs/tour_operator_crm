/**
	Tourist application
	Набор классов объектов

	компания
*/
#ifndef TOURIST_APPLICATION_OBJECTS_COMPANY_H
#define TOURIST_APPLICATION_OBJECTS_COMPANY_H

namespace UdSDK {

	class CProjectCompany : public udBase {

	public:
		CProjectCompany( );

		virtual ~CProjectCompany( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectCompany

} // namespace UdSDK

#endif
