
#ifndef PROJECTTOUR_H
#define PROJECTTOUR_H

namespace UdSDK {

	udPBase TourSpecAppendEmployer( udBinaryData& data, udPBase obj );
	udPBase	GetProjectTour2( );
	udPBase	GetProjectServicePeriodForCost( );
	udPBase	GetProjectServiceForPrint( );

	class CHTour : public udStateHandlerSimple {
	public:
		enum {
			opidSummAdd = opidGet + 1,	// операция добавления суммы
			opidSummBack,				// снятие
			opidSummConvert,			// конвертация
			opidSummLoad,				// загрузка сумм
			opidCalculateFinal,			// вычисление итоговой цены
			opidAddToGroup,				// добавление в группу
			opidGetPersAcc,				// получение лицевого счета заказчик-компания для тура
			opidGetServicePrice			// получение услуг тура с их ценами
		};

		enum {
			priceCost,			// цена - себестоимость
			priceRecommend		// цена - рекомендованная
		};

	private:

	public:
		CHTour( );
		CHTour( const CHTour& objCopy );

		virtual ~CHTour( );

		CHTour& operator =( const CHTour& objCopy );

		virtual void	Init( );
		virtual void	SchemeCenter( udInt opId, udPSocket sock );
		virtual udInt	FindObj( udPBase obj, const string& attr, bool apply = true );
		//virtual void	AfterLoadList( udArrBase& arr );
		virtual void	HandleAddRequest( udPSocket sock );
		virtual udInt	AddObj( udPBase obj, bool save_info = true );
		virtual udInt	DelObj( udPBase obj, bool save_info = true );
		virtual void	ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual string	GetWhereCondition( );
		virtual void	HandleUpdRequest( udPSocket sock );
		
		void	DeleteLinkedObjects( udPBase obj );

		void	OpCalculateFinal( udPSocket sock );
		void	OpAddToGroup( udPSocket sock );

		void	OpSummAdd( udPSocket sock );
		void	OpSummBack( udPSocket sock );
		void	OpSummConvert( udPSocket sock );
		void	OpSummLoad( udPSocket sock );
		char	AccumulateAdd( udArrBase& arr, udUInt id, udInt type, udPBase obj );
		char	AccumulateBack( udArrBase& arr, udUInt id, udInt type, udPBase obj );
		char	AccumulateConvert( udArrBase& arr, udUInt id, udInt type, udPBase obj );
		udPBase	GetPersAcc( udUInt iTourId, udInt type, udUInt iPartnerId = 0 );
		udPBase	FindCurrencyPair( udUInt c1, udUInt c2 );
		char	ClientPay( udUInt iTourId, udPBase obj );
		char	ClientRet( udUInt iTourId, udPBase obj );
		char	CompanyPay( udUInt iTourId, udPBase obj );
		char	CompanyRet( udUInt iTourId, udPBase obj );
		// высчитывает себестоимость тура
		static void	Cost( udPBase obj );

		void	LoadSumm( udArrBase& arr, udPBase obj, udInt type );
		void	LoadSumm( udArrBase& arr, udUInt id, udInt type );
		void	LoadOperations( udArrBase& arr, udUInt id );

		static void	CalculateFinal( udArrBase& arrRet, udPBase pTour );
		static void	CollectServicePriceFinal( udArrBase& arrRet, udPBase pTour );

		void	OpGetPersAcc( udPSocket sock );
		void	OpGetServicePrice( udPSocket sock );

	}; // class CHTour

} // namespace UdSDK

#endif
