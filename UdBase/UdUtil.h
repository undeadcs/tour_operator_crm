
#ifndef UDUTIL_H
#define UDUTIL_H

namespace UdSDK {

	void	ClearArrType( udAscType& a );
	void	ClearArrAssoc( udArrAsc& a );
	void	ClearArrAssoc( udPArrAsc a );
	void	ClearArrMorph( udArrMorph& a );
	void	ClearArrMorph( udPArrMorph a );
	void	ClearArrBase( udArrBase& a );
	void	ClearArrBase( udPArrBase a );
	void	ClearAscBase( udAscBase& a );
	void	ClearAscBase( udPAscBase a );
	void	ClearDwAscBase( udDwAscBase& a );

	udPWord AlignToDWord( udPWord w );

namespace mb {

	void err( LPCWSTR szTitle );
	void err( HWND hWnd, LPCWSTR szTitle );
	void inf( LPCWSTR szTitle );
	void inf( HWND hWnd, LPCWSTR szTitle );

	void err( Gtk::Window& parent, const Glib::ustring& szTitle, const Glib::ustring& szText = "" );
	void inf( Gtk::Window& parent, const Glib::ustring& szTitle, const Glib::ustring& szText = "" );

}

} // namespace UdSDK

#endif
