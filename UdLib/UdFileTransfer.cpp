
#include "includes.h"

namespace UdSDK {

	CUdFileTransfer::CUdFileTransfer( ) {
		m_szTmpDir		= L"";
		m_szFileName	= L"";
		m_szFilePath	= L"";
		m_dwFrameSize	= CUdFileTransfer::frame_size;
	}

	CUdFileTransfer::~CUdFileTransfer( ) {
	}

	udInt CUdFileTransfer::Send( udPSocket sock ) {
		if ( m_szFileName.empty( ) ) {
			return FileTransferErrorEmptyName;
		}
		if ( m_szFilePath.empty( ) ) {
			return FileTransferErrorEmptyPath;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		HANDLE hFile = CreateFile(
			m_szFilePath.data( ), GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
		);
		if ( hFile == INVALID_HANDLE_VALUE ) {
			udBinaryData data;
			data.add( ( udDWord ) 0 );	// стартовый фрэйм
			data.add( ( udDWord ) 0 ); // всего фрэймов
			sock->Send( data );
			return FileTransferErrorFileNotFound;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udDWord size = GetFileSize( hFile, NULL );
		if ( size > m_dwFrameSize ) { // делаем разбиение
			udDWord	dwBytesLeft	= size,
					dwFrameNum	= size / m_dwFrameSize,
					dwRead		= 0,
					dwIndex		= 0;
			udBinaryData data;
			if ( size % m_dwFrameSize ) {
				++dwFrameNum;
			}
			/////////////////////////////////////////////////////////////////////////////////////
			WriteUdLibLog( "frame num=" + toString( dwFrameNum ) + "\n" );
			data.add( ( udDWord ) 0 );	// стартовый фрэйм
			data.add( dwFrameNum ); // всего фрэймов
			data.add( m_szFileName ); // имя файла
			data.add( m_dwFrameSize ); // размер фрэймов
			sock->Send( data );
			data.clear( );
			// отсылаем файл частями
			dwIndex = 1;
			char *bytes = new char[ m_dwFrameSize ];
			while( m_dwFrameSize < dwBytesLeft ) {
				WriteUdLibLog( "//////////////////// отсылаем фрэйм ////////////////////\n" );
				WriteUdLibLog( "index=" + toString( dwIndex ) + "\n" );
				memset( bytes, 0, m_dwFrameSize );
				data.clear( );
				ReadFile( hFile, bytes, m_dwFrameSize, &dwRead, NULL );
				data.add( dwIndex++ ); // индекс фрэйма
				data.add( m_szFileName ); // имя файла
				data.add( m_dwFrameSize ); // размер фрэйма
				data.add( ( udPByte ) bytes, m_dwFrameSize );
				sock->Send( data );
				dwBytesLeft -= m_dwFrameSize;
				WriteUdLibLog( "////////////////////////////////////////////////////////\n" );
			}
			if ( dwBytesLeft > 0 ) { // еще остался кусок
				WriteUdLibLog( "//////////////////// отсылаем остатки ////////////////////\n" );
				WriteUdLibLog( "index=" + toString( dwIndex ) + "\n" );
				memset( bytes, 0, m_dwFrameSize );
				data.clear( );
				ReadFile( hFile, bytes, dwBytesLeft, &dwRead, NULL );
				data.add( dwIndex );
				data.add( m_szFileName );
				data.add( dwBytesLeft );
				data.add( ( udPByte ) bytes, dwBytesLeft );
				sock->Send( data );
				WriteUdLibLog( "//////////////////////////////////////////////////////////\n" );
			}
			udDELETE_( bytes );
			/////////////////////////////////////////////////////////////////////////////////////
		} else {
			/////////////////////////////////////////////////////////////////////////////////////
			udDWord read = 0;
			udPByte bytes = new udByte[ size ];
			if ( ( ReadFile( hFile, bytes, size, &read, NULL ) == 0 ) || ( size != read ) ) {
				udDELETE_( bytes );
				CloseHandle( hFile );
				return FileTransferErrorIOError;
			}
			/////////////////////////////////////////////////////////////////////////////////////
			udBinaryData data;
			data.add( ( udDWord ) 0 );	// стартовый фрэйм
			data.add( ( udDWord ) 1 ); // всего фрэймов
			data.add( m_szFileName ); // имя файла
			data.add( size ); // размер файла
			data.add( bytes, size );
			sock->Send( data );
			udDELETE_( bytes );
			/////////////////////////////////////////////////////////////////////////////////////
		}
		CloseHandle( hFile );
		//////////////////////////////////////////////////////////////////////////////////////////
		return FileTransferErrorOk;
	} // udInt Send

	udInt CUdFileTransfer::Recv( udPSocket sock ) {
		if ( m_szTmpDir.empty( ) ) {
			return FileTransferErrorEmptyTmpFolder;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udType num( ( udDWord ) 0 );
		udDWord	dwFrameIndex = 0;
		if ( num.read( sock ) == 0 ) { // индекс фрэйма
			return FileTransferErrorSocketFailed;
		}
		dwFrameIndex = num.toDWord( );
		if ( dwFrameIndex == 0 ) { // первый фрэйм несет инфу о всей структуре файла
			if ( num.read( sock ) == 0 ) { // количество фрэймов
				return FileTransferErrorSocketFailed;
			}
			udDWord dwFrameNum = num.toDWord( );
			if ( dwFrameNum == 0 ) {
				return FileTransferErrorOk;
			}
			if ( dwFrameNum > 1 ) { // файл толстый, необходимо дождаться нескольких фрэймов
				WriteUdLibLog( "заявлено фрэймов: " + toString( dwFrameNum ) + "\n" );
				udType name( wstring( L"" ) );
				if ( name.read( sock ) == 0 ) {
					return FileTransferErrorSocketFailed;
				}
				if ( num.read( sock ) == 0 ) {
					return FileTransferErrorSocketFailed;
				}
				udDWord dwFrameSize = num.toDWord( ), cnt = 0, dwIndex = 0, dwBytesToRead = 0, dwWritten = 0;
				WriteUdLibLog( "размер фрэйма=" + num.toString( ) + "\n" );
				wstring filename = name.toWString( ), path = m_szTmpDir + L"\\" + filename;
				HANDLE hFile = CreateFile(
					path.data( ), GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
					CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
				);
				if ( hFile == INVALID_HANDLE_VALUE ) {
					return FileTransferErrorIOError;
				}
				m_szFileName = filename;
				map< udDWord, int > mapLoaded;
				HANDLE hTmpFile = NULL;
				char *bytes = NULL;//new char[ dwFrameSize ];
				wstring filename1, path1;
				while( cnt < dwFrameNum ) {
					WriteUdLibLog( "//////////////////// считываем фрэйм ////////////////////\n" );
					// индекс фрэйма
					if ( num.read( sock ) == 0 ) {
						WriteUdLibLog( "CUdFileTransfer::Recv, не удалось считать индекс фрэйма\n" );
						//udDELETE_( bytes );
						CloseHandle( hFile );
						return FileTransferErrorSocketFailed;
					}
					WriteUdLibLog( "frame index=" + num.toString( ) + "\n" );
					dwIndex = num.toDWord( );
					// имя файла
					if ( name.read( sock ) == 0 ) {
						WriteUdLibLog( "CUdFileTransfer::Recv, не удалось считать имя файла\n" );
						//udDELETE_( bytes );
						CloseHandle( hFile );
						return FileTransferErrorSocketFailed;
					}
					WriteUdLibLog( "file name=\"" + name.toString( ) + "\"\n" );
					filename1 = name.toWString( );
					path1 = m_szTmpDir + L"\\" + filename1 + L".part" + toWString( dwIndex );
					WriteUdLibLog( "CUdFileTransfer::Recv, файл '" + toString( filename1 ) + "', путь '" + toString( path1 ) + "'\n" );
					// размер фрэйма
					if ( num.read( sock ) == 0 ) {
						WriteUdLibLog( "CUdFileTransfer::Recv, не удалось считать размер фрэйма\n" );
						//udDELETE_( bytes );
						CloseHandle( hFile );
						return FileTransferErrorSocketFailed;
					}
					WriteUdLibLog( "frame size=" + num.toString( ) + "\n" );
					dwBytesToRead = num.toDWord( );
					// считываем байты
					if ( sock->Read( &bytes, dwBytesToRead ) != dwBytesToRead ) {
						WriteUdLibLog( "CUdFileTransfer::Recv, не удалось считать все байты фрэйма\n" );
						udDELETE_( bytes );
						CloseHandle( hFile );
						return FileTransferErrorSocketFailed;
					}
					// сохраняем часть файла
					if ( WriteFile( hFile, bytes, dwBytesToRead, &dwWritten, NULL ) == 0 ) {
						WriteUdLibLog( "CUdFileTransfer::Recv, не удалось записать байты во временный файл\n" );
						udDELETE_( bytes );
						CloseHandle( hFile );
						return FileTransferErrorIOError;
					}
					WriteUdLibLog( "/////////////////////////////////////////////////////////\n" );
					++cnt;
				}
				udDELETE_( bytes );
				CloseHandle( hFile );
			} else { // файл мелкий и влез в один фрэйм
				udType name( wstring( L"" ) );
				if ( name.read( sock ) == 0 ) {
					return FileTransferErrorSocketFailed;
				}
				udType size( ( udDWord ) 0 );
				if ( size.read( sock ) == 0 ) {
					return FileTransferErrorSocketFailed;
				}
				wstring filename = name.toWString( ), path = m_szTmpDir + L"\\" + filename;
				HANDLE hFile = CreateFile(
					path.data( ), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL, NULL
				);
				if ( hFile == INVALID_HANDLE_VALUE ) {
					return FileTransferErrorIOError;
				}
				m_szFileName = filename;
				if ( size.toDWord( ) > 0 ) {
					WriteUdLibLog( "считываем файл '" + name.toString( ) + "', размер=" + size.toString( ) + "\n" );
					char *tmp = NULL;// = new char[ size.toDWord( ) ];
					udDWord dwSize = size.toDWord( );
					if ( sock->Read( &tmp, dwSize ) != dwSize ) {
						udDELETE_( tmp );
						return FileTransferErrorNotTransfered;
					}
					udDWord dwWritten = 0;
					if ( hFile == INVALID_HANDLE_VALUE ) {
						udDELETE_( tmp );
						WriteUdLibLog( "CUdFileTransfer::Recv, CreateFile failed, error code=" + toString( GetLastError( ) ) + "\n" );
						return FileTransferErrorIOError;
					}
					WriteFile( hFile, tmp, dwSize, &dwWritten, NULL );
					udDELETE_( tmp );
				} else {
					WriteUdLibLog( "файл '" + name.toString( ) + "' объявлен пустым\n" );
					if ( hFile == INVALID_HANDLE_VALUE ) {
						WriteUdLibLog( "CUdFileTransfer::Recv, CreateFile failed, error code=" + toString( GetLastError( ) ) + "\n" );
						return FileTransferErrorIOError;
					}
				}
				CloseHandle( hFile );
			}
		} else { // первое чтение и другой фрэйм? тогда в кэш
			WriteUdLibLog( "[!!!] первым получен не стартовый фрэйм\n" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		return FileTransferErrorOk;
	} // udInt Recv

	string CUdFileTransfer::ErrorExplain( udInt err ) {
		string ret;
		switch( err ) {
			case FileTransferErrorOk:
				ret = "FileTransferErrorOk, не является кодом ошибки";
				break;

			case FileTransferErrorEmptyTmpFolder:
				ret = "FileTransferErrorEmptyTmpFolder, не был задан путь к папке временных файлов";
				break;

			case FileTransferErrorEmptyName:
				ret = "FileTransferErrorEmptyName, не было задано имя файла";
				break;

			case FileTransferErrorEmptyPath:
				ret = "FileTransferErrorEmptyPath, не был задан путь до файла";
				break;

			case FileTransferErrorFileNotFound:
				ret = "FileTransferErrorFileNotFound, файл не найден";
				break;

			case FileTransferErrorFileNotOpened:
				ret = "FileTransferErrorFileNotOpened, не удалось открыть файл";
				break;

			case FileTransferErrorSocketFailed:
				ret = "FileTransferErrorSocketFailed, возникли ошибки при операции с сокетом";
				break;

			case FileTransferErrorNotTransfered:
				ret = "FileTransferErrorNotTransfered, соединение было закрыто до транспортировки всего файла"; 
				break;

			case FileTransferErrorIOError:
				ret = "FileTransferErrorIOError, ошибки при работе с файловой системой";
				break;

			default:
				ret = "udFileTransfer::ErrorExplain, неизвестный код ошибки";
				break;
		}
		return ret;
	} // string ErrorExplain

} // namespace UdSDK