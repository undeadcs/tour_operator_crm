/*
	Tourist application
	Клиентское приложение
	Отчет "Оплата за день"
*/
#ifndef TOURIST_CLIENT_REPORT_DAY_PAY_H
#define TOURIST_CLIENT_REPORT_DAY_PAY_H

namespace UdSDK {

	class CDAReportDayPay : public udDAList {

	public:
		CDAReportDayPay( );
		CDAReportDayPay( const CDAReportDayPay& objCopy );

		virtual ~CDAReportDayPay( );

		CDAReportDayPay& operator =( const CDAReportDayPay& objCopy );

		virtual void	Load( );

		int	LoadRemote( );

	}; // class CDAReportGroupPay

	udPBase	GetProjectReportDayPayList( );

	class CHReportDayPay : public udListHandler {

	public:
		CHReportDayPay( );
		CHReportDayPay( const CHReportDayPay& objCopy );

		virtual ~CHReportDayPay( );

		CHReportDayPay& operator =( const CHReportDayPay& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelReportDayPay : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelReportDayPay( ) {
				add( m_col_id );
				add( m_col_fio );
				add( m_col_price );
				add( m_col_pay );
				add( m_col_date );
				add( m_col_place );
				add( m_col_hotel );
				add( m_col_group );
				add( m_col_period );
				add( m_col_manager );
				add( m_col_object );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_fio;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_price;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_pay;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_date;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_place;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_hotel;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_group;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_period;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_manager;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		};

	protected:
		ColModelReportDayPay *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltDate1,
						m_lblFltDate2;
		udWgtDatePick	m_inpFltDate1,
						m_inpFltDate2;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// дополнительные кнопки

		Gtk::Button		m_btnLoad,
						m_btnExport;

		void	OnButtonLoad( );
		void	OnButtonExport( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

	}; // class CHReportGroupPay

	class CHReportDayPayModeless : public udListHandlerModeless, public CHReportDayPay {

	public:
		CHReportDayPayModeless( );
		CHReportDayPayModeless( const CHReportDayPayModeless& objCopy );

		virtual ~CHReportDayPayModeless( );

		CHReportDayPayModeless& operator =( const CHReportDayPayModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHReportGroupPayModeless

} // namespace UdSDK

#endif
