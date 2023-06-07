
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	void ModalWindow( Gtk::Window* wndParent, udPWndHandler wndChild, const Glib::ustring& szTitle, udInt iWidth, udInt iHeight ) {
		Gtk::ScrolledWindow wndScroll;
		Gtk::Window wnd;
		wnd.set_position( Gtk::WIN_POS_CENTER_ON_PARENT );
		wnd.set_size_request( iWidth, iHeight );
		wnd.set_modal( true );
		wnd.set_title( szTitle );
		wnd.set_transient_for( *wndParent );
		wnd.add( wndScroll );
		wndScroll.add( *wndChild );
		wnd.set_transient_for( *wndParent );
		wndChild->show( );
		wndScroll.show( );
		wnd.show( );
		Gtk::Main::run( wnd );
	} // void ModalWindow

} // namespace UdSDK
