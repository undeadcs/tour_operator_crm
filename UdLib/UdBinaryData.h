/**
	UndeadCS SDK
	Базовая библиотека

	бинарные данные
*/
#ifndef UNDEADCS_SDK_LIBRARY_BINARY_DATA_H
#define UNDEADCS_SDK_LIBRARY_BINARY_DATA_H

namespace UdSDK {

	class CBinaryData {
	public:
		enum {
			frame_size	= 1024,
			//invalid_byte = ( udByte ) ( ~0 )
		};

	private:
		udPByte	m_pData;
		udUInt	m_iSize,
				m_iCapacity;

	public:
		CBinaryData( );
		CBinaryData( const CBinaryData& objCopy );

		virtual ~CBinaryData( );

		CBinaryData& operator =( const CBinaryData& objCopy );

		udInt	add( char v );
		udInt	add( udByte v );
		udInt	add( udInt v );
		udInt	add( udUInt v );
		udInt	add( udLong v );
		udInt	add( const string& v );
		udInt	add( const wstring& v );
		udInt	add( const string* v );
		udInt	add( const wstring* v );
		udInt	add( udFloat v );
		udInt	add( udDouble v );
		udInt	add( udWord v );
		udInt	add( udDWord v );
		udInt	add( const datetime& v );
		udInt	add( const pdatetime v );
		udInt	add( udLongLong v );
		udInt	add( const udBinaryData& data );
		udInt	add( udPBinaryData data );
		udInt	add( udPByte bytes, udDWord size );

		void	clear( );

		udByte	pos( udUInt pos ) const;
		udUInt	read( udPByte* data, udUInt iPos, udUInt iBytesRead );
		udUInt	read( udPByte* data, const udBinaryDataIterator& itCur, udUInt iBytesRead );

		udUInt	size( ) const { return m_iSize; }
		udPByte	get( ) const { return m_pData; }

		CBinaryDataIterator	begin( ) const;
		CBinaryDataIterator	end( ) const;

	private:
		void	AddFrame( );
		void	CheckSize( udUInt size );

	}; // class CBinaryData

} // namespace UdSDK

#endif
