
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CDAList::CDAList( ) :
		udDataAccess( ),
		m_szName( "" ),
		m_fnGetObject( NULL ),
		m_szOrderBy( "id" ),
		m_iOrderType( ordDesc ),
		m_pPager( new udPager( 1, 15, 10, 0 ) ),
		m_pFilter( new udMorph ),
		m_arrObject( new udArrBase )
	{
	} // CDAList

	CDAList::CDAList( const CDAList& objCopy ) {
	} // CDAList

	CDAList::~CDAList( ) {
		ClearArrBase( *m_arrObject );
		udDELETE( m_arrObject );
		udDELETE( m_pPager );
		udDELETE( m_pFilter );
	} // ~CDAList

	CDAList& CDAList::operator =( const CDAList& objCopy ) {
		return *this;
	} // CDAList& operator =

	CDAList::CDAList(
		const string& szName,
		pfnGetObject fnGetObject,
		const string &szOrderBy,
		udInt iOrder,
		udPPager pPager,
		udPMorph pFilter
	) :
		udDataAccess( ),
		m_szName( szName ),
		m_fnGetObject( fnGetObject ),
		m_szOrderBy( szOrderBy ),
		m_iOrderType( iOrder ),
		m_pPager( pPager ),
		m_pFilter( pFilter ),
		m_arrObject( new udArrBase )
	{
	} // CDAList

	void CDAList::Load( ) {
		if ( !m_fnGetObject || m_szName.empty( ) ) {
			return;
		}

		udPSocket sock = g_pApp->BeginRequest( );
		if ( sock ) {
			// ListRequest
			udBinaryData data;
			data.add( m_szName );
			data.add( ( udInt ) opidLst1 );
			data.add( m_pPager->GetPage( ) );
			data.add( m_pPager->GetPageSize( ) );
			data.add( m_szOrderBy );
			data.add( m_iOrderType );

			string log = "\nфильтр, ";
			m_pFilter->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			m_pFilter->dump( data );

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );

			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			if ( code > 0 ) { // необходимо считать объекты
				udType total( ( udUInt ) 0 );
				total.read( sock );
				m_pPager->SetTotal( total.toUInt( ) );
				udDataAccess::Read( sock, *m_arrObject, m_fnGetObject );
			}

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );

			sock->Shutdown( );
			sock->Close( );
			delete sock;
		}
	} // void Load

	void CDAList::Clear( ) {
		ClearArrBase( *m_arrObject );
	} // void Clear

	void CDAList::ClearExtraData( ) {
		for( t_vctArrFde::iterator i = m_arrExtra.begin( ); i != m_arrExtra.end( ); ++i ) {
			( *i )->Clear( );
		}
	} // void ClearExtraData

	void CDAList::ResetFilter( ) {
		m_pFilter->reset( );
	} // void ResetFilter

	void CDAList::ResetPager( ) {
		udDELETE( m_pPager );
		m_pPager = new udPager( 1, 15, 10, 0 );
	} // void ResetPager

	void CDAList::AddExtraData( udPFormDataExtra pFde ) {
		m_arrExtra.push_back( pFde );
	} // void AddExtraData

	void CDAList::MakeObject( ) {
		for( t_vctArrFde::iterator i = m_arrExtra.begin( ); i != m_arrExtra.end( ); ++i ) {
			( *i )->Make( );
		}
	} // void MakeObject

	udPFormDataExtra CDAList::GetExtraData( udInt iMark ) const {
		for( t_vctArrFde::const_iterator i = m_arrExtra.begin( ); i != m_arrExtra.end( ); ++i ) {
			if ( ( *i )->GetMark( ) == iMark ) {
				return *i;
			}
		}
		return NULL;
	} // udPFormDataExtra GetExtraData

} // namespace UdSDK
