
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndHandler::CWndHandler( ) : Gtk::VBox( ) {
		set_border_width( 0 );
	} // CWndHandler

	CWndHandler::CWndHandler( const CWndHandler& objCopy ) : Gtk::VBox( ) {
	} // CWndHandler

	CWndHandler::~CWndHandler( ) {
	} // ~CWndHandler

	CWndHandler& CWndHandler::operator =( const CWndHandler& objCopy ) {
		return *this;
	} // CWndHandler& operator =

} // namespace UdSDK
