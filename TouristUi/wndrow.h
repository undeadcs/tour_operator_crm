/**
	Окно строка из списка строк
*/
#ifndef UDSDK_WNDROW_H
#define UDSDK_WNDROW_H

namespace UdSDK {

	class CWndRow : public udWindow {
		udPWndStatic	m_wndStatic;	// окно надписи
		udPWndLink		m_wndEdit,		// ссылка редактирования
						m_wndDelete;	// ссылка удаления

	public:
		CWndRow( wstring title = L"" );
		CWndRow( const CWndRow& objCopy );

		virtual ~CWndRow( );

		CWndRow& operator =( const CWndRow& objCopy );

		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );
		virtual LRESULT	msgEraseBackground( HDC hDC );
		virtual LRESULT msgLinkClick( LPARAM lParam );
		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		udPWndLink		GetLinkEdit( )		const { return m_wndEdit;	}
		udPWndLink		GetLinkDelete( )	const { return m_wndDelete;	}
		udPWndStatic	GetWndStatic( )		const { return m_wndStatic;	}

	}; // class CWndRow

} // namespace UdSDK

#endif
