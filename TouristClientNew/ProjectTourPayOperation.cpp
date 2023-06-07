
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectTourPayOperationList( ) {
		udPBase ret = new udBase;
		ret->get( "created"		)->ini( udDatetime( ) );
		ret->get( "type"		)->ini( ( udInt ) udProjectFinanceOperation::fotNone );
		ret->get( "summ"		)->ini( ( udFloat )	0 );
		ret->get( "currency"	)->ini( ( udUInt )	0 );
		ret->get( "summ_type"	)->ini( ( udInt ) udProjectFinanceSumm::SummWhite );
		ret->get( "currency_"	)->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTourPayOperationList

	CFDETourPayOperation::CFDETourPayOperation( ) : CFDEList( ) {
		SetCallback( GetProjectTourPayOperationList );
	} // CFDETourPayOperation

	CFDETourPayOperation::CFDETourPayOperation( const CFDETourPayOperation& objCopy ) : CFDEList( ) {
	} // CFDETourPayOperation

	CFDETourPayOperation::~CFDETourPayOperation( ) {
	} // ~CFDETourPayOperation

	CFDETourPayOperation& CFDETourPayOperation::operator =( const CFDETourPayOperation& objCopy ) {
		return *this;
	} // CFDETourPayOperation& operator =

	void CFDETourPayOperation::Send( udPSocket sock ) {
	} // void Send

} // namespace UdSDK
