
#ifndef WNDMENU_H
#define WNDMENU_H

namespace UdSDK {

	class CWndMenu : public udWndGraphic {
		typedef map< string, udPWndMenuBtn > _asc_btn; // ассоциативный массив кнопок
		typedef map< udInt, udPWndMenuBtn > _int_asc_btn; // массив кнопок
		
		udPModelWork	m_pModel;		// модель
		_int_asc_btn	m_arrButton;	// массив кнопок
		_asc_btn		m_mapButton;	// карта кнопок
		udInt			m_iSelected;	// выбранный

	public:
		CWndMenu( );
		CWndMenu( const CWndMenu& objCopy );

		virtual ~CWndMenu( );

		CWndMenu& operator =( const CWndMenu& objCopy );

		udPWndMenuBtn	AddItem( string index, wstring title, bool selected = false, bool create = true );
		bool			SelectItem( string index );
		string			CurrentItemIndex( ) const;
		void			ClearCurrentItem( );

		virtual void	Render( );

		virtual LRESULT	msgBtnClick( udPWndMenuBtn btn );

		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void SetModel( udPModelWork pModel ) { m_pModel = pModel; }

		udPBitmap	GetBackground( )	const ;

	}; // class CWndMenu

} // namespace UdSDK

#endif
