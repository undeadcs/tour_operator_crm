
#ifndef PROJECTCITY_H
#define PROJECTCITY_H

namespace UdSDK {

	udPBase GetProjectCity2( );
	//udPListHandler GetHCityList( );
	//udPFormHandler GetHCityForm( );

	udPArrBase	LoadCitiesById( const map< udUInt, udUInt >& ids );

	class CHCity : public udStateHandlerInfo {
	public:
		enum {
			opidGet2 = opidGet + 1,	// выборка городов по набору ID
		};

		enum {
			ctrlidName,
			ctrlidCountry
		};

		enum {
			fltName,
			fltCountry,

			LIST_CONTROL_ID_LAST
		};

	private:
		udPWndLink	m_lnkPrint;

	public:
		CHCity( );
		CHCity( const CHCity& objCopy );

		virtual ~CHCity( );

		CHCity& operator =( const CHCity& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual void	AddExtraAttributes( udPBase tpl );

	}; // class CHCity

} // namespace UdSDK

#endif
