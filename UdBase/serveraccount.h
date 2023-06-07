/**
 *	Классический вид аккаунта сервера
 *	содержит имя хоста, имя пользователя, пароль
 */

#ifndef SERVERACCOUNT_H
#define SERVERACCOUNT_H

namespace UdSDK {

	/**
	 *	Аккаунт
	 */
	class CServerAccount {
		string	m_szHost,
				m_szUsername,
				m_szPassword;

	public:
		CServerAccount( );
		CServerAccount( const string& szHost, const string& szUsername, const string& szPassword );

		virtual ~CServerAccount( );

		string	GetHostname( )	const { return m_szHost;		}
		string	GetUsername( )	const { return m_szUsername;	}
		string	GetPassword( )	const { return m_szPassword;	}

		void	SetHostname( const string& szHost )		{ m_szHost		= szHost;		}
		void	SetUsername( const string& szUsername )	{ m_szUsername	= szUsername;	}
		void	SetPassword( const string& szPassword )	{ m_szPassword	= szPassword;	}

	}; // class CServerAccount

} // namespace UdSDK

#endif
