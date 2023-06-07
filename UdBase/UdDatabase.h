
#ifndef UDDATABASE_H
#define UDDATABASE_H

namespace UdSDK {

	/**
	 *	Интерфейс работы с базой данных
	 */
	class CUdDatabase {

	public:
		CUdDatabase( );
		CUdDatabase( const CUdDatabase& objCopy );

		virtual ~CUdDatabase( );

		CUdDatabase& operator =( const CUdDatabase& objCopy );

		virtual bool	Query( string query ) = 0;
		virtual bool	GetAssoc( string query, udArrAsc& out ) = 0;
		virtual bool	GetAssoc( string query, udArrMorph& out ) = 0;
		virtual udError	GetError( ) = 0;
		virtual bool	EscapeString( char* dest, const char* src, size_t len ) = 0;

	}; // class CUdDatabase

} // namespace UdSDK

#endif
