
#include "includes.h"

namespace UdSDK {

	CBinaryDataIterator::CBinaryDataIterator( ) {
		m_pData	= NULL;
		m_iPos	= 0;
	}

	CBinaryDataIterator::CBinaryDataIterator( const udPBinaryData pData, udUInt iPos ) {
		assert( pData );
		m_pData	= pData;
		m_iPos	= iPos;
	}

	CBinaryDataIterator::CBinaryDataIterator( const CBinaryDataIterator& objCopy ) {
		m_pData	= objCopy.m_pData;
		m_iPos	= objCopy.m_iPos;
	}

	CBinaryDataIterator::~CBinaryDataIterator( ) {
		m_pData	= NULL;
		m_iPos	= 0;
	}

	CBinaryDataIterator& CBinaryDataIterator::operator =( const CBinaryDataIterator& objCopy ) {
		m_pData	= objCopy.m_pData;
		m_iPos	= objCopy.m_iPos;
		return *this;
	}

	void CBinaryDataIterator::SetData( udPBinaryData pData ) {
		assert( pData );
		m_pData = pData;
	} // void SetData

	void CBinaryDataIterator::SetPos( udUInt pos ) {
		assert( m_pData );
		if ( pos < m_pData->size( ) ) {
			m_iPos = pos;
		}
	} // void SetPos

	udByte CBinaryDataIterator::operator *( ) {
		return m_pData->pos( m_iPos );
	} // udPByte operator *

	CBinaryDataIterator& CBinaryDataIterator::operator ++( ) {
		if ( m_iPos < m_pData->size( ) ) {
			++m_iPos;
		}
		return *this;
	} // CBinaryDataIterator& operator ++

	CBinaryDataIterator CBinaryDataIterator::operator ++( int ) {
		CBinaryDataIterator old( m_pData, m_iPos );
		if ( m_iPos < m_pData->size( ) ) {
			++m_iPos;
		}
		return old;
	} // CBinaryDataIterator& operator ++

	CBinaryDataIterator& CBinaryDataIterator::operator --( ) {
		if ( m_iPos > 0 ) {
			--m_iPos;
		}
		return *this;
	} // CBinaryDataIterator& operator --

	CBinaryDataIterator CBinaryDataIterator::operator --( int ) {
		CBinaryDataIterator old( m_pData, m_iPos );
		if ( m_iPos > 0 ) {
			--m_iPos;
		}
		return old;
	} // CBinaryDataIterator operator --

	bool CBinaryDataIterator::operator ==( const CBinaryDataIterator& r ) const {
		return ( m_pData == r.m_pData ) && ( m_iPos == r.m_iPos );
	} // bool operator ==

	bool CBinaryDataIterator::operator !=( const CBinaryDataIterator& r ) const {
		return ( m_pData != r.m_pData ) || ( m_iPos != r.m_iPos );
	} // bool operator !=

} // namespace UdSDK
