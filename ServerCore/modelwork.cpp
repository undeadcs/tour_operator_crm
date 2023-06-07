
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CModelWork::CModelWork( ) {
		m_pLockHandler	= NULL;
	}

	CModelWork::CModelWork( const CModelWork& objCopy ) {
	}

	CModelWork::~CModelWork( ) {
		udDELETE( m_pLockHandler );
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
	}

	CModelWork& CModelWork::operator =( const CModelWork& objCopy ) {
		return *this;
	}

	void CModelWork::Init( ) {
		udPLog __log = g_pApp->GetLog( );
		for( udAscStateHandler::iterator i = m_arrHandler.begin( ); i != m_arrHandler.end( ); ++i ) {
			__log->Write( "Init, '" + i->first + "'\n" );
			i->second->Init( );
		}
	} // void Init

	void CModelWork::InitSync( ) {
	} // void InitSync

	void CModelWork::SetHandler( string state, udPStateHandler pHandler ) {
		udAscStateHandler::iterator i = m_arrHandler.find( state );
		if ( i != m_arrHandler.end( ) ) {
			delete i->second;
		}
		m_arrHandler[ state ] = pHandler;
	} // void SetHandler

	udPStateHandler CModelWork::GetHandler( const string& state ) const {
		udAscStateHandler::const_iterator i = m_arrHandler.find( state );
		if ( i != m_arrHandler.end( ) ) {
			return i->second;
		}
		return NULL;
	} // udPStateHandler GetHandler

} // namespace UdSDK
