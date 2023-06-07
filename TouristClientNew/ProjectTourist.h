/*
	Tourist application
	Клиентское приложение
	Турист
*/
#ifndef TOURIST_CLIENT_TOURIST_H
#define TOURIST_CLIENT_TOURIST_H

namespace UdSDK {

	class CDAListTourist : public udDAList {
		udPHTourForm	m_pTourForm;

	public:
		CDAListTourist( );
		CDAListTourist( const CDAListTourist& objCopy );

		virtual ~CDAListTourist( );

		CDAListTourist& operator =( const CDAListTourist& objCopy );

		virtual void	Load( );

		void	SetTourForm( udPHTourForm pTourForm ) { m_pTourForm = pTourForm; }

	}; // class CDAListTouristTourist

	class CHTourist : public udListHandler {
	protected:
		udPHTourForm m_pTourForm;
		map< Glib::ustring, udPBase >	m_mapObject; // соотношение строка => объект

	public:
		enum {
			opidGet2 = opidGet + 1,	// получение списка клиентов, по заданным ID
			opidGet3,				// получение клиентов по ID (без фильтров и пейджера)
		};

	public:
		CHTourist( );
		CHTourist( const CHTourist& objCopy );

		virtual ~CHTourist( );

		CHTourist& operator =( const CHTourist& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		void	SetTourForm( udPHTourForm pTourForm );

		class ColModelTourist : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelTourist( ) {
				add( m_col_id );
				add( m_col_fio );
				add( m_col_born );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_fio;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_born;
		};

	protected:
		ColModelTourist *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltFio,
						m_lblFltDate1,
						m_lblFltDate2;
		udWgtInput		m_inpFltFio;
		udWgtDatePick	m_inpFltDate1,
						m_inpFltDate2;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHTourist

	class CHTouristPick : public udListHandlerPick, public CHTourist {

	public:
		CHTouristPick( );
		CHTouristPick( const CHTouristPick& objCopy );

		virtual ~CHTouristPick( );

		CHTouristPick& operator =( const CHTouristPick& objCopy );

		virtual void	Init( );
		virtual void	OnButtonForm( );
		virtual void	OnButtonOk( );
		virtual void	OnButtonCancel( );
		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

		virtual void BeforeCreate( ) {
			Init( );
			Load( );
		} // void BeforeCreate

		//virtual udPWndHandler	GetWindowPick( ) const { return m_wndListWrap; }
		virtual udPWndHandler	GetWindow( ) const { return m_wndListWrap; }

	}; // class CHTouristPick

} // namespace UdSDK

#endif
