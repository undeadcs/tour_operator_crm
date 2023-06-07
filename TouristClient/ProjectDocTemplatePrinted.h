/**
	Tourist application
	Клиентская часть

	распечатанный шаблон документа
*/
#ifndef TOURIST_APPLICATION_TOURIST_CLIENT_DOC_TEMPLATE_PRINTED_H
#define TOURIST_APPLICATION_TOURIST_CLIENT_DOC_TEMPLATE_PRINTED_H

namespace UdSDK {

	class CHDocTemplatePrinted : public udStateHandlerInfo {
	public:
		enum {
			fltSeries,
			fltNumber,
			fltDate1,
			fltDate2,
			fltCreatedBy,
			fltType,
			fltTour,
			LIST_CONTROL_ID_LAST
		};

	private:
		bool			m_bCreated;		// создавались ли элементы формы
		udPBase			m_pTour,		// тур
						m_pUser;		// пользователь
		udPWndStatic	m_lblCreated,	// дата распечатки
						m_lblCreatedBy,	// пользователь, который распечатывал
						m_lblTour,		// тур
						m_lblSeries,	// серия
						m_lblNumber,	// номер
						m_lblName,		// имя шаблона
						m_lblType;		// тип шаблона
		udPWndLink		m_lnkPrint;		// ссылка распечатать
		wstring			m_szDir;		// папка для сохранения подгруженных файликов

	public:
		CHDocTemplatePrinted( );
		CHDocTemplatePrinted( const CHDocTemplatePrinted& objCopy );

		virtual ~CHDocTemplatePrinted( );

		CHDocTemplatePrinted& operator =( const CHDocTemplatePrinted& objCopy );

		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual void	ClearFilter( );
		virtual void	AfterLoadFilterData( );
		virtual void	AddExtraAttributes( udPBase tpl );
		virtual void	RequestList( );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual void	ExtraDataRecieve( udPSocket sock, udPBase obj );
		virtual void	LinkClick( udPWndLink lnk );

	}; // class CHCity

} // namespace UdSDK

#endif
