/**
	UndeadCS SDK
	Базовая библиотека

	предварительное объявление
*/
#ifndef UNDEADCS_SDK_LIBRARY_FORWARD_H
#define UNDEADCS_SDK_LIBRARY_FORWARD_H

namespace UdSDK {

	udInt	OpenUdLibLog( string path, const char* mode = "ab" );
	udInt	CloseUdLibLog( );
	udInt	WriteUdLibLog( string log );

} // namespace UdSDK

#endif
