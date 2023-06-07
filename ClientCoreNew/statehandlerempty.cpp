
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CStateHandlerEmpty::CStateHandlerEmpty( ) : udStateHandler( ) {
		m_wndEmpty = new udWndHandler;
	} // CStateHandlerEmpty

	CStateHandlerEmpty::CStateHandlerEmpty( const CStateHandlerEmpty& objCopy ) : udStateHandler( ) {
		m_wndEmpty = new udWndHandler;
	} // CStateHandlerEmpty

	CStateHandlerEmpty::~CStateHandlerEmpty( ) {
	} // ~CStateHandlerEmpty

	CStateHandlerEmpty& CStateHandlerEmpty::operator =( const CStateHandlerEmpty& objCopy ) {
		return *this;
	} // CStateHandlerEmpty& operator =

	void CStateHandlerEmpty::Init( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetWindow( m_szName, m_wndEmpty );
	} // void Init

	void CStateHandlerEmpty::Load( ) {
	} // void Load

	void CStateHandlerEmpty::Show( ) {
		g_pApp->GetWndWork( )->GetWndModel( )->SetCurrent( m_szName );
	} // void Show

	void CStateHandlerEmpty::Hide( ) {
	} // void Hide

	void CStateHandlerEmpty::Clear( ) {
	} // void Clear

} // namespace UdSDK
