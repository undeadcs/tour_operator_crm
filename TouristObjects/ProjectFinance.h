/**
	Tourist application
	Набор классов объектов

	финансы
*/
#ifndef TOURIST_APPLICATION_OBJECTS_FINANCE_H
#define TOURIST_APPLICATION_OBJECTS_FINANCE_H

namespace UdSDK {

	/**
	 *	Сумма на счету
	 */
	class CProjectFinanceSumm : public udBase {

	public:
		// тип
		enum SummType {
			SummTypeNone,	// неопределенный тип
			SummWhite,		// белая
			SummBlack		// черная
		};

		// к каким данным привазяна
		enum SummAt {
			SummAtNone,			// хз откуда
			SummAtAcc,			// сумма на счету
			SummAtOperation,	// сумма в операции
			SummAtTour,			// сумма фактически за тур
			SummAtTourPrice,	// сумма итоговой цены за тур
			SummAtTourCost,		// сумма себестоимости тура
			SummAtService,		// сумма фактически за услуги в туре
			SummAtTourPayOrder,	// сумма указанная менеджером кассиру (указание на оплату тура)
		};

		static wstring	GetTypeName( udInt type );

	private:

	public:
		CProjectFinanceSumm( );

		virtual ~CProjectFinanceSumm( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectFinanceSumm


	class CProjectFinanceOperation : public udBase {

	public:
		// тип
		enum FinanceOperationType {
			fotNone,		// неизвестный тип
			fotRoll,		// зачисление
			fotCut,			// снятие
			fotConv,		// конвертация
			fotPayTour,		// оплата тура
			fotPayServ,		// оплата услуг
			fotBackTour,	// возврат с тура
			fotBackServ,	// возврат с услуг
		};

		// для зачислений/снятий
		enum MoneyType {
			fctNone,	// неизвестный способ
			fctCash,	// наличные
			fctCard,	// карта
			fctNonCash,	// безнал
		};

		static wstring	GetMoneyTypeName( udInt type );
		static wstring	GetOperationTypeName( udInt type );

	private:

	public:
		CProjectFinanceOperation( );

		virtual ~CProjectFinanceOperation( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectFinanceOperation

	class CProjectFinanceSummAdd : public udBase {

	public:
		CProjectFinanceSummAdd( );

		virtual ~CProjectFinanceSummAdd( );

		virtual void	GetConfig( udBaseConfig& config );

	}; // class CProjectFinanceSummAdd

	class CProjectFinanceSummBack : public udBase {

	public:
		CProjectFinanceSummBack( );

		virtual ~CProjectFinanceSummBack( );

		virtual void	GetConfig( udBaseConfig& config );

	}; // class CProjectFinanceSummBack

	class CProjectFinanceSummConvert : public udBase {

	public:
		enum ConvType {
			ConvCurrency,	// конвертация из валюты в валюту
			ConvType,		// конвертация из чер/бел
		};

	public:
		CProjectFinanceSummConvert( );

		virtual ~CProjectFinanceSummConvert( );

		virtual void	GetConfig( udBaseConfig& config );

	}; // class CProjectFinanceSummConvert

	class CProjectFinanceSummToPartner : public udBase {

	public:
		CProjectFinanceSummToPartner( );

		virtual ~CProjectFinanceSummToPartner( );

		virtual void	GetConfig( udBaseConfig& config );

	}; // class CProjectFinanceSummToPartner

} // namespace UdSDK

#endif
