/**
	Tourist application
	Серверное приложение
	Период группы
*/
#ifndef TOURIST_SERVER_GROUP_PERIOD_H
#define TOURIST_SERVER_GROUP_PERIOD_H

namespace UdSDK {

	udPBase GetProjectGroupPeriodList( );

	class CHGroupPeriod : public udStateHandlerSimple {

	public:
		CHGroupPeriod( );
		CHGroupPeriod( const CHGroupPeriod& objCopy );

		virtual ~CHGroupPeriod( );

		CHGroupPeriod& operator =( const CHGroupPeriod& objCopy );

		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );

	}; // class CHGroupPeriod

} // namespace UdSDK

#endif
