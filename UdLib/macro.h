/**
	UndeadCS SDK
	Базовая библиотека

	макросы
*/
#ifndef UDNEADCS_SDK_LIBRARY_MACRO_H
#define UDNEADCS_SDK_LIBRARY_MACRO_H

#define udDELETE( del )		if ( del ) { delete del; del = NULL; }
#define udDELETE_( del )	if ( del ) { delete [ ] del; del = NULL; }

#endif
