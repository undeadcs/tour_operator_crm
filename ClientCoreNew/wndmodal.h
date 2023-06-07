/**
	Tourist application
	Библиотека ядра клиентского приложения
	Модальные окна
*/
#ifndef TOURIST_CLIENT_CORE_WND_MODAL_H
#define TOURIST_CLIENT_CORE_WND_MODAL_H

namespace UdSDK {

	void ModalWindow(
		Gtk::Window* wndParent, udPWndHandler wndChild,
		const Glib::ustring& szTitle,
		udInt iWidth = 800, udInt iHeight = 600
	);

} // namespace UdSDK

#endif
