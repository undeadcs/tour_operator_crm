/**
	Tourist application
	Библиотека сервера

	обработчик блокировок
*/
#ifndef TOURIST_APPLICATION_TOURIST_SERVER_LOCK_HANDLER_H
#define TOURIST_APPLICATION_TOURIST_SERVER_LOCK_HANDLER_H

namespace UdSDK {

	class CHObjectLock {

	public:
		enum lockedResult {
			lockedOk,		// блокировка/разблокировка прошла успешно
			lockedAlready,	// блокировка/разблокировка уже присутствует (или отсутствует)
			lockedFailed,	// блокировка/разблокировка не прошла (лучше повторить попытку)
		};

	public:
		CHObjectLock( ) { }

		virtual ~CHObjectLock( ) { }

		virtual lockedResult	LockObject( udPBase obj, pfnGetObject fnGetObject ) = 0;
		virtual lockedResult	UnlockObject( udPBase obj, pfnGetObject fnGetObject ) = 0;
		virtual bool			IsLockedObject( udPBase obj, pfnGetObject fnGetObject ) = 0;

	}; // class CHObjectLock

} // namespace UdSDK

#endif
