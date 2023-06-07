
#ifndef PROJECT_H
#define PROJECT_H

namespace UdSDK {

	class CAppInitProject : public CAppInit {
		typedef map< string, pfnGetObject > _asc_tables;
		typedef struct structMergeParentInfo {
			string table;
			pfnGetObject fnGetObject;
			string attr;
		} _tomerge;
		typedef vector< _tomerge > vctmerge;

		typedef map< string, SessionData > _asc_session;

		_asc_tables		m_arrTables;	// таблицы менеджемнта
		_asc_session	m_ascSession;	// набор сессий клиентов
		udDWord			m_dwTimeout;	// таймаут сессии в секундах
		string			m_szCurrentSession;	// ID сессии текущего пользователя, который обратился с запросом

	public:
		CAppInitProject( );
		CAppInitProject( const CAppInitProject& objCopy );

		virtual ~CAppInitProject( );

		CAppInitProject& operator =( const CAppInitProject& objCopy );

		virtual void	LoadFonts( );
		virtual void	LoadImages( );
		virtual void	InitSync( udPDatabaseSync sync );
		virtual void	InitModel( udPModelWork pModel );
		virtual udInt	Login( udPSocket sock );
		virtual void	HandleSync( udPSocket sock, udAscBase& ascClient, udAscBase& ascServer );

		string	GetCurrentSessionId( ) const { return m_szCurrentSession; }
		SessionData	GetCurrentSessionData( ) const;

	private:
		void	InitTablesMap( );
		void	FeedServerMode( udPSocket sock, udDWord dwClientVersion, udAscBase& ascClient, udAscBase& ascServer );
		void	FeedClientMode( udPSocket sock, udAscBase& ascClient, udAscBase& ascServer );
		void	CollectInstances( udBinaryData& data, const string& name );
		void	LessClientMode( udPSocket sock, udDWord dwClientVersion, udAscBase& ascClient, udAscBase& ascServer );
		void	MergeDatabase( udPSocket sock, udAscBase& ascClient );
		//////////////////////////////////////////////////////////////////////////////////////////
		void	SendMetadata( udPSocket sock );
		void	SendDatabase( udPSocket sock, udPHandler handler );
		void	ReadAndSaveDatabase( udPSocket sock, udPHandler handler );
		void	TruncateTables( udPHandler handler, udPDatabase db );
		//////////////////////////////////////////////////////////////////////////////////////////
		void	MergeObjects( udPHandler tmphandler, const string& table, pfnGetObject fnGetObject );
		void	MergeSimpleObjects( udPHandler tmphandler,
					const string& table, pfnGetObject fnGetObject, const string& attr_find_clone,
					const string& table_parent, pfnGetObject fnGetObjectParent, const string& attr_parent
				);
		void	MergeSimpleObjectsMultiple( udPHandler tmphandler,
					const string& table, pfnGetObject fnGetObject, const string& attr_find_clone,
					const vctmerge& arrMerge
				);
		//////////////////////////////////////////////////////////////////////////////////////////
		void	CollectDatabases( const string& srcdb );
		void	FillTestData( );

	}; // class CAppInitProject

	class CHMainPage : public udStateHandler {

	public:
		CHMainPage( );
		CHMainPage( const CHMainPage& objCopy );

		virtual ~CHMainPage( );

		CHMainPage& operator =( const CHMainPage& objCopy );

		virtual void	Init( );
		virtual void	InitSync( udPDatabaseSync sync );
		virtual void	ProcRequest( udPSocket sock );

	}; // class CHRequest

} // namespace UdSDK

#endif
