
#ifndef UDSDK_WNDFORMLIST_H
#define UDSDK_WNDFORMLIST_H

namespace UdSDK {

	class CWndFormList : public udWindow {
	public:
		// режимы вывода строк
		enum {
			modNormal,	// обычный
			modView,	// есть только ссылка "подробнее" в каждой строке
			modList		// просто список надписей, без ссылок
		};

	private:
		udPFormHandler	m_pHandler;			// обработчик состояния
		udPWndLink		m_wndAdd,			// ссылка добавить
						m_wndFolder;		// ссылка открыть папку
		udPWndRows		m_wndRows;			// окно строк
		udPWndScroller	m_wndScroller;		// окно для прокрутки списка
		udPDwAscBase	m_arrObject;		// набор объектов
		//wstring			m_szHeader;			// надпись
		udArrBase		m_arrToDel;			// набор на удаление
		wstring			m_szFolder;			// папка с файлами

	public:
		CWndFormList( udPFormHandler handler = NULL );
		CWndFormList( const CWndFormList& objCopy );

		virtual ~CWndFormList( );

		CWndFormList& operator =( const CWndFormList& objCopy );

		void	AddAttr( string name );
		udPBase	AddItem( udPBase item );
		bool	DeleteItem( udDWord row );
		void	EditItem( udDWord row );
		udPBase	GetItem( udDWord row );
		void	Clear( );
		void	AddToDel( udPBase obj );

		virtual void	Render( HDC hDC, LPPAINTSTRUCT pPs );
		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );
		virtual LRESULT msgLinkClick( LPARAM lParam );
		virtual LRESULT	msgEraseBackground( HDC hDC );
		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void	SetHandler( udPFormHandler handler ) { m_pHandler = handler; }
		//void	SetHeader( const wstring& header ) { m_szHeader = header; }
		void	SetFolder( const wstring& folder ) { m_szFolder = folder; }

		udPWndRows		GetWndRows( )	const { return m_wndRows;	}
		udPDwAscBase	GetItems( )		const { return m_arrObject;	}
		wstring			GetFolder( )	const { return m_szFolder;	}
		udPWndLink		GetLnkFolder( )	const { return m_wndFolder;	}
		udPWndLink		GetLnkAdd( )	const { return m_wndAdd;	}

		udPArrBase		GetDelItems( )	{ return &m_arrToDel;	}

	}; // class CWndFormList

} // namespace UdSDK

#endif
