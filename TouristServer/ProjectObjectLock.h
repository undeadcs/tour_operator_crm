/**
	Tourist application
	Серверное приложения

	блокировки
*/
#ifndef TOURIST_APPLICATION_TOURIST_SERVER_OBJECT_LOCK_H
#define TOURIST_APPLICATION_TOURIST_SERVER_OBJECT_LOCK_H

namespace UdSDK {

	class CHObjectLock2 : public udHObjectLock {

	private:
		struct stLockInfo {
			datetime date;
			string session_id;
			udUInt user_id;
		};

		typedef map< udUInt, stLockInfo >	map_lock_info;
		typedef map< pfnGetObject, map_lock_info > map_lock;

		map_lock	m_mapLock;	// объекты, которые залочены

	public:
		CHObjectLock2( );

		virtual ~CHObjectLock2( );

		CHObjectLock::lockedResult	LockObject( udPBase obj, pfnGetObject fnGetObject );
		CHObjectLock::lockedResult	UnlockObject( udPBase obj, pfnGetObject fnGetObject );
		bool			IsLockedObject( udPBase obj, pfnGetObject fnGetObject );

	}; // class CHObjectLock2

} // namespace UdSDK

#endif
