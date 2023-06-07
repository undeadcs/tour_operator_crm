/*
	Tourist application
	Библиотека ядра клиентского приложения
	Обработчик с кнопкой на форму
*/
#ifndef TOURIST_CLIENT_CORE_LISTHANDLER_BTN_FORM_H
#define TOURIST_CLIENT_CORE_LISTHANDLER_BTN_FORM_H

namespace UdSDK {

	class CListHandlerBtnForm {
	protected:
		Gtk::Button		m_btnForm;

	public:
		CListHandlerBtnForm( );
		CListHandlerBtnForm( const CListHandlerBtnForm& objCopy );

		virtual ~CListHandlerBtnForm( );

		CListHandlerBtnForm& operator =( const CListHandlerBtnForm& objCopy );

		virtual void	OnButtonForm( ) = 0;

	}; // class CListHandlerBtnForm

} // namespace UdSDK

#endif
