
#ifndef PROJECT_INFOHANDLER_H
#define PROJECT_INFOHANDLER_H

namespace UdSDK {

	/**
	 *	Страница справочников
	 */
	class CHInfoPage : public udStateHandler {
		//udArrMorph	m_arrLink;			// набор ссылок
		typedef map< udInt, udPWndLink > _int_asc_lnk;

		_int_asc_lnk	m_arrLink;
		udPWndGraphic	m_wndModel;

	public:
		CHInfoPage( );
		CHInfoPage( const CHInfoPage& objCopy );

		virtual ~CHInfoPage( );

		CHInfoPage& operator =( const CHInfoPage& objCopy );

		virtual void	Show( udPWndModel wnd );
		virtual void	Init( );
		virtual void	InitSync( udPDatabaseSync sync );
		virtual void	Clear( );
		virtual void	Load( );
		virtual void	ClickToolbar( string index );
		virtual void	ClickModel( udInt x, udInt y, WPARAM wParam );
		virtual void	MouseMove( udInt x, udInt y, WPARAM wParam );
		virtual udInt	ToolbarAction( string index );
		virtual void	LinkClick( udPWndLink link );

	}; // class CHInfoPage

} // namespace UdSDK

#endif
