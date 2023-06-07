
#ifndef LOG_H
#define LOG_H

namespace UdSDK {

	/**
	 * 	Лог
	 */
	class CUdLog {
	public:
		enum LogError {
			UDERROR_OK = 0,
			UDERROR_CANT_OPEN_LOG,
			UDERROR_WRITE_LOG_FAILED,
			UDERROR_NOT_OPENED,
			UDERROR_INVALID_ARG
		};

		enum LogLevel {
			UDLOG_ERROR		= 0,
			UDLOG_WARNING	= 1,
			UDLOG_NOTICE	= 2,
			UDLOG_INFO		= 4
		};

	private:
		string		m_szPath;	// путь до файла лога
		udPStdFile	m_pFile;	// дескриптор файла

	public:
		CUdLog( );
		CUdLog( const CUdLog& objCopy );

		virtual ~CUdLog( );

		LogError	Open( const string& szPath, const char* mode = "ab" );
		LogError	Close( );
		LogError	Write( const string& szText, udDWord dwShowTime = 1 );
		void		ShowWarning( );
		void		ShowNotice( );
		void		ShowInfo( );

	}; // class CUdLog

} // namespace UdSDK

#endif
