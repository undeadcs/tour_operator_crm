/**
	Tourist application
	Серверная часть

	шаблон документа
*/
#ifndef TOURIST_APPLICATION_TOURIST_SERVER_DOC_TEMPLATE_H
#define TOURIST_APPLICATION_TOURIST_SERVER_DOC_TEMPLATE_H

namespace UdSDK {

	udPBase GetProjectGroupContractF( );
	udPBase GetProjectGroupContractApp2F( );

	class CHDocTemplate : public udStateHandlerSimple {
	public:
		enum {
			opidGetFile = opidGet + 1,	// подгрузка шаблона
			opidGetResult,				// получение результата распечатки (договор,путевка)
		};

	private:

	public:
		CHDocTemplate( );

		virtual ~CHDocTemplate( );

		virtual void	Init( );
		virtual void	SchemeCenter( udInt opId, udPSocket sock );
		virtual void	ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual string	GetWhereCondition( );

		static void	ProcContractTemplate(
						udPBase pTour, udPBase pClient, udPBase pTpl,
						udPBase pClDoc, const wstring& szAmount,
						udPArrBase pClients = NULL, udPBase pInfo = NULL,
						udPBase pApp1 = NULL, udPBase pApp2 = NULL
					);

	}; // class CHDocTemplate

} // namespace UdSDK

#endif
