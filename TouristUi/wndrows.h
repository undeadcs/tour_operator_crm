/**
	Окно, содержащее набор строк (каждая строка - это морф с атрибутами)
	Так же в каждой строке присутствуют ссылки для удаления и редактирования
	Отправка сообщений ссылок отправляется по назначенному хэндлу
*/
#ifndef UDSDK_WNDROWS_H
#define UDSDK_WNDROWS_H

namespace UdSDK {

	class CWndRows : public udWindow {
	public:
		enum {
			WNDROWS_DEL		= 1<<0,	// показывать ссылку удалить
			WNDROWS_EDIT	= 1<<1,	// показывать ссылку редактировать
			WNDROWS_ORD		= 1<<2	// показывать кнопки изменения порядка
		};

	private:
		udArrString		m_arrAttr;			// набор атрибутов, которые показываются
		udInt			m_iFormat;			// режим показа строк
		udPWndFormList	m_pList;			// головной список
		udDWord			m_dwRowNum;			// количество строк
		udDwAscWndRow	m_arrRow;			// строки

	public:
		CWndRows( udPWndFormList wndList = NULL, udInt format = WNDROWS_DEL | WNDROWS_EDIT | WNDROWS_ORD );
		CWndRows( const CWndRows& objCopy );

		virtual ~CWndRows( );

		CWndRows& operator =( const CWndRows& objCopy );

		void		AddAttr( string name );				// добавление атрибута
		udPWndRow	AddRow( udPBase obj );				// добавление строки
		void		DeleteRow( udDWord id );			// удаление строки
		void		UpdateRow( udDWord id );			// обновляет контент строки
		void		CalculateDimensions( HWND hWnd );	// вычисляет нужные размеры окна
		udPWndRow	GetRow( udDWord id );				// полчение строки
		void		ClearRows( );

		virtual LRESULT	msgEraseBackground( HDC hDC );
		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual LRESULT msgRowEdit( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgRowDelete( WPARAM wParam, LPARAM lParam );

		void SetWndList( udPWndFormList wndList ) { m_pList = wndList; }

	}; // class CWndRows

} // namespace UdSDK

#endif
