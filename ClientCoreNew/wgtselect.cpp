
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWgtSelect::CWgtSelect( ) :
		Gtk::ComboBox( ),
		m_iDefault( 0 ),
		m_iValue( m_iDefault ),
		m_pColModelSelect( new CWgtSelect::ColModelSelect )
	{
		m_objSelectModel = Gtk::ListStore::create( *m_pColModelSelect );
		set_model( m_objSelectModel );
		pack_start( m_pColModelSelect->m_col_name );
		set_active( m_iValue );
	} // CWgtSelect

	CWgtSelect::CWgtSelect( udUInt iValue, udUInt iDefaultValue ) :
		Gtk::ComboBox( ),
		m_iDefault( iDefaultValue ),
		m_iValue( iValue ),
		m_pColModelSelect( new CWgtSelect::ColModelSelect )
	{
		m_objSelectModel = Gtk::ListStore::create( *m_pColModelSelect );
		set_model( m_objSelectModel );
		pack_start( m_pColModelSelect->m_col_name );
		set_active( m_iValue );
	} // CWgtSelect

	CWgtSelect::CWgtSelect( const CWgtSelect& objCopy ) {
	} // CWgtSelect

	CWgtSelect::~CWgtSelect( ) {
		udDELETE( m_pColModelSelect );
	} // ~CWgtSelect

	CWgtSelect& CWgtSelect::operator =( const CWgtSelect& objCopy ) {
		return *this;
	} // CWgtSelect& operator =

	void CWgtSelect::AddRow( udUInt iValue, const Glib::ustring& szTitle ) {
		Gtk::TreeModel::Row row;

		row = *( m_objSelectModel->append( ) );
		row[ m_pColModelSelect->m_col_code ] = iValue;
		row[ m_pColModelSelect->m_col_name ] = szTitle;
	} // void AddRow

	void CWgtSelect::Reset( ) {
		SetValue( m_iDefault );
	} // void Reset

	void CWgtSelect::SetValue( udUInt iValue ) {
		m_iValue = iValue;
		set_active( m_iValue );
	} // void SetValue

	CWgtSelect& CWgtSelect::LoadValue( ) {
		Gtk::TreeModel::Row row;

		row = *( get_active( ) );
		m_iValue = row[ m_pColModelSelect->m_col_code ];

		return *this;
	} // CWgtSelect& LoadValue

} // namespace UdSDK
