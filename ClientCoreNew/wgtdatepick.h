/*
	Tourist appliction
	Библиотека ядра клиентского приложения
	Виджет выборка даты
*/
#ifndef TOURIST_CLIENT_CORE_WGT_DATE_PICK_H
#define TOURIST_CLIENT_CORE_WGT_DATE_PICK_H

namespace UdSDK {

	class CWgtDatePick : public Gtk::EventBox {
	public:
		class Entry : public Gtk::Entry {
			udUInt	m_iYear,
					m_iMonth,
					m_iDay;

		public:
			Entry( );
			Entry( const Entry& objCopy );
			Entry( const udDatetime& dtValue );

			virtual ~Entry( );

			Entry& operator =( const Entry& objCopy );

			void	SetValue( const udDatetime& dtValue );

			udDatetime	GetValue( ) const { return udDatetime( toString( m_iYear, "%02d" ) + "-" + toString( m_iMonth, "%02d" ) + "-" + toString( m_iDay, "%02d" ) ); }
			
		protected:
			virtual void	on_insert_text( const Glib::ustring& szText, int* iPosition );
			virtual void	on_delete_text( int iBegin, int iEnd );

		private:
			void	InitValue( );
		};

	private:
		Gtk::HBox	m_boxInput;
		Entry*		m_inpText;
		udUInt		m_iDefaultYear,
					m_iDefaultMonth,
					m_iDefaultDay;

	public:
		CWgtDatePick( );
		CWgtDatePick( const CWgtDatePick& objCopy );
		CWgtDatePick( const udDatetime& dtValue, const udDatetime& dtDefaultValue );

		virtual ~CWgtDatePick( );

		CWgtDatePick& operator =( const CWgtDatePick& objCopy );

		void	Reset( )												{ m_inpText->SetValue( GetDefaultValue( ) ); }
		void	SetValue( const udDatetime& dtValue )					{ m_inpText->SetValue( dtValue ); }
		void	SetDefaultValue( const udDatetime& dtDefaultValue );

		udDatetime	GetValue( )			const { return m_inpText->GetValue( );	}
		udDatetime	GetDefaultValue( )	const { return udDatetime( toString( m_iDefaultYear, "%02d" ) + "-" + toString( m_iDefaultMonth, "%02d" ) + "-" + toString( m_iDefaultDay, "%02d" ) ); }
		Gtk::Entry*	GetEntry( )			const { return m_inpText;				}
		
	private:
		virtual bool	OnExpose( GdkEventExpose* evtExpose );
	};
/*
	class CWgtDatePick : public Gtk::HBox {
		Gtk::SpinButton	m_inpDay,
						m_inpMonth,
						m_inpYear;
		udDouble		m_fDay,
						m_fMonth,
						m_fYear,
						m_fDefaultDay,
						m_fDefaultMonth,
						m_fDefaultYear;

	public:
		CWgtDatePick( );
		CWgtDatePick( const CWgtDatePick& objCopy );
		CWgtDatePick(
			udDouble fYear, udDouble fMonth, udDouble fDay,
			udDouble fDefaultYear, udDouble fDefaultMonth, udDouble fDefaultDay
		);
		CWgtDatePick( const string& szValue, const string& szDefaultValue );
		CWgtDatePick( const wstring& szValue, const wstring& szDefaultValue );
		CWgtDatePick( const udDatetime& dtValue, const udDatetime& dtDefaultValue );

		virtual ~CWgtDatePick( );

		CWgtDatePick& operator =( const CWgtDatePick& objCopy );

		void	Reset( );
		void	SetValue( const string& szValue );
		void	SetValue( const wstring& szValue );
		void	SetValue( const udDatetime& dtValue );
		void	SetValue( udDouble fYear, udDouble fMonth, udDouble fDay );
		void	SetDefaultValue( const string& szValue );
		void	SetDefaultValue( const wstring& szValue );
		void	SetDefaultValue( const udDatetime& dtValue );
		void	SetDefaultValue( udDouble fYear, udDouble fMonth, udDouble fDay );
		CWgtDatePick&	LoadValue( );

		string		GetValue( const string& ) const;
		wstring		GetValue( const wstring& ) const;
		udDatetime	GetValue( const udDatetime& ) const;

		string		GetDefaultValue( const string& ) const;
		wstring		GetDefaultValue( const wstring& ) const;
		udDatetime	GetDefaultValue( const udDatetime& ) const;

		udDouble	GetYear( )	const { return m_fYear;		}
		udDouble	GetMonth( )	const { return m_fMonth;	}
		udDouble	GetDay( )	const { return m_fDay;		}

		udDouble	GetDefaultYear( )	const { return m_fDefaultYear;	}
		udDouble	GetDefaultMonth( )	const { return m_fDefaultMonth;	}
		udDouble	GetDefaultDay( )	const { return m_fDefaultDay;	}

		bool	OnOutputDay( );
		bool	OnOutputMonth( );
		bool	OnOutputYear( );

	private:
		void	Init( );
		void	InitValue( );

	};
//*/
} // namespace UdSDK

#endif
