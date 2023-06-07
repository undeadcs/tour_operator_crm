
#include "includes.h"
#include "externals.h"

namespace UdSDK {
	
	CWgtFilePick::CWgtFilePick( ) :
		Gtk::HBox( ),
		m_btnPick( "+" ),
		m_lblTitle( "" ),
		m_pFde( NULL )
	{
		pack_start( m_btnPick, Gtk::PACK_SHRINK, 0 );
		pack_start( m_lblTitle );
		show_all_children( );

		m_btnPick.signal_clicked( ).connect( sigc::mem_fun( this, &CWgtFilePick::OnButtonClick ) );
	} // CWgtFilePick

	CWgtFilePick::CWgtFilePick( const CWgtFilePick& objCopy ) : Gtk::HBox( ) {
	} // CWgtFilePick

	CWgtFilePick::~CWgtFilePick( ) {
	} // ~CWgtFilePick

	CWgtFilePick& CWgtFilePick::operator =( const CWgtFilePick& objCopy ) {
		return *this;
	} // CWgtFilePick& operator =

	void CWgtFilePick::OnButtonClick( ) {
		if ( !m_pFde ) {
			return;
		}

		Gtk::Window *wndParent = ( Gtk::Window* ) get_toplevel( );
		Gtk::FileChooserDialog dlgPickFile( *wndParent, m_szWindowTitle );
		dlgPickFile.add_button( Glib::locale_to_utf8( "Отмена" ), Gtk::RESPONSE_CANCEL );
		dlgPickFile.add_button( Glib::locale_to_utf8( "Выбрать" ), Gtk::RESPONSE_OK );

		int code = dlgPickFile.run( );

		if ( code == Gtk::RESPONSE_OK ) {
			Glib::RefPtr< Gio::File > file = dlgPickFile.get_file( );
			wstring szFileName	= ConvertString( file->get_basename( ), CP_UTF8 ),
					szFilePath	= L"";

			if ( file->has_parent( ) ) {
				szFilePath = ConvertString( file->get_parent( )->get_path( ), CP_UTF8 );
			}

			wstring wFileTmpPath = m_pFde->GetFolder( ) + L"\\" + szFileName;
			Glib::RefPtr< Gio::File > fileTo = Gio::File::create_for_path( ConvertString( wFileTmpPath, CP_UTF8 ) );

			if ( file->copy( fileTo, Gio::FILE_COPY_OVERWRITE ) ) {
				m_pFde->SetName( szFileName );
				m_lblTitle.set_text( Glib::locale_to_utf8( ConvertString( szFileName, CP_UTF8 ) ) );
			} else {
				Gtk::MessageDialog dialog(
					*wndParent,
					Glib::locale_to_utf8( "Не удалось скопировать файл" ),
					false,
					Gtk::MESSAGE_ERROR,
					Gtk::BUTTONS_OK
				);
				dialog.set_secondary_text(
					Glib::locale_to_utf8( "Файл '" ) +
					file->get_basename( ) +
					Glib::locale_to_utf8( "'\nПовторите попытку или выберите другой." )
				);
				dialog.run( );
			}
		}
	} // void OnButtonClick

} // namespace UdSDK
