/**
	Tourist application
	Клиентская часть

	списки туристов
*/
#ifndef TOURIST_APPLICATION_TOURIST_CLIENT_TOURIST_LIST_H
#define TOURIST_APPLICATION_TOURIST_CLIENT_TOURIST_LIST_H

namespace UdSDK {

	class CHTouristList : public udStateHandlerInfo {
	public:
		enum {
			ctrlidDate1,	// дата выезда
			ctrlidDate2,	// дата въезда
			ctrlidExit,		// пункт выезда
			ctrlidEnter,	// пункт въезда
			ctrlidHost,		// принимающая сторона
			ctrlidRoute,	// маршрут
			ctrlidLeader,	// руководитель группы
			ctrlidClient	// клиенты
		};

		enum {
			fltDate1,
			fltDate2,
			fltLeader,

			LIST_CONTROL_ID_LAST
		};

	private:
		udPWndLink	m_lnkPrint;

	public:
		CHTouristList( );
		CHTouristList( const CHTouristList& objCopy );

		virtual ~CHTouristList( );

		CHTouristList& operator =( const CHTouristList& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	LinkClick( udPWndLink lnk );
		virtual void	RequestList( );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual void	FormListOnAdd( udPWndFormList wnd );
		virtual void	FormListOnEdit( udPWndFormList wnd, udPBase obj );
		virtual bool	FormListOnDelete( udPWndFormList wnd, udPBase obj );
		virtual void	AddExtraAttributes( udPBase tpl );

	}; // class CHTouristList

} // namespace UdSDK

#endif
