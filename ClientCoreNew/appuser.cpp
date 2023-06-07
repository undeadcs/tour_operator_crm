
#include "includes.h"

namespace UdSDK {

	CAppUser::CAppUser( ) : udMorph( ) {
		get( "id"		)->ini(	( udUInt )	0	);	// ID
		get( "login"	)->ini( wstring( L"" )	);	// логин
		get( "password"	)->ini( wstring( L"" )	);	// пароль
		get( "email"	)->ini( wstring( L"" )	);	// e-mail
		get( "fio"		)->ini( wstring( L"" )	);	// ФИО
		get( "rank"		)->ini( ( udInt )	0	);	// ранг
		get( "series"	)->ini( wstring( L"" )	);	// серия для распечатки шаблонов документов
	} // CAppUser

	CAppUser::CAppUser( const CAppUser& objCopy ) : udMorph( ) {
	} // CAppUser

	CAppUser::~CAppUser( ) {
	} // ~CAppUser

	CAppUser& CAppUser::operator =( const CAppUser& objCopy ) {
		return *this;
	} // CAppUser& operator =

} // namespace UdSDK
