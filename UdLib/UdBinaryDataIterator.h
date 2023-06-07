/**
	UndeadCS SDK
	Базовая библиотека

	итератор по бинарным данным
*/
#ifndef UNDEADCS_SDK_LIBRARY_BINARY_DATA_ITERATOR_H
#define UNDEADCS_SDK_LIBRARY_BINARY_DATA_ITERATOR_H

namespace UdSDK {

	class CBinaryDataIterator {
		udPBinaryData	m_pData;	// указатель на объект бинарных данных
		udUInt			m_iPos;		// текущая позиция

	public:
		CBinaryDataIterator( );
		CBinaryDataIterator( const udPBinaryData pData, udUInt iPos = 0 );
		CBinaryDataIterator( const CBinaryDataIterator& objCopy );

		virtual ~CBinaryDataIterator( );

		CBinaryDataIterator& operator =( const CBinaryDataIterator& objCopy );

		udByte	operator *( );

		CBinaryDataIterator&	operator ++( );
		CBinaryDataIterator		operator ++( int );
		CBinaryDataIterator&	operator --( );
		CBinaryDataIterator		operator --( int );

		void	SetData( udPBinaryData pData );
		void	SetPos( udUInt pos );

		udPBinaryData	GetData( )	const { return m_pData;	}
		udUInt			GetPos( )	const { return m_iPos;	}

		bool	operator ==( const CBinaryDataIterator& r )	const;
		bool	operator !=( const CBinaryDataIterator& r )	const;
	};

} // namespace UdSDK

#endif
