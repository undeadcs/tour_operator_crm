/**
	Кнопка тулбара
*/
#ifndef WNDTOOLBARBTN_H
#define WNDTOOLBARBTN_H

namespace UdSDK {

	class CWndToolbarBtn : public udWndGraphic {
		udPWndToolbar	m_pToolbar;		// тулбар
		udInt			m_iBackX,		// X позиция фона
						m_iBackY;		// Y позиция фона
		Rect			m_rctNormal,	// область спрайта для бэкграунда
						m_rctDisabled;	// область спрайта для бэкграунда
		bool			m_bEnabled,		// активна или нет
						m_bHovered;		// находится ли курсор над кнопкой

	public:
		CWndToolbarBtn( udPWndToolbar toolbar = NULL, bool enabled = true );
		CWndToolbarBtn( const CWndToolbarBtn& objCopy );

		virtual ~CWndToolbarBtn( );

		CWndToolbarBtn& operator =( const CWndToolbarBtn& objCopy );

		void	SetEnabled( bool enabled );

		virtual void	Render( );

		virtual LRESULT msgMouseMove( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgLButtonDown( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgMouseHover( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgMouseLeave( );

		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void	SetBackX( int x ) { m_iBackX = x; }
		void	SetBackY( int y ) { m_iBackY = y; }
		void	SetRectNormal( const Rect& rct ) { m_rctNormal = rct; }
		void	SetRectDisabled( const Rect& rct ) { m_rctDisabled = rct; }

		Rect	GetRectNormal( ) const { return m_rctNormal; }
		Rect	GetRectDisabled( ) const { return m_rctDisabled; }

	private:
		void	Track( udDWord dwFlags );

	}; // class CWndToolbarBtn

} // namespace UdSDK

#endif
