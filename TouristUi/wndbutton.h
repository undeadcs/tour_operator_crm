
#ifndef WNDBUTTON_H
#define WNDBUTTON_H

namespace UdSDK {

	class CWndButton : public udWndGraphic {
		udUInt		m_iSize;		// шрифт надписи
		Color		m_clrText,		// цвет текста
					m_clrBack;		// цвет фона
		bool		m_bHovered;		// мышь над ссылкой
		HWND		m_hWndTarget;	// окно, которому отсылается сообщение о нажатии
		Rect		*m_rctNormal;	// область из спрайта, которую использовать как фон
		udInt		m_iPadL,		// отступ текста слева
					m_iPadT;		// отступ текста сверху

	public:
		CWndButton( );
		CWndButton( udUInt size, const Color& clr, const Color& bck );

		virtual ~CWndButton( );

		virtual void	Render( );

		virtual LRESULT msgMouseMove( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgLButtonDown( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgMouseHover( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgMouseLeave( );
		virtual LRESULT msgSetCursor( WPARAM wParam, LPARAM lParam );

		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void	SetPaddingLeft( udInt i ) { m_iPadL = i; }
		void	SetPaddingTop( udInt i ) { m_iPadT = i; }
		void	SetRectNormal( Rect* pRect ) { m_rctNormal = pRect; }
		void	SetFontSize( udUInt size ) { m_iSize = size; }
		void	SetColor( const Color& clr ) { m_clrText = clr; }
		void	SetBackColor( const Color& clr ) { m_clrBack = clr; }
		void	SetTargetWnd( HWND hWnd ) { m_hWndTarget = hWnd; }

	private:
		void	Track( udDWord dwFlags );

	}; // class CWndButton

} // namespace UdSDK

#endif
