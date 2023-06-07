/*
	Tourist application
	Библиотека ядра клиентского приложения
	Обработчик формы
*/
#ifndef TOURIST_CLIENT_CORE_FORM_HANDLER_H
#define TOURIST_CLIENT_CORE_FORM_HANDLER_H

namespace UdSDK {

	class CFormHandler : public udStateHandler {
	protected:
		udPDAForm		m_pData;
		Gtk::Button		m_btnOk,
						m_btnCancel;
		udPWndHandler	m_wndFormWrap;

	public:
		CFormHandler( );
		CFormHandler( const CFormHandler& objCopy );

		virtual ~CFormHandler( );

		CFormHandler& operator =( const CFormHandler& objCopy );
		
		virtual void	Init( ) { };//= 0;
		virtual void	Load( );
		virtual void	Show( ) { };//= 0;
		virtual void	Hide( ) { };//= 0;
		virtual void	Clear( );

		virtual void	OnButtonOk( ) = 0;
		virtual void	OnButtonCancel( ) = 0;

		udPWndHandler	GetWindow( )	const { return m_wndFormWrap;	}
		udPDAForm		GetData( )		const { return m_pData;			}

	}; // class CFormHandler

} // namespace UdSDK

#endif
