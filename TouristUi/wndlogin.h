/**

Окно формы входа рисует фон и кнопки само
Кнопки не создаются как отдельные окна, а только лишь обрабатывается клик мыши
по области, где находятся эти кнопки

*/

#ifndef WNDLOGIN_H
#define WNDLOGIN_H

namespace UdSDK {

	/**
	 *	Окно входа
	 */
	class CWndLogin : public udWindow {
		HWND	m_hWndLogin,
				m_hWndPassword;

	public:
		CWndLogin( HINSTANCE hInstance = NULL );
		CWndLogin( const CWndLogin& objCopy );

		virtual ~CWndLogin( );

		CWndLogin& operator =( const CWndLogin& objCopy );

		void		SetWindowPathRegion( );
		
		udPMorph	GetData( );

		virtual void	Render( HDC hDC, LPPAINTSTRUCT pPs );
		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );
		virtual LRESULT msgPaint( );
		virtual LRESULT msgLButtonDown( WPARAM wParam, udInt iX, udInt iY );
		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	}; // class CWndLogin

} // namespace UdSDK

#endif
