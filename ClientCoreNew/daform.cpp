
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CDAForm::CDAForm( ) :
		udDataAccess( ),
		m_fnGetObject( NULL ),
		m_pObject( NULL ),
		m_bHasFiles( false )
	{
	} // CDAForm

	CDAForm::CDAForm( const CDAForm& objCopy ) :
		udDataAccess( ),
		m_fnGetObject( NULL ),
		m_pObject( NULL ),
		m_bHasFiles( false )
	{
	} // CDAForm

	CDAForm::~CDAForm( ) {
		g_pApp->GetLog( )->Write( "CDAForm::~CDAForm, 1\n" );
		for( t_vctArrFde::iterator i = m_arrExtra.begin( ); i != m_arrExtra.end( ); ++i ) {
			delete *i;
		}
		g_pApp->GetLog( )->Write( "CDAForm::~CDAForm, 2\n" );
		udDELETE( m_pObject );
		g_pApp->GetLog( )->Write( "CDAForm::~CDAForm, 3\n" );
	} // ~CDAForm

	CDAForm& CDAForm::operator =( const CDAForm& objCopy ) {
		return *this;
	} // CDAForm& operator =

	void CDAForm::Load( ) {
		g_pApp->GetLog( )->Write( "CDAForm::Load, enter\n" );
		if ( m_pObject && m_pObject->get( "id" )->toUInt( ) ) {
			udPSocket sock = g_pApp->BeginRequest( );
			if ( sock ) {
				udBinaryData data;
				data.add( m_szName );
				data.add( ( udInt ) opidEdt );
				m_pObject->dump( data );
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
				char code = 0;
				sock->ReadByte( &code );
				if ( code > 0 ) {
					if ( code == 2 ) {
						//watch_mode = true;
					}
					string log = "\nread object\n";
					m_pObject->dump( log );
					// объект
					m_pObject->read( sock );

					log += "\n";
					m_pObject->dump( log );
					g_pApp->GetLog( )->Write( log + "\n" );
					// дочерние сущности
					g_pApp->GetLog( )->Write( "CDAForm::Load, m_arrExtra, Load\n" );
					for( t_vctArrFde::iterator i = m_arrExtra.begin( ); i != m_arrExtra.end( ); ++i ) {
						( *i )->Load( sock );
					}
					// файлы
					if ( m_bHasFiles ) {
						g_pApp->GetLog( )->Write( "CDAForm::Load, loading files\n" );
						char code = 0;
						sock->ReadByte( &code );
						if ( code > 0 ) {
							udType num( ( udUInt ) 0 );
							if ( num.read( sock ) == 0 ) {
								return;
							}
							udPLog __log = g_pApp->GetLog( );
							string log;
							udUInt cnt = 0, num1 = num.toUInt( );
							wstring tmp_dir = m_szFolder;
							udInt err = 0;
							udFileTransfer objFileTransfer;
							// очищаем временную папку от левых файлов
							DirectoryClear( tmp_dir );
							objFileTransfer.SetTmpDir( tmp_dir );
							while( cnt < num1 ) {
								err = objFileTransfer.Recv( sock );
								log = udFileTransfer::ErrorExplain( err ) + "\n";
								__log->Write( log );
								if ( err != udFileTransfer::FileTransferErrorOk ) {
									break;
								}
								++cnt;
							}
						}
					}
				}
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 80 );

				sock->Shutdown( );
				sock->Close( );
				delete sock;
			}
		}
		g_pApp->GetLog( )->Write( "CDAForm::Load, enter\n" );
	} // void Load

	void CDAForm::Send( udPArrBase pArrFiles ) {
		if ( m_pObject ) {
			udPSocket sock = g_pApp->BeginRequest( );
			if ( sock ) {
				udPLog __log = g_pApp->GetLog( );
				string log = "\n";
				udBinaryData data;
				data.add( m_szName );

				log = "\nsend object\n";
				m_pObject->dump( log );
				__log->Write( log + "\n" );

				if ( m_pObject->get( "id" )->toUInt( ) > 0 ) {
					data.add( ( udInt ) opidUpd1 );
				} else {
					data.add( ( udInt ) opidAdd1 );
				}
				// объект
				m_pObject->dump( data );
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->Send( data );
				// дочерние сущности
				for( t_vctArrFde::iterator i = m_arrExtra.begin( ); i != m_arrExtra.end( ); ++i ) {
					( *i )->Send( sock );
				}
				// файлы
				if ( m_bHasFiles ) {
					udBinaryData data;
					data.add( ( char ) 80 );
					if ( pArrFiles ) {
						data.add( pArrFiles->size( ) );
					} else {
						data.add( ( udUInt ) 0 );
					}
					sock->Send( data );

					if ( pArrFiles && !pArrFiles->empty( ) ) {
						udFileTransfer objFileTransfer;
						udInt err = 0;
						string log = "\n";
						udPBase obj1 = NULL;
						for( udArrBase::iterator i = pArrFiles->begin( ); i != pArrFiles->end( ); ++i ) {
							obj1 = *i;
							obj1->dump( log );
							log += "\n";
							objFileTransfer.SetFileName( obj1->get( "name" )->toWString( ) );
							objFileTransfer.SetFilePath( obj1->get( "tmp_path" )->toWString( ) );
							err = objFileTransfer.Send( sock );
							log += udFileTransfer::ErrorExplain( err ) + "\n";
						}
						g_pApp->GetLog( )->Write( log );
					}
				}

				udType id( ( udUInt ) 0 );
				id.read( sock );
				m_pObject->get( "id" )->ini( id.toUInt( ) );

				g_pApp->GetLog( )->Write( "CDAForm::Send, id=" + id.toString( ) + "\n" );

				char code = 0;
				sock->ReadByte( &code );

				sock->Shutdown( );
				sock->Close( );
				delete sock;
			}
		}
	} // void Send

	void CDAForm::Clear( ) {
		udDELETE( m_pObject );
		// почему-то воткнул здесь MakeObject, не понять, надо еще раз проверить.
		//MakeObject( );
		if ( !m_arrExtra.empty( ) ) {
			for( t_vctArrFde::iterator i = m_arrExtra.begin( ); i != m_arrExtra.end( ); ++i ) {
				( *i )->Clear( );
			}
		}
	} // void Clear

	void CDAForm::AddExtraData( udPFormDataExtra pFde ) {
		m_arrExtra.push_back( pFde );
	} // void AddExtraData

	void CDAForm::SetId( udUInt id ) {
		udDELETE( m_pObject );
		MakeObject( );
		if ( m_pObject ) {
			m_pObject->get( "id" )->ini( id );
		}
	} // void SetId

	udPFormDataExtra CDAForm::GetExtraData( udInt iMark ) const {
		for( t_vctArrFde::const_iterator i = m_arrExtra.begin( ); i != m_arrExtra.end( ); ++i ) {
			if ( ( *i )->GetMark( ) == iMark ) {
				return *i;
			}
		}
		return NULL;
	} // udPFormDataExtra GetExtraData

	void CDAForm::MakeObject( ) {
		if ( m_fnGetObject && !m_pObject ) {
			m_pObject = m_fnGetObject( );

			for( t_vctArrFde::iterator i = m_arrExtra.begin( ); i != m_arrExtra.end( ); ++i ) {
				( *i )->Make( );
			}
		}
	} // void MakeObject

	void CDAForm::RemoveObject( ) {
		udDELETE( m_pObject );
		if ( !m_arrExtra.empty( ) ) {
			for( t_vctArrFde::iterator i = m_arrExtra.begin( ); i != m_arrExtra.end( ); ++i ) {
				( *i )->Clear( );
			}
		}
	} // void RemoveObject

} // namespace UdSDK
