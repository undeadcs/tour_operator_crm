/*
	Tourist application
	Клиентское приложение
	Логи
*/
#ifndef TOURIST_CLIENT_LOG_H
#define TOURIST_CLIENT_LOG_H

namespace UdSDK {

	udPBase	GetProjectLogList( );

	class CHLog : public udListHandler {

	public:
		CHLog( );
		CHLog( const CHLog& objCopy );

		virtual ~CHLog( );

		CHLog& operator =( const CHLog& objCopy );

		virtual void	Init( );
		virtual void	Load( );

		class ColModelLog : public Gtk::TreeModel::ColumnRecord {
		public:
			ColModelLog( ) {
				add( m_col_id );
				add( m_col_created );
				add( m_col_user );
				add( m_col_action );
			}

			Gtk::TreeModelColumn< udUInt > m_col_id;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_created;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_user;
			Gtk::TreeModelColumn< Glib::ustring	> m_col_action;
		};

	protected:
		ColModelLog *m_pColModel;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// фильтр
	public:
		virtual void	FilterUiClear( );
		virtual void	FilterUiToObject( );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	}; // class CHLog

	class CHLogModeless : public udListHandlerModeless, public CHLog {

	public:
		CHLogModeless( );
		CHLogModeless( const CHLogModeless& objCopy );

		virtual ~CHLogModeless( );

		CHLogModeless& operator =( const CHLogModeless& objCopy );

		virtual void	Init( );
		virtual void	Show( ) { udListHandlerModeless::Show( ); }

		virtual string			GetModelessName( ) const { return m_szName; }
		virtual udPWndHandler	GetModelessWindow( ) const { return m_wndListWrap; }

	}; // class CHLogModeless

} // namespace UdSDK

#endif
