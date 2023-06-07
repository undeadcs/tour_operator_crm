/**
	UndeadCS SDK
	Библиотека интерфейса пользователя

	предварительное объявление
*/
#ifndef UNDEADCS_SDK_USERINTERFACE_FORWARD_H
#define UNDEADCS_SDK_USERINTERFACE_FORWARD_H

namespace UdSDK {

	udInt	OpenUdUiLog( string path, const char* mode = "ab" );
	udInt	CloseUdUiLog( );
	udInt	WriteUdUiLog( string log );
	void	UdUiLogMessage( UINT msg );

} // namespace UdSDK

#endif