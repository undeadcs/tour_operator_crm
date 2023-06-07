
#ifndef WNDWORK_H
#define WNDWORK_H

namespace UdSDK {

	class CWndWork : public udWindow {
		udPWndToolbar	m_pToolbar;
		udPWndMenu		m_pMenu;
		udPWndModel		m_pWndModel;
		udInt			m_iLineHeight,
						m_iColumnWidth,
						m_iWindowHeight,
						m_iWindowWidth;
						

	public:
		CWndWork( HINSTANCE hInstance = NULL );
		CWndWork( const CWndWork& objCopy );

		virtual ~CWndWork( );

		CWndWork& operator =( const CWndWork& objCopy );

		virtual void	Render( HDC hDC, LPPAINTSTRUCT pPs );
		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );
		virtual LRESULT	msgEraseBackground( HDC hDC );
		virtual LRESULT msgHScroll( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgVScroll( WPARAM wParam, LPARAM lParam );
		virtual LRESULT	msgSize( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgMaxMinInfo( LPMINMAXINFO pInfo );
		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgMouseWheel( WPARAM wParam, LPARAM lParam );

		udPWndToolbar	GetToolbar( )	const { return m_pToolbar;	}
		udPWndMenu		GetMenu( )		const { return m_pMenu;		}
		udPWndModel		GetWndModel( )	const { return m_pWndModel;	}

	private:
		void	ResizeScroll( udInt iPrevW, udInt iPrevH );

	}; // class CWndWork

} // namespace UdSDK

#endif
