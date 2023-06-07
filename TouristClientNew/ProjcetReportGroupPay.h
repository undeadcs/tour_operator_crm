/*
	Tourist application
	Клиентское приложение
	Отчет "Оплата группы"
*/
#ifndef TOURIST_CLIENT_REPORT_GROUP_PAY_H
#define TOURIST_CLIENT_REPORT_GROUP_PAY_H

namespace UdSDK {

	class CDAReportGroupPay : public udDAList {
		udPBase	m_pGroup,
				m_pGroupPeriod,
				m_pGroupLeader;

	public:
		CDAReportGroupPay( );
		CDAReportGroupPay( const CDAReportGroupPay& objCopy );

		virtual ~CDAReportGroupPay( );

		CDAReportGroupPay& operator =( const CDAReportGroupPay& objCopy );

		virtual void	Load( );

		void	LoadRemote( );

		udPBase	GetGroup( )		const { return m_pGroup;		}
		udPBase	GetPeriod( )	const { return m_pGroupPeriod;	}
		udPBase	GetLeader( )	const { return m_pGroupLeader;	}

	}; // class CDAReportGroupPay

	udPBase	GetProjectReportGroupPayList( );

	class CHReportGroupPay : public udListHandler {

	public:
		CHReportGroupPay( );
		CHReportGroupPay( const CHReportGroupPay& objCopy );

		virtual ~CHReportGroupPay( );

		CHReportGroupPay& operator =( const CHReportGroupPay& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelReportGroupPay : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelReportGroupPay( ) {
				add( m_col_id );
				add( m_col_fio );
				add( m_col_price );
				add( m_col_pay );
				add( m_col_date );
				add( m_col_place );
				add( m_col_hotel );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_fio;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_price;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_pay;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_date;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_place;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_hotel;
		};

	protected:
		ColModelReportGroupPay *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );

		Gtk::Label		m_lblFltGroup,
						m_lblFltPeriod;
		udWgtListPick	m_inpFltGroup;
		udWgtGroupPeriodLoadPick
						m_inpFltPeriod;

		enum {
			fltGroup,
			fltPeriod
		};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// дополнительные кнопки

		Gtk::Button		m_btnLoad,
						m_btnExport;

		void	OnButtonLoad( );
		void	OnButtonExport( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHReportGroupPay

	class CHReportGroupPayModeless : public udListHandlerModeless, public CHReportGroupPay {

	public:
		CHReportGroupPayModeless( );
		CHReportGroupPayModeless( const CHReportGroupPayModeless& objCopy );

		virtual ~CHReportGroupPayModeless( );

		CHReportGroupPayModeless& operator =( const CHReportGroupPayModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHReportGroupPayModeless

} // namespace UdSDK

#endif
