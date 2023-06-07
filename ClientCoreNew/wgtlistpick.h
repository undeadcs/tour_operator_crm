/*
	Tourist application
	Библиотека ядра клиентского приложения
	Виджет выбора из списка выбора
*/
#ifndef TOURIST_CLIENT_CORE_WGT_LISTPICK_H
#define TOURIST_CLIENT_CORE_WGT_LISTPICK_H

namespace UdSDK {

	class CWgtListPick : public Gtk::HBox {
	protected:
		Gtk::Button*		m_btnList;
		Gtk::Label			m_lblTitle;
		pfnGetListHandlerPick
							m_fnMakeHandler;
		pfnGetFormHandler	m_fnMakeFormHandler;
		Glib::ustring		m_szWindowTitle,
							m_szFormWindowTitle;
		udPFDEObject		m_pFde;
		string				m_szAttr;
		Gtk::EventBox		m_boxLabel;
		Gtk::HBox			m_boxLabelWrap;

	public:
		CWgtListPick( );
		CWgtListPick( const CWgtListPick& objCopy );

		virtual ~CWgtListPick( );

		CWgtListPick& operator =( const CWgtListPick& objCopy );

		void	SetWindowTitle( const Glib::ustring& szTitle )		{ m_szWindowTitle	= szTitle;		}
		void	SetFormWindowTitle( const Glib::ustring& szTitle )	{ m_szFormWindowTitle	= szTitle;	}
		void	SetDataObject( udPFDEObject pFde )					{ m_pFde			= pFde;			}
		void	SetAttr( const string& szName )						{ m_szAttr			= szName;		}
		void	SetTitle( const Glib::ustring& szTitle )			{ m_lblTitle.set_text( szTitle );	}
		void	SetButtonTitle( const Glib::ustring& szTitle )		{ m_btnList->set_label( szTitle );	}
		void	SetGetListHandler( pfnGetListHandlerPick fn )		{ m_fnMakeHandler = fn;				}
		void	SetGetFormHandler( pfnGetFormHandler fn )			{ m_fnMakeFormHandler = fn;			}

		Gtk::Button*	GetButton( ) const { return m_btnList; }

	protected:
		virtual void	OnButtonClick( );
		virtual bool	OnExpose( GdkEventExpose* evtExpose );
		virtual bool	OnClick( GdkEventButton* evtClick );

	public:
		class Handler {
		public:
			virtual void	AfterPick( udPWgtListPick wgt ) = 0;
		};

		void	SetHandler( Handler* pHandler ) { m_pHandler = pHandler; }

	protected:
		Handler*	m_pHandler;

	}; // class CWgtListPick

} // namespace UdSDK

#endif
