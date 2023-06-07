
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CFormHandler::CFormHandler( ) {
		m_wndForm		= NULL;
		//m_wndForm		= new udWndModeForm( this );
		m_iFormState	= FormHandlerStateConstructed;
		m_pFrmOpt		= new udMorph;
	}

	CFormHandler::~CFormHandler( ) {
		udDELETE( m_wndForm );
		udDELETE( m_pFrmOpt );
	}

	udInt CFormHandler::CreateForm( HWND hParent, bool modal ) {
		if ( !m_wndForm ) {
			m_wndForm = new udWndModeForm( this );
		}
		udPLog __log = g_pApp->GetLog( );
		m_wndForm->SetIsModal( modal );
		m_wndForm->SetGetObjectCallback( this->GetObjectCallback( ) );
		// создаем контролы (аля модель)
		if ( modal ) {
			m_wndForm->get( "style" )->ini( ( udDWord ) WS_CHILD );
			m_wndForm->get( "x" )->ini( ( udInt ) 0 );
			m_wndForm->get( "y" )->ini( ( udInt ) 0 );
			m_wndForm->Create( hParent );
		}
		GetFormModel( );
		// устанавливаем нужные размеры
		m_wndForm->get( "style" )->ini( ( udDWord ) WS_CHILD );
		m_wndForm->get( "x" )->ini( ( udInt ) 0 );
		m_wndForm->get( "y" )->ini( ( udInt ) 0 );
		if ( modal ) { // размеры формы должны быть установлены
		} else {
			udPWndModel wnd = g_pApp->GetModel( )->GetWndModel( );
			m_wndForm->get( "width" )->ini( wnd->get( "width" )->toInt( ) );
			m_wndForm->get( "height" )->ini( wnd->get( "height" )->toInt( ) );
		}
		// создаем объект
		if ( !modal && !m_wndForm->Create( hParent ) ) {
			return FormHandlerErrorFailed;
		}
		// распологаем контролы (аля вид - верстка)
		GetFormView( );
		// создаем окна контролов
		m_wndForm->MakeControls( );
		// показываем контролы
		m_wndForm->ShowControls( );
		if ( m_wndForm->IsWatch( ) ) {
			m_wndForm->GetButtonOk( )->Hide( );
		} else {
			m_wndForm->GetButtonOk( )->Show( );
		}
		m_wndForm->GetButtonCancel( )->Show( );
		//
		m_iFormState = FormHandlerStateCreated;
		return FormHandlerErrorOk;
	} // udInt CreateForm

	void CFormHandler::AddExtraLoadInfo( udInt ctrlid, pfnGetObject fnGetObject, udInt type, pfnAssignTpl fnAssignTpl, pfnAfterLoad fnAfterLoad, pfnSpecLoad fnSpecLoad ) {
		_extra_load_info info;
		info.ctrlid = ctrlid;
		info.fnGetObject = fnGetObject;
		info.type = type;
		info.fnAssignTpl = fnAssignTpl;
		info.fnAfterLoad = fnAfterLoad;
		info.fnSpecLoad = fnSpecLoad;
		m_arrExtraLoad.push_back( info );
	} // void AddExtraLoadInfo

} // namespace UdSDK
