/**
	UndeadCS SDK
	Библиотека интерфейса пользователя
	
	окно
*/
#ifndef UNDEADCS_SDK_USER_INTERFACE_WINDOW_H
#define UNDEADCS_SDK_USER_INTERFACE_WINDOW_H

namespace UdSDK {

	class CUdWindow : public udMorph {
	protected:
		HWND	m_hWnd,
				m_hWndParent;

	public:
		CUdWindow( HINSTANCE hInstance = NULL );
		CUdWindow( const CUdWindow& objCopy );

		virtual ~CUdWindow( );

		CUdWindow& operator =( const CUdWindow& objCopy );

		virtual void	Render( HDC hDC = NULL, LPPAINTSTRUCT pPs = NULL );
		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );
		virtual void	Show( );
		virtual void	Hide( );
		virtual void	Destroy( );
		virtual void	Update( );
		virtual void	Enable( );
		virtual void	Disable( );
		virtual void	SetAsForeground( );
		virtual void	Move( );
		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual HWND	GetHWnd( ) const;
		virtual HWND	GetHWndParent( ) const;
		// обработчики сообщений
		virtual LRESULT msgMove( WPARAM wParam, LPARAM lParam );
		virtual LRESULT	msgSize( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgEraseBackground( HDC hDC );
		virtual LRESULT msgPaint( );
		virtual LRESULT msgMouseMove( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgLButtonDown( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgSetCursor( WPARAM wParam, LPARAM lParam );

		virtual void	DestroyChildren( );
		virtual void	ShowChildren( );
		virtual void	HideChildren( );
		virtual void	EnableChildren( );
		virtual void	DisableChildren( );
		virtual void	UpdateChildren( );

		static HINSTANCE	GetHInstance( );

	}; // class CUdWindow

} // namespace UdSDK

#endif
