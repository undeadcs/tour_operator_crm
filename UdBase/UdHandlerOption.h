
#ifndef OHOPTION_H
#define OHOPTION_H

namespace UdSDK {

	/**
	 *	Опции функций класса обработчика объектов
	 */
	class CUdHandlerOption {
		typedef struct structLeftJoinInfo {
			string attr, alias, table, condition, custom_attr, custom_join;
			pfnGetObject fnGetObject;
		} _lji;
		typedef vector< _lji > _vct_info;

		string			m_szTable,			// имя таблицы
						m_szIndexAttr,		// индексный атрибут
						m_szWhere,			// условие WHERE
						m_szGroup,			// условие GROUP BY
						m_szOrder,			// условие ORDER BY
						m_szLimit;			// условие LIMIT
		pfnGetObject	m_fnCreateObject;	// функция создания объекта ( Class опция )
		udAscInt		m_arrIgnoreAttr,	// игнорируемые атрибуты
						m_arrOnlyAttr;		// обязательные атрибуты
		bool			m_bForceTable,		// брать таблицу указанную в опциях
						m_bSqlCalcFoundRows;	// SQL_CALC_FOUND_ROWS option
		_vct_info		m_arrLeftJoin;		// дополнительные джоины таблиц

	public:
		CUdHandlerOption( );
		CUdHandlerOption( const CUdHandlerOption& objCopy );

		~CUdHandlerOption( );

		CUdHandlerOption& operator =( const CUdHandlerOption& objCopy );

		void	Clear( );

		void	ApplyIgnoreAttr( udPArrPairStr pAttr ) const;
		void	ApplyIgnoreAttr( udPArrPairStr pAttr, udPArrPairStr pValue ) const;
		string	GetLeftJoinAttr( ) const;
		string	GetLeftJoinTable( ) const;

		void	SetTable( string name );
		void	SetIndexAttr( string name );
		void	SetWhere( string szWhere );
		void	SetGroup( string szGroup );
		void	SetOrder( string szOrder );
		void	SetLimit( string szLimit );
		void	SetClass( pfnGetObject pfnCreateObject );
		void	SetIgnoreAttr( string name );
		void	SetOnlyAttr( string name );
		void	SetForceTable( bool force );
		void	SetSqlCalcFoundRows( bool calc );
		void	AddLeftJoin( const string& attr, const string& alias, const string& table, const string& condition, pfnGetObject fnGetObject );
		void	AddLeftJoin( const string& custom_attr, const string& custom_join );

		string			GetTable( )		const { return m_szTable;			}
		string			GetIndexAttr( )	const { return m_szIndexAttr;		}
		string			GetWhere( )		const { return m_szWhere;			}
		string			GetGroup( )		const { return m_szGroup;			}
		string			GetOrder( )		const { return m_szOrder;			}
		string			GetLimit( )		const { return m_szLimit;			}
		pfnGetObject	GetClass( )		const { return m_fnCreateObject;	}

		bool	issetTable( )		const { return !m_szTable.empty( );				}
		bool	issetClass( )		const { return ( m_fnCreateObject != NULL );	}
		bool	issetIgnoreAttr( )	const { return !m_arrIgnoreAttr.empty( );		}
		bool	issetOnlyAttr( )	const { return !m_arrOnlyAttr.empty( );			}
		bool	issetIndexAttr( )	const { return !m_szIndexAttr.empty( );			}
		bool	issetWhere( )		const { return !m_szWhere.empty( );				}
		bool	issetGroup( )		const { return !m_szGroup.empty( );				}
		bool	issetOrder( )		const { return !m_szOrder.empty( );				}
		bool	issetLimit( )		const { return !m_szLimit.empty( );				}
		bool	ForceTable( )		const { return m_bForceTable;					}
		bool	issetLeftJoin( )	const { return !m_arrLeftJoin.empty( );			}
		bool	issetSqlCalcFoundRows( )	const { return m_bSqlCalcFoundRows;		}

	}; // class CHOption

} // namespace UdSDK

#endif
