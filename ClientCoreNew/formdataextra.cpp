
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CFormDataExtra::CFormDataExtra( ) : m_iMark( 0 ) {
	} // CFormDataExtra

	CFormDataExtra::CFormDataExtra( const CFormDataExtra& objCopy ) {
	} // CFormDataExtra

	CFormDataExtra::~CFormDataExtra( ) {
	} // ~CFormDataExtra

	CFormDataExtra& CFormDataExtra::operator =( const CFormDataExtra& objCopy ) {
		return *this;
	} // CFormDataExtra& operator =

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CFDECallback::CFDECallback( ) :
		CFormDataExtra( ),
		m_fnGetObject( NULL )
	{
	} // CFDECallback

	CFDECallback::CFDECallback( const CFDECallback& objCopy ) : CFormDataExtra( ) {
	} // CFDECallback

	CFDECallback::~CFDECallback( ) {
	} // ~CFDECallback

	CFDECallback& CFDECallback::operator =( const CFDECallback& objCopy ) {
		return *this;
	} // CFDECallback& operator =

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CFDEObject::CFDEObject( ) :
		CFDECallback( ),
		m_pObject( NULL )
	{
		m_iType = EXTRA_DATA_OBJECT;
	} // CFDEObject

	CFDEObject::CFDEObject( const CFDEObject& objCopy ) :
		CFDECallback( ),
		m_pObject( NULL )
	{
	} // CFDEObject

	CFDEObject::~CFDEObject( ) {
		udDELETE( m_pObject );
	} // ~CFDEObject

	CFDEObject& CFDEObject::operator =( const CFDEObject& objCopy ) {
		return *this;
	} // CFDEObject& operator =

	void CFDEObject::Load( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CFDEObject::Load, enter\n" );
		g_pApp->GetLog( )->Write( "CFDEObject::Load, udDELETE\n" );
		udDELETE( m_pObject );
		if ( m_fnGetObject ) {
			g_pApp->GetLog( )->Write( "CFDEObject::Load, call m_fnGetObject\n" );
			m_pObject = m_fnGetObject( );
			if ( m_pObject ) {
				string log = "\nwait object: ";
				m_pObject->dump( log );
				g_pApp->GetLog( )->Write( log + "\n" );

				char code = 0;
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				if ( sock->ReadByte( &code ) == 1 ) {
					if ( code > 0 ) {
						m_pObject->read( sock );
						log = "\nCFDEObject::Load, object:\n";
						m_pObject->dump( log );
						g_pApp->GetLog( )->Write( log + "\n" );
					} else {
						g_pApp->GetLog( )->Write( "CFDEObject::Load, code is 0\n" );
					}
				} else {
					g_pApp->GetLog( )->Write( "CFDEObject::Load, sock->ReadByte, failed\n" );
				}
			}
		}
		g_pApp->GetLog( )->Write( "CFDEObject::Load, exit\n" );
	} // void Load

	void CFDEObject::Clear( ) {
		udDELETE( m_pObject );
	} // void Clear

	void CFDEObject::Send( udPSocket sock ) {
		/*if ( m_pObject ) {
			udBinaryData data;
			m_pObject->dump( data );
			sock->Send( data );
		}*/
	} // void Send

	void CFDEObject::Make( ) {
		if ( m_fnGetObject && !m_pObject ) {
			m_pObject = m_fnGetObject( );
		}
	} // void Make

	void CFDEObject::Make( bool bDelete ) {
		if ( bDelete ) {
			udDELETE( m_pObject );
		}
		Make( );
	} // void Make

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CFDEList::CFDEList( ) :
		CFDECallback( ),
		m_fnGetObjectSend( NULL ),
		m_arrObject( new udArrBase( ) )
	{
		m_iType = EXTRA_DATA_LIST;
	} // CFDEList

	CFDEList::CFDEList( const CFDEList& objCopy ) {
	} // CFDEList

	CFDEList::~CFDEList( ) {
		ClearArrBase( m_arrObject );
		udDELETE( m_arrObject );
	} // ~CFDEList

	CFDEList& CFDEList::operator =( const CFDEList& objCopy ) {
		return *this;
	} // CFDEList& operator =

	void CFDEList::Load( udPSocket sock ) {
		g_pApp->GetLog( )->Write( "CFDEList::Load, enter\n" );
		ClearArrBase( m_arrObject );
		if ( m_fnGetObject ) {
			char code = 0;
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			g_pApp->GetLog( )->Write( "CFDEList::Load, reading byte\n" );
			if ( ( sock->ReadByte( &code ) == 1 ) && ( code > 0 ) ) {
				g_pApp->GetLog( )->Write( "CFDEList::Load, udDataAccess::Read\n" );
				udDataAccess::Read( sock, *m_arrObject, m_fnGetObject );
			}
		}
		g_pApp->GetLog( )->Write( "CFDEList::Load, exit\n" );
	} // void Load

	void CFDEList::Clear( ) {
		ClearArrBase( m_arrObject );
	} // void Clear

	void CFDEList::Send( udPSocket sock ) {
		if ( m_arrObject ) {
			pfnGetObject fn = m_fnGetObjectSend ? m_fnGetObjectSend : m_fnGetObject;
			udPBase tmp = fn( );
			string log = "\nCFDEList::Send, struct\n";
			tmp->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			udDELETE( tmp );

			udBinaryData data;
			udDataAccess::Write( *m_arrObject, data, m_fnGetObjectSend ? m_fnGetObjectSend : m_fnGetObject );
			sock->Send( data );
		}
	} // void Send

	void CFDEList::Make( ) {
	} // void Make

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CFDEFile::CFDEFile( ) : CFormDataExtra( ) {
		m_iType = EXTRA_DATA_FILE;
	} // CFDEFile

	CFDEFile::CFDEFile( const CFDEFile& objCopy ) : CFormDataExtra( ) {
	} // CFDEFile

	CFDEFile::~CFDEFile( ) {
	} // ~CFDEFile

	CFDEFile& CFDEFile::operator =( const CFDEFile& objCopy ) {
		return *this;
	} // CFDEFile& operator =

	void CFDEFile::Load( udPSocket sock ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		udPLog __log = g_pApp->GetLog( );
		__log->Write( "CFDEFile::Load, enter\n" );
		WriteUdLibLog( "CFDEFile::Load, enter\n" );
		char code = 0;
		sock->ReadByte( &code );
		if ( code > 0 ) {
			string log;
			udUInt cnt = 0;
			wstring tmp_dir = m_szFolder;
			udInt err = 0;
			udFileTransfer objFileTransfer;
			// очищаем временную папку от левых файлов
			//DirectoryClear( tmp_dir );
			objFileTransfer.SetTmpDir( tmp_dir );
			err = objFileTransfer.Recv( sock );
			log = udFileTransfer::ErrorExplain( err ) + "\n";
			__log->Write( log );
			if ( err == udFileTransfer::FileTransferErrorOk ) {
				m_szName = objFileTransfer.GetFileName( );

				g_pApp->GetLog( )->Write( "filename='" + toString( m_szName ) + "'\n" );
			}
		} else {
			__log->Write( "файл отсутствует на сервере\n" );
		}
		WriteUdLibLog( "CFDEFile::Load, exit\n" );
		__log->Write( "CFDEFile::Load, exit\n" );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void Load

	void CFDEFile::Clear( ) {
		m_szName = L"";
	} // void Clear

	void CFDEFile::Send( udPSocket sock ) {
		udUInt num = m_szName.empty( ) ? 0 : 1;
		udBinaryData data;
		data.add( ( char ) 80 );
		data.add( num );
		sock->Send( data );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( num == 1 ) { // отправляем файл
			udFileTransfer objFileTransfer;
			udInt err = 0;
			string log = "";
			objFileTransfer.SetFileName( m_szName );
			objFileTransfer.SetFilePath( m_szFolder + L"\\" + m_szName );
			err = objFileTransfer.Send( sock );
			log += udFileTransfer::ErrorExplain( err ) + "\n";
			g_pApp->GetLog( )->Write( log );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void Send

	void CFDEFile::Make( ) {
		CreateDirectory( m_szFolder.data( ), NULL );
	} // void Make

} // namespace UdSDK
