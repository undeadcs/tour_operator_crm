/**
	Окно использует буферизацию, то есть используется 2 картинки для фона и переднего плана
	при рисованиии в бэкграунд изменяется фоновая картинка. При вызове BeginForeground
	происходит копирование (попиксельное) картинки фона в картинку переднего плана (сделано из-за висты).
	Рендеринг в окно модели должно происходить где угодно, но при получении WM_PAINT окно тупо
	копирует картинку переднего плана в окно. Поэтому манимуляции с ней происходят вне обработки
	этого сообщения. Таким образом избавляемся от моргания и получаем доступ к фону, чтоб его можно было
	использовать для частичной перерисовки переднего плана. Так же у окна вводится функция обновления экрана,
	в которой происходит копирование переднего плана.
*/
#ifndef WNDMODEL_H
#define WNDMODEL_H

namespace UdSDK {

	class CWndModel : public udWndGraphic {
		udPModelWork	m_pModel;			// модель работы
		udPStateHandler	m_pHandler;			// обработчик состояния

	public:
		CWndModel( );
		CWndModel( const CWndModel& objCopy );

		virtual ~CWndModel( );

		CWndModel& operator =( const CWndModel& objCopy );

		virtual void	Render( );

		virtual LRESULT msgMouseMove( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgLButtonDown( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgLinkClick( LPARAM lParam );
		virtual LRESULT msgButtonClick( LPARAM lParam );
		virtual LRESULT msgListRowSelect( LPARAM lParam );
		virtual LRESULT msgListHeaderClick( LPARAM lParam );

		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void	SetModel( udPModelWork pModel ) { m_pModel = pModel; }
		void	SetHandler( udPStateHandler handler ) { m_pHandler = handler; }

	}; // class CWndModel

} // namespace UdSDK

#endif
