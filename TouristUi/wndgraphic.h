
#ifndef WNDGRAPHIC_H
#define WNDGRAPHIC_H

namespace UdSDK {

	class CWndGraphic : public udWindow {
		udPGdi		m_gdiBuffer;	// устройство рисования в буфер
		udPBitmap	m_imgBuffer;	// буфер рисования

	public:
		CWndGraphic( );

		virtual ~CWndGraphic( );

		virtual void	Render( HDC hDC, LPPAINTSTRUCT pPs );
		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );
		virtual void	Render( );
		virtual void	Flush( HDC hDC = NULL );
		virtual void	RemakeBuffer( udInt prev_w, udInt prev_h, udInt now_w, udInt now_h );

		virtual LRESULT	msgEraseBackground( HDC hDC );
		//virtual LRESULT	msgSize( WPARAM wParam, LPARAM lParam );

		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		udPGdi		GetGdi( )		const { return m_gdiBuffer; }
		udPBitmap	GetBitmap( )	const { return m_imgBuffer; }

	}; // class CWndGraphic

} // namespace UdSDK

#endif
