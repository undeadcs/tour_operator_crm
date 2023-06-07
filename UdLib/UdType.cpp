
#include "includes.h"

namespace UdSDK {

	CUdType::CUdType( udPVoid pData ) {
		m_pData		= pData;
		m_dwBytes	= 0;
		m_iType		= CUdType::UDSDK_TYPE_UNKNOWN;
	}

	CUdType::CUdType( const CUdType& objCopy ) {
		this->NullData( );
		switch( objCopy.m_iType ) {
			case CUdType::UDSDK_TYPE_INT:
				this->ini( *( ( udPInt ) objCopy.m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_FLOAT:
				this->ini( *( ( udPFloat ) objCopy.m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DOUBLE:
				this->ini( *( ( udPDouble ) objCopy.m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_UINT:
				this->ini( *( ( udPUInt ) objCopy.m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_WORD:
				this->ini( *( ( udPWord ) objCopy.m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DWORD:
				this->ini( *( ( udPDWord ) objCopy.m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_STRING:
				this->ini( *( ( string* ) objCopy.m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_WSTRING:
				this->ini( *( ( wstring* ) objCopy.m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DATETIME:
				this->ini( *( ( udPDatetime ) objCopy.m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_LLONG:
				this->ini( *( ( udPLongLong ) objCopy.m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_LONG:
				this->ini( *( ( udPLong ) objCopy.m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_BINARY:
				this->ini( *( ( udPBinaryData ) objCopy.m_pData ) );
				break;

			default:
				break;
		}
	}

	CUdType::CUdType( udInt iIni ) {
		this->NullData( );
		this->ini( iIni );
	}

	CUdType::CUdType( udFloat fIni ) {
		this->NullData( );
		this->ini( fIni );
	}

	CUdType::CUdType( udDouble dIni ) {
		this->NullData( );
		this->ini( dIni );
	}

	CUdType::CUdType( udUInt iIni ) {
		this->NullData( );
		this->ini( iIni );
	}

	CUdType::CUdType( udWord wIni ) {
		this->NullData( );
		this->ini( wIni );
	}

	CUdType::CUdType( udDWord dwIni ) {
		this->NullData( );
		this->ini( dwIni );
	}

	CUdType::CUdType( const string& szIni ) {
		this->NullData( );
		this->ini( szIni );
	}

	CUdType::CUdType( const wstring& szIni ) {
		this->NullData( );
		this->ini( szIni );
	}

	CUdType::CUdType( const udDatetime& tDatetime ) {
		this->NullData( );
		this->ini( tDatetime );
	}

	CUdType::CUdType( udLongLong lIni ) {
		this->NullData( );
		this->ini( lIni );
	}

	CUdType::CUdType( const udBinaryData& ini ) {
		this->NullData( );
		this->ini( ini );
	}

	CUdType::~CUdType( ) {
		if ( m_pData ) {
			this->DelData( );
		}

		m_pData		= NULL;
		m_dwBytes	= 0;
		m_iType		= CUdType::UDSDK_TYPE_UNKNOWN;
	}

	void CUdType::ini( udInt iIni ) {
		if ( m_pData ) { // данные уже были занесены
			if ( m_iType == CUdType::UDSDK_TYPE_INT ) { // тип совпал, заменяем
				*( ( udPInt ) m_pData ) = iIni;
			} else {
				// тут действия, если типы не совпадают. переколбашиваем текущий объект
				this->DelData( );
				this->NullData( );
				this->ini( iIni );
			}
		} else { // данные не были занесены
			m_pData = new udInt;
			*( ( udPInt ) m_pData ) = iIni;
			m_iType		= CUdType::UDSDK_TYPE_INT;
			m_dwBytes	= sizeof( udInt );
		}
	} // void ini

	void CUdType::ini( udFloat fIni ) {
		if ( m_pData ) {
			if ( m_iType == CUdType::UDSDK_TYPE_FLOAT ) {
				*( ( udPFloat ) m_pData ) = fIni;
			} else {
				this->DelData( );
				this->NullData( );
				this->ini( fIni );
			}
		} else {
			m_pData = new udFloat;
			*( ( udPFloat ) m_pData ) = fIni;
			m_iType		= CUdType::UDSDK_TYPE_FLOAT;
			m_dwBytes	= sizeof( udFloat );
		}
	} // void ini

	void CUdType::ini( udDouble dIni ) {
		if ( m_pData ) {
			if ( m_iType == CUdType::UDSDK_TYPE_DOUBLE ) {
				*( ( udPDouble ) m_pData ) = dIni;
			} else {
				this->DelData( );
				this->NullData( );
				this->ini( dIni );
			}
		} else {
			m_pData = new udDouble;
			*( ( udPDouble ) m_pData ) = dIni;
			m_iType		= CUdType::UDSDK_TYPE_DOUBLE;
			m_dwBytes	= sizeof( udDouble );
		}
	} // void ini

	void CUdType::ini( udUInt iIni ) {
		if ( m_pData ) {
			if ( m_iType == CUdType::UDSDK_TYPE_UINT ) {
				*( ( udPUInt ) m_pData ) = iIni;
			} else {
				this->DelData( );
				this->NullData( );
				this->ini( iIni );
			}
		} else {
			m_pData = new udUInt;
			*( ( udPUInt ) m_pData ) = iIni;
			m_iType		= CUdType::UDSDK_TYPE_UINT;
			m_dwBytes	= sizeof( udUInt );
		}
	} // void ini

	void CUdType::ini( udWord wIni ) {
		if ( m_pData ) {
			if ( m_iType == CUdType::UDSDK_TYPE_WORD ) {
				*( ( udPWord ) m_pData ) = wIni;
			} else {
				this->DelData( );
				this->NullData( );
				this->ini( wIni );
			}
		} else {
			m_pData = new udWord;
			*( ( udPWord ) m_pData ) = wIni;
			m_iType		= CUdType::UDSDK_TYPE_WORD;
			m_dwBytes	= sizeof( udWord );
		}
	} // void ini

	void CUdType::ini( udDWord dwIni ) {
		if ( m_pData ) {
			if ( m_iType == CUdType::UDSDK_TYPE_DWORD ) {
				*( ( udPDWord ) m_pData ) = dwIni;
			} else {
				this->DelData( );
				this->NullData( );
				this->ini( dwIni );
			}
		} else {
			m_pData = new udDWord;
			*( ( udPDWord ) m_pData ) = dwIni;
			m_iType		= CUdType::UDSDK_TYPE_DWORD;
			m_dwBytes	= sizeof( udDWord );
		}
	} // void ini

	void CUdType::ini( const string& szIni ) {
		if ( m_pData ) {
			if ( m_iType == CUdType::UDSDK_TYPE_STRING ) {
				*( ( string* ) m_pData ) = szIni;
			} else {
				this->DelData( );
				this->NullData( );
				this->ini( szIni );
			}
		} else {
			m_pData = new string;
			*( ( string* ) m_pData ) = szIni;
			m_iType		= CUdType::UDSDK_TYPE_STRING;
			m_dwBytes	= szIni.size( );
		}
	} // void ini

	void CUdType::ini( const wstring& szIni ) {
		if ( m_pData ) {
			if ( m_iType == CUdType::UDSDK_TYPE_WSTRING ) {
				*( ( wstring* ) m_pData ) = szIni;
			} else {
				this->DelData( );
				this->NullData( );
				this->ini( szIni );
			}
		} else {
			m_pData = new wstring;
			*( ( wstring* ) m_pData ) = szIni;
			m_iType		= CUdType::UDSDK_TYPE_WSTRING;
			m_dwBytes	= szIni.size( ) * sizeof( wchar_t );
		}
	} // void ini

	void CUdType::ini( const udDatetime& tIni ) {
		if ( m_pData ) {
			if ( m_iType == CUdType::UDSDK_TYPE_DATETIME ) {
				*( ( udPDatetime ) m_pData ) = tIni;
			} else {
				this->DelData( );
				this->NullData( );
				this->ini( tIni );
			}
		} else {
			m_pData = new udDatetime;
			*( ( udPDatetime ) m_pData ) = tIni;
			m_iType		= CUdType::UDSDK_TYPE_DATETIME;
			m_dwBytes	= sizeof( udDatetime );
		}
	} // void ini

	void CUdType::ini( udLongLong lIni ) {
		if ( m_pData ) {
			if ( m_iType == CUdType::UDSDK_TYPE_LLONG ) {
				*( ( udPLongLong ) m_pData ) = lIni;
			} else {
				this->DelData( );
				this->NullData( );
				this->ini( lIni );
			}
		} else {
			m_pData = new udLongLong;
			*( ( udPLongLong ) m_pData ) = lIni;
			m_iType		= CUdType::UDSDK_TYPE_LLONG;
			m_dwBytes	= sizeof( udLongLong );
		}
	} // void ini

	void CUdType::ini( udLong lIni ) {
		if ( m_pData ) {
			if ( m_iType == CUdType::UDSDK_TYPE_LONG ) {
				*( ( udPLong ) m_pData ) = lIni;
			} else {
				this->DelData( );
				this->NullData( );
				this->ini( lIni );
			}
		} else {
			m_pData = new udLong;
			*( ( udPLong ) m_pData ) = lIni;
			m_iType		= CUdType::UDSDK_TYPE_LONG;
			m_dwBytes	= sizeof( udLong );
		}
	} // void ini

	void CUdType::ini( const udBinaryData& ini ) {
		char tmp[ 256 ];
		WriteUdLibLog( "CUdType::ini( udBinaryData ini )\n" );
		if ( m_pData ) {
			sprintf_s( tmp, 256, "адрес данных: %p\n", m_pData );
			WriteUdLibLog( tmp );
			WriteUdLibLog( "CUdType::ini( udBinaryData ini ) 1\n" );
			if ( m_iType == CUdType::UDSDK_TYPE_BINARY ) {
				*( ( udPBinaryData ) m_pData ) = ini;
			} else {
				WriteUdLibLog( "CUdType::ini( udBinaryData ini ) 3\n" );
				this->DelData( );
				this->NullData( );
				this->ini( ini );
			}
		} else {
			WriteUdLibLog( "CUdType::ini( udBinaryData ini ) 2\n" );
			m_pData = new udBinaryData;
			sprintf_s( tmp, 256, "адрес данных: %p\n", m_pData );
			WriteUdLibLog( tmp );
			( ( udPBinaryData ) m_pData )->add( ini );
			//*( ( udPBinaryData ) m_pData ) = ini;
			m_iType		= CUdType::UDSDK_TYPE_BINARY;
			m_dwBytes	= sizeof( udBinaryData );
		}
		WriteUdLibLog( "CUdType::ini( udBinaryData ini ) exit\n" );
	} // void ini

	udInt CUdType::toInt( ) const {
		switch( m_iType ) {
			case CUdType::UDSDK_TYPE_INT:
				return *( ( udPInt ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_FLOAT:
				return ( udInt ) ( *( ( udPFloat ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DATETIME:
				return ( udInt ) ( ( ( udPDatetime ) m_pData )->get( ) );
				break;

			case CUdType::UDSDK_TYPE_DOUBLE:
				return ( udInt ) ( *( ( udPDouble ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_UINT:
				return ( udInt ) ( *( ( udPUInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_WORD:
				return ( udInt ) ( *( ( udPWord ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DWORD:
				return ( udInt ) ( *( ( udPDWord ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_STRING:
				{
					udInt	iRet		= 0;
					string	*szValue	= ( string* ) m_pData;

					if ( szValue->size( ) ) {
						sscanf_s( szValue->c_str( ), "%d", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_WSTRING:
				{
					udInt	iRet		= 0;
					wstring	*szValue	= ( wstring* ) m_pData;

					if ( szValue->size( ) ) {
						swscanf_s( szValue->c_str( ), L"%d", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_LLONG:
				return ( udInt ) *( ( udPLongLong ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_LONG:
				return ( udInt ) *( ( udPLong ) m_pData );
				break;

			default:
				break;
		}

		return 0;
	} // udInt

	udFloat CUdType::toFloat( ) const {
		switch( m_iType ) {
			case CUdType::UDSDK_TYPE_INT:
				return ( udFloat ) ( *( ( udPInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_FLOAT:
				return *( ( udPFloat ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_DATETIME:
				return ( udFloat ) ( ( ( udPDatetime ) m_pData )->get( ) );
				break;

			case CUdType::UDSDK_TYPE_DOUBLE:
				return ( udFloat ) ( *( ( udPDouble ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_UINT:
				return ( udFloat ) ( *( ( udPUInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_WORD:
				return ( udFloat ) ( *( ( udPWord ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DWORD:
				return ( udFloat ) ( *( ( udPDWord ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_STRING:
				{
					udFloat	iRet		= 0;
					string	*szValue	= ( string* ) m_pData;

					if ( szValue->size( ) ) {
						sscanf_s( szValue->c_str( ), "%f", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_WSTRING:
				{
					udFloat	iRet		= 0;
					wstring	*szValue	= ( wstring* ) m_pData;

					if ( szValue->size( ) ) {
						swscanf_s( szValue->c_str( ), L"%f", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_LLONG:
				return ( udFloat ) *( ( udPLongLong ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_LONG:
				return ( udFloat ) *( ( udPLong ) m_pData );
				break;

			default:
				break;
		}

		return 0.0f;
	} // udFloat toFloat

	udDouble CUdType::toDouble( ) const {
		switch( m_iType ) {
			case CUdType::UDSDK_TYPE_INT:
				return ( udDouble ) ( *( ( udPInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_FLOAT:
				return ( udDouble ) ( *( ( udPFloat ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DATETIME:
				return ( udDouble ) ( ( ( udPDatetime ) m_pData )->get( ) );
				break;

			case CUdType::UDSDK_TYPE_DOUBLE:
				return *( ( udPDouble ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_UINT:
				return ( udDouble ) ( *( ( udPUInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_WORD:
				return ( udDouble ) ( *( ( udPWord ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DWORD:
				return ( udDouble ) ( *( ( udPDWord ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_STRING:
				{
					udDouble	iRet		= 0;
					string		*szValue	= ( string* ) m_pData;

					if ( szValue->size( ) ) {
						sscanf_s( szValue->c_str( ), "%lf", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_WSTRING:
				{
					udDouble	iRet		= 0;
					wstring		*szValue	= ( wstring* ) m_pData;

					if ( szValue->size( ) ) {
						swscanf_s( szValue->c_str( ), L"%lf", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_LLONG:
				return ( udDouble ) *( ( udPLongLong ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_LONG:
				return ( udDouble ) *( ( udPLong ) m_pData );
				break;

			default:
				break;
		}

		return 0.0;
	} // udDouble toDouble

	udUInt CUdType::toUInt( ) const {
		switch( m_iType ) {
			case CUdType::UDSDK_TYPE_INT:
				return ( udUInt ) ( *( ( udPInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_FLOAT:
				return ( udUInt ) ( *( ( udPFloat ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DATETIME:
				return ( udUInt ) ( ( ( udPDatetime ) m_pData )->get( ) );
				break;

			case CUdType::UDSDK_TYPE_DOUBLE:
				return ( udUInt ) *( ( udPDouble ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_UINT:
				return *( ( udPUInt ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_WORD:
				return ( udUInt ) ( *( ( udPWord ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DWORD:
				return ( udUInt ) ( *( ( udPDWord ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_STRING:
				{
					udUInt	iRet		= 0;
					string	*szValue	= ( string* ) m_pData;

					if ( szValue->size( ) ) {
						sscanf_s( szValue->c_str( ), "%u", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_WSTRING:
				{
					udUInt	iRet		= 0;
					wstring	*szValue	= ( wstring* ) m_pData;

					if ( szValue->size( ) ) {
						swscanf_s( szValue->c_str( ), L"%u", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_LLONG:
				return ( udUInt ) *( ( udPLongLong ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_LONG:
				return ( udUInt ) *( ( udPLong ) m_pData );
				break;

			default:
				break;
		}

		return 0;
	} // udUInt toUInt

	udWord CUdType::toWord( ) const {
		switch( m_iType ) {
			case CUdType::UDSDK_TYPE_INT:
				return ( udWord ) ( *( ( udPInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_FLOAT:
				return ( udWord ) ( *( ( udPFloat ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DATETIME:
				return ( udWord ) ( ( ( udPDatetime ) m_pData )->get( ) );
				break;

			case CUdType::UDSDK_TYPE_DOUBLE:
				return ( udWord ) *( ( udPDouble ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_UINT:
				return ( udWord ) *( ( udPUInt ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_WORD:
				return *( ( udPWord ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_DWORD:
				return ( udWord ) ( *( ( udPDWord ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_STRING:
				{
					udWord	iRet		= 0;
					string	*szValue	= ( string* ) m_pData;

					if ( szValue->size( ) ) {
						sscanf_s( szValue->c_str( ), "%u", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_WSTRING:
				{
					udWord	iRet		= 0;
					wstring	*szValue	= ( wstring* ) m_pData;

					if ( szValue->size( ) ) {
						swscanf_s( szValue->c_str( ), L"%u", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_LLONG:
				return ( udWord ) *( ( udPLongLong ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_LONG:
				return ( udWord ) *( ( udPLong ) m_pData );
				break;

			default:
				break;
		}

		return 0;
	} // udWord

	udDWord CUdType::toDWord( ) const {
		switch( m_iType ) {
			case CUdType::UDSDK_TYPE_INT:
				return ( udDWord ) ( *( ( udPInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_FLOAT:
				return ( udDWord ) ( *( ( udPFloat ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DATETIME:
				return ( udDWord ) ( ( ( udPDatetime ) m_pData )->get( ) );
				break;

			case CUdType::UDSDK_TYPE_DOUBLE:
				return ( udDWord ) *( ( udPDouble ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_UINT:
				return ( udDWord ) ( *( ( udPUInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_WORD:
				return ( udDWord ) ( *( ( udPWord ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DWORD:
				return *( ( udPDWord ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_STRING:
				{
					udDWord	iRet		= 0;
					string	*szValue	= ( string* ) m_pData;

					if ( szValue->size( ) ) {
						sscanf_s( szValue->c_str( ), "%u", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_WSTRING:
				{
					udDWord	iRet		= 0;
					wstring	*szValue	= ( wstring* ) m_pData;

					if ( szValue->size( ) ) {
						swscanf_s( szValue->c_str( ), L"%u", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_LLONG:
				return ( udDWord ) *( ( udPLongLong ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_LONG:
				return ( udDWord ) *( ( udPLong ) m_pData );
				break;

			default:
				break;
		}

		return 0;
	} // udDWord toDWord

	string CUdType::toString( const string& szFormat ) const {
		//udChar	tmp[ 128 ];
		//string	szRet( "" );

		switch( m_iType ) {
			case CUdType::UDSDK_TYPE_INT:
				if ( szFormat.empty( ) ) {
					return UdSDK::toString( *( ( udPInt ) m_pData ) );
				} else {
					return UdSDK::toString( *( ( udPInt ) m_pData ), szFormat.data( ) );
				}
				//sprintf_s( tmp, 128, "%d", *( ( udPInt ) m_pData ) );
				//szRet = tmp;
				break;

			case CUdType::UDSDK_TYPE_FLOAT:
				if ( szFormat.empty( ) ) {
					return UdSDK::toString( *( ( udPFloat ) m_pData ) );
				} else {
					return UdSDK::toString( *( ( udPFloat ) m_pData ), szFormat.data( ) );
				}
				//sprintf_s( tmp, 128, "%.5f", *( ( udPFloat ) m_pData ) );
				//szRet = tmp;
				break;

			case CUdType::UDSDK_TYPE_DOUBLE:
				if ( szFormat.empty( ) ) {
					return UdSDK::toString( *( ( udPDouble ) m_pData ) );
				} else {
					return UdSDK::toString( *( ( udPDouble ) m_pData ), szFormat.data( ) );
				}
				//sprintf_s( tmp, 128, "%.5f", *( ( udPDouble ) m_pData ) );
				//szRet = tmp;
				break;

			case CUdType::UDSDK_TYPE_UINT:
				if ( szFormat.empty( ) ) {
					return UdSDK::toString( *( ( udPUInt ) m_pData ) );
				} else {
					return UdSDK::toString( *( ( udPUInt ) m_pData ), szFormat.data( ) );
				}
				//sprintf_s( tmp, 128, "%u", *( ( udPUInt ) m_pData ) );
				//szRet = tmp;
				break;

			case CUdType::UDSDK_TYPE_WORD:
				if ( szFormat.empty( ) ) {
					return UdSDK::toString( *( ( udPWord ) m_pData ) );
				} else {
					return UdSDK::toString( *( ( udPWord ) m_pData ), szFormat.data( ) );
				}
				//sprintf_s( tmp, 128, "%hu", *( ( udPWord ) m_pData ) );
				//szRet = tmp;
				break;

			case CUdType::UDSDK_TYPE_DWORD:
				if ( szFormat.empty( ) ) {
					return UdSDK::toString( *( ( udPDWord ) m_pData ) );
				} else {
					return UdSDK::toString( *( ( udPDWord ) m_pData ), szFormat.data( ) );
				}
				//sprintf_s( tmp, 128, "%lu", *( ( udPDWord ) m_pData ) );
				//szRet = tmp;
				break;

			case CUdType::UDSDK_TYPE_STRING:
				return *( ( string* ) m_pData );
				//szRet = *( ( string* ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_WSTRING:
				return ConvertString( *( ( wstring* ) m_pData ) );
				//szRet = ConvertString( *( ( wstring* ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DATETIME:
				if ( szFormat.empty( ) ) {
					return ( ( udPDatetime ) m_pData )->toString( "%Y-%m-%d %H:%M:%S" );
				} else {
					return ( ( udPDatetime ) m_pData )->toString( szFormat );
				}
				//szRet = ( ( udPDatetime ) m_pData )->toString( "%Y-%m-%d %H:%M:%S" );
				break;

			case CUdType::UDSDK_TYPE_LLONG:
				if ( szFormat.empty( ) ) {
					return UdSDK::toString( *( ( udPLongLong ) m_pData ) );
				} else {
					return UdSDK::toString( *( ( udPLongLong ) m_pData ), szFormat.data( ) );
				}
				//sprintf_s( tmp, 128, "%I64d", *( ( udPLongLong ) m_pData ) );
				//szRet = tmp;
				break;

			case CUdType::UDSDK_TYPE_LONG:
				if ( szFormat.empty( ) ) {
					return UdSDK::toString( *( ( udPLong ) m_pData ) );
				} else {
					return UdSDK::toString( *( ( udPLong ) m_pData ), szFormat.data( ) );
				}
				//sprintf_s( tmp, 128, "%ld", *( ( udPLong ) m_pData ) );
				//szRet = tmp;
				break;

			default:
				break;
		}

		return string( "" );
		//return szRet;
	} // string toString

	wstring CUdType::toWString( const wstring& szFormat ) const {
		//udWChar	tmp[ 128 ];
		//wstring	szRet( L"" );

		switch( m_iType ) {
			case CUdType::UDSDK_TYPE_INT:
				if ( szFormat.empty( ) ) {
					return UdSDK::toWString( *( ( udPInt ) m_pData ) );
				} else {
					return UdSDK::toWString( *( ( udPInt ) m_pData ), szFormat.data( ) );
				}
				//swprintf_s( tmp, 128, L"%d", *( ( udPInt ) m_pData ) );
				//szRet = tmp;
				break;

			case CUdType::UDSDK_TYPE_FLOAT:
				if ( szFormat.empty( ) ) {
					return UdSDK::toWString( *( ( udPFloat ) m_pData ) );
				} else {
					return UdSDK::toWString( *( ( udPFloat ) m_pData ), szFormat.data( ) );
				}
				//swprintf_s( tmp, 128, L"%.5f", *( ( udPFloat ) m_pData ) );
				//szRet = tmp;
				break;

			case CUdType::UDSDK_TYPE_DOUBLE:
				if ( szFormat.empty( ) ) {
					return UdSDK::toWString( *( ( udPDouble ) m_pData ) );
				} else {
					return UdSDK::toWString( *( ( udPDouble ) m_pData ), szFormat.data( ) );
				}
				//swprintf_s( tmp, 128, L"%.5f", *( ( udPDouble ) m_pData ) );
				//szRet = tmp;
				break;

			case CUdType::UDSDK_TYPE_UINT:
				if ( szFormat.empty( ) ) {
					return UdSDK::toWString( *( ( udPUInt ) m_pData ) );
				} else {
					return UdSDK::toWString( *( ( udPUInt ) m_pData ), szFormat.data( ) );
				}
				//swprintf_s( tmp, 128, L"%u", *( ( udPUInt ) m_pData ) );
				//szRet = tmp;
				break;

			case CUdType::UDSDK_TYPE_WORD:
				if ( szFormat.empty( ) ) {
					return UdSDK::toWString( *( ( udPWord ) m_pData ) );
				} else {
					return UdSDK::toWString( *( ( udPWord ) m_pData ), szFormat.data( ) );
				}
				//swprintf_s( tmp, 128, L"%hu", *( ( udPWord ) m_pData ) );
				//szRet = tmp;
				break;

			case CUdType::UDSDK_TYPE_DWORD:
				if ( szFormat.empty( ) ) {
					return UdSDK::toWString( *( ( udPDWord ) m_pData ) );
				} else {
					return UdSDK::toWString( *( ( udPDWord ) m_pData ), szFormat.data( ) );
				}
				//swprintf_s( tmp, 128, L"%lu", *( ( udPDWord ) m_pData ) );
				//szRet = tmp;
				break;

			case CUdType::UDSDK_TYPE_STRING:
				return ConvertString( *( ( string* ) m_pData ) );
				//szRet = ConvertString( *( ( string* ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_WSTRING:
				return *( ( wstring* ) m_pData );
				//szRet = *( ( wstring* ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_DATETIME:
				if ( szFormat.empty( ) ) {
					return ( ( udPDatetime ) m_pData )->toWString( "%Y-%m-%d %H:%M:%S" );
				} else {
					return ( ( udPDatetime ) m_pData )->toWString( ConvertString( szFormat ) );
				}
				//szRet = ConvertString( ( ( udPDatetime ) m_pData )->toString( "%Y-%m-%d %H:%M:%S" ) );
				break;

			case CUdType::UDSDK_TYPE_LLONG:
				if ( szFormat.empty( ) ) {
					return UdSDK::toWString( *( ( udPLongLong ) m_pData ) );
				} else {
					return UdSDK::toWString( *( ( udPLongLong ) m_pData ), szFormat.data( ) );
				}
				//swprintf_s( tmp, 128, L"%I64d", *( ( udPLongLong ) m_pData ) );
				//szRet = tmp;
				break;

			case CUdType::UDSDK_TYPE_LONG:
				if ( szFormat.empty( ) ) {
					return UdSDK::toWString( *( ( udPLong ) m_pData ) );
				} else {
					return UdSDK::toWString( *( ( udPLong ) m_pData ), szFormat.data( ) );
				}
				//swprintf_s( tmp, 128, L"%ld", *( ( udPLong ) m_pData ) );
				//szRet = tmp;
				break;

			default:
				break;
		}

		return wstring( L"" );
		//return szRet;
	} // wstring toWString

	udDatetime CUdType::toTime( ) const {
		if ( m_iType == CUdType::UDSDK_TYPE_DATETIME ) {
			return *( ( udPDatetime ) m_pData );
		}
		return udDatetime( );
	} // udDatetime toTime

	udLongLong CUdType::toLLong( ) const {
		switch( m_iType ) {
			case CUdType::UDSDK_TYPE_INT:
				return ( udLongLong ) ( *( ( udPInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_STRING:
				{
					udLongLong	iRet	= 0;
					string	*szValue	= ( string* ) m_pData;

					if ( szValue->size( ) ) {
						sscanf_s( szValue->c_str( ), "%I64d", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_FLOAT:
				return ( udLongLong ) ( *( ( udPFloat ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DATETIME:
				return ( udLongLong ) ( ( ( udPDatetime ) m_pData )->get( ) );
				break;

			case CUdType::UDSDK_TYPE_DOUBLE:
				return ( udLongLong ) *( ( udPDouble ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_UINT:
				return ( udLongLong ) ( *( ( udPUInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_WORD:
				return ( udLongLong ) ( *( ( udPWord ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DWORD:
				return ( udLongLong ) *( ( udPDWord ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_WSTRING:
				{
					udLongLong	iRet	= 0;
					wstring	*szValue	= ( wstring* ) m_pData;

					if ( szValue->size( ) ) {
						swscanf_s( szValue->c_str( ), L"%I64d", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_LLONG:
				return *( ( udPLongLong ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_LONG:
				return ( udLongLong ) *( ( udPLong ) m_pData );
				break;

			default:
				break;
		}

		return 0L;
	} // udLongLong toLLong

	udLong CUdType::toLong( ) const {
		switch( m_iType ) {
			case CUdType::UDSDK_TYPE_INT:
				return ( udLong ) ( *( ( udPInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_STRING:
				{
					udLong	iRet		= 0;
					string	*szValue	= ( string* ) m_pData;

					if ( szValue->size( ) ) {
						sscanf_s( szValue->c_str( ), "%ld", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_FLOAT:
				return ( udLong ) ( *( ( udPFloat ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DATETIME:
				return ( udLong ) ( ( ( udPDatetime ) m_pData )->get( ) );
				break;

			case CUdType::UDSDK_TYPE_DOUBLE:
				return ( udLong ) *( ( udPDouble ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_UINT:
				return ( udLong ) ( *( ( udPUInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_WORD:
				return ( udLong ) ( *( ( udPWord ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DWORD:
				return ( udLong ) *( ( udPDWord ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_WSTRING:
				{
					udLong	iRet		= 0;
					wstring	*szValue	= ( wstring* ) m_pData;

					if ( szValue->size( ) ) {
						swscanf_s( szValue->c_str( ), L"%ld", &iRet );
						return iRet;
					}
				}
				break;

			case CUdType::UDSDK_TYPE_LLONG:
				return ( udLong ) *( ( udPLongLong ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_LONG:
				return *( ( udPLong ) m_pData );
				break;

			default:
				break;
		}

		return 0L;
	} // udLong toLong

	udBinaryData CUdType::toBinary( ) const {
		WriteUdLibLog( "CUdType::toBinary( )\n" );

		udPBinaryData data = new udBinaryData;
		WriteUdLibLog( "type=" + UdSDK::toString( m_iType ) + "\n" );

		switch( m_iType ) {
			case CUdType::UDSDK_TYPE_INT:
				//data.add( *( ( udPInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_STRING:
				//data.add( ( string* ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_FLOAT:
				//data.add( *( ( udPFloat ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DATETIME:
				//data.add( ( ( udPDatetime ) m_pData )->get( ) );
				break;

			case CUdType::UDSDK_TYPE_DOUBLE:
				//data.add( *( ( udPDouble ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_UINT:
				//data.add( *( ( udPUInt ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_WORD:
				//data.add( *( ( udPWord ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_DWORD:
				//data.add( *( ( udPDWord ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_WSTRING:
				//data.add( ( wstring* ) m_pData );
				break;

			case CUdType::UDSDK_TYPE_LLONG:
				//data.add( *( ( udPLongLong ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_LONG:
				//data.add( *( ( udPLong ) m_pData ) );
				break;

			case CUdType::UDSDK_TYPE_BINARY:
				data->add( ( udPBinaryData ) m_pData );
				//data = *( ( udPBinaryData ) m_pData );
				break;

			default:
				break;
		}

		WriteUdLibLog( "CUdType::toBinary( ) exit\n" );

		char tmp[ 256 ];
		sprintf_s( tmp, 256, "данные перед возвратом: %p, %d\n", data->get( ), data->size( ) );
		WriteUdLibLog( tmp );

		return *data;
	} // udBinaryData toBinary

	void CUdType::DelData( ) {
		switch( m_iType ) {
			case CUdType::UDSDK_TYPE_UNKNOWN:
				break;

			case CUdType::UDSDK_TYPE_INT:
				delete ( udPInt ) m_pData;
				break;

			case CUdType::UDSDK_TYPE_STRING:
				delete ( string* ) m_pData;
				break;

			case CUdType::UDSDK_TYPE_FLOAT:
				delete ( udPFloat ) m_pData;
				break;

			case CUdType::UDSDK_TYPE_DATETIME:
				delete ( udPDatetime ) m_pData;
				break;

			case CUdType::UDSDK_TYPE_DOUBLE:
				delete ( udPDouble ) m_pData;
				break;

			case CUdType::UDSDK_TYPE_UINT:
				delete ( udPUInt ) m_pData;
				break;

			case CUdType::UDSDK_TYPE_WORD:
				delete ( udPWord ) m_pData;
				break;

			case CUdType::UDSDK_TYPE_DWORD:
				delete ( udPDWord ) m_pData;
				break;

			case CUdType::UDSDK_TYPE_WSTRING:
				delete ( wstring* ) m_pData;
				break;

			case CUdType::UDSDK_TYPE_LLONG:
				delete ( udPLongLong ) m_pData;
				break;

			case CUdType::UDSDK_TYPE_LONG:
				delete ( udPLong ) m_pData;
				break;

			case CUdType::UDSDK_TYPE_BINARY:
				delete ( udPBinaryData ) m_pData;
				break;

			default: // тип нам не известен, его удалением должен заниматься внешний код
				break;
		}

		m_pData = NULL;
	} // void DelData

	void CUdType::NullData( ) {
		m_pData		= NULL;
		m_dwBytes	= 0;
		m_iType		= CUdType::UDSDK_TYPE_UNKNOWN;
	} // void NullData
//*
	CUdType& CUdType::operator=( udInt iIni ) {
		this->ini( iIni );
		return *this;
	} // CUdType& operator=

	CUdType& CUdType::operator=( udFloat fIni ) {
		this->ini( fIni );
		return *this;
	} // CUdType& operator=

	CUdType& CUdType::operator =( udDouble dIni ) {
		this->ini( dIni );
		return *this;
	} // CUdType& operator=

	CUdType& CUdType::operator =( udUInt iIni ) {
		this->ini( iIni );
		return *this;
	} // CUdType& operator=

	CUdType& CUdType::operator=( udWord wIni ) {
		this->ini( wIni );
		return *this;
	} // CUdType& operator=

	CUdType& CUdType::operator=( udDWord dwIni ) {
		this->ini( dwIni );
		return *this;
	} // CUdType& operator=

	CUdType& CUdType::operator=( const string& szIni ) {
		this->ini( szIni );
		return *this;
	} // CUdType& operator=

	CUdType& CUdType::operator=( const wstring& szIni ) {
		this->ini( szIni );
		return *this;
	} // CUdType& operator=

	CUdType& CUdType::operator=( const udDatetime& tDatetime ) {
		this->ini( tDatetime );
		return *this;
	} // CUdType& operator=

	CUdType& CUdType::operator=( udLongLong lIni ) {
		this->ini( lIni );
		return *this;
	} // CUdType& operator=

	CUdType& CUdType::operator=( udLong lIni ) {
		this->ini( lIni );
		return *this;
	} // CUdType& operator=

	CUdType& CUdType::operator=( const udBinaryData& ini ) {
		this->ini( ini );
		return *this;
	} // CUdType& operator=

	CUdType& CUdType::operator=( const CUdType& objCopy ) {
		if ( m_iType == objCopy.m_iType ) { // равны, то тупо копируем
			switch( m_iType ) {
				case CUdType::UDSDK_TYPE_INT:
					*( ( udPInt ) m_pData ) = *( ( udPInt ) objCopy.m_pData );
					break;

				case CUdType::UDSDK_TYPE_FLOAT:
					*( ( udPFloat ) m_pData ) = *( ( udPFloat ) objCopy.m_pData );
					break;

				case CUdType::UDSDK_TYPE_DOUBLE:
					*( ( udPDouble ) m_pData ) = *( ( udPDouble ) objCopy.m_pData );
					break;

				case CUdType::UDSDK_TYPE_UINT:
					*( ( udPUInt ) m_pData ) = *( ( udPUInt ) objCopy.m_pData );
					break;

				case CUdType::UDSDK_TYPE_WORD:
					*( ( udPWord ) m_pData ) = *( ( udPWord ) objCopy.m_pData );
					break;

				case CUdType::UDSDK_TYPE_DWORD:
					*( ( udPDWord ) m_pData ) = *( ( udPDWord ) objCopy.m_pData );
					break;

				case CUdType::UDSDK_TYPE_STRING:
					*( ( string* ) m_pData ) = *( ( string* ) objCopy.m_pData );
					break;

				case CUdType::UDSDK_TYPE_WSTRING:
					*( ( wstring* ) m_pData ) = *( ( wstring* ) objCopy.m_pData );
					break;

				case CUdType::UDSDK_TYPE_DATETIME:
					*( ( udPDatetime ) m_pData ) = *( ( udPDatetime ) objCopy.m_pData );
					break;

				case CUdType::UDSDK_TYPE_LLONG:
					*( ( udPLongLong ) m_pData ) = *( ( udPLongLong ) objCopy.m_pData );
					break;

				case CUdType::UDSDK_TYPE_LONG:
					*( ( udPLong ) m_pData ) = *( ( udPLong ) objCopy.m_pData );
					break;

				case CUdType::UDSDK_TYPE_BINARY:
					*( ( udPBinaryData ) m_pData ) = *( ( udPBinaryData ) m_pData );
					break;

				default:
					break;
			}
		} else if ( m_iType == CUdType::UDSDK_TYPE_UNKNOWN ) {
			this->NullData( );	// а вдруг проскользнет m_pData != NULL
			// объект слева не создан, а справа создан
			switch( objCopy.m_iType ) {
				case CUdType::UDSDK_TYPE_INT:
					this->ini( *( ( udPInt ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_STRING:
					this->ini( *( ( string* ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_DATETIME:
					this->ini( *( ( udPDatetime ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_FLOAT:
					this->ini( *( ( udPFloat ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_DOUBLE:
					this->ini( *( ( udPDouble ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_UINT:
					this->ini( *( ( udPUInt ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_WORD:
					this->ini( *( ( udPWord ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_DWORD:
					this->ini( *( ( udPDWord ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_WSTRING:
					this->ini( *( ( wstring* ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_LLONG:
					this->ini( *( ( udPLongLong ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_LONG:
					this->ini( *( ( udPLong ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_BINARY:
					this->ini( *( ( udPBinaryData ) objCopy.m_pData ) );
					break;

				default:
					break;
			}
		} else { // типы не равны, сносим текущий и приравниваем
			this->DelData( );
			this->NullData( );
			// объект слева не создан, а справа создан
			switch( objCopy.m_iType ) {
				case CUdType::UDSDK_TYPE_INT:
					this->ini( *( ( udPInt ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_FLOAT:
					this->ini( *( ( udPFloat ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_DOUBLE:
					this->ini( *( ( udPDouble ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_UINT:
					this->ini( *( ( udPUInt ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_WORD:
					this->ini( *( ( udPWord ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_DWORD:
					this->ini( *( ( udPDWord ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_STRING:
					this->ini( *( ( string* ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_WSTRING:
					this->ini( *( ( wstring* ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_DATETIME:
					this->ini( *( ( udPDatetime ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_LLONG:
					this->ini( *( ( udPLongLong ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_LONG:
					this->ini( *( ( udPLong ) objCopy.m_pData ) );
					break;

				case CUdType::UDSDK_TYPE_BINARY:
					this->ini( *( ( udPBinaryData ) objCopy.m_pData ) );
					break;

				default:
					break;
			}
		}

		return *this;
	} // CUdType& operator=

	void CUdType::dump( string& szOut ) const {
		// добавляем тип
		switch( m_iType ) {
			case UDSDK_TYPE_UNKNOWN:
				szOut += "unknown";
				break;

			case UDSDK_TYPE_INT:
				szOut += "int";
				break;

			case UDSDK_TYPE_STRING:
				szOut += "string";
				break;

			case UDSDK_TYPE_FLOAT:
				szOut += "float";
				break;

			case UDSDK_TYPE_DATETIME:
				szOut += "datetime";
				break;

			case UDSDK_TYPE_DOUBLE:
				szOut += "double";
				break;

			case UDSDK_TYPE_UINT:
				szOut += "unsigned int";
				break;

			case UDSDK_TYPE_WORD:
				szOut += "word";
				break;

			case UDSDK_TYPE_DWORD:
				szOut += "dword";
				break;

			case UDSDK_TYPE_WSTRING:
				szOut += "wstring";
				break;

			case UDSDK_TYPE_LLONG:
				szOut += "llong";
				break;

			case UDSDK_TYPE_LONG:
				szOut += "long";
				break;

			case UDSDK_TYPE_BINARY:
				szOut += "binary";
				break;

			default:
				break;
		}
		// добавляем информацию по размерам
		udChar tmp[ 256 ];
		sprintf_s( tmp, 256, "( %lu Bytes )", m_dwBytes );
		szOut += string( tmp );
		// сливаем значение
		szOut += string( " \"" ) + this->toString( ) + string( "\"" );
	} // void dump

	void CUdType::dump( wstring& szOut ) const {
		// добавляем тип
		switch( m_iType ) {
			case UDSDK_TYPE_UNKNOWN:
				szOut += L"unknown";
				break;

			case UDSDK_TYPE_INT:
				szOut += L"int";
				break;

			case UDSDK_TYPE_STRING:
				szOut += L"string";
				break;

			case UDSDK_TYPE_FLOAT:
				szOut += L"float";
				break;

			case UDSDK_TYPE_DATETIME:
				szOut += L"datetime";
				break;

			case UDSDK_TYPE_DOUBLE:
				szOut += L"double";
				break;

			case UDSDK_TYPE_UINT:
				szOut += L"unsigned int";
				break;

			case UDSDK_TYPE_WORD:
				szOut += L"word";
				break;

			case UDSDK_TYPE_DWORD:
				szOut += L"dword";
				break;

			case UDSDK_TYPE_WSTRING:
				szOut += L"wstring";
				break;

			case UDSDK_TYPE_LLONG:
				szOut += L"llong";
				break;

			case UDSDK_TYPE_LONG:
				szOut += L"long";
				break;

			case UDSDK_TYPE_BINARY:
				szOut += L"binary";
				break;

			default:
				break;
		}
		// добавляем информацию по размерам
		udWChar tmp[ 256 ];
		swprintf_s( tmp, 256, L"( %lu Bytes )", m_dwBytes );
		szOut += wstring( tmp );
		// сливаем значение
		szOut += wstring( L" \"" ) + this->toWString( ) + wstring( L"\"" );
	} // void dump

	void CUdType::dump( char* dst ) const {
		if ( dst == NULL ) {
			WriteUdLibLog( "CUdType::dump( char* dst ), dst is NULL\r\n" );
			return;
		}

		switch( m_iType ) {
			case UDSDK_TYPE_UNKNOWN:
				break;

			case UDSDK_TYPE_INT:
				memcpy( dst, ( udPInt ) m_pData, sizeof( udInt ) );
				break;

			case UDSDK_TYPE_STRING:
				{
					string* tmp = ( string* ) m_pData;
					memcpy( dst, tmp->data( ), tmp->size( ) );
				}
				break;

			case UDSDK_TYPE_FLOAT:
				memcpy( dst, ( udPFloat ) m_pData, sizeof( udFloat ) );
				break;

			case UDSDK_TYPE_DATETIME:
				{
					time_t time = ( ( pdatetime ) m_pData )->get( );
					memcpy( dst, &time, sizeof( time_t ) );
				}
				break;

			case UDSDK_TYPE_DOUBLE:
				memcpy( dst, ( udPDouble ) m_pData, sizeof( udDouble ) );
				break;

			case UDSDK_TYPE_UINT:
				memcpy( dst, ( udPUInt ) m_pData, sizeof( udUInt ) );
				break;

			case UDSDK_TYPE_WORD:
				memcpy( dst, ( udPWord ) m_pData, sizeof( udWord ) );
				break;

			case UDSDK_TYPE_DWORD:
				memcpy( dst, ( udPDWord ) m_pData, sizeof( udDWord ) );
				break;

			case UDSDK_TYPE_WSTRING:
				{
					wstring *tmp = ( wstring* ) m_pData;
					memcpy( dst, tmp->data( ), tmp->size( ) );
				}
				break;

			case UDSDK_TYPE_LLONG:
				memcpy( dst, ( udPLongLong ) m_pData, sizeof( udLongLong ) );
				break;

			case UDSDK_TYPE_LONG:
				memcpy( dst, ( udPLong ) m_pData, sizeof( udLong ) );
				break;

			case UDSDK_TYPE_BINARY:
				break;

			default:
				break;
		}
	} // void dump

	void CUdType::dump( udBinaryData& data ) const {
		assert( m_pData );

		switch( m_iType ) {
			case UDSDK_TYPE_UNKNOWN:
				break;

			case UDSDK_TYPE_INT:
				data.add( *( ( udPInt ) m_pData ) );
				break;

			case UDSDK_TYPE_STRING:
				data.add( ( string* ) m_pData );
				break;

			case UDSDK_TYPE_FLOAT:
				data.add( *( ( udPFloat ) m_pData ) );
				break;

			case UDSDK_TYPE_DATETIME:
				data.add( ( pdatetime ) m_pData );
				break;

			case UDSDK_TYPE_DOUBLE:
				data.add( *( ( udPDouble ) m_pData ) );
				break;

			case UDSDK_TYPE_UINT:
				data.add( *( ( udPUInt ) m_pData ) );
				break;

			case UDSDK_TYPE_WORD:
				data.add( *( ( udPWord ) m_pData ) );
				break;

			case UDSDK_TYPE_DWORD:
				data.add( *( ( udPDWord ) m_pData ) );
				break;

			case UDSDK_TYPE_WSTRING:
				data.add( ( wstring* ) m_pData );
				break;

			case UDSDK_TYPE_LLONG:
				data.add( *( ( udPLongLong ) m_pData ) );
				break;

			case UDSDK_TYPE_LONG:
				data.add( *( ( udPLong ) m_pData ) );
				break;

			default:
				break;
		}
	} // void dump

	void CUdType::dump( udPBinaryData data ) const {
		if ( data == NULL ) {
			WriteUdLibLog( "CUdType::dump( udPBinaryData data ), data is NULL\r\n" );
			return;
		}
		if ( m_pData == NULL ) {
			WriteUdLibLog( "CUdType::dump( udPBinaryData data ), m_pData is NULL\r\n" );
			return;
		}

		switch( m_iType ) {
			case UDSDK_TYPE_UNKNOWN:
				break;

			case UDSDK_TYPE_INT:
				data->add( *( ( udPInt ) m_pData ) );
				break;

			case UDSDK_TYPE_STRING:
				data->add( ( string* ) m_pData );
				break;

			case UDSDK_TYPE_FLOAT:
				data->add( *( ( udPFloat ) m_pData ) );
				break;

			case UDSDK_TYPE_DATETIME:
				data->add( ( pdatetime ) m_pData );
				break;

			case UDSDK_TYPE_DOUBLE:
				data->add( *( ( udPDouble ) m_pData ) );
				break;

			case UDSDK_TYPE_UINT:
				data->add( *( ( udPUInt ) m_pData ) );
				break;

			case UDSDK_TYPE_WORD:
				data->add( *( ( udPWord ) m_pData ) );
				break;

			case UDSDK_TYPE_DWORD:
				data->add( *( ( udPDWord ) m_pData ) );
				break;

			case UDSDK_TYPE_WSTRING:
				data->add( ( wstring* ) m_pData );
				break;

			case UDSDK_TYPE_LLONG:
				data->add( *( ( udPLongLong ) m_pData ) );
				break;

			case UDSDK_TYPE_LONG:
				data->add( *( ( udPLong ) m_pData ) );
				break;

			default:
				break;
		}
	} // void dump

	udInt CUdType::read( udPSocket sock ) {
		//WriteUdLibLog( "CUdType::read\n" );

		assert( sock );

		/*if ( sock->Select( udSocket::UDSOCKET_SELECT_READ, 1000 ) != 1 ) {
			WriteUdLibLog( "не дождались данных в сокете\n" );
			return 0;
		}*/

		switch( m_iType ) {
			case UDSDK_TYPE_UNKNOWN:
				return 1;
				break;

			case UDSDK_TYPE_INT:
				{
					char *_data = NULL;
					if ( sock->Read( &_data, sizeof( udInt ) ) != sizeof( udInt ) ) {
						udDELETE_( _data );
						return 0;
					}

					udInt v = 0;
					memcpy( &v, _data, sizeof( udInt ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_STRING:
				{
					string tmp;
					char ch = 0;
					udInt read = 0;
					udInt cnt = 0;
					while( true ) {
						read = sock->ReadByte( &ch );
						if ( read == 1 ) {
							++cnt;
							if ( ch == '\0' ) {
								break;
							} else {
								tmp += ch;
							}
						} else if ( read == -1 ) {
							//WriteUdLibLog( "произведено итераций чтения: " + UdSDK::toString( cnt ) + "\n" );
							return 0;
						} else {
							//WriteUdLibLog( );
							break;
						}
					}

					//WriteUdLibLog( "произведено итераций чтения: " + UdSDK::toString( cnt ) + "\n" );
					this->ini( tmp );
					return 1;
				}
				break;

			case UDSDK_TYPE_FLOAT:
				{
					char *_data = NULL;
					if ( sock->Read( &_data, sizeof( udFloat ) ) != sizeof( udFloat ) ) {
						udDELETE_( _data );
						return 0;
					}

					udFloat v = 0;
					memcpy( &v, _data, sizeof( udFloat ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_DATETIME:
				{
					char *_data = NULL;
					if ( sock->Read( &_data, sizeof( time_t ) ) != sizeof( time_t ) ) {
						udDELETE_( _data );
						return 0;
					}

					time_t v = 0;
					memcpy( &v, _data, sizeof( time_t ) );
					this->ini( udDatetime( v ) );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_DOUBLE:
				{
					char *_data = NULL;
					if ( sock->Read( &_data, sizeof( udDouble ) ) != sizeof( udDouble ) ) {
						udDELETE_( _data );
						return 0;
					}

					udFloat v = 0;
					memcpy( &v, _data, sizeof( udDouble ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_UINT:
				{
					char *_data = NULL;
					if ( sock->Read( &_data, sizeof( udUInt ) ) != sizeof( udUInt ) ) {
						udDELETE_( _data );
						return 0;
					}

					udUInt v = 0;
					memcpy( &v, _data, sizeof( udUInt ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_WORD:
				{
					char *_data = NULL;
					if ( sock->Read( &_data, sizeof( udWord ) ) != sizeof( udWord ) ) {
						udDELETE_( _data );
						return 0;
					}

					udWord v = 0;
					memcpy( &v, _data, sizeof( udWord ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_DWORD:
				{
					char *_data = NULL;
					if ( sock->Read( &_data, sizeof( udDWord ) ) != sizeof( udDWord ) ) {
						udDELETE_( _data );
						return 0;
					}

					udDWord v = 0;
					memcpy( &v, _data, sizeof( udDWord ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_WSTRING:
				{
					wstring tmp;
					wchar_t ch = 0;
					char *_data = NULL;
					udUInt size = sizeof( wchar_t );
					udInt read = 0;
					while( true ) {
						read = sock->Read( &_data, sizeof( wchar_t ) );
						if ( read == size ) {
							memcpy( &ch, _data, size );
							udDELETE_( _data );
							if ( ch == L'\0' ) {
								break;
							} else {
								tmp += ch;
							}
						} else if ( read == -1 ) {
							return 0;
						} else {
							break;
						}
					}

					this->ini( tmp );
					return 1;
				}
				break;

			case UDSDK_TYPE_LLONG:
				{
					char *_data = NULL;
					if ( sock->Read( &_data, sizeof( udLongLong ) ) != sizeof( udLongLong ) ) {
						udDELETE_( _data );
						return 0;
					}

					udLongLong v = 0;
					memcpy( &v, _data, sizeof( udLongLong ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_LONG:
				{
					char *_data = NULL;
					if ( sock->Read( &_data, sizeof( udLong ) ) != sizeof( udLong ) ) {
						udDELETE_( _data );
						return 0;
					}

					udLong v = 0;
					memcpy( &v, _data, sizeof( udLong ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_BINARY:
				WriteUdLibLog( "binary udType from socket\n" );
				return 1;
				break;

			default:
				break;
		}

		return 0;
	} // udInt read

	udInt CUdType::read( udBinaryDataIterator& itCur, const udBinaryDataIterator& itEnd ) {
		switch( m_iType ) {
			case UDSDK_TYPE_UNKNOWN:
				break;

			case UDSDK_TYPE_INT:
				{
					udPByte _data = new udByte[ sizeof( udInt ) ];
					udUInt cnt = 0;
					while( cnt < sizeof( udInt ) ) {
						_data[ cnt++ ] = *itCur++;
						if ( itCur == itEnd ) {
							break;
						}
					}
					if ( cnt != sizeof( udInt ) ) {
						udDELETE_( _data );
						return 0;
					}

					udInt v = 0;
					memcpy( &v, _data, sizeof( udInt ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_STRING:
				{
					string tmp;
					char ch = 0;
					udInt read = 0;
					udInt cnt = 0;
					while( true ) {
						ch = *itCur++;
						++cnt;
						if ( ch == '\0' ) {
							break;
						} else {
							tmp += ch;
						}
					}

					this->ini( tmp );
					return 1;
				}
				break;

			case UDSDK_TYPE_FLOAT:
				{
					udPByte _data = new udByte[ sizeof( udFloat ) ];
					udUInt cnt = 0;
					while( cnt < sizeof( udFloat ) ) {
						_data[ cnt++ ] = *itCur++;
						if ( itCur == itEnd ) {
							break;
						}
					}
					if ( cnt != sizeof( udFloat ) ) {
						udDELETE_( _data );
						return 0;
					}

					udFloat v = 0;
					memcpy( &v, _data, sizeof( udFloat ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_DATETIME:
				{
					udPByte _data = new udByte[ sizeof( time_t ) ];
					udUInt cnt = 0;
					while( cnt < sizeof( time_t ) ) {
						_data[ cnt++ ] = *itCur++;
						if ( itCur == itEnd ) {
							break;
						}
					}
					if ( cnt != sizeof( time_t ) ) {
						udDELETE_( _data );
						return 0;
					}

					time_t v = 0;
					memcpy( &v, _data, sizeof( time_t ) );
					this->ini( udDatetime( v ) );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_DOUBLE:
				{
					udPByte _data = new udByte[ sizeof( udDouble ) ];
					udUInt cnt = 0;
					while( cnt < sizeof( udDouble ) ) {
						_data[ cnt++ ] = *itCur++;
						if ( itCur == itEnd ) {
							break;
						}
					}
					if ( cnt != sizeof( udDouble ) ) {
						udDELETE_( _data );
						return 0;
					}

					udFloat v = 0;
					memcpy( &v, _data, sizeof( udDouble ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_UINT:
				{
					udPByte _data = new udByte[ sizeof( udUInt ) ];
					udUInt cnt = 0;
					while( cnt < sizeof( udUInt ) ) {
						_data[ cnt++ ] = *itCur++;
						if ( itCur == itEnd ) {
							break;
						}
					}
					if ( cnt != sizeof( udUInt ) ) {
						udDELETE_( _data );
						return 0;
					}

					udUInt v = 0;
					memcpy( &v, _data, sizeof( udUInt ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_WORD:
				{
					udPByte _data = new udByte[ sizeof( udWord ) ];
					udUInt cnt = 0;
					while( cnt < sizeof( udWord ) ) {
						_data[ cnt++ ] = *itCur++;
						if ( itCur == itEnd ) {
							break;
						}
					}
					if ( cnt != sizeof( udWord ) ) {
						udDELETE_( _data );
						return 0;
					}

					udWord v = 0;
					memcpy( &v, _data, sizeof( udWord ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_DWORD:
				{
					udPByte _data = new udByte[ sizeof( udDWord ) ];
					udUInt cnt = 0;
					while( cnt < sizeof( udDWord ) ) {
						_data[ cnt++ ] = *itCur++;
						if ( itCur == itEnd ) {
							break;
						}
					}
					if ( cnt != sizeof( udDWord ) ) {
						udDELETE_( _data );
						return 0;
					}

					udDWord v = 0;
					memcpy( &v, _data, sizeof( udDWord ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_WSTRING:
				{
					wstring tmp;
					wchar_t ch = 0;
					udPByte _data = new udByte[ sizeof( wchar_t ) ];
					udInt read = 0, cnt = 0;
					while( true ) {
						cnt = 0;
						memset( _data, 0, sizeof( wchar_t ) );
						while( cnt < sizeof( wchar_t ) ) {
							_data[ cnt++ ] = *itCur++;
							if ( itCur == itEnd ) {
								break;
							}
						}
						if ( cnt == sizeof( wchar_t ) ) {
							memcpy( &ch, _data, sizeof( wchar_t ) );
							if ( ch == L'\0' ) {
								break;
							} else {
								tmp += ch;
							}
						} else {
							udDELETE_( _data );
							return 0;
						}
					}
					udDELETE_( _data );
					this->ini( tmp );
					return 1;
				}
				break;

			case UDSDK_TYPE_LLONG:
				{
					udPByte _data = new udByte[ sizeof( udLongLong ) ];
					udUInt cnt = 0;
					while( cnt < sizeof( udLongLong ) ) {
						_data[ cnt++ ] = *itCur++;
						if ( itCur == itEnd ) {
							break;
						}
					}
					if ( cnt != sizeof( udLongLong ) ) {
						udDELETE_( _data );
						return 0;
					}

					udLongLong v = 0;
					memcpy( &v, _data, sizeof( udLongLong ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_LONG:
				{
					udPByte _data = new udByte[ sizeof( udLong ) ];
					udUInt cnt = 0;
					while( cnt < sizeof( udLong ) ) {
						_data[ cnt++ ] = *itCur++;
						if ( itCur == itEnd ) {
							break;
						}
					}
					if ( cnt != sizeof( udLong ) ) {
						udDELETE_( _data );
						return 0;
					}

					udLong v = 0;
					memcpy( &v, _data, sizeof( udLong ) );
					this->ini( v );
					udDELETE_( _data );
					return 1;
				}
				break;

			case UDSDK_TYPE_BINARY:
				WriteUdLibLog( "binary udType from binary data\n" );
				return 1;
				break;

			default:
				break;
		}

		return 0;
	} // udInt read

	void CUdType::assign( udPType type, bool strict ) {
		if ( strict ) {
			switch( m_iType ) {
				case UDSDK_TYPE_INT:		this->ini( type->toInt( ) );		break;
				case UDSDK_TYPE_STRING:		this->ini( type->toString( ) );		break;
				case UDSDK_TYPE_FLOAT:		this->ini( type->toFloat( ) );		break;
				case UDSDK_TYPE_DATETIME:	this->ini( type->toTime( ) );		break;
				case UDSDK_TYPE_DOUBLE:		this->ini( type->toDouble( ) );		break;
				case UDSDK_TYPE_UINT:		this->ini( type->toUInt( ) );		break;
				case UDSDK_TYPE_WORD:		this->ini( type->toWord( ) );		break;
				case UDSDK_TYPE_DWORD:		this->ini( type->toDWord( ) );		break;
				case UDSDK_TYPE_WSTRING:	this->ini( type->toWString( ) );	break;
				case UDSDK_TYPE_LLONG:		this->ini( type->toLLong( ) );		break;
				case UDSDK_TYPE_LONG:		this->ini( type->toLong( ) );		break;
				case UDSDK_TYPE_BINARY:		this->ini( type->toBinary( ) );		break;
			}
		} else {
			*this = *type;
		}
	} // void assign

	void CUdType::reset( ) {
		switch( m_iType ) {
			case UDSDK_TYPE_INT:		ini( ( udInt ) 0 );			break;
			case UDSDK_TYPE_STRING:		ini( string( "" ) );		break;
			case UDSDK_TYPE_FLOAT:		ini( ( udFloat ) 0 );		break;
			case UDSDK_TYPE_DATETIME:	ini( datetime( ) );			break;
			case UDSDK_TYPE_DOUBLE:		ini( ( udDouble ) 0 );		break;
			case UDSDK_TYPE_UINT:		ini( ( udUInt ) 0 );		break;
			case UDSDK_TYPE_WORD:		ini( ( udWord ) 0 );		break;
			case UDSDK_TYPE_DWORD:		ini( ( udDWord ) 0 );		break;
			case UDSDK_TYPE_WSTRING:	ini( wstring( L"" ) );		break;
			case UDSDK_TYPE_LLONG:		ini( ( udLongLong ) 0 );	break;
			case UDSDK_TYPE_LONG:		ini( ( udLong ) 0 );		break;
			case UDSDK_TYPE_BINARY:		ini( udBinaryData( ) );		break;
		}
	} // void reset

} // namespace UdSDK
