/**
	Tourist application
	Клиентская часть

	шаблон документа
*/
#ifndef TOURIST_APPLICATION_TOURIST_CLIENT_DOC_TEMPLATE_H
#define TOURIST_APPLICATION_TOURIST_CLIENT_DOC_TEMPLATE_H

namespace UdSDK {

	class CHDocTemplate : public udStateHandlerInfo {
	public:
		enum {
			opidGetFile = opidGet + 1,	// подгрузка шаблона
			opidGetResult,				// получение результата распечатки (договор,путевка)
		};

		enum {
			ctrlidName,
			ctrlidType,
			ctrlidText,
		};

		enum {
			fltName,

			LIST_CONTROL_ID_LAST = fltName
		};

	private:

	public:
		CHDocTemplate( );

		virtual ~CHDocTemplate( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	GetListModel( );
		virtual udInt	GetListView( );
		virtual void	ClearFilter( );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual void	ExtraDataRecieve( udPSocket sock, udPBase obj );

		static void	ProcContractTemplate(
						udPWndModeForm formTour, udPBase pClient, udPBase pTpl,
						udPBase pClDoc, const wstring& szAmount,
						udPArrBase pClients = NULL, udPBase pApp1 = NULL/*udPBase pInfo = NULL,
						udPBase pApp1 = NULL, udPBase pApp2 = NULL*/
					);

	}; // class CHDocTemplate

} // namespace UdSDK

#endif
