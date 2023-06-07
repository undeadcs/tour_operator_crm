
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CDataAccess::CDataAccess( ) {
	} // CDataAccess

	CDataAccess::CDataAccess( const CDataAccess& objCopy ) {
	} // CDataAccess

	CDataAccess::~CDataAccess( ) {
	} // ~CDataAccess

	CDataAccess& CDataAccess::operator =( const CDataAccess& objCopy ) {
		return *this;
	} // CDataAccess& operator =

	udInt CDataAccess::Read( udPSocket sock, udArrBase& arr, pfnGetObject fnGetObject ) {
		udType num( ( udUInt ) 0 );
		udUInt cnt = 0;

		if ( num.read( sock ) == 0 ) {
			g_pApp->GetLog( )->Write( "не удалось скачать количество объектов" );
			return -1;
		}
		string log;
		udPBase obj = NULL;

		log = "ожидаются объекты такой структуры: ";
		obj = fnGetObject( );
		obj->dump( log );
		udDELETE( obj );
		g_pApp->GetLog( )->Write( log + "\n" );

		log = "заявлено объектов: " + num.toString( ) + "\n";

		while( cnt < num.toUInt( ) ) {
			obj = fnGetObject( );
			if ( obj->read( sock ) == 0 ) {
				delete obj;
				break;
			}
			obj->dump( log );
			log += "\n";
			arr.push_back( obj );
			++cnt;
		}
		log += "считано объектов: " + toString( cnt ) + "\n";
		g_pApp->GetLog( )->Write( log );

		return cnt;
	} // udInt Read

	udInt CDataAccess::Read( udPSocket sock, udArrBase& arr, pfnGetObject fnGetObject, udPBase tpl ) {
		udType num( ( udUInt ) 0 );
		udUInt cnt = 0;

		if ( num.read( sock ) == 0 ) {
			g_pApp->GetLog( )->Write( "не удалось скачать количество объектов" );
			return -1;
		}
		string log;
		log = "заявлено объектов: " + num.toString( ) + "\n";
		udPBase obj = NULL;
		while( cnt < num.toUInt( ) ) {
			obj = fnGetObject( );
			tpl->apply( obj );
			if ( obj->read( sock ) == 0 ) {
				delete obj;
				break;
			}
			obj->dump( log );
			log += "\n";
			arr.push_back( obj );
			++cnt;
		}
		log += "считано объектов: " + toString( cnt ) + "\n";
		g_pApp->GetLog( )->Write( log );

		return cnt;
	} // udInt Read

	udInt CDataAccess::Write( udArrBase& arr, udBinaryData& data, pfnGetObject fnGetObject ) {
		string log;
		log = "\nsize=" + toString( arr.size( ) ) + "\n";
		data.add( arr.size( ) );

		if ( fnGetObject ) {
			udPBase from = NULL, to = NULL;
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				from = *i;
				to = fnGetObject( );
				to->apply( from, true, true );
				to->dump( log );
				log += "\n";
				to->dump( data );
				udDELETE( to );
			}
		} else {
			udPBase obj = NULL;
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				obj->dump( data );
				obj->dump( log );
				log += "\n";
			}
		}

		g_pApp->GetLog( )->Write( log );
		return 1;
	} // udInt Write

	udPBase CDataAccess::LoadById( const string& handler_name, udUInt id, pfnGetObject fnGetObject ) {
		udPBase ret = NULL;
		udPSocket sock = g_pApp->BeginRequest( );
		if ( sock ) {
			udBinaryData data;
			data.add( handler_name );
			data.add( ( udInt ) udStateHandler::opidGet );
			data.add( ( udUInt ) id );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			char code = 0;
			sock->ReadByte( &code );
			if ( code > 0 ) {
				udPBase obj = fnGetObject( );
				if ( obj->read( sock ) == 1 ) {
					ret = obj;
				} else {
					udDELETE( obj );
				}
			}
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );

			sock->Shutdown( );
			sock->Close( );
			delete sock;
		}
		return ret;
	} // udPBase LoadById

} // namespace UdSDK
