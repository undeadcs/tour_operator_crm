
#include "includes.h"

namespace UdSDK {

	void ClearArrType( udAscType& a ) {
		for( udAscType::iterator i = a.begin( ), j = a.end( ); i != j; ++i ) {
			delete i->second;
		}
		a.clear( );
	} // void ClearArrType

	void ClearArrAssoc( udArrAsc& a ) {
		for( udArrAsc::iterator i = a.begin( ), j = a.end( ); i != j; ++i ) {
			for( udAscType::iterator v = i->begin( ), w = i->end( ); v != w; ++v ) {
				delete v->second;
			}
			i->clear( );
		}
		a.clear( );
	} // void ClearAssoc

	void ClearArrAssoc( udPArrAsc a ) {
		for( udArrAsc::iterator i = a->begin( ); i != a->end( ); ++i ) {
			for( udAscType::iterator v = i->begin( ), w = i->end( ); v != w; ++v ) {
				delete v->second;
			}
			i->clear( );
		}
		a->clear( );
	} // void ClearAssoc

	void ClearArrMorph( udArrMorph& a ) {
		for( udArrMorph::iterator i = a.begin( ); i != a.end( ); ++i ) {
			delete *i;
		}
		a.clear( );
	} // void ClearArrMorph

	void ClearArrMorph( udPArrMorph a ) {
		for( udArrMorph::iterator i = a->begin( ); i != a->end( ); ++i ) {
			delete *i;
		}
		a->clear( );
	} // void ClearArrMorph

	void ClearArrBase( udArrBase& a ) {
		for( udArrBase::iterator i = a.begin( ); i != a.end( ); ++i ) {
			delete *i;
		}
		a.clear( );
	} // void ClearArrBase

	void ClearArrBase( udPArrBase a ) {
		for( udArrBase::iterator i = a->begin( ); i != a->end( ); ++i ) {
			delete *i;
		}
		a->clear( );
	} // void ClearArrBase

	void ClearAscBase( udAscBase& a ) {
		for( udAscBase::iterator i = a.begin( ); i != a.end( ); ++i ) {
			delete i->second;
		}
		a.clear( );
	} // void ClearAscBase

	void ClearAscBase( udPAscBase a ) {
		for( udAscBase::iterator i = a->begin( ); i != a->end( ); ++i ) {
			delete i->second;
		}
		a->clear( );
	} // void ClearAscBase

	void ClearDwAscBase( udDwAscBase& a ) {
		for( udDwAscBase::iterator i = a.begin( ); i != a.end( ); ++i ) {
			delete i->second;
		}
		a.clear( );
	} // void ClearDwAscBase

	udPWord AlignToDWord( udPWord w ) {
		udDWord ret;

		ret = ( udDWord ) w;
		ret ++;
		ret >>=1;
		ret <<=1;
		return ( udPWord ) ret;
	} // udPWord AlignToDWord

namespace mb {

	void err( LPCWSTR szTitle ) {
		MessageBox( NULL, szTitle, L"ERROR", MB_OK | MB_ICONERROR );
	} // void err

	void err( HWND hWnd, LPCWSTR szTitle ) {
		MessageBox( hWnd, szTitle, L"ERROR", MB_OK | MB_ICONERROR );
	} // void err

	void inf( LPCWSTR szTitle ) {
		MessageBox( NULL, szTitle, L"INFO", MB_OK | MB_ICONINFORMATION );
	} // void inf

	void inf( HWND hWnd, LPCWSTR szTitle ) {
		MessageBox( hWnd, szTitle, L"INFO", MB_OK | MB_ICONINFORMATION );
	} // void inf

	void err( Gtk::Window& wndParent, const Glib::ustring& szTitle, const Glib::ustring& szText ) {
		Gtk::MessageDialog dialog(
			wndParent,
			szTitle,
			false,
			Gtk::MESSAGE_ERROR,
			Gtk::BUTTONS_OK
		);
		if ( !szText.empty( ) ) {
			dialog.set_secondary_text( szText );
		}
		dialog.run( );
	} // void err

	void inf( Gtk::Window& wndParent, const Glib::ustring& szTitle, const Glib::ustring& szText ) {
		Gtk::MessageDialog dialog(
			wndParent,
			szTitle,
			false,
			Gtk::MESSAGE_INFO,
			Gtk::BUTTONS_OK
		);
		if ( !szText.empty( ) ) {
			dialog.set_secondary_text( szText );
		}
		dialog.run( );
	} // void err

} // namespace mb

} // namespace UdSDK