
#ifndef WNDTOOLBAR_H
#define WNDTOOLBAR_H

namespace UdSDK {

	class CWndToolbar : public udWndGraphic {
		typedef map< string, udPWndToolbarBtn > _asc_btn;
		typedef map< udInt, udPWndToolbarBtn > _int_asc_btn;

		_int_asc_btn	m_arrButton;	// массив кнопок
		_asc_btn		m_mapButton;	// карта кнопок
		udPModelWork	m_pModel;		// модель

	public:
		CWndToolbar( );
		CWndToolbar( const CWndToolbar& objCopy );

		virtual ~CWndToolbar( );

		CWndToolbar& operator =( const CWndToolbar& objCopy );

		void		AddItem( udPWndToolbarBtn btn, string index );
		udPWndToolbarBtn AddItem( string index, wstring title, bool enabled = true, bool create = true );
		void		EnableItem( string index, bool enabled = true );
		
		virtual void	Render( );
		
		virtual LRESULT	msgBtnClick( udPWndToolbarBtn btn );
		
		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		udPBitmap	GetBackground( ) const;

		void SetModel( udPModelWork pModel ) { m_pModel = pModel; }

	}; // class CWndMenu

} // namespace UdSDK

#endif
