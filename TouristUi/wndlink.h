/**
	Окно ссылка
*/
#ifndef UDSDK_WNDLINK_H
#define UDSDK_WNDLINK_H

namespace UdSDK {

	/**
	 *	Окно ссылка
	 */
	class CWndLink : public udWndGraphic {
	public:
		typedef enum calcType {
			WNDLINK_CALCULATE_BOTH,
			WNDLINK_CALCULATE_WIDTH,
			WNDLINK_CALCULATE_HEIGHT
		} calcType;

	private:
		udUInt		m_iSize;		// шрифт надписи
		Color		m_clrText;		// цвет
		bool		m_bHovered;		// мышь над ссылкой
		HWND		m_hWndTarget;	// окно, которому отсылается сообщение о нажатии

	public:
		CWndLink( );
		CWndLink( udUInt size, udDWord color );
		CWndLink( udUInt size );
		CWndLink( udDWord color );
		CWndLink( const CWndLink& objCopy );

		virtual ~CWndLink( );

		CWndLink& operator =( const CWndLink& objCopy );

		virtual void	CalculateWidthHeight( HWND hParent, calcType calc = WNDLINK_CALCULATE_BOTH );
		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );
		virtual void	Render( );

		virtual LRESULT msgMouseMove( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgLButtonDown( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgMouseHover( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgMouseLeave( );
		virtual LRESULT msgSetCursor( WPARAM wParam, LPARAM lParam );

		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void	SetFontSize( udUInt size ) { m_iSize = size; }
		void	SetColor( const Color& clr ) { m_clrText = clr; }
		void	SetTargetWnd( HWND hWnd ) { m_hWndTarget = hWnd; }

	private:
		void	Track( udDWord dwFlags );

	}; // class CWndLink

} // namespace UdSDK

#endif
