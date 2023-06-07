/**
	Tourist application
	Библиотека интерфейса пользователя

	окно пейджера
*/
#ifndef TOURIST_APPLICATION_USER_INTERFACE_WNDPAGER_H
#define TOURIST_APPLICATION_USER_INTERFACE_WNDPAGER_H

namespace UdSDK {

	class CWndPager : public udWndGraphic {
	public:
		enum {
			page_min = 1,
			pagesize_min = 1,
			pagelen_min = 10,
		};

	private:
		udDWord			m_dwBackgroundColor;
		udPWndButton	m_wndPrevFrame,
						m_wndNextFrame,
						m_wndPrevPage,
						m_wndNextPage;
		udArrMorph		m_arrWndLink;

	public:
		CWndPager( udDWord dwBackgroundColor = Color::MakeARGB( 255, 255, 255, 255 ) );

		virtual ~CWndPager( );

		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );
		virtual void	Render( );

		virtual LRESULT msgLinkClick( LPARAM lParam );
		virtual LRESULT msgButtonClick( LPARAM lParam );

		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void	Clear( );
		void	Generate( udPPager pager );
		void	PositionControls( );

	}; // class CWndPager

} // namespace UdSDK

#endif
