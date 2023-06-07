/**
	UndeadCS SDK
	Базовая библиотека

	класс типа данных

	бинарный атрибут:
	дампится в бинарный вид в формате [размер в байтах long][последовательность байт]
	таким же образом читается из сокета
	в строку не дампится в чистом виде (а то может поломать вывод)
*/
#ifndef UDNEADCS_SDK_LIBRARY_TYPE_H
#define UDNEADCS_SDK_LIBRARY_TYPE_H

namespace UdSDK {

	class CUdType {
		typedef enum xtType {
			UDSDK_TYPE_UNKNOWN,		// нет типа
			UDSDK_TYPE_INT,			// int
			UDSDK_TYPE_STRING,		// string
			UDSDK_TYPE_FLOAT,		// float
			UDSDK_TYPE_DATETIME,	// datetime
			UDSDK_TYPE_DOUBLE,		// double
			UDSDK_TYPE_UINT,		// unsigned int
			UDSDK_TYPE_WORD,		// Word - unsigned short
			UDSDK_TYPE_DWORD,		// DWord - unsigned long
			UDSDK_TYPE_WSTRING,		// wstring
			UDSDK_TYPE_LLONG,		// long long
			UDSDK_TYPE_LONG,		// long
			UDSDK_TYPE_BINARY,		// BinaryData - бинарный атрибут
		} xtType;

		udPVoid	m_pData;	// указатель на начало данных
		udDWord	m_dwBytes;	// количество байт
		xtType	m_iType;	// тип инициализации данных

	public:
		CUdType( udPVoid pData = NULL );
		CUdType( const CUdType& objCopy );

		CUdType( udInt iIni );
		CUdType( udFloat fIni );
		CUdType( udDouble dIni );
		CUdType( udUInt iIni );
		CUdType( udWord wIni );
		CUdType( udDWord dwIni );
		CUdType( const string& szIni );
		CUdType( const wstring& szIni );
		CUdType( const udDatetime& tDatetime );
		CUdType( udLongLong lIni );
		CUdType( udLong lIni );
		CUdType( const udBinaryData& ini );

		virtual ~CUdType( );

		CUdType& operator =( const CUdType& objCopy );

		// инициализация
		void	ini( udInt					iIni	);
		void	ini( udFloat				fIni	);
		void	ini( udDouble				dIni	);
		void	ini( udUInt					iIni	);
		void	ini( udWord					wIni	);
		void	ini( udDWord				dwIni	);
		void	ini( const string&			szIni	);
		void	ini( const wstring&			szIni	);
		void	ini( const udDatetime&		tIni	);
		void	ini( udLongLong				lIni	);
		void	ini( udLong					lIni	);
		void	ini( const udBinaryData&	ini		);

		void	ini( udPVoid pData = NULL ) { m_pData = pData; }

		void	reset( );

		udInt	read( udPSocket sock );
		udInt	read( udBinaryDataIterator& itCur, const udBinaryDataIterator& itEnd );

		void	assign( udPType type, bool strict = false );

		// получение данных определенного типа
		udInt			toInt( )		const;
		udFloat			toFloat( )		const;
		udDouble		toDouble( )		const;
		udUInt			toUInt( )		const;
		udWord			toWord( )		const;
		udDWord			toDWord( )		const;
		string			toString( const string& szFormat = "" )		const;
		wstring			toWString( const wstring& szFormat = L"" )	const;
		udDatetime		toTime( )		const;
		udLongLong		toLLong( )		const;
		udLong			toLong( )		const;
		udBinaryData	toBinary( )		const;

		bool	isInt( )		const { return m_iType == CUdType::UDSDK_TYPE_INT;		}
		bool	isFloat( )		const { return m_iType == CUdType::UDSDK_TYPE_FLOAT;	}
		bool	isDouble( )		const { return m_iType == CUdType::UDSDK_TYPE_DOUBLE;	}
		bool	isUInt( )		const { return m_iType == CUdType::UDSDK_TYPE_UINT;		}
		bool	isWord( )		const { return m_iType == CUdType::UDSDK_TYPE_WORD;		}
		bool	isDWord( )		const { return m_iType == CUdType::UDSDK_TYPE_DWORD;	}
		bool	isString( )		const { return m_iType == CUdType::UDSDK_TYPE_STRING;	}
		bool	isWString( )	const { return m_iType == CUdType::UDSDK_TYPE_WSTRING;	}
		bool	isTime( )		const { return m_iType == CUdType::UDSDK_TYPE_DATETIME;	}
		bool	isLLong( )		const { return m_iType == CUdType::UDSDK_TYPE_LLONG;	}
		bool	isLong( )		const { return m_iType == CUdType::UDSDK_TYPE_LONG;		}
		bool	isBinary( )		const { return m_iType == CUdType::UDSDK_TYPE_BINARY;	}

		bool	isNull( )		const { return ( m_iType == CUdType::UDSDK_TYPE_UNKNOWN ) && ( !m_pData ); }

		udPVoid	get( )			const { return m_pData;	}
		xtType	type( )			const { return m_iType;	}

		void	dump( string& szOut ) const;
		void	dump( wstring& szOut ) const;
		void	dump( char* dst ) const;
		void	dump( udBinaryData& data ) const;
		void	dump( udPBinaryData data ) const;

		// присваивание
		CUdType& operator =( udInt			iIni		);
		CUdType& operator =( udFloat		fIni		);
		CUdType& operator =( udDouble		dIni		);
		CUdType& operator =( udUInt			iIni		);
		CUdType& operator =( udWord			wIni		);
		CUdType& operator =( udDWord		dwIni		);
		CUdType& operator =( const string&			szIni		);
		CUdType& operator =( const wstring&		szIni		);
		CUdType& operator =( const udDatetime&		tDatetime	);
		CUdType& operator =( udLongLong		lIni		);
		CUdType& operator =( udLong			lIni		);
		CUdType& operator =( const udBinaryData&	ini			);

	private:
		void	DelData( );
		void	NullData( );

	}; // class CUdType

} // namespace UdSDK

#endif
