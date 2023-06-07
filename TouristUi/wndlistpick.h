/**
	Окно показывает список сущностей, позволяет выбирать их из списка
*/
#ifndef UDSDK_WNDLISTPICK_H
#define UDSDK_WNDLISTPICK_H

namespace UdSDK {

	class CWndListPick : public udWndGraphic {
		enum {
			CONTROL_ID_BUTTON_OK = IDOK,
			CONTROL_ID_BUTTON_CANCEL = IDCANCEL,
			CONTROL_ID_LIST = 1024
		};

		udPStateHandler		m_pHandler;		// обработчик
		string				m_szIndex;		// индекс
		udPPager			m_pPager;		// пейджер списка
		udPWndInstanceList	m_wndInsLst;	// окно списка сущностей
		udPWndPager			m_wndPager;		// окно пейджера
		udPWndButton		m_btnOk,		// кнопка подтверждения
							m_btnCancel;	// кнопка отмена
		udArrBase			m_arrObject;	// массив объектов
		udArrLink			m_arrLink;		// набор ссылок

	public:
		CWndListPick( );
		CWndListPick( const CWndListPick& objCopy );

		virtual ~CWndListPick( );

		CWndListPick& operator =( const CWndListPick& objCopy );

		void	AddLink( const wstring& title, const string& handler, const string& index );
		void	LoadData( );
		void	ClearData( );
		void	ClearSelection( );
		udPBase	GetSelectedObj( );
		void	ClearLinks( );
		void	RemakeWindow( );
		void	CreateLinks( );

		virtual void	Render( );
		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );
		virtual LRESULT msgButtonClick( LPARAM lParam );
		virtual LRESULT msgListRowSelect( LPARAM lParam );
		virtual LRESULT msgListHeaderClick( LPARAM lParam );
		virtual LRESULT msgPagerSelect( LPARAM lParam );
		virtual LRESULT msgLinkClick( LPARAM lParam );
		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void	SetIndex( const string& index )			{ m_szIndex = index;	}
		void	SetHandler( udPStateHandler handler )	{ m_pHandler = handler;	}

		udPWndInstanceList	GetInsLst( )	const { return m_wndInsLst; }
		udPPager			GetPager( )		const { return m_pPager;	}
		udPWndPager			GetWndPager( )	const { return m_wndPager;	}
		string				GetIndex( )		const { return m_szIndex;	}

	private:
		void	MakeWindow( );
		udInt	CalcLinksHeight( );

	}; // class CWndListPick

} // namespace UdSDK

#endif
