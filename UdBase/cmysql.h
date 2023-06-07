
#ifndef CMYSQL_H
#define CMYSQL_H

namespace UdSDK {

	/**
	 *	Класс для работы с MySQL
	 */
	class CMySql : public udDatabase {
	public:
		typedef enum cmysqlError {
			UDSDK_MYSQL_ERROR_OK,			// нет ошибки
			UDSDK_MYSQL_ERROR_NOINIT,		// провалилась функция mysql_init
			UDSDK_MYSQL_ERROR_NOCONNECT,	// провалилась функция connect
			UDSDK_MYSQL_ERROR_EMPTYDBNAME,	// было задано пустое имя БД
			UDSDK_MYSQL_ERROR_NODATABASE,	// не получилось выбрать базу данных
			UDSDK_MYSQL_ERROR_CREATEDBFAIL,	// не удалось создать базу данных
			UDSDK_MYSQL_ERROR_SELECTFAIL,	// не удалось выбрать базу данных
		} udMySQLError;

	private:
		string				m_szCharacterSet;
		udPStdMySQL			m_pConnection;
		udUInt				m_iPort;
		udPDatabaseAccount	m_pAccount;

	public:
		CMySql( );

		virtual ~CMySql( );

		// создание объекта MYSQL и соединение с СУБД
		/*udMySQLError Create(
			const string&	szHost,
			const string&	szUser,
			const string&	szPass,
			const string&	szDb	= "",
			const udUInt	iPort	= 0
		);*/
		void			SetAccData( const string& host, const string& user, const string& passw, const string& db );
		void			SetPortNumber( udUInt port ) { m_iPort = port; }
		udMySQLError	Connect( bool create_db = false );
		void			Close( );
		// закрытие соединения и обнуление объекта MYSQL
		udUInt		GetErrorNumber( );
		string		GetErrorMessage( );
		udULongLong	GetAffectedRows( );
		udULongLong	GetInsertId( );
		udInt		SetCharacterSet( const udPChar szCharacterSet );
		udInt		SelectDb( const string& szDb );
		udPMySQLRes	UseResult( );
		udPMySQLRes	StoreResult( );
		void		FreeResult( udPMySQLRes pResult );
		udUInt		FieldCount( );
		void		EnumFields( udPMySQLRes pResult );

		void		SetCharSet( const string& name ) { m_szCharacterSet = name; }

		udPDatabaseAccount	GetAccount( )	const { return m_pAccount;	}
		udUInt				GetPort( )		const { return m_iPort;		}

		virtual bool	Query( string query );
		virtual bool	GetAssoc( string query, udArrAsc& out );
		virtual bool	GetAssoc( string query, udArrMorph& out );
		virtual udError	GetError( );
		virtual bool	EscapeString( char* dest, const char* src, size_t len );

	}; // class CMySql

} // namespace UdSDK

#endif
