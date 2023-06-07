
#ifndef WNDSPLASH_H
#define WNDSPLASH_H

namespace UdSDK {

	class CWndSplash : public udWindow {
		HICON	m_hIcon;

	public:
		CWndSplash( HINSTANCE hInstance = NULL );
		CWndSplash( const CWndSplash& objCopy );

		virtual ~CWndSplash( );

		CWndSplash& operator =( const CWndSplash& objCopy );

		void	SetState( const wstring& szTitle, udFloat percent );
		void	SetState( const wstring& szTitle );
		void	SetState( udFloat percent );

		virtual void	Render( HDC hDC, LPPAINTSTRUCT pPs );
		virtual LRESULT msgUpdateProgressBar( );
		virtual LRESULT msgPaint( );
		virtual LRESULT msgGetIcon( WPARAM wParam );
		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void	SetIcon( HICON hIcon ) { m_hIcon = hIcon; }

	}; // class CWndSplash

} // namespace UdSDK

#endif
