
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CListHandler::CListHandler( ) {
		m_wndModeList	= NULL;
		//m_wndModeList	= new udWndModeList( this );
		m_iListState	= ListHandlerStateConstructed;
		m_pPager		= new udPager( 1, 15, 10, 0 );
		m_pLstOpt		= new udMorph;
		m_iOrderType	= ordDesc;
	}

	CListHandler::~CListHandler( ) {
		udDELETE( m_wndModeList );
		udDELETE( m_pPager );
		udDELETE( m_pLstOpt );
		ClearArrBase( m_arrObject );
		m_szOrderBy.clear( );
	}

	udInt CListHandler::CreateList( HWND hParent, bool modal ) {
		if ( !m_wndModeList ) {
			m_wndModeList = new udWndModeList( this );
		}
		udPLog __log = g_pApp->GetLog( );
		//__log->Write( "CListHandler::CreateList enter\n" );
		m_wndModeList->SetModal( modal );
		//__log->Write( "CListHandler::CreateList, GetListModel\n" );
		/*if ( modal ) {
			__log->Write( "CListHandler::CreateList, GetListModel, modal mode\n" );
			m_wndModeList->get( "style" )->ini( ( udDWord ) WS_CHILD  );
			m_wndModeList->get( "x" )->ini( ( udInt ) 0 );
			m_wndModeList->get( "y" )->ini( ( udInt ) 0 );
			m_wndModeList->Create( hParent );
			m_wndModeList->MakeControls( );
		}*/
		GetListModel( );
		m_wndModeList->get( "style" )->ini( ( udDWord ) WS_CHILD  );
		m_wndModeList->get( "x" )->ini( ( udInt ) 0 );
		m_wndModeList->get( "y" )->ini( ( udInt ) 0 );
		if ( modal ) {
		} else {
			udPWndModel wnd = g_pApp->GetModel( )->GetWndModel( );
			m_wndModeList->get( "width" )->ini( wnd->get( "width" )->toInt( ) );
			m_wndModeList->get( "height" )->ini( wnd->get( "height" )->toInt( ) );
		}
		//__log->Write( "CListHandler::CreateList, m_wndModeList->Create\n" );
		if ( !m_wndModeList->Create( hParent ) ) {
			return ListHandlerErrorFailed;
		}
		//__log->Write( "CListHandler::CreateList, GetListView\n" );
		GetListView( );
		m_wndModeList->MakeControls( );
		m_wndModeList->FillControls( );
		m_wndModeList->ShowControls( );
		m_wndModeList->GetList( )->Show( );
		m_wndModeList->GetPager( )->Show( );
		m_iListState = ListHandlerStateCreated;
		//__log->Write( "CListHandler::CreateList exit\n" );
		return ListHandlerErrorOk;
	} // udInt CreateList

	void CListHandler::ClearList( ) {
		if ( !m_wndModeList ) {
			return;
		}
		m_wndModeList->Hide( );
		m_wndModeList->GetList( )->ClearRows( );
		m_wndModeList->GetPager( )->Clear( );
		ClearArrBase( m_arrObject );
		m_pPager->SetPage( 1 );
		m_pPager->SetTotal( 0 );
	} // void ClearList

} // namespace UdSDK
