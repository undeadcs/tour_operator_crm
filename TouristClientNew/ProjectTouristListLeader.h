/*
	TouristListLeader application
	Клиентское приложение
	Руководитель группы
*/
#ifndef TOURIST_CLIENT_TOURIST_LIST_LEADER_H
#define TOURIST_CLIENT_TOURIST_LIST_LEADER_H

namespace UdSDK {

	class CWgtTouristListLeaderPick : public udWgtListPick {
		udPHTouristListForm	m_pTouristListForm;

	public:
		CWgtTouristListLeaderPick( );
		CWgtTouristListLeaderPick( const CWgtTouristListLeaderPick& objCopy );

		virtual ~CWgtTouristListLeaderPick( );

		CWgtTouristListLeaderPick& operator =( const CWgtTouristListLeaderPick& objCopy );

		virtual void	OnButtonClick( );

		void	SetTouristListForm( udPHTouristListForm pTouristListForm ) { m_pTouristListForm = pTouristListForm; }

	}; // class CWgtTouristListLeaderPick

	class CDAListTouristListLeader : public udDAList {
		udPHTouristListForm	m_pTouristListForm;

	public:
		CDAListTouristListLeader( );
		CDAListTouristListLeader( const CDAListTouristListLeader& objCopy );

		virtual ~CDAListTouristListLeader( );

		CDAListTouristListLeader& operator =( const CDAListTouristListLeader& objCopy );

		virtual void	Load( );

		void	SetTourForm( udPHTouristListForm pTouristListForm ) { m_pTouristListForm = pTouristListForm; }

	}; // class CDAListTouristListLeaderTouristListLeader

	class CHTouristListLeader : public udListHandler {
	protected:
		udPHTouristListForm m_pTouristListForm;
		map< Glib::ustring, udPBase >	m_mapObject; // соотношение строка => объект

	public:
		enum {
			opidGet2 = opidGet + 1,	// получение списка клиентов, по заданным ID
			opidGet3,				// получение клиентов по ID (без фильтров и пейджера)
		};

	public:
		CHTouristListLeader( );
		CHTouristListLeader( const CHTouristListLeader& objCopy );

		virtual ~CHTouristListLeader( );

		CHTouristListLeader& operator =( const CHTouristListLeader& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		void	SetTouristListForm( udPHTouristListForm pTouristListForm );

		class ColModelTouristListLeader : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelTouristListLeader( ) {
				add( m_col_id );
				add( m_col_fio );
				add( m_col_born );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_fio;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_born;
		};

	protected:
		ColModelTouristListLeader *m_pColModel;

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

	}; // class CHTouristListLeader

	class CHTouristListLeaderPick : public udListHandlerPick, public CHTouristListLeader {

	public:
		CHTouristListLeaderPick( );
		CHTouristListLeaderPick( const CHTouristListLeaderPick& objCopy );

		virtual ~CHTouristListLeaderPick( );

		CHTouristListLeaderPick& operator =( const CHTouristListLeaderPick& objCopy );

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

	}; // class CHTouristListLeaderPick

} // namespace UdSDK

#endif
