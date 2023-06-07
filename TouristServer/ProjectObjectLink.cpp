
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHObjectLock2::CHObjectLock2( ) : udHObjectLock( ) {
	}

	CHObjectLock2::~CHObjectLock2( ) {
	}

	CHObjectLock::lockedResult CHObjectLock2::LockObject( udPBase obj, pfnGetObject fnGetObject ) {
		assert( obj );
		assert( fnGetObject );
		udUInt id = obj->get( "id" )->toUInt( );
		map_lock::iterator i = m_mapLock.find( fnGetObject );
		if ( i != m_mapLock.end( ) ) {
			map_lock_info::iterator j = i->second.find( id );
			if ( j != i->second.end( ) ) {
				// тут стоит проверить время блокировки
				return CHObjectLock::lockedAlready;
			}
		}
		stLockInfo info;
		info.date.load( ); // дата
		CAppInitProject *init = ( CAppInitProject* ) g_pApp->GetInitObj( );
		SessionData sess = init->GetCurrentSessionData( );
		info.session_id = sess.id;
		info.user_id = sess.user->get( "id" )->toUInt( );
		m_mapLock[ fnGetObject ][ id ] = info;
		return CHObjectLock::lockedOk;
	} // lockedResult LockObject

	CHObjectLock::lockedResult CHObjectLock2::UnlockObject( udPBase obj, pfnGetObject fnGetObject ) {
		assert( obj );
		assert( fnGetObject );
		udUInt id = obj->get( "id" )->toUInt( );
		map_lock::iterator i = m_mapLock.find( fnGetObject );
		if ( i != m_mapLock.end( ) ) {
			map_lock_info::iterator j = i->second.find( id );
			if ( j != i->second.end( ) ) {
				CAppInitProject *init = ( CAppInitProject* ) g_pApp->GetInitObj( );
				SessionData sess = init->GetCurrentSessionData( );
				if ( ( j->second.session_id == sess.id ) && ( j->second.user_id == sess.user->get( "id" )->toUInt( ) ) ) {
					i->second.erase( j ); // сносим блокировку данного объекта
				} else {
					return CHObjectLock::lockedOk;
				}
			}
			if ( i->second.empty( ) ) {
				m_mapLock.erase( i ); // если нет залоченых объектов данного типа, то сносим лишний тип
			}
		}
		return CHObjectLock::lockedAlready;
	} // lockedResult UnlockObject

	bool CHObjectLock2::IsLockedObject( udPBase obj, pfnGetObject fnGetObject ) {
		assert( obj );
		assert( fnGetObject );
		map_lock::iterator i = m_mapLock.find( fnGetObject );
		if ( i != m_mapLock.end( ) ) {
			map_lock_info::iterator j = i->second.find( obj->get( "id" )->toUInt( ) );
			if ( j != i->second.end( ) ) {
				return true;
			}
		}
		return false;
	} // bool IsLockedObject

} // namespace UdSDK
