/*
	Tourist application
	Клиентское приложение
	Финансы указания об оплате
*/
#ifndef TOURIST_CLIENT_TOUR_PAY_ORDER_FINANCE_H
#define TOURIST_CLIENT_TOUR_PAY_ORDER_FINANCE_H

namespace UdSDK {

	class CHTourPayOrderFinanceSummAddFormModal : public CHFinanceSummAddFormModal {

	public:
		CHTourPayOrderFinanceSummAddFormModal( );
		CHTourPayOrderFinanceSummAddFormModal( const CHTourPayOrderFinanceSummAddFormModal& objCopy );

		virtual ~CHTourPayOrderFinanceSummAddFormModal( );

		CHTourPayOrderFinanceSummAddFormModal& operator =( const CHTourPayOrderFinanceSummAddFormModal& objCopy );

		virtual void	Init( );

	}; // class CHTourPayOrderFinanceSummAddFormModal

	class CHTourPayOrderFinanceSummBackFormModal : public CHFinanceSummBackFormModal {

	public:
		CHTourPayOrderFinanceSummBackFormModal( );
		CHTourPayOrderFinanceSummBackFormModal( const CHTourPayOrderFinanceSummBackFormModal& objCopy );

		virtual ~CHTourPayOrderFinanceSummBackFormModal( );

		CHTourPayOrderFinanceSummBackFormModal& operator =( const CHTourPayOrderFinanceSummBackFormModal& objCopy );

		virtual void	Init( );

	}; // class CHTourPayOrderFinanceSummBackFormModal

	class CHTourPayOrderFinanceSummConvertFormModal : public CHFinanceSummConvertFormModal {

	public:
		CHTourPayOrderFinanceSummConvertFormModal( );
		CHTourPayOrderFinanceSummConvertFormModal( const CHTourPayOrderFinanceSummConvertFormModal& objCopy );

		virtual ~CHTourPayOrderFinanceSummConvertFormModal( );

		CHTourPayOrderFinanceSummConvertFormModal& operator =( const CHTourPayOrderFinanceSummConvertFormModal& objCopy );

		virtual void	Init( );

	}; // class CHTourPayOrderFinanceSummConvertFormModal

} // namespace UdSDK

#endif
