/**
	Tourist application
	Клиентское приложение

	обработчик формы
*/
#ifndef TOURIST_APPLICATION_CLIENT_CORE_FORM_HANLDER_H
#define TOURIST_APPLICATION_CLIENT_CORE_FORM_HANLDER_H

namespace UdSDK {

	enum extra_load_type {
		extra_load_extlst,
		extra_load_frmlst
	};

	typedef void	( *pfnAssignTpl )( udPBase obj );
	typedef void	( *pfnAfterLoad )( udArrBase& arr );
	typedef bool	( *pfnSpecLoad )( udPBase main, udPBase& obj, udPWndModeForm form, udPSocket sock );

	typedef struct structExtraLoadInfo {
		udInt ctrlid;
		pfnGetObject fnGetObject;
		udInt type;
		pfnAssignTpl fnAssignTpl; // чтобы прикреплять дополнительные атрибуты
		pfnAfterLoad fnAfterLoad; // фильтрация после загрузки
		pfnSpecLoad fnSpecLoad; // спецзагрузка объекта
	} _extra_load_info;

	typedef vector< _extra_load_info > vctExtraLoad;

	class CFormHandler {
	public:
		enum FormHandlerError {
			FormHandlerErrorOk,		// ошибок не произошло
			FormHandlerErrorFailed,	// просто фиктивная ошибка
		};

		enum FormHandlerState {
			FormHandlerStateConstructed,
			FormHandlerStateCreated,
		};

	private:

	protected:
		udPWndModeForm		m_wndForm;
		vctExtraLoad		m_arrExtraLoad;	// дополнительная подгрузка данных при редактировании
		FormHandlerState	m_iFormState;	// состояние формы
		udPMorph			m_pFrmOpt;		// опции формы

	public:
		CFormHandler( );

		virtual ~CFormHandler( );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void		AddExtraLoadInfo( udInt ctrlid, pfnGetObject fnGetObject, udInt type, pfnAssignTpl fnAssignTpl = NULL, pfnAfterLoad fnAfterLoad = NULL, pfnSpecLoad fnSpecLoad = NULL );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual udInt	CreateForm( HWND hParent, bool modal = false );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual udInt	GetFormModel( ) = 0;
		virtual udInt	GetFormView( ) = 0;
		virtual udInt	ValidateForm( ) = 0;
		virtual udInt	FormOk( ) = 0;
		virtual udInt	FormCancel( ) = 0;
		virtual udInt	ProcForm( bool modal = false ) = 0;
		virtual pfnGetObject	GetObjectCallback( ) = 0;
		virtual void	FormListOnAdd( udPWndFormList wnd ) { }
		virtual void	FormListOnEdit( udPWndFormList wnd, udPBase obj ) { }
		virtual bool	FormListOnDelete( udPWndFormList wnd, udPBase obj ) { return false; }
		virtual void	LoadAfterSetObject( ) { }
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void			SetForm( udPWndModeForm wndForm ) { m_wndForm = wndForm; }
		////////////////////////////////////////////////////////////////////////////////////////////////////
		udPWndModeForm	GetWndForm( )	const { return m_wndForm;	}
		udPMorph		GetFrmOpt( )	const { return m_pFrmOpt;	}
		////////////////////////////////////////////////////////////////////////////////////////////////////
	}; // class CFormHandler

} // namespace UdSDK

#endif
