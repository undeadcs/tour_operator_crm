/*
	Tourist application
	Библиотека ядра клиентского приложения
	Виджет списка сущностей
*/
#ifndef TOURIST_CLIENT_CORE_WGT_FORMLIST_H
#define TOURIST_CLIENT_CORE_WGT_FORMLIST_H

namespace UdSDK {

	class CWgtFormList : public Gtk::VBox {
		Gtk::ScrolledWindow		m_wndScroll;
		Gtk::TreeView*			m_wndList;
		Gtk::HButtonBox			m_boxButton;
		vector< Gtk::Button* >	m_arrButton;

	public:
		CWgtFormList( );
		CWgtFormList( const CWgtFormList& objCopy );

		virtual ~CWgtFormList( );

		CWgtFormList& operator =( const CWgtFormList& objCopy );

		void	SetColumnDefaults( );

		Gtk::Button*	AddButton( const Glib::ustring& szTitle );

		Gtk::TreeView*	GetTreeView( ) const { return m_wndList; }

	}; // class CWgtFormList

} // namespace UdSDK

#endif
