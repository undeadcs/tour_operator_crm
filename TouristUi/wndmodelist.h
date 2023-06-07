/**
	Окно режима списка
*/
#ifndef WNDMODELIST_H
#define WNDMODELIST_H

namespace UdSDK {

	class CWndModeList : public udWndGraphic {
		typedef ModeFormControlInfo* _control;
		typedef map< udInt, _control > _int_asc_control;

		_int_asc_control	m_arrControl;
		udPWndInstanceList	m_wndList;		// окно списка
		udPWndPager			m_wndPager;		// окно пейджера
		udPListHandler		m_pHandler;		// обработчик
		udLAscWndLink		m_arrLink;		// дополнительные ссылки
		bool				m_bIsModal,		// в модальном ли диалоге
							m_bShowFilter;	// показывать фильтр
		udPWndButton		m_btnOk,		// кнопка подтверждения
							m_btnCancel,	// кнопка отмены
							m_btnFilter,	// кнопка фильтровать
							m_btnReset;		// кнопка сброса фильтра
		udPMorph			m_pObjFilter;	// набор значений фильтра

	public:
		CWndModeList( udPListHandler handler = NULL );

		virtual ~CWndModeList( );

		void	AddLink( udPWndLink lnk );
		void	SetButtonOkPosition( udInt x, udInt y );
		void	SetButtonCancelPosition( udInt x, udInt y );
		void	SetButtonFilterPosition( udInt x, udInt y );
		void	SetButtonResetPosition( udInt x, udInt y );
		void	MakeControls( );
		void	ClearFilter( );

		void	AddInput( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font, udDWord addstyle );
		void	AddText( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font );
		void	AddDate( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font, udInt bString = FALSE );
		void	AddDatetime( udInt iIndex, string attr, ModeFormControlBox* box );
		void	AddSelect( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font, const udArrWString& arr );
		// attr - атрибут сущности, по значению которого выбирать другой объект
		// title - заголовок контрола
		// table - таблица сущностей, которые надо выбирать при загрузке
		// fnGetObject - калбэк создания объекта сущности
		// attrpick - атрибут, значение которого будет показываться контролом
		// index - индекс контрола
		udPExtList	AddExtlist(
			udInt iIndex, string attr,
			ModeFormControlBox* box, const string& attrpick,
			//const string& index, const wstring& title,
			//const string& table, pfnGetObject fnGetObject,
			//bool auto_load = true
			const wstring& title,
			pfnGetListHandler fnGetListHandler,
			udPMorph opt = NULL,
			pfnExtLstAfterPick fnAfterPick = NULL
			);
		udPWndFormList	AddFormlist( udInt iIndex, string attr, ModeFormControlBox* box );
		ModeFormControlInfo* GetInfo( udInt iIndex );
		void	FillControls( );
		void	ShowControls( );
		void	LoadControlData( );
		void	DestroyControls( );
		void	RenderControls( );

		virtual void	Render( );
		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );

		virtual LRESULT	msgLinkClick( LPARAM lParam );
		virtual LRESULT msgPagerSelect( LPARAM lParam );
		virtual LRESULT	msgButtonClick( LPARAM lParam );
		virtual LRESULT	msgListRowSelect( LPARAM lParam );
		virtual LRESULT	msgListHeaderClick( LPARAM lParam );

		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void	SetList( udPWndInstanceList lst )	{ m_wndList = lst;		}
		void	SetPager( udPWndPager pager )		{ m_wndPager = pager;	}
		void	SetModal( bool modal )				{ m_bIsModal = modal;	}
		void	SetShowFilter( bool show )			{ m_bShowFilter	= show;	}

		udPWndInstanceList	GetList( )			const { return m_wndList;	}
		udPWndPager			GetPager( )			const { return m_wndPager;	}
		udPListHandler		GetHandler( )		const { return m_pHandler;	}
		bool				IsModal( )			const { return m_bIsModal;	}
		udPWndButton		GetButtonOk( )		const { return m_btnOk;		}
		udPWndButton		GetButtonCancel( )	const { return m_btnCancel;	}
		udPWndButton		GetButtonFilter( )	const { return m_btnFilter;	}
		udPWndButton		GetButtonReset( )	const { return m_btnReset;	}
		const udLAscWndLink*	GetLinks( )		const { return &m_arrLink;	}
		udPMorph			GetObjFilter( )		const { return m_pObjFilter;	}

	private:
		// функции создания контролов
		void	MakeInput( udPGdi gdi, ModeFormControlInfo* info );
		void	MakeText( udPGdi gdi, ModeFormControlInfo* info );
		void	MakeExtlist( udPGdi gdi, ModeFormControlInfo* info );
		void	MakeSelect( udPGdi gdi, ModeFormControlInfo* info );
		void	MakeDate( udPGdi gdi, ModeFormControlInfo* info );
		void	MakeFormlist( udPGdi gdi, ModeFormControlInfo* info );
		// функции наполнения контролов данными из атрибутов объекта
		void	FillInput( ModeFormControlInfo* info );
		void	FillText( ModeFormControlInfo* info );
		void	FillExtlst( ModeFormControlInfo* info );
		void	FillSelect( ModeFormControlInfo* info );
		void	FillDate( ModeFormControlInfo* info );
		void	FillFormlist( ModeFormControlInfo* info );
		// функции подгрузки данных из контролов в атрибуты объекта
		void	LoadInput( ModeFormControlInfo* info );
		void	LoadText( ModeFormControlInfo* info );
		void	LoadExtlst( ModeFormControlInfo* info );
		void	LoadSelect( ModeFormControlInfo* info );
		void	LoadDate( ModeFormControlInfo* info );
		void	LoadFormlist( ModeFormControlInfo* info );
		// фунции отрисовки контролов
		void	RenderInput( ModeFormControlInfo* info );
		void	RenderText( ModeFormControlInfo* info );
		void	RenderExtlist( ModeFormControlInfo* info );
		void	RenderSelect( ModeFormControlInfo* info );
		void	RenderDate( ModeFormControlInfo* info );
		void	RenderFormlist( ModeFormControlInfo* info );
		//
	}; // class CWndModeList

} // namespace UdSDK

#endif
