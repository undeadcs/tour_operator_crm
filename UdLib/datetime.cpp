
#include "includes.h"

namespace UdSDK {

	CDatetime::CDatetime( time_t tData, bool load ) {
		m_tData = tData;
		if ( load ) {
			this->load( );
		}
	}

	CDatetime::CDatetime( string date ) {
		m_tData = 0;
		this->load( date );
	}

	CDatetime::CDatetime( wstring date ) {
		m_tData = 0;
		this->load( date );
	}

	CDatetime::CDatetime( const udDatetime& objCopy ) {
		m_tData = objCopy.m_tData;
	}

	CDatetime::~CDatetime( ) {
	}

	CDatetime& CDatetime::operator=( const CDatetime& objCopy ) {
		m_tData = objCopy.m_tData;
		return *this;
	} // CDatetime& operator =

	void CDatetime::load( ) {
		time( &m_tData );
	} // void load

	void CDatetime::load( string date ) {
		WriteUdLibLog( "CDatetime::load, enter\n" );
		tm tmp;
		ZeroMemory( &tmp, sizeof( tm ) );
		tmp.tm_isdst = -1;

		if ( date.size( ) == 10 ) {
			sscanf_s( date.data( ), "%d-%d-%d",
				&( tmp.tm_year ), &( tmp.tm_mon ), &( tmp.tm_mday )
			);

			WriteUdLibLog(
				string( "string='" ) + date +
				"', year=" + UdSDK::toString( tmp.tm_year ) + ", mon=" + UdSDK::toString( tmp.tm_mon ) +
				", day=" + UdSDK::toString( tmp.tm_mday )
			);

			tmp.tm_year -= 1900;
			--( tmp.tm_mon );
			m_tData = mktime( &tmp );
		} else if ( date.size( ) == 19 ) {
			sscanf_s( date.data( ), "%d-%d-%d %d:%d:%d",
				&( tmp.tm_year ), &( tmp.tm_mon ), &( tmp.tm_mday ),
				&( tmp.tm_hour ), &( tmp.tm_min ), &( tmp.tm_sec )
			);

			WriteUdLibLog(
				string( "string='" ) + date +
				"', year=" + UdSDK::toString( tmp.tm_year ) + ", mon=" + UdSDK::toString( tmp.tm_mon ) +
				", day=" + UdSDK::toString( tmp.tm_mday )
			);

			tmp.tm_year -= 1900;
			--( tmp.tm_mon );
			m_tData = mktime( &tmp );
		}
		if ( m_tData == -1 ) {
			m_tData = 0;
			WriteUdLibLog( ", mktime failed\n" );
		} else {
			WriteUdLibLog( ", mktime ok\n" );
		}

		WriteUdLibLog(
			"CDatetime::load, m_tData=" + UdSDK::toString( m_tData ) + ", "
			"date=\"" + this->toString( "%Y-%m-%d %H-%M-%S" ) + "\"\n"
		);
	} // void load

	void CDatetime::load( wstring date ) {
		tm tmp;
		ZeroMemory( &tmp, sizeof( tm ) );
		tmp.tm_isdst = -1;

		if ( date.size( ) == 10 ) {
			swscanf_s( date.data( ), L"%d-%d-%d",
				&( tmp.tm_year ), &( tmp.tm_mon ), &( tmp.tm_mday )
			);
			tmp.tm_year -= 1900;
			--( tmp.tm_mon );
			m_tData = mktime( &tmp );
		} else if ( date.size( ) == 19 ) {
			swscanf_s( date.data( ), L"%d-%d-%d %d:%d:%d",
				&( tmp.tm_year ), &( tmp.tm_mon ), &( tmp.tm_mday ),
				&( tmp.tm_hour ), &( tmp.tm_min ), &( tmp.tm_sec )
			);
			tmp.tm_year -= 1900;
			--( tmp.tm_mon );
			m_tData = mktime( &tmp );
		}
		if ( m_tData == -1 ) {
			m_tData = 0;
		}

		WriteUdLibLog(
			"CDatetime::load, m_tData=" + UdSDK::toString( m_tData ) + ", "
			"date=\"" + this->toString( "%Y-%m-%d %H-%M-%S" ) + "\n"
		);
	} // void load

	string CDatetime::toString( const string& szFormat ) {
		string	szRet( "" );
		tm		tTime;

		/*udChar log[ 1024 ];
		sprintf_s( log, 1024, "CDatetime::toString, now='%I64d'\n", m_tData );
		WriteUdLibLog( log );*/

		if ( m_tData == 0 ) {
			tTime.tm_sec = 0;
			tTime.tm_min = 0;
			tTime.tm_hour = 0;
			tTime.tm_mday = 1;
			tTime.tm_mon = 0;
			tTime.tm_wday = 4;
			tTime.tm_year = 70;
			tTime.tm_yday = 0;
			tTime.tm_isdst = 0;
		} else {
			errno_t	iErr = localtime_s( &tTime, &m_tData );
			if ( iErr != 0 ) {
				udChar log[ 1024 ];
				sprintf_s( log, 1024, "CDatetime::toString, now='%I64d', err='%d'\n", m_tData, iErr );
				WriteUdLibLog( log );

				tTime.tm_sec = 0;
				tTime.tm_min = 0;
				tTime.tm_hour = 0;
				tTime.tm_mday = 1;
				tTime.tm_mon = 0;
				tTime.tm_wday = 4;
				tTime.tm_year = 70;
				tTime.tm_yday = 0;
				tTime.tm_isdst = 0;
			}
		}
		 
		udChar tmp[ 128 ];
		strftime( tmp, 128, szFormat.c_str( ), &tTime );
		szRet = tmp;
		return szRet;
	} // string toString

