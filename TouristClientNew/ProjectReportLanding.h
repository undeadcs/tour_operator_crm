/*
	Tourist application
	Клиентское приложение
	Отчет "Посадочная ведомость"
*/
#ifndef TOURIST_CLIENT_REPORT_LANDING_H
#define TOURIST_CLIENT_REPORT_LANDING_H

namespace UdSDK {

	class CDAReportLanding : public udDAList {

	public:
		CDAReportLanding( );
		CDAReportLanding( const CDAReportLanding& objCopy );

		virtual ~CDAReportLanding( );

		CDAReportLanding& operator =( const CDAReportLanding& objCopy );

		virtual void	Load( );

		void	LoadRemote( );

	}; // class CDAReportLanding

	udPBase	GetProjectReportLandingList( );

	class CHReportLanding : public udListHandler {

	public:
		CHReportLanding( );
		CHReportLanding( const CHReportLanding& objCopy );

		virtual ~CHReportLanding( );

		CHReportLanding& operator =( const CHReportLanding& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelReportLanding : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelReportLanding( ) {
				add( m_col_id );
				add( m_col_group );
				add( m_col_fio );
				add( m_col_phone );
				add( m_col_state );
				add( m_col_has_packet );
				add( m_col_has_landing );
				add( m_col_comment );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_group;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_fio;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_phone;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_state;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_has_packet;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_has_landing;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_comment;
		};

	protected:
		ColModelReportLanding *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltDate1,
						m_lblFltDate2,
						m_lblFltGroup,
						m_lblFltDirect;
		udWgtDatePick	m_inpFltDate1,
						m_inpFltDate2;
		udWgtListPick	m_inpFltGroup,
						m_inpFltDirect;

		enum {
			fltGroup,
			fltDirect
		};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// дополнительные кнопки

		Gtk::Button		m_btnLoad,
						m_btnExport;

		void	OnButtonLoad( );
		void	OnButtonExport( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHReportLanding

	class CHReportLandingModeless : public udListHandlerModeless, public CHReportLanding {

	public:
		CHReportLandingModeless( );
		CHReportLandingModeless( const CHReportLandingModeless& objCopy );

		virtual ~CHReportLandingModeless( );

		CHReportLandingModeless& operator =( const CHReportLandingModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHReportLandingModeless

} // namespace UdSDK

#endif
