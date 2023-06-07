/**
	UndeadCS SDK
	Базовая библиоткеа

	отправщик файлов по сети
*/
#ifndef UNDEADCS_SDK_LIBRARY_FILE_TRANSFER_H
#define UNDEADCS_SDK_LIBRARY_FILE_TRANSFER_H

namespace UdSDK {

	class CUdFileTransfer {
	public:
		enum FileTransferError {
			FileTransferErrorOk,				// ошибок не произошло
			FileTransferErrorEmptyTmpFolder,	// пустое имя папки временных файлов
			FileTransferErrorEmptyName,			// пустое имя файла
			FileTransferErrorEmptyPath,			// пустой путь до файла
			FileTransferErrorFileNotFound,		// файл не найден по заданному пути
			FileTransferErrorFileNotOpened,		// не удалось открыть файл
			FileTransferErrorSocketFailed,		// ошибка сокета во время операции
			FileTransferErrorNotTransfered,		// соединение было закрыто, до окончания операции
			FileTransferErrorIOError,			// ошибка чтения/записи файла на диск
		};

	private:
		enum {
			frame_size = 4096,	// размер куска файла
		};

		wstring	m_szTmpDir,		// папка хранения временных файлов
				m_szFileName,	// имя файла
				m_szFilePath;	// путь до файла
		udDWord	m_dwFrameSize;	// размер куска файла

	public:
		CUdFileTransfer( );

		~CUdFileTransfer( );

		udInt	Send( udPSocket sock );
		udInt	Recv( udPSocket sock );

		void	SetTmpDir( const wstring& dir )		{ m_szTmpDir	= dir;	}
		void	SetFileName( const wstring& name )	{ m_szFileName	= name;	}
		void	SetFilePath( const wstring& path )	{ m_szFilePath	= path;	}
		void	SetFrameSize( udDWord size )		{ m_dwFrameSize	= size;	}

		wstring	GetTmpDir( )	const { return m_szTmpDir;		}
		wstring	GetFileName( )	const { return m_szFileName;	}
		wstring	GetFilePath( )	const { return m_szFilePath;	}
		udDWord	GetFrameSize( )	const { return m_dwFrameSize;	}

		static string	ErrorExplain( udInt err );

	}; // class CUdFileTransfer

} // namespace UdSDK

#endif
