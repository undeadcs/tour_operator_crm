/*
	Tourist application
	Библиотека ядра клиентского приложения
	Виджет выбора файла
*/
#ifndef TOURIST_CLIENT_CORE_WGT_FILEPICK_H
#define TOURIST_CLIENT_CORE_WGT_FILEPICK_H

namespace UdSDK {

	class CWgtFilePick : public Gtk::HBox {
	protected:
		Gtk::Button		m_btnPick;
		Gtk::Label		m_lblTitle;
		Glib::ustring	m_szWindowTitle;
		udPFDEFile		m_pFde;

	public:
		CWgtFilePick( );
		CWgtFilePick( const CWgtFilePick& objCopy );

		virtual ~CWgtFilePick( );

		CWgtFilePick& operator =( const CWgtFilePick& objCopy );

		void	SetWindowTitle( const Glib::ustring& szTitle )	{ m_szWindowTitle	= szTitle;		}
		void	SetDataObject( udPFDEFile pFde )				{ m_pFde			= pFde;			}
		void	SetTitle( const Glib::ustring& szTitle )		{ m_lblTitle.set_text( szTitle );	}
		void	SetButtonTitle( const Glib::ustring& szTitle )	{ m_btnPick.set_label( szTitle );	}

		virtual void	OnButtonClick( );

	}; // class CWgtListPick

} // namespace UdSDK

#endif
