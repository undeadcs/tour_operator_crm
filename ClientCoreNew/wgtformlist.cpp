
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWgtFormList::CWgtFormList( ) :
		Gtk::VBox( ),
		m_boxButton( Gtk::BUTTONBOX_CENTER, 5 ),
		m_wndList( new Gtk::TreeView )
	{
		m_wndScroll.add( *m_wndList );
		pack_start( m_wndScroll );
		pack_start( m_boxButton, Gtk::PACK_SHRINK, 0 );
		show_all( );

		m_wndList->set_enable_search( false );
		m_wndList->set_headers_clickable( false );
	} // CWgtFormList

	CWgtFormList::CWgtFormList( const CWgtFormList& objCopy ) : Gtk::VBox( ) {
	} // CWgtFormList

	CWgtFormList::~CWgtFormList( ) {
		for( vector< Gtk::Button* >::iterator i = m_arrButton.begin( ); i != m_arrButton.end( ); ++i ) {
			udDELETE( *i );
		}
		udDELETE( m_wndList );
	} // ~CWgtFormList

	CWgtFormList& CWgtFormList::operator =( const CWgtFormList& objCopy ) {
		return *this;
	} // CWgtFormList& operator =

	Gtk::Button* CWgtFormList::AddButton( const Glib::ustring& szTitle ) {
		Gtk::Button* btn = new Gtk::Button( szTitle );
		m_boxButton.pack_start( *btn );
		btn->show( );
		return btn;
	} // Gtk::Button* AddButton

	void CWgtFormList::SetColumnDefaults( ) {
		Glib::ListHandle< Gtk::TreeViewColumn* > arrColumn = m_wndList->get_columns( );
		Gtk::CellRenderer* renderer = NULL;
		Gtk::TreeViewColumn *col = NULL;

		for( Glib::ListHandle< Gtk::TreeViewColumn* >::iterator i = arrColumn.begin( ); i != arrColumn.end( ); ++i ) {
			col = *i;
			renderer = col->get_first_cell_renderer( );
			renderer->set_padding( 5, 8 );
			col->set_resizable( true );
			col->set_min_width( 100 );
		}
	} // void SetColumnDefaults

} // namespace UdSDK
