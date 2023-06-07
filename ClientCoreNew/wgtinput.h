/**
	Tourist application
	Библиотека ядра клиентского приложения
	Виджет однострочного поля ввода
*/
#ifndef TOURIST_CLIENT_CORE_WGT_INPUT_H
#define TOURIST_CLIENT_CORE_WGT_INPUT_H

namespace UdSDK {

	class CWgtInput : public Gtk::EventBox {
	public:
		class Entry : public Gtk::Entry {
			Glib::ustring	m_szValue;

		public:
			Entry( const Glib::ustring& szValue = "" );
			Entry( const Entry& objCopy );

			virtual ~Entry( );

			Entry& operator =( const Entry& objCopy );

			void	SetValue( const Glib::ustring& szValue );

			Glib::ustring	GetValue( ) const { return m_szValue; }

		protected:
			virtual void	on_changed( );

		private:
			void	InitValue( );
		};

	private:
		Glib::ustring	m_szDefaultValue;
		Gtk::HBox		m_boxInput;
		Entry*			m_inpText;

	public:
		CWgtInput( );
		CWgtInput( const Glib::ustring& szDefault, const Glib::ustring& szValue );
		CWgtInput( const CWgtInput& objCopy );

		virtual ~CWgtInput( );

		CWgtInput& operator =( const CWgtInput& objCopy );

		void	Reset( )											{ m_inpText->SetValue( GetDefaultValue( ) );	}
		void	SetValue( const Glib::ustring& szValue )			{ m_inpText->SetValue( szValue );				}
		void	SetDefaultValue( const Glib::ustring& szDefault )	{ m_szDefaultValue = szDefault;					}

		Glib::ustring	GetValue( )			const { return m_inpText->GetValue( );	}
		Glib::ustring	GetDefaultValue( )	const { return m_szDefaultValue;		}
		Gtk::Entry*		GetEntry( )			const { return m_inpText;				}

	protected:
		virtual bool	OnExpose( GdkEventExpose* evtExpose );

	}; // class CWgtInput
/*
	class CWgtInput : public Gtk::EventBox {
		Glib::ustring	m_szDefault,
						m_szValue;
		Gtk::HBox		m_boxInput;
		Gtk::Entry*		m_inpText;

	public:
		CWgtInput( );
		CWgtInput( const Glib::ustring& szDefault, const Glib::ustring& szValue );
		CWgtInput( const CWgtInput& objCopy );

		virtual ~CWgtInput( );

		CWgtInput& operator =( const CWgtInput& objCopy );

		void	Reset( );
		void	SetValue( const Glib::ustring& szValue );
		void	SetDefaultValue( const Glib::ustring& szDefault );
		CWgtInput&	LoadValue( );

		Glib::ustring	GetValue( )			const { return m_szValue;	}
		Glib::ustring	GetDefaultValue( )	const { return m_szDefault;	}
		Gtk::Entry*		GetEntry( )			const { return m_inpText;	}

	protected:
		virtual bool	OnExpose( GdkEventExpose* evtExpose );

	}; // class CWgtInput
//*/
} // namespace UdSDK

#endif
