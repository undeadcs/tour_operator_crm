/*
	Tourist application
	Клиентское приложение
	Отчет "Оплата гостиниц"
*/
#ifndef TOURIST_CLIENT_REPORT_HOTEL_PAY_H
#define TOURIST_CLIENT_REPORT_HOTEL_PAY_H

namespace UdSDK {

	class CDAReportHotelPay : public udDAList {

	public:
		CDAReportHotelPay( );
		CDAReportHotelPay( const CDAReportHotelPay& objCopy );

		virtual ~CDAReportHotelPay( );

		CDAReportHotelPay& operator =( const CDAReportHotelPay& objCopy );

		virtual void	Load( );

		int	LoadRemote( );

	}; // class CDAReportHotelPay

	udPBase	GetProjectReportHotelPayList( );

	class CHReportHotelPay : public udListHandler {

	public:
		CHReportHotelPay( );
		CHReportHotelPay( const CHReportHotelPay& objCopy );

		virtual ~CHReportHotelPay( );

		CHReportHotelPay& operator =( const CHReportHotelPay& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelReportHotelPay : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelReportHotelPay( ) {
				add( m_col_id );
				add( m_col_date1 );
				add( m_col_date2 );
				add( m_col_leader );
				add( m_col_num );
				add( m_col_name );
				add( m_col_price );
				add( m_col_price_and_num );
				add( m_col_price_cny );
				add( m_col_price_cny_and_num );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_date1;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_date2;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_leader;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_num;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_name;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_price;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_price_and_num;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_price_cny;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_price_cny_and_num;
			Gtk::TreeModelColumn< udPBase > m_col_object;
		};

	protected:
		ColModelReportHotelPay *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltDate1,
						m_lblFltDate2,
						m_lblFltDirect;
		udWgtDatePick	m_inpFltDate1,
						m_inpFltDate2;
		udWgtListPick	m_inpFltDirect;

		enum {
			fltDirect
		};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// дополнительные кнопки

		Gtk::Button		m_btnLoad,
						m_btnExport;

		void	OnButtonLoad( );
		void	OnButtonExport( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

		virtual void	OnRowActivated( const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column );

	}; // class CHReportHotelPay

	class CHReportHotelPayModeless : public udListHandlerModeless, public CHReportHotelPay {

	public:
		CHReportHotelPayModeless( );
		CHReportHotelPayModeless( const CHReportHotelPayModeless& objCopy );

		virtual ~CHReportHotelPayModeless( );

		CHReportHotelPayModeless& operator =( const CHReportHotelPayModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHReportHotelPayModeless

} // namespace UdSDK

#endif
