/**
	Tourist application
	Клиентское приложение

	обработчик списка
*/
#ifndef TOURIST_APPLICATION_CLIENT_CORE_LIST_HANLDER_H
#define TOURIST_APPLICATION_CLIENT_CORE_LIST_HANLDER_H

namespace UdSDK {

	class CListHandler {
	public:
		enum ListHandlerError {
			ListHandlerErrorOk,		//
			ListHandlerErrorFailed,	// просто фиктивная ошибка
		};

		enum ListHandlerState {
			ListHandlerStateConstructed,
			ListHandlerStateCreated,
		};

	protected:
		udInt			m_iListState;	// состояние списка
		udPWndModeList	m_wndModeList;	// окно со списком
		udArrBase		m_arrObject;	// набор объектов
		udPPager		m_pPager;		// пейджер
		string			m_szOrderBy;	// атрибут упорядочивания
		udInt			m_iOrderType;	// тип упорядочивания
		udPMorph		m_pLstOpt;		// опции списка

	public:
		CListHandler( );

		virtual ~CListHandler( );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual udInt	CreateList( HWND hParent, bool modal = false );
		virtual void	ClearList( );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void	LoadList( ) = 0;
		virtual udInt	GetListModel( ) = 0;
		virtual udInt	GetListView( ) = 0;
		virtual udInt	ListOk( ) = 0;
		virtual udInt	ListCancel( ) = 0;
		virtual void	OnLinkClick( udPWndLink lnk ) = 0;
		virtual void	OnSelectRow( udLong row ) = 0;
		virtual void	OnHeaderClick( const string& name, const string& attr ) = 0;
		virtual void	OnPagerSelect( udUInt page ) = 0;
		virtual void	OnButtonClick( udPWndButton btn ) = 0;
		virtual void	OnFilter( ) { }
		virtual void	OnFilterReset( ) { }
		virtual void	ClearFilter( ) { }
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void	SetOrderBy( const string& orderby ) { m_szOrderBy = orderby; }
		void	SetOrderType( udInt ordertype ) { m_iOrderType = ordertype; }
		////////////////////////////////////////////////////////////////////////////////////////////////////
		udPWndModeList		GetWndList( )	const { return m_wndModeList;	}
		const udArrBase*	GetItems( )		const { return &m_arrObject;	}
		udPPager			GetPager( )		const { return m_pPager;		}
		udPMorph			GetLstOpt( )	const { return m_pLstOpt;		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
	}; // class CListHandler

} // namespace UdSDK

#endif
