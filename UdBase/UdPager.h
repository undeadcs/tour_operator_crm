
#ifndef UDPAGER_H
#define UDPAGER_H

namespace UdSDK {

	/**
	 *	Пейджер
	 */
	class CUdPager {
		udUInt	m_iPage,			// текущая страница
				m_iPageSize,		// количество элементов на странице
				m_iPageLength,		// количество страниц для показа
				m_iTotal;			// общее количество сущностей

	public:
		CUdPager( );
		CUdPager( udUInt iPage, udUInt iPageSize, udUInt iPageLength, udUInt iTotal );
		CUdPager( const CUdPager& objCopy );

		~CUdPager( );

		string	GetSQLLimit( ) const;

		void	SetPage( udUInt iPage );
		void	SetPageSize( udUInt iPageSize );
		void	SetPageLength( udUInt iPageLength );
		void	SetTotal( udUInt iTotal );

		udUInt	GetPage( )			const { return m_iPage;			}
		udUInt	GetPageSize( )		const { return m_iPageSize;		}
		udUInt	GetPageLength( )	const { return m_iPageLength;	}
		udUInt	GetTotal( )			const { return m_iTotal;		}

		CUdPager& operator=( const CUdPager& objCopy );

	}; // class CUdPager

} // namespace UdSDK

#endif
