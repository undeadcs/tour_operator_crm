
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CModelWork::CModelWork( udPWndWork pWndWork ) {
		m_szState		= "";
		m_pCurHandler	= NULL;
		m_pWndWork		= pWndWork;
		m_pMainHandler	= NULL;
	}

	CModelWork::CModelWork( const CModelWork& objCopy ) {
	}

	CModelWork::~CModelWork( ) {
		if ( !m_arrHandler.empty( ) ) {
			udChar log[ 256 ];
			sprintf_s( log, 256, "Количество зарегистрированных обработчиков %lu\n", m_arrHandler.size( ) );
			g_pApp->GetLog( )->Write( log );

			for( udAscStateHandler::iterator i = m_arrHandler.begin( ); i != m_arrHandler.end( ); ++i ) {
				g_pApp->GetLog( )->Write( string( "Удаляем обработчик состояния '" ) + i->first + string( "'\n" ) );
				delete i->second;
			}
			m_arrHandler.clear( );
		}
		m_szState.clear( );
		m_mapTbState.clear( );
		m_pCurHandler = NULL;
		m_pWndWork = NULL;
		if ( m_pMainHandler ) {
			delete m_pMainHandler;
			m_pMainHandler = NULL;
		}
	}

	CModelWork& CModelWork::operator =( const CModelWork& objCopy ) {
		return *this;
	}

	void CModelWork::Init( ) {
		udPLog __log = g_pApp->GetLog( );
		__log->Write( "CModelWork::Init enter\n" );
		if ( m_pMainHandler ) {
			__log->Write( "CModelWork::Init, m_pMainHandler->Init\n" );
			m_pMainHandler->Init( );
		}
		for( udAscStateHandler::iterator i = m_arrHandler.begin( ); i != m_arrHandler.end( ); ++i ) {
			__log->Write( "CModelWork::Init, handler_name='" + i->first + "'\n" );
			i->second->Init( );
		}
		__log->Write( "CModelWork::Init exit\n" );
	} // void Init

	void CModelWork::InitSync( ) {
		/*udPDatabaseSync sync = g_pApp->GetSync( );
		if ( m_pMainHandler ) {
			m_pMainHandler->InitSync( sync );
		}
		for( udAscStateHandler::iterator i = m_arrHandler.begin( ); i != m_arrHandler.end( ); ++i ) {
			i->second->InitSync( sync );
		}*/
	} // void InitSync

	void CModelWork::SetToolbarSwitcher( string index, string state ) {
		m_mapTbState[ index ] = state;
	} // void SetToolbarSwitcher

	void CModelWork::SwitchToMain( ) {
		if ( m_pCurHandler ) { // смена состояния
			m_pCurHandler->Clear( );
		}
		udPWndModel wnd = m_pWndWork->GetWndModel( );
		m_pWndWork->GetMenu( )->ClearCurrentItem( );
		m_szState = "";
		m_pCurHandler = m_pMainHandler;
		wnd->SetHandler( m_pCurHandler );
		m_pCurHandler->Load( );
		m_pCurHandler->Show( wnd );
		//wnd->Flush( );
		wnd->Update( );
	} // void SwitchToMain

	void CModelWork::SetState( string state ) {
		udPLog __log = g_pApp->GetLog( );
		__log->Write( "CModelWork::SetState enter\nstate='" + state + "'\n" );
		if ( m_pCurHandler ) { // смена состояния
			__log->Write( "CModelWork::SetState, m_pCurHandler Clear\n" );
			m_pCurHandler->Clear( );
		}
		udPWndModel wnd = m_pWndWork->GetWndModel( );
		m_szState = state;
		m_pCurHandler = m_arrHandler[ state ];
		wnd->SetHandler( m_pCurHandler );
		__log->Write( "CModelWork::SetState, m_pCurHandler Load\n" );
		m_pCurHandler->Load( );
		__log->Write( "CModelWork::SetState, m_pCurHandler Show\n" );
		m_pCurHandler->Show( wnd );
		wnd->Update( );
		__log->Write( "CModelWork::SetState exit\n" );
	} // void SetState

	void CModelWork::SetHandler( string state, udPStateHandler pHandler ) {
		udAscStateHandler::iterator i = m_arrHandler.find( state );
		if ( i != m_arrHandler.end( ) ) {
			delete i->second;
		}
		m_arrHandler[ state ] = pHandler;
	} // void SetHandler

	udInt CModelWork::ClickMenu( string index ) {
		if ( m_arrHandler.find( index ) != m_arrHandler.end( ) && ( index != m_szState ) ) {
			this->SetState( index );
			return 1;
		}
		return 0;
	} // udInt ClickMenu

	udInt CModelWork::ClickToolbar( string index ) {
		udAscStr::iterator i = m_mapTbState.find( index );
		if ( i != m_mapTbState.end( ) ) { // переключатель состояния
			string state = i->second;
			udPStateHandler handler = m_arrHandler[ state ];
			if ( handler->ToolbarAction( index ) == 1 ) { // переключение возможно
				m_pCurHandler->Clear( );
				m_szState = state;
				m_pCurHandler = handler;
				m_pWndWork->GetMenu( )->SelectItem( state );
				udPWndModel wnd = m_pWndWork->GetWndModel( );
				wnd->SetHandler( m_pCurHandler );
				m_pCurHandler->ClickToolbar( index );
			}
		} else if ( m_pCurHandler ) { // обычная кнопка
			m_pCurHandler->ClickToolbar( index );
			return 1;
		}
		return 0;
	} // udInt ClickToolbar

	void CModelWork::Show( ) {
		if ( !m_pCurHandler ) {
			m_pCurHandler = m_pMainHandler;
		}
		udPWndModel wnd = m_pWndWork->GetWndModel( );
		wnd->SetHandler( m_pCurHandler );
		m_pCurHandler->Load( );
		m_pCurHandler->Show( wnd );
		//wnd->Flush( );
		m_pWndWork->Show( );
		m_pWndWork->Update( );
	} // void Show

	udPWndModel CModelWork::GetWndModel( ) const {
		return m_pWndWork->GetWndModel( );
	}

	udPStateHandler CModelWork::GetHandler( const string& state ) const {
		udAscStateHandler::const_iterator i = m_arrHandler.find( state );
		if ( i != m_arrHandler.end( ) ) {
			return i->second;
		}
		return NULL;
	} // udPStateHandler GetHandler

} // namespace UdSDK
