
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectLogList( ) {
		udPBase ret = GetProjectLog( );
		ret->get( "user_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectLogList

	CHLog::CHLog( ) :
		udListHandler( )
	{
		m_szName = "log";

		udPMorph filter = m_pData->GetFilter( );
		filter->get( "date1" )->ini( datetime( ) );
		filter->get( "date2" )->ini( datetime( 0, false ) );
		filter->get( "user" )->ini( ( udUInt ) 0 );
		filter->get( "text" )->ini( wstring( L"" ) );

		m_pRowModel = m_pColModel = new CHLog::ColModelLog( );

		m_pData->SetName( "log" );
		m_pData->SetGetObject( GetProjectLogList );
	} // CHLog

	CHLog::CHLog( const CHLog& objCopy ) : udListHandler( ) {
		m_szName = "log";
	} // CHLog

	CHLog::~CHLog( ) {
	} // ~CLog

	CHLog& CHLog::operator =( const CHLog& objCopy ) {
		return *this;
	} // CHLog& operator =

	void CHLog::Init( ) {
		// добавляем колонки
		m_objTreeView.append_column( "#", m_pColModel->m_col_id );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Дата и время" ), m_pColModel->m_col_created );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Пользователь" ), m_pColModel->m_col_user );
		m_objTreeView.append_column( Glib::locale_to_utf8( "Операция" ), m_pColModel->m_col_action );

		// остальная инициализация
		udListHandler::Init( );

		Gtk::TreeViewColumn *col = NULL;
		col = m_objTreeView.get_column( 0 );
		col->set_sizing( Gtk::TREE_VIEW_COLUMN_FIXED );
		col->set_fixed_width( 50 );
		col->set_resizable( false );
	} // void Init

	void CHLog::Load( ) {
		udListHandler::Load( );

		udPBase obj = NULL;
		udPArrBase pData = m_pData->GetData( );
		Gtk::TreeModel::Row row;
		for( udArrBase::iterator i = pData->begin( ); i != pData->end( ); ++i ) {
			obj = *i;
			row = *( m_objTreeModel->append( ) );
			row[ m_pColModel->m_col_id ] = obj->get( "id" )->toUInt( );
			row[ m_pColModel->m_col_created ] = Glib::locale_to_utf8( obj->get( "created" )->toString( ) );
			row[ m_pColModel->m_col_user ] = Glib::locale_to_utf8( obj->get( "user_" )->toString( ) );
			row[ m_pColModel->m_col_action ] = Glib::locale_to_utf8( obj->get( "action" )->toString( ) );
		}
	} // void Load

	void CHLog::FilterUiClear( ) {
	} // void FilterUiClear

	void CHLog::FilterUiToObject( ) {
	} // void FilterUiToObject




	CHLogModeless::CHLogModeless( ) :
		udListHandlerModeless( ),
		CHLog( )
	{
	} // CHLogModeless

	CHLogModeless::CHLogModeless( const CHLogModeless& objCopy ) : CHLog( ) {
	} // CHLogModeless

	CHLogModeless::~CHLogModeless( ) {
	} // ~CHLogModeless

	CHLogModeless& CHLogModeless::operator =( const CHLogModeless& objCopy ) {
		return *this;
	} // CHLogModeless& operator =

	void CHLogModeless::Init( ) {
		m_hbbTop.pack_start( m_btnMain, false, false, 0 );
		CHLog::Init( );
		udListHandlerModeless::Init( );
	} // void Init

} // namespace UdSDK
