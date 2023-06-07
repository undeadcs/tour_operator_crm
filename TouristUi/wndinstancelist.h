/**
	Список сущностей
*/
#ifndef WNDINSTANCELIST_H
#define WNDINSTANCELIST_H

namespace UdSDK {

	class CWndInstanceList : public udWndGraphic {
		typedef CWndButton* _pcell;
		typedef map< udLong, _pcell > _l_asc_row;
		typedef map< udLong, _l_asc_row > _l_asc_rows;
		typedef map< udLong, udPMorph > _l_asc_attr;

		udLong		m_lSelected;
		_l_asc_rows	m_arrRow;
		_l_asc_attr	m_arrAttr;
		_l_asc_row	m_rowHeader;
		HWND		m_hWndTarget;

	public:
		CWndInstanceList( );
		CWndInstanceList( const CWndInstanceList& objCopy );

		virtual ~CWndInstanceList( );

		CWndInstanceList& operator =( const CWndInstanceList& objCopy );

		udPMorph	AddAttr( const string& name, const wstring& title, const string& attr = "" );
		void		GenerateRows( udPArrBase arrObject );
		void		ClearAttr( );
		void		ClearRows( );
		void		SelectRow( udLong row );
		void		CalculateDimensions( udInt& w, udInt& h );	// высчитывает необходимую ширину и высоту

		virtual void	Render( );

		virtual LRESULT	msgButtonClick( LPARAM lParam );

		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void	SetTargetWnd( HWND hWnd ) { m_hWndTarget = hWnd; }

		udLong	GetSelected( ) const { return m_lSelected; }

	}; // class CWndInstanceList

} // namespace UdSDK

#endif
