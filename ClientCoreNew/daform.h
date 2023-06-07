/*
	Tourist application
	Библиотека ядра клиентского приложения
	Поставщик данных формы
*/
#ifndef TOURIST_CLIENT_CORE_DAFORM_H
#define TOURIST_CLIENT_CORE_DAFORM_H

namespace UdSDK {

	class CDAForm : public udDataAccess {
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

	protected:
		string			m_szName;		// наименование обработчика
		pfnGetObject	m_fnGetObject;	// калбэк создания объекта
		udPBase			m_pObject;		// объект редактирования
		t_vctArrFde		m_arrExtra;		// дополнительные данные
		bool			m_bHasFiles;	// при загрузке и выгрузке присутствуют файлы
		wstring			m_szFolder;		// папка для хранения временных файлов

	public:
		CDAForm( );
		CDAForm( const CDAForm& objCopy );

		virtual ~CDAForm( );

		CDAForm& operator =( const CDAForm& objCopy );

		virtual void	Load( );
		virtual void	Clear( );

		void	Send( udPArrBase pArrFiles = NULL );

		void	AddExtraData( udPFormDataExtra pFde );
		void	SetId( udUInt id );
		void	MakeObject( );
		void	RemoveObject( );

		void	SetName( const string& szName )				{ m_szName		= szName;		}
		void	SetGetObject( pfnGetObject fnGetObject )	{ m_fnGetObject	= fnGetObject;	}
		void	SetHasFiles( bool bHasFiles )				{ m_bHasFiles	= bHasFiles;	}
		void	SetFolder( const wstring& szFolder )		{ m_szFolder	= szFolder;		}

		udPFormDataExtra	GetExtraData( udInt iMark ) const;

		string	GetName( )		const { return m_szName;	}
		udPBase	GetData( )		const { return m_pObject;	}
		bool	GetHasFiles( )	const { return m_bHasFiles;	}
		wstring	GetFolder( )	const { return m_szFolder;	}

	}; // class CDAForm

} // namespace UdSDK

#endif
