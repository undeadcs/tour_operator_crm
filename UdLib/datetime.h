/**
	UndeadCS SDK
	Базовая библиотека

	тип данных дата+время
*/
#ifndef UDNEADCS_SDK_LIBRARY_DATETIME_H
#define UDNEADCS_SDK_LIBRARY_DATETIME_H

namespace UdSDK {

	class CDatetime {
	public:
		enum datetimeProperty {
			datetimeSecond,
			datetimeMinute,
			datetimeHour,
			datetimeDay,
			datetimeMonth,
			datetimeYear
		};

	private:
		time_t	m_tData;

	public:
		CDatetime( time_t tData = ( time_t ) 0, bool load = false );
		CDatetime( string date );
		CDatetime( wstring date );
		CDatetime( const udDatetime& objCopy );

		virtual ~CDatetime( );

		CDatetime& operator=( const CDatetime& objCopy );

		void	load( );
		void	load( string date );
		void	load( wstring date );
		string	toString( const string& szFormat );
		wstring	toWString( const string& szFormat );
		void	inc( time_t v, datetimeProperty iProperty );
		void	dec( time_t v, datetimeProperty iProperty );

		void	set( time_t t ) { m_tData = t; }
		time_t	get( ) const { return m_tData; }

		void	get( udInt& year, udInt& month, udInt& day ) const;
		void	get( udInt& year, udInt& month, udInt& day, udInt& hour, udInt& minute, udInt& second ) const;

	}; // class CDatetime

	bool operator ==( const CDatetime& d1, const CDatetime& d2 );
	bool operator !=( const CDatetime& d1, const CDatetime& d2 );
	bool operator <( const CDatetime& d1, const CDatetime& d2 );
	bool operator <=( const CDatetime& d1, const CDatetime& d2 );
	bool operator >( const CDatetime& d1, const CDatetime& d2 );
	bool operator >=( const CDatetime& d1, const CDatetime& d2 );

} // namespace UdSDK

#endif
