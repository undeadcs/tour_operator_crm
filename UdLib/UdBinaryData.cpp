
#include "includes.h"

namespace UdSDK {

	CBinaryData::CBinaryData( ) {
		m_pData		= NULL;
		m_iSize		= 0;
		m_iCapacity	= 0;
	}

	CBinaryData::CBinaryData( const CBinaryData& objCopy ) {
		m_pData		= NULL;
		m_iSize		= 0;
		m_iCapacity	= 0;

		if ( objCopy.m_pData ) {
			m_iSize		= objCopy.m_iSize;
			m_iCapacity	= objCopy.m_iCapacity;

			this->m_pData = new udByte[ m_iCapacity ];
			memset( this->m_pData, 0, m_iCapacity );
			memcpy( this->m_pData, objCopy.m_pData, m_iSize );
		}
	}

	CBinaryData::~CBinaryData( ) {
		udDELETE_( m_pData );
		m_iSize		= 0;
		m_iCapacity	= 0;
	}

	CBinaryData& CBinaryData::operator =( const CBinaryData& objCopy ) {
		if ( !m_pData || !objCopy.m_pData || ( m_pData == objCopy.m_pData ) ) { // нефиг друг друга копировать
			return *this;
		}
		// удаляем старые данные
		this->clear( );
		// копируем новые
		if ( objCopy.m_pData ) {
			m_iSize		= objCopy.m_iSize;
			m_iCapacity	= objCopy.m_iCapacity;

			this->m_pData = new udByte[ m_iCapacity ];
			memset( this->m_pData, 0, m_iCapacity );
			memcpy( this->m_pData, objCopy.m_pData, m_iSize );
		}
		return *this;
	}

	udInt CBinaryData::add( char v ) {
		udUInt size = sizeof( char );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, &v, size );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( udByte v ) {
		udUInt size = sizeof( udByte );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, &v, size );
		m_iSize += size;
		return 1;
	} // void add

	udInt CBinaryData::add( udInt v ) {
		udUInt size = sizeof( udInt );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, &v, size );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( udUInt v ) {
		udUInt size = sizeof( udUInt );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, &v, size );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( udLong v ) {
		udUInt size = sizeof( udLong );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, &v, size );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( const string& v ) {
		udUInt size = ( v.size( ) + 1 ) * sizeof( char );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, v.data( ), size - 1 * sizeof( char ) );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( const wstring& v ) {
		udUInt size = ( v.size( ) + 1 ) * sizeof( wchar_t );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, v.data( ), size - 1 * sizeof( wchar_t ) );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( const string* v ) {
		udUInt size = ( v->size( ) + 1 ) * sizeof( char );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, v->data( ), size - 1 * sizeof( char ) );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( const wstring* v ) {
		udUInt size = ( v->size( ) + 1 ) * sizeof( wchar_t );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, v->data( ), size - 1 * sizeof( wchar_t ) );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( udFloat v ) {
		udUInt size = sizeof( udFloat );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, &v, size );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( udDouble v ) {
		udUInt size = sizeof( udDouble );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, &v, size );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( udWord v ) {
		udUInt size = sizeof( udWord );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, &v, size );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( udDWord v ) {
		udUInt size = sizeof( udDWord );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, &v, size );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( const datetime& v ) {
		udUInt size = sizeof( time_t );
		this->CheckSize( size );
		time_t t = v.get( );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, &t, sizeof( time_t ) );
		m_iSize += sizeof( time_t );
		return 1;
	} // udInt add

	udInt CBinaryData::add( const pdatetime v ) {
		udUInt size = sizeof( time_t );
		this->CheckSize( size );
		time_t t = v->get( );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, &t, sizeof( time_t ) );
		m_iSize += sizeof( time_t );
		return 1;
	} // udInt add

	udInt CBinaryData::add( udLongLong v ) {
		udUInt size = sizeof( udLongLong );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, &v, size );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( const udBinaryData& data ) {
		udUInt size = data.size( );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, data.m_pData, size );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( udPBinaryData data ) {
		udUInt size = data->size( );
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, data->m_pData, size );
		m_iSize += size;
		return 1;
	} // udInt add

	udInt CBinaryData::add( udPByte bytes, udDWord size ) {
		this->CheckSize( size );
		udPByte _data = m_pData;
		_data += m_iSize;
		memcpy( _data, bytes, size );
		m_iSize += size;
		return 1;
	} // udInt add

	void CBinaryData::AddFrame( ) {
		m_iCapacity += ( udUInt ) frame_size;

		udPByte tmp = m_pData;
		m_pData = new udByte[ m_iCapacity ];
		memset( m_pData, 0, m_iCapacity );

		if ( tmp ) { // память надо скопировать
			memcpy( m_pData, tmp, m_iSize );
			delete [ ] tmp;
		}
	} // void AddFrame

	void CBinaryData::CheckSize( udUInt size ) {
		if ( ( m_iSize + size ) >= m_iCapacity ) {
			this->AddFrame( );

			if ( ( m_iSize + size ) >= m_iCapacity ) {
				// наращиваем объем, пока не хватит места под строку
				while( ( m_iSize + size ) >= m_iCapacity ) {
					this->AddFrame( );
				}
			}
		}
	} // void CheckSize

	void CBinaryData::clear( ) {
		udDELETE_( m_pData );
		m_iSize		= 0;
		m_iCapacity	= 0;
	} // void clear

	udByte CBinaryData::pos( udUInt pos ) const {
		if ( pos >= m_iSize ) {
			WriteUdLibLog( "CBinaryData::pos, invalid position\n" );
			return 0;
			//return invalid_byte;
		}
		return m_pData[ pos ];
	} // udByte pos

	udUInt CBinaryData::read( udPByte* data, udUInt iPos, udUInt iBytesRead ) {
		if ( m_iSize > 0 ) {
			udUInt read = iBytesRead;
			if ( ( iPos + iBytesRead ) >= m_iSize ) {
				read = m_iSize - iPos;
			}
			*data = new udByte[ read ];
			memcpy( *data, ( m_pData + iPos ), read );
			return read;
		}
		return 0;
	} // udUInt read

	udUInt CBinaryData::read( udPByte* data, const udBinaryDataIterator& itCur, udUInt iBytesRead ) {
		if ( m_iSize > 0 ) {
			udUInt read = iBytesRead, iPos = itCur.GetPos( );
			if ( ( iPos + iBytesRead ) >= m_iSize ) {
				read = m_iSize - iPos;
			}
			*data = new udByte[ read ];
			memcpy( *data, ( m_pData + iPos ), read );
			return read;
		}
		return 0;
	} // udUInt read

	CBinaryDataIterator CBinaryData::begin( ) const {
		return CBinaryDataIterator( ( const udPBinaryData ) this, 0 );
	} // CBinaryDataIterator begin

	CBinaryDataIterator CBinaryData::end( ) const {
		return CBinaryDataIterator( ( const udPBinaryData ) this, m_iSize );
	} // CBinaryDataIterator end

} // namespace UdSDK
