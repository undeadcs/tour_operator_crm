/*
	Tourist application
	Библиотека ядра клиентского приложения
	Обработчик списка
*/
#ifndef TOURIST_CLIENT_CORE_LIST_HANDLER_H
#define TOURIST_CLIENT_CORE_LIST_HANDLER_H

namespace UdSDK {

	class CListHandler : public udStateHandler {
	public:
		typedef vector< udPFormDataExtra > t_vctArrFde;

		enum {
			ordDesc,
			ordAsc
		};

	protected:
		udPWndHandler	m_wndListWrap;	// окно списка объектов
		Gtk::TreeModel::ColumnRecord*
						m_pRowModel;	// модель списка
		Gtk::TreeView	m_objTreeView;	// вью списка
		Glib::RefPtr< Gtk::ListStore >
						m_objTreeModel;	// модель списка
		udPDAList		m_pData;		// загрузчик данных
		Gtk::HButtonBox	m_hbbTop;		// область с кнопками общего назначения
		Gtk::HBox		m_hbbPager;		// область кнопок пейджера
		Gtk::Table		m_tblFilter;	// таблица фильтра
		Gtk::Button		m_btnFilter,	// кнопка фильтрации
						m_btnReset;		// кнопка сброса

	public:
		CListHandler( );
		CListHandler( const CListHandler& objCopy );

		virtual ~CListHandler( );

		CListHandler& operator =( const CListHandler& objCopy );

		virtual void	Init( );
		virtual void	Load( );
		virtual void	Show( );
		virtual void	Hide( );
		virtual void	Clear( );
		
		virtual void	OnButtonPager( udInt iPage );
		virtual void	GeneratePager( );

		virtual void	FilterUiClear( ) = 0;
		virtual void	FilterUiToObject( ) = 0;
		virtual void	OnFilter( );
		virtual void	OnReset( );

		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column ) { }

		virtual void	ReplaceDataAccess( udPDAList pData );

		udPWndHandler	GetWindow( ) const { return m_wndListWrap; }

	}; // class CListHandler

} // namespace UdSDK

#endif
