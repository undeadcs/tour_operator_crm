/**
	Tourist application
	Серверная часть

	список туристов
*/
#ifndef TOURIST_APPLICATION_TOURIST_SERVER_TOURIST_LIST_H
#define TOURIST_APPLICATION_TOURIST_SERVER_TOURIST_LIST_H

namespace UdSDK {

	udPBase	GetProjectTouristList2( );

	class CHTouristList : public udStateHandlerSimple {

	public:
		CHTouristList( );
		CHTouristList( const CHTouristList& objCopy );

		virtual ~CHTouristList( );

		CHTouristList& operator =( const CHTouristList& objCopy );

		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );

	}; // class CHTouristList

} // namespace UdSDK

#endif
