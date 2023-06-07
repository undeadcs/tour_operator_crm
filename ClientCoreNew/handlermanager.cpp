
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHandlerManager::CHandlerManager( ) :
		m_pDefaultHandler( NULL ),
		m_pCurrentHandler( NULL )
	{
	} // CHandlerManager

	CHandlerManager::CHandlerManager( const CHandlerManager& objCopy ) {
	} // CHandlerManager

	CHandlerManager::~CHandlerManager( ) {
		// TODO прикрутить маркер удаленных адресов
		for( t_ascStateHandler::iterator i = m_ascHandler.begin( ); i != m_ascHandler.end( ); ++i ) {
			delete i->second;
		}
	} // ~CHandlerManager

	CHandlerManager& CHandlerManager::operator =( const CHandlerManager& objCopy ) {
		return *this;
	} // CHandlerManager& operator =

	void CHandlerManager::Init( ) {
		for( t_ascStateHandler::iterator i = m_ascHandler.begin( ); i != m_ascHandler.end( ); ++i ) {
			i->second->Init( );
		}
	} // void Init

	void CHandlerManager::SetState( const string& szName ) {
		udPLog __log = g_pApp->GetLog( );
		__log->Write( "CHandlerManager::SetState\n" );

		t_ascStateHandler::iterator i = m_ascHandler.find( szName );
		if ( i == m_ascHandler.end( ) ) {
			__log->Write( "CHandlerManager::SetState, handler named '" + szName + "' not found\n" );
			return;
		}
		
		if ( m_pCurrentHandler ) {
			__log->Write( "CHandlerManager::SetState, current handler name is \"" + m_pCurrentHandler->GetName( ) + "\"\n" );
			__log->Write( "CHandlerManager::SetState, current handler Clear\n" );
			m_pCurrentHandler->Clear( );
			__log->Write( "CHandlerManager::SetState, current handler Hide\n" );
			m_pCurrentHandler->Hide( );
		}
		__log->Write( "CHandlerManager::SetState, switch to handler name \"" + szName + "\"\n" );
		m_pCurrentHandler = i->second;
		__log->Write( "CHandlerManager::SetState, new handler Load\n" );
		m_pCurrentHandler->Load( );
		__log->Write( "CHandlerManager::SetState, new handler Show\n" );
		m_pCurrentHandler->Show( );
		__log->Write( "CHandlerManager::SetState exit\n" );
	} // void SetState

	void CHandlerManager::SetHandler( const string& szName, udPStateHandler pHandler ) {
		t_ascStateHandler::iterator i = m_ascHandler.find( szName );
		if ( i != m_ascHandler.end( ) ) {
			g_pApp->GetLog( )->Write( "handler named '" + szName + "' already exists\n" );
			return;
		}
		m_ascHandler[ szName ] = pHandler;
	} // void SetHandler

	void CHandlerManager::SetDefaultHandler( const string& szName ) {
		t_ascStateHandler::iterator i = m_ascHandler.find( szName );
		if ( i != m_ascHandler.end( ) ) {
			m_pDefaultHandler = i->second;
		}
	} // void SetDefaultHandler

	void CHandlerManager::SwitchToDefault( ) {
		if ( !m_pDefaultHandler ) {
			return;
		}
		if ( m_pCurrentHandler ) {
			m_pCurrentHandler->Clear( );
			m_pCurrentHandler->Hide( );
		}
		m_pCurrentHandler = m_pDefaultHandler;
		m_pCurrentHandler->Load( );
		m_pCurrentHandler->Show( );
	} // void SwitchToDefault

	udPStateHandler CHandlerManager::GetHandler( const string& szName ) const {
		t_ascStateHandler::const_iterator i = m_ascHandler.find( szName );
		if ( i != m_ascHandler.end( ) ) {
			return i->second;
		}
		return NULL;
	} // udPStateHandler GetHandler

} // namespace UdSDK
