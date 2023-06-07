/**
	Окно, которое занимается прокруткой своего контента
*/
#ifndef UDSDK_WNDSCROLLER_H
#define UDSDK_WNDSCROLLER_H

namespace UdSDK {

	class CWndScroller : public udWindow {
		udPWndRows	m_wndRows;

	public:
		CWndScroller( );
		CWndScroller( const CWndScroller& objCopy );

		virtual ~CWndScroller( );

		CWndScroller& operator =( const CWndScroller& objCopy );

		virtual void	Render( HDC hDC, LPPAINTSTRUCT pPs );
		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );
		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual LRESULT	msgLButtonDown( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgMouseWheel( WPARAM wParam, LPARAM lParam );
		virtual LRESULT	msgSize( WPARAM wParam, LPARAM lParam );

		virtual LRESULT msgVScroll( WPARAM wParam, LPARAM lParam );

		void	SetChild( udPWndRows wnd ) { m_wndRows = wnd; }

	}; // class CWndScroller

} // namespace UdSDK

#endif
