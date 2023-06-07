
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHTourPayOrderFinanceSummAddFormModal::CHTourPayOrderFinanceSummAddFormModal( ) : CHFinanceSummAddFormModal( ) {
	} // CHTourPayOrderFinanceSummAddFormModal

	CHTourPayOrderFinanceSummAddFormModal::CHTourPayOrderFinanceSummAddFormModal( const CHTourPayOrderFinanceSummAddFormModal& objCopy ) : CHFinanceSummAddFormModal( ) {
	} // CHTourPayOrderFinanceSummAddFormModal

	CHTourPayOrderFinanceSummAddFormModal::~CHTourPayOrderFinanceSummAddFormModal( ) {
	} // ~CHTourPayOrderFinanceSummAddFormModal

	CHTourPayOrderFinanceSummAddFormModal& CHTourPayOrderFinanceSummAddFormModal::operator =( const CHTourPayOrderFinanceSummAddFormModal& objCopy ) {
		return *this;
	} // CHTourPayOrderFinanceSummAddFormModal& operator =

	void CHTourPayOrderFinanceSummAddFormModal::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Сумма
		m_tblInput.attach( m_lblSumm, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSumm, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Валюта
		m_tblInput.attach( m_lblCurrency, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тип
		//m_tblInput.attach( m_lblType, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		//m_tblInput.attach( m_inpType, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );
		/*
		// Способ
		m_tblInput.attach( m_lblHow, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpHow, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Комментарий
		m_tblInput.attach( m_lblComment, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpComment, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );
		//*/
		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
	} // void Init

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourPayOrderFinanceSummBackFormModal::CHTourPayOrderFinanceSummBackFormModal( ) : CHFinanceSummBackFormModal( ) {
	} // CHTourPayOrderFinanceSummBackFormModal

	CHTourPayOrderFinanceSummBackFormModal::CHTourPayOrderFinanceSummBackFormModal( const CHTourPayOrderFinanceSummBackFormModal& objCopy ) : CHFinanceSummBackFormModal( ) {
	} // CHTourPayOrderFinanceSummBackFormModal

	CHTourPayOrderFinanceSummBackFormModal::~CHTourPayOrderFinanceSummBackFormModal( ) {
	} // ~CHTourPayOrderFinanceSummBackFormModal

	CHTourPayOrderFinanceSummBackFormModal& CHTourPayOrderFinanceSummBackFormModal::operator =( const CHTourPayOrderFinanceSummBackFormModal& objCopy ) {
		return *this;
	} // CHTourPayOrderFinanceSummBackFormModal& operator =

	void CHTourPayOrderFinanceSummBackFormModal::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Сумма
		m_tblInput.attach( m_lblSumm, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSumm, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Валюта
		m_tblInput.attach( m_lblCurrency, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тип
		//m_tblInput.attach( m_lblType, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		//m_tblInput.attach( m_inpType, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );
		/*
		// Способ
		m_tblInput.attach( m_lblHow, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpHow, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Комментарий
		m_tblInput.attach( m_lblComment, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpComment, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );
		//*/
		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
	} // void Init

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	CHTourPayOrderFinanceSummConvertFormModal::CHTourPayOrderFinanceSummConvertFormModal( ) : CHFinanceSummConvertFormModal( ) {
	} // CHTourPayOrderFinanceSummConvertFormModal

	CHTourPayOrderFinanceSummConvertFormModal::CHTourPayOrderFinanceSummConvertFormModal( const CHTourPayOrderFinanceSummConvertFormModal& objCopy ) : CHFinanceSummConvertFormModal( ) {
	} // CHTourPayOrderFinanceSummConvertFormModal

	CHTourPayOrderFinanceSummConvertFormModal::~CHTourPayOrderFinanceSummConvertFormModal( ) {
	} // ~CHTourPayOrderFinanceSummConvertFormModal

	CHTourPayOrderFinanceSummConvertFormModal& CHTourPayOrderFinanceSummConvertFormModal::operator =( const CHTourPayOrderFinanceSummConvertFormModal& objCopy ) {
		return *this;
	} // CHTourPayOrderFinanceSummConvertFormModal& operator =

	void CHTourPayOrderFinanceSummConvertFormModal::Init( ) {
		udInt l = 0, r = 1;

		// заголовок
		m_wndFormWrap->pack_start( m_lblHeader, Gtk::PACK_SHRINK );

		// Сумма
		m_tblInput.attach( m_lblSumm, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpSumm, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Валюта из
		m_tblInput.attach( m_lblCurrency1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency1, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Валюта в
		m_tblInput.attach( m_lblCurrency2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpCurrency2, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тип из
		//m_tblInput.attach( m_lblType1, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		//m_tblInput.attach( m_inpType1, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );

		// Тип в
		//m_tblInput.attach( m_lblType2, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		//m_tblInput.attach( m_inpType2, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );
		/*
		// Комментарий
		m_tblInput.attach( m_lblComment, 0, 1, l, r, Gtk::SHRINK | Gtk::FILL, Gtk::SHRINK );
		m_tblInput.attach( m_inpComment, 1, 2, l++, r++, Gtk::SHRINK, Gtk::SHRINK );
		//*/
		// кнопки
		m_boxButton.pack_start( m_btnOk, Gtk::PACK_SHRINK );
		m_boxButton.pack_start( m_btnCancel, Gtk::PACK_SHRINK );
		m_boxButton.show_all( );
		m_tblInput.attach( m_boxButton, 1, 2, l, r, Gtk::SHRINK, Gtk::SHRINK );

		m_tblInput.show_all( );
		m_wndFormWrap->pack_start( m_tblInput, Gtk::PACK_SHRINK );
		m_wndFormWrap->show_all_children( );
	} // void Init

} // namespace UdSDK
