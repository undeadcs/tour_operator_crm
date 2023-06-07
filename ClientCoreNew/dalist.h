/*
	Tourist application
	Библиотека ядра клиентского приложения
	Загрузчик данных списка
*/
#ifndef TOURIST_CLIENT_CORE_DA_LIST_H
#define TOURIST_CLIENT_CORE_DA_LIST_H

namespace UdSDK {

	class CDAList : public udDataAccess {
	public:
		typedef vector< udPFormDataExtra > t_vctArrFde;

		enum {
			opidAdd,	// добавление
			opidUpd,	// обновление
			opidDel,	// удаление
			opidAdd1,	// добавление (center)
			opidUpd1,	// обновление (center)
			opidDel1,	// удаление (center)
			opidLst1,	// подгрузка списка (center)
			opidEdt,	// запрос на редактирование
			opidGet,	// получение объекта по его ID
		};

		enum OrderType {
			ordDesc,
			ordAsc
		};

	protected:
		string			m_szName;		// имя обработчика
		udPMorph		m_pFilter;		// фильтр
		udPArrBase		m_arrObject;	// набор объектов
		pfnGetObject	m_fnGetObject;	// калбэк создания объекта
		udPPager		m_pPager;		// пейджер
		string			m_szOrderBy;	// атрибут упорядочивания
		udInt			m_iOrderType;	// тип упорядочивания
		t_vctArrFde		m_arrExtra;		// дополнительные данные

	public:
		CDAList( );
		CDAList( const CDAList& objCopy );
		CDAList(
			const string& szName,
			pfnGetObject fnGetObject,
			const string& szOrderBy = "id",
			udInt iOrder = ordDesc,
			udPPager pPager = new udPager( 1, 25, 10, 0 ),
			udPMorph pFilter = new udMorph
		);

		virtual ~CDAList( );

		CDAList& operator =( const CDAList& objCopy );

		virtual void	Load( );
		virtual void	Clear( );

		void	SetName( const string& szName )				{ m_szName		= szName;		}
		void	SetGetObject( pfnGetObject fnGetObject )	{ m_fnGetObject = fnGetObject;	}
		void	SetOrderBy( const string& szOrder )			{ m_szOrderBy	= szOrder;		}
		void	SetOrderType( udInt iType )					{ m_iOrderType	= iType;		}

		void	AddExtraData( udPFormDataExtra pFde );
		void	MakeObject( );
		void	ClearExtraData( );
		void	ResetFilter( );
		void	ResetPager( );

		udPFormDataExtra	GetExtraData( udInt iMark ) const;

		string		GetName( )		const { return m_szName;		}
		udPArrBase	GetData( )		const { return m_arrObject;		}
		udPPager	GetPager( )		const { return m_pPager;		}
		udPMorph	GetFilter( )	const { return m_pFilter;		}
		string		GetOrderBy( )	const { return m_szOrderBy;		}
		udInt		GetOrderType( )	const { return m_iOrderType;	}

	}; // class CDACommon

} // namespace UdSDK

#endif