	wstring CDatetime::toWString( const string& szFormat ) {
		string str = this->toString( szFormat );
		return ConvertString( str );
	} // wstring toWString

	void CDatetime::inc( time_t v, datetimeProperty iProperty ) {
		time_t m = 0;

		switch( iProperty ) {
			case datetimeSecond:
				m = 1;
				break;

			case datetimeMinute:
				m = 60;
				break;

			case datetimeHour:
				m = 60 * 60;
				break;

			case datetimeDay:
				m = 60 * 60 * 24;
				break;

			case datetimeMonth:
				m = 60 * 60 * 24 * 30;
				break;

			case datetimeYear:
				m = 60 * 60 * 24 * 365;
				break;
		};
		m_tData += v * m;
	} // void inc

	void CDatetime::dec( time_t v, datetimeProperty iProperty ) {
		time_t m = 0;

		switch( iProperty ) {
			case datetimeSecond:
				m = 1;
				break;

			case datetimeMinute:
				m = 60;
				break;

			case datetimeHour:
				m = 60 * 60;
				break;

			case datetimeDay:
				m = 60 * 60 * 24;
				break;

			case datetimeMonth:
				m = 60 * 60 * 24 * 30;
				break;

			case datetimeYear:
				m = 60 * 60 * 24 * 365;
				break;
		};
		m_tData -= v * m;
	} // void dec

	void CDatetime::get( udInt& year, udInt& month, udInt& day ) const {
		string	szRet( "" );
		tm		tTime;

		if ( m_tData == 0 ) {
			tTime.tm_sec = 0;
			tTime.tm_min = 0;
			tTime.tm_hour = 0;
			tTime.tm_mday = 1;
			tTime.tm_mon = 0;
			tTime.tm_wday = 4;
			tTime.tm_year = 70;
			tTime.tm_yday = 0;
			tTime.tm_isdst = 0;
		} else {
			errno_t	iErr = localtime_s( &tTime, &m_tData );
			if ( iErr != 0 ) {
				udChar log[ 1024 ];
				sprintf_s( log, 1024, "CDatetime::get, now='%I64d', err='%d'\n", m_tData, iErr );
				WriteUdLibLog( log );

				tTime.tm_sec = 0;
				tTime.tm_min = 0;
				tTime.tm_hour = 0;
				tTime.tm_mday = 1;
				tTime.tm_mon = 0;
				tTime.tm_wday = 4;
				tTime.tm_year = 70;
				tTime.tm_yday = 0;
				tTime.tm_isdst = 0;
			}
		}
		 
		udChar tmp[ 128 ];
		strftime( tmp, 128, "%Y%m%d", &tTime );
		szRet = tmp;
		
		year = toInt( szRet.substr( 0, 4 ) );
		month = toInt( szRet.substr( 4, 2 ) );
		day = toInt( szRet.substr( 6, 2 ) );
	} // void get

	void CDatetime::get( udInt& year, udInt& month, udInt& day, udInt& hour, udInt& minute, udInt& second ) const {
		string	szRet( "" );
		tm		tTime;

		if ( m_tData == 0 ) {
			tTime.tm_sec = 0;
			tTime.tm_min = 0;
			tTime.tm_hour = 0;
			tTime.tm_mday = 1;
			tTime.tm_mon = 0;
			tTime.tm_wday = 4;
			tTime.tm_year = 70;
			tTime.tm_yday = 0;
			tTime.tm_isdst = 0;
		} else {
			errno_t	iErr = localtime_s( &tTime, &m_tData );
			if ( iErr != 0 ) {
				udChar log[ 1024 ];
				sprintf_s( log, 1024, "CDatetime::toString, now='%I64d', err='%d'\n", m_tData, iErr );
				WriteUdLibLog( log );

				tTime.tm_sec = 0;
				tTime.tm_min = 0;
				tTime.tm_hour = 0;
				tTime.tm_mday = 1;
				tTime.tm_mon = 0;
				tTime.tm_wday = 4;
				tTime.tm_year = 70;
				tTime.tm_yday = 0;
				tTime.tm_isdst = 0;
			}
		}
		 
		udChar tmp[ 128 ];
		strftime( tmp, 128, "%Y%m%d%H%M%S", &tTime );
		szRet = tmp;

		year = toInt( szRet.substr( 0, 4 ) );
		month = toInt( szRet.substr( 4, 2 ) );
		day = toInt( szRet.substr( 6, 2 ) );
		hour = toInt( szRet.substr( 8, 2 ) );
		minute = toInt( szRet.substr( 10, 2 ) );
		second = toInt( szRet.substr( 12, 2 ) );
	} // void get

	bool operator ==( const CDatetime& d1, const CDatetime& d2 ) {
		return d1.get( ) == d2.get( );
	}

	bool operator !=( const CDatetime& d1, const CDatetime& d2 ) {
		return d1.get( ) != d2.get( );
	}

	bool operator <( const CDatetime& d1, const CDatetime& d2 ) {
		return d1.get( ) < d2.get( );
	}

	bool operator <=( const CDatetime& d1, const CDatetime& d2 ) {
		return d1.get( ) <= d2.get( );
	}

	bool operator >( const CDatetime& d1, const CDatetime& d2 ) {
		return d1.get( ) > d2.get( );
	}

	bool operator >=( const CDatetime& d1, const CDatetime& d2 ) {
		return d1.get( ) >= d2.get( );
	}

} // namespace UdSDK
