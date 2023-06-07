/**
	UndeadCS SDK
	Базовая библиотека
	Набор шаблонных функций
*/
#ifndef UNDEADCS_SDK_LIBRARY_TEMPLATE_FUNCTIONS_H
#define UNDEADCS_SDK_LIBRARY_TEMPLATE_FUNCTIONS_H

namespace UdSDK {

	// уснатавливает значение, но возвращает старое
	template< typename T >
	T set( T* p, const T& v ) {
		T x = *p;
		*p = v;
		return x;
	} // T set

	// увеличивает значение на заданную величину, но возвращает старое
	template< typename T >
	T inc( T* p, const T& v ) {
		T x = *p;
		*p += v;
		return x;
	} // udInt inc

	// уменьшает значение на заданную величину, но возвращает старое
	template< typename T >
	T dec( T* p, const T& v ) {
		T x = *p;
		*p -= v;
		return x;
	} // udInt dec

} // namespace UdSDK

#endif
