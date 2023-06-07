/**
	Tourist application
	Ядро сервера

	модель работы
*/
#ifndef TOURIST_APPLICATION_SERVER_CORE_MODELWORK_H
#define TOURIST_APPLICATION_SERVER_CORE_MODELWORK_H

namespace UdSDK {

	class CModelWork {
		udAscStateHandler	m_arrHandler;
		udPHObjectLock		m_pLockHandler;

	public:
		CModelWork( );
		CModelWork( const CModelWork& objCopy );

		~CModelWork( );

		CModelWork& operator =( const CModelWork& objCopy );

		void	Init( );
		void	InitSync( );
		void	SetHandler( string state, udPStateHandler pHandler );

		void	SetLockHandler( udPHObjectLock hLock ) { m_pLockHandler = hLock; }

		udPStateHandler	GetHandler( const string& state ) const;

		udPHObjectLock	GetLockHandler( ) const { return m_pLockHandler; }

	}; // class CModelWork

} // namespace UdSDK

#endif
