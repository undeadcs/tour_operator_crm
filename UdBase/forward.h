/**
	UndeadCS SDK
	Расширение базовой библиотеки
*/
#ifndef UNDEADCS_SDK_LIBRARY_BASE_FORWARD_H
#define UNDEADCS_SDK_LIBRARY_BASE_FORWARD_H

namespace UdSDK {

	udInt	OpenUdBaseLog( string path, const char* mode = "ab" );
	udInt	CloseUdBaseLog( );
	udInt	WriteUdBaseLog( string log );

} // namespace UdSDK

#endif
