
#include "includes.h"

namespace UdSDK {

	CListHandlerBtnForm::CListHandlerBtnForm( ) :
		m_btnForm( Glib::locale_to_utf8( "Создать" ) )
	{
		m_btnForm.signal_clicked( ).connect( sigc::mem_fun( *this, &CListHandlerBtnForm::OnButtonForm ) );
	} // CListHandlerBtnForm

	CListHandlerBtnForm::CListHandlerBtnForm( const CListHandlerBtnForm& objCopy ) {
	} // CListHandlerBtnForm

	CListHandlerBtnForm::~CListHandlerBtnForm( ) {
	} // ~CListHandlerBtnForm

	CListHandlerBtnForm& CListHandlerBtnForm::operator =( const CListHandlerBtnForm& objCopy ) {
		return *this;
	} // CListHandlerBtnForm& operator =

} // namespace UdSDK
