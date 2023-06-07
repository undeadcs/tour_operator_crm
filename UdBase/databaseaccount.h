/**
 *	Расширенный вид серверного аккаунта
 *	дополнительно содержит имя базы данных
 */

#ifndef DATABASEACCOUNT_H
#define DATABASEACCOUNT_H

namespace UdSDK {

	/**
	 *	Аккаунт для базы данных
	 */
	class CDatabaseAccount : public CServerAccount {
		string	m_szDatabase;

	public:
		CDatabaseAccount( );
		CDatabaseAccount( const string& szHost, const string& szUsername, const string& szPassword, const string& szDatabase );

		virtual ~CDatabaseAccount( );

		string	GetDatabaseName( )	const { return m_szDatabase;	}

		void	SetDatabaseName( const string& name ) { m_szDatabase = name; }

	}; // class CDatabaseAccount

} // namespace UdSDK

#endif
