
#include "includes.h"

namespace UdSDK {

	CUdPager::CUdPager( ) {
		m_iPage			= 0;
		m_iPageSize		= 0;
		m_iPageLength	= 0;
		m_iTotal		= 0;
	}

	CUdPager::CUdPager( udUInt iPage, udUInt iPageSize, udUInt iPageLength, udUInt iTotal ) {
		m_iPage			= iPage;
		m_iPageSize		= iPageSize;
		m_iPageLength	= iPageLength;
		m_iTotal		= iTotal;
	}

	CUdPager::CUdPager( const CUdPager& objCopy ) {
		m_iPage			= objCopy.m_iPage;
		m_iPageSize		= objCopy.m_iPageSize;
		m_iPageLength	= objCopy.m_iPageLength;
		m_iTotal		= objCopy.m_iTotal;
	}

	CUdPager::~CUdPager( ) {
		m_iPage			= 0;
		m_iPageSize		= 0;
		m_iPageLength	= 0;
		m_iTotal		= 0;
	}

	string CUdPager::GetSQLLimit( ) const {
		udChar tmp[ 128 ];
		sprintf_s( tmp, 128, "%d,%d", ( m_iPage - 1 ) * m_iPageSize, m_iPageSize );
		return string( tmp );
	} // string GetSQLLimit

	void CUdPager::SetPage( udUInt iPage ) {
		m_iPage = iPage;

		if ( m_iPage < 1 ) {
			m_iPage = 1;
		}
	} // void SetPage

	void CUdPager::SetPageSize( udUInt iPageSize ) {
		m_iPageSize = iPageSize;
	} // void SetPageSize

	void CUdPager::SetPageLength( udUInt iPageLength ) {
		m_iPageLength = iPageLength;
	} // void SetPageLength

	void CUdPager::SetTotal( udUInt iTotal ) {
		m_iTotal = iTotal;
	} // void SetTotal

	CUdPager& CUdPager::operator=( const UdSDK::CUdPager &objCopy ) {
		m_iPage			= objCopy.m_iPage;
		m_iPageSize		= objCopy.m_iPageSize;
		m_iPageLength	= objCopy.m_iPageLength;
		m_iTotal		= objCopy.m_iTotal;

		return *this;
	}

} // namespace UdSDK