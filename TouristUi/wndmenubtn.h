/**
	Кнопка в меню
*/
#ifndef WNDMENUBTN_H
#define WNDMENUBTN_H

namespace UdSDK {

	class CWndMenuBtn : public udWndGraphic {
		udPWndMenu	m_pMenu;
		Rect		m_rctNormal,	// обычная
					m_rctSelected,	// выбрана
					m_rctHovered;	// при наведении мыши
		bool		m_bHovered,		// находится курсор над кнопкой
					m_bSelected;	// выбрана ли кнопка
		//udPBitmap	m_imgBuffer;	// картинка

	public:
		CWndMenuBtn( );
		CWndMenuBtn( udPWndMenu menu, const Rect& rctNormal, const Rect& rctSelected, const Rect& rctHovered, bool selected );
		CWndMenuBtn( const CWndMenuBtn& objCopy );

		virtual ~CWndMenuBtn( );

		CWndMenuBtn& operator =( const CWndMenuBtn& objCopy );

		void	SetSelected( bool selected );

		virtual void	Render( );
		//virtual void	Render( HDC hDC, LPPAINTSTRUCT pPs );
		//virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );

		//virtual LRESULT	msgEraseBackground( HDC hDC );
		virtual LRESULT msgMouseMove( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgLButtonDown( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgMouseHover( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgMouseLeave( );
		//virtual LRESULT msgSetCursor( WPARAM wParam, LPARAM lParam );

		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void	SetRectNormal( const Rect& rct ) { m_rctNormal = rct; }
		void	SetRectSelected( const Rect& rct ) { m_rctSelected = rct; }
		void	SetRectHovered( const Rect& rct ) { m_rctHovered = rct; }

		Rect	GetRectNormal( )	const { return m_rctNormal;		}
		Rect	GetRectSelected( )	const { return m_rctSelected;	}
		Rect	GetRectHovered( )	const { return m_rctHovered;	}

	private:
		/*void	Flush( HDC hDC = NULL );
		void	Render( );*/
		void	Track( udDWord dwFlags );

	}; // class CWndMenuBtn

} // namespace UdSDK

#endif
