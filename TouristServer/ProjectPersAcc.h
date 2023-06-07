
#ifndef PROJECTPERSACC_H
#define PROJECTPERSACC_H

namespace UdSDK {

	udPBase	GetProjectPersAcc2( );
	udPBase	GetProjectFinanceSumm2( );
	udPBase	GetProjectFinanceSumm3( );

	class CHPersAcc : public udStateHandlerSimple {

	public:
		enum {
			opidSummAdd = opidGet + 1,	// операция добавления суммы
			opidSummBack,				// снятие
			opidSummConvert,			// конвертация
		};

	public:
		CHPersAcc( );
		CHPersAcc( const CHPersAcc& objCopy );

		virtual ~CHPersAcc( );

		CHPersAcc& operator =( const CHPersAcc& objCopy );

		virtual void	Init( );
		virtual void	SchemeCenter( udInt opId, udPSocket sock );
		virtual string	GetWhereCondition( );
		virtual udInt	DelObj( udPBase obj, bool save_info = true );
		virtual bool	CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt );
		virtual void	ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj );
		virtual void	ExtraDataSending( udPSocket sock, udPBase obj );

		void	OpSummAdd( udPSocket sock );
		void	OpSummBack( udPSocket sock );
		void	OpSummConvert( udPSocket sock );
		void	AccumulateAdd( udUInt id, udPBase add );
		char	AccumulateBack( udUInt id, udPBase back );
		char	AccumulateConvert( udUInt id, udPBase conv );
		udPBase	FindCurrencyPair( udUInt c1, udUInt c2 );
		void	LoadAccSumm( udArrBase& arr, udUInt iAccId );
		void	LoadAccOp( udArrBase& arr, udUInt iAccId );

	}; // class CHPersAcc

} // namespace UdSDK

#endif
