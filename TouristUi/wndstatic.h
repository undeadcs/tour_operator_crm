
#ifndef UDSDK_WNDSTATIC_H
#define UDSDK_WNDSTATIC_H

namespace UdSDK {

	class CWndStatic : public udWndGraphic {
	public:
		typedef enum calcType {
			WNDSTATIC_CALCULATE_BOTH,
			WNDSTATIC_CALCULATE_WIDTH,
			WNDSTATIC_CALCULATE_HEIGHT
		} calcType;

	private:
		udUInt		m_iSize;			// шрифт надписи
		Color		m_clrText;			// цвет

	public:
		CWndStatic( );
		CWndStatic( udUInt size, udDWord color );
		CWndStatic( udUInt size );
		CWndStatic( udDWord color );
		CWndStatic( const CWndStatic& objCopy );

		virtual ~CWndStatic( );

		CWndStatic& operator =( const CWndStatic& objCopy );

		void	CalculateWidthHeight( HWND hParent, calcType calc = WNDSTATIC_CALCULATE_BOTH );

		virtual void	Render( );
		//virtual void	Render( HDC hDC, LPPAINTSTRUCT pPs );
		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );

		//virtual LRESULT	msgEraseBackground( HDC hDC );
		//virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	}; // class CWndStatic

} // namespace UdSDK

#endif
