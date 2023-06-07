
#include "includes.h"

namespace UdSDK {

	CError::CError( ) {
		m_iCode		= 0;
		m_szText	= "";
	}

	CError::CError( const CError& objErr ) {
		m_iCode		= objErr.m_iCode;
		m_szText	= objErr.m_szText;
	}

	CError::CError( udLong iCode ) {
		m_iCode		= iCode;
		m_szText	= "";
	}

	CError::CError( string szText ) {
		m_iCode		= 0;
		m_szText	= szText;
	}

	CError::CError( udLong iCode, const string& szText ) {
		m_iCode		= iCode;
		m_szText	= szText;
	}

	CError::~CError( ) {
		if ( !m_szText.empty( ) ) {
			m_szText.clear( );
		}

		m_iCode		= 0;
		m_szText	= "";
	}

	void WriteErrors( udPLog log, const udArrError& err ) {
		for( udArrError::const_iterator i = err.begin( ); i != err.end( ); ++i ) {
			log->Write( "код: " + toString( i->GetCode( ) ) + ", сообщение: " + i->GetText( ) + "\n" );
		}
	} // void WriteErrors

} // namespace UdSDK
