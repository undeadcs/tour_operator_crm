
#ifndef PROJECT_H
#define PROJECT_H

namespace UdSDK {

	class CAppInitProject : public CAppInit {
		typedef map< string, pfnGetObject > _asc_tables;

		_asc_tables	m_arrTables;
		string		m_szSessionId;	// ID сессии, выданной сервером
		datetime	m_dtConnect;	// время получения сессии

	public:
		CAppInitProject( );
		CAppInitProject( const CAppInitProject& objCopy );

		virtual ~CAppInitProject( );

		CAppInitProject& operator =( const CAppInitProject& objCopy );

		virtual void	LoadFonts( udPWndSplash pSplash );
		virtual void	LoadImages( udPWndSplash pSplash );
		virtual void	InitToolbar( udPWndSplash pSplash, udPWndToolbar pToolbar );
		virtual void	InitMenu( udPWndSplash pSplash, udPWndMenu pMenu );
		virtual void	InitSync( udPDatabaseSync sync );
		virtual void	InitModel( udPWndSplash pSplash, udPModelWork pModel );
		virtual udInt	Login( udPWndLogin wnd, wstring login, wstring password );
		virtual udInt	Login( udPSocket sock );
		virtual void	HandleSync( udPSocket sock, udAscBase& ascClient );

	private:
		void	InitTablesMap( );
		void	FeedServerMode( udPSocket sock );
		void	FeedClientMode( udPSocket sock );
		void	LessClientMode( udPSocket sock );
		//////////////////////////////////////////////////////////////////////////////////////////
		void	SendDatabase( udPSocket sock, udPHandler handler );
		void	ReadAndSaveDatabase( udPSocket sock, udPHandler handler );
		void	ReadAndSaveMetadata( udPSocket sock, udPHandler handler );
		void	TruncateTables( udPHandler handler, udPDatabase db );
		//////////////////////////////////////////////////////////////////////////////////////////

	}; // class CAppInitProject

	class CHMainPage : public udStateHandler {

	public:
		CHMainPage( );
		CHMainPage( const CHMainPage& objCopy );

		virtual ~CHMainPage( );

		CHMainPage& operator =( const CHMainPage& objCopy );

		virtual void	Show( udPWndModel pWndModel );
		virtual void	Init( );
		virtual void	Clear( );
		virtual void	Load( );
		virtual void	ClickToolbar( string index );
		virtual udInt	ToolbarAction( string index );

	}; // class CHRequest

} // namespace UdSDK

#endif
