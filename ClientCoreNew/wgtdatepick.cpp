
#include "includes.h"
#include "externals.h"

namespace UdSDK {
	
	CWgtDatePick::Entry::Entry( ) :
	Gtk::Entry( ),
	m_iYear( 0 ),
	m_iMonth( 0 ),
	m_iDay( 0 )
	{
		InitValue( );
	} // CWgtDatePick::Entry

	CWgtDatePick::Entry::Entry( const CWgtDatePick::Entry& objCopy ) :
	Gtk::Entry( ),
	m_iYear( objCopy.m_iYear ),
	m_iMonth( objCopy.m_iMonth ),
	m_iDay( objCopy.m_iDay )
	{
		InitValue( );
	} // Entry

	CWgtDatePick::Entry::Entry( const udDatetime& dtValue ) :
	Gtk::Entry( ),
	m_iYear( 0 ),
	m_iMonth( 0 ),
	m_iDay( 0 )
	{
		SetValue( dtValue );
	} // Entry

	CWgtDatePick::Entry::~Entry( ) {
	} // ~Entry

	CWgtDatePick::Entry& CWgtDatePick::Entry::operator =( const CWgtDatePick::Entry& objCopy ) {
		m_iYear		= objCopy.m_iYear;
		m_iMonth	= objCopy.m_iMonth;
		m_iDay		= objCopy.m_iDay;

		InitValue( );

		return *this;
	} // Entry& operator =

	void CWgtDatePick::Entry::SetValue( const udDatetime& dtValue ) {
		int y = 0, m = 0, d = 0;

		dtValue.get( y, m, d );

		m_iYear		= ( udUInt ) y;
		m_iMonth	= ( udUInt ) m;
		m_iDay		= ( udUInt ) d;

		InitValue( );
	} // void SetValue

	void CWgtDatePick::Entry::InitValue( ) {
		Glib::ustring tmp = "";

		if ( m_iDay ) {
			tmp += toString( m_iDay, "%02d" );
		} else {
			tmp += "__";
		}

		tmp += ".";

		if ( m_iMonth ) {
			tmp += toString( m_iMonth, "%02d" );
		} else {
			tmp += "__";
		}

		tmp += ".";

		if ( m_iYear ) {
			tmp += toString( m_iYear, "%04d" );
		} else {
			tmp += "____";
		}

		Glib::RefPtr< Gtk::EntryBuffer > buff = get_buffer( );
		if ( buff ) {
			buff->delete_text( 0, tmp.size( ) );
			buff->insert_text( 0, tmp );
		}
	} // void InitValue

	void CWgtDatePick::Entry::on_insert_text( const Glib::ustring &szText, int *iPosition ) {
		if ( szText == "__.__.____" ) {
			Gtk::Entry::on_insert_text( szText, iPosition );
		} else if ( szText.length( ) == 1 ) {
			int	pos		= *iPosition,
				value	= toInt( szText.c_str( ) );
			Glib::RefPtr< Gtk::EntryBuffer > buff = get_buffer( );

			if ( ( pos >= 0 ) && ( pos <= 1 ) ) { // день
				if ( pos == 0 ) {
					if ( value == 3 ) {
						if ( ( m_iDay % 10 ) <= 1 ) {
							m_iDay = 10 * value + ( m_iDay % 10 );
						}
					} else if ( value <= 2 ) {
						m_iDay = 10 * value + ( m_iDay % 10 );
					}
				} else {
					if ( m_iDay >= 30 ) {
						if ( value <= 1 ) {
							m_iDay = 30 + value;
						}
					} else if ( value <= 9 ) {
						m_iDay = m_iDay - ( m_iDay % 10 ) + value;
					}

					*iPosition += 1;
				}

				*iPosition += 1;
				buff->delete_text( 0, 2 );
				buff->insert_text( 0, Glib::locale_to_utf8( toString( m_iDay, "%02d" ) ) );
			} else if ( ( pos >= 3 ) && ( pos <= 4 ) ) { // месяц
				if ( pos == 3 ) {
					if ( value == 1 ) {
						if ( m_iMonth <= 2 ) {
							m_iMonth += 10;
						}
					} else if ( value == 0 ) {
						m_iMonth = m_iMonth % 10;
					}
				} else {
					if ( m_iMonth >= 10 ) {
						if ( value <= 2 ) {
							m_iMonth = ( m_iMonth / 10 ) * 10 + value;
							*iPosition += 1;
						}
					} else {
						m_iMonth = value;
						*iPosition += 1;
					}
				}

				*iPosition += 1;
				buff->delete_text( 3, 2 );
				buff->insert_text( 3, Glib::locale_to_utf8( toString( m_iMonth, "%02d" ) ) );
			} else if ( ( pos >= 6 ) && ( pos <= 9 ) ) { // год
				unsigned	multiplier		= ( unsigned ) pow( ( double ) 10, ( 9 - pos ) ),
							multiplier_x_10	= multiplier * 10;

				// все что после вводимой позиции считается хвостом
				// все что до вводимой позиции считается головой
				// голова берется как целое от деления года на умножитель, умноженный на 10 (multiplier_x_10),
				// и помноженный на него же (multiplier_x_10)
				// например 1978, позиция равна 2 (multiplier = 10, multiplier_x_10 = 100), то голова будет 1900
				//
				// хвост берется как остаток от деления года на умножитель
				// для того же примера хвост - 8
				//
				// в итоге год должен сложиться из 3х частей
				// head + foot + value * multiplier;

				m_iYear = ( m_iYear / multiplier_x_10 ) * multiplier_x_10 + m_iYear % multiplier + value * multiplier;

				*iPosition += 1;
				buff->delete_text( 6, 4 );
				buff->insert_text( 6, Glib::locale_to_utf8( toString( m_iYear, "%04d" ) ) );
			} else if ( ( pos == 2 ) || ( pos == 5 ) ) {
				*iPosition += 1;
			}
		}
	} // void on_insert_text

	void CWgtDatePick::Entry::on_delete_text( int iBegin, int iEnd ) {
		if ( ( iBegin < 0 ) || ( iEnd < 0 ) ) {
			return;
		}

		Glib::RefPtr< Gtk::EntryBuffer > buff = get_buffer( );
		int n = iEnd;
		char mark[ 10 ];

		// __.__.____
		// 0123456789

		memset( &mark, 0, sizeof( char ) * 10 );

		for( int i = iBegin; i < n; ++i ) {
			mark[ i ] = 1;
		}

		Glib::ustring tmp = buff->get_text( );

		for( int i = 0; i < 10; ++i ) {
			if ( mark[ i ] && ( i != 2 ) && ( i != 5 ) ) {
				tmp.replace( i, 1, "0" );
			}
		}

		m_iDay		= toInt( tmp.substr( 0, 2 ).c_str( ) );
		m_iMonth	= toInt( tmp.substr( 3, 2 ).c_str( ) );
		m_iYear		= toInt( tmp.substr( 6, 4 ).c_str( ) );

		buff->delete_text( iBegin, iEnd - iBegin );
		buff->insert_text( iBegin, tmp.substr( iBegin, iEnd - iBegin ) );
	} // void on_delete_text

	CWgtDatePick::CWgtDatePick( ) :
	Gtk::EventBox( ),
	m_inpText( new CWgtDatePick::Entry )
	{
		set_size_request( 80, -1 );
		m_inpText->set_has_frame( false );
		m_boxInput.set_border_width( 2 );
		m_boxInput.pack_start( *m_inpText, ( Gtk::PackOptions ) Gtk::PACK_EXPAND_WIDGET, ( guint ) 2 );
		add( m_boxInput );
		show_all_children( );
		signal_expose_event( ).connect( sigc::mem_fun( this, &CWgtDatePick::OnExpose ) );
	} // CWgtDatePick

	CWgtDatePick::CWgtDatePick( const CWgtDatePick& objCopy ) :
	Gtk::EventBox( ),
	m_inpText( new CWgtDatePick::Entry ),
	m_iDefaultYear( objCopy.m_iDefaultYear ),
	m_iDefaultMonth( objCopy.m_iDefaultMonth ),
	m_iDefaultDay( objCopy.m_iDefaultDay )
	{
		set_size_request( 80, -1 );
		m_inpText->set_has_frame( false );
		m_boxInput.set_border_width( 2 );
		m_boxInput.pack_start( *m_inpText, ( Gtk::PackOptions ) Gtk::PACK_EXPAND_WIDGET, ( guint ) 2 );
		add( m_boxInput );
		show_all_children( );
		signal_expose_event( ).connect( sigc::mem_fun( this, &CWgtDatePick::OnExpose ) );

		*m_inpText = *( objCopy.m_inpText );
	} // CWgtDatePick

	CWgtDatePick::CWgtDatePick( const udDatetime& dtValue, const udDatetime& dtDefaultValue ) :
	Gtk::EventBox( ),
	m_inpText( new CWgtDatePick::Entry( dtValue ) )
	{
		set_size_request( 80, -1 );
		m_inpText->set_has_frame( false );
		m_boxInput.set_border_width( 2 );
		m_boxInput.pack_start( *m_inpText, ( Gtk::PackOptions ) Gtk::PACK_EXPAND_WIDGET, ( guint ) 2 );
		add( m_boxInput );
		show_all_children( );
		signal_expose_event( ).connect( sigc::mem_fun( this, &CWgtDatePick::OnExpose ) );

		SetDefaultValue( dtDefaultValue );
	} // CWgtDatePick

	CWgtDatePick::~CWgtDatePick( ) {
		udDELETE( m_inpText );
	} // ~CWgtDatePick

	CWgtDatePick& CWgtDatePick::operator =( const CWgtDatePick& objCopy ) {
		m_iDefaultYear	= objCopy.m_iDefaultYear;
		m_iDefaultMonth	= objCopy.m_iDefaultMonth;
		m_iDefaultDay	= objCopy.m_iDefaultDay;

		*m_inpText = *( objCopy.m_inpText );

		return *this;
	} // CWgtDatePick& operator =

	bool CWgtDatePick::OnExpose( GdkEventExpose* evtExpose ) {
		Cairo::RefPtr< Cairo::Context > myContext = get_window( )->create_cairo_context( );
		if ( myContext ) {
			Gtk::Allocation allocation = get_allocation( );
			int width	= allocation.get_width( ),
				height	= allocation.get_height( );

			myContext->save( );

			myContext->set_source_rgb( 0.7, 0.7, 0.7 );
			myContext->set_line_width( 1.0 );
			myContext->move_to( 0, 0 );
			myContext->line_to( width, 0 );
			myContext->line_to( width, height );
			myContext->line_to( 0, height );
			myContext->line_to( 0, 0 );
			myContext->stroke( );

			myContext->restore( );
		}

		return false;
	} // bool on_expose_event

	void CWgtDatePick::SetDefaultValue( const udDatetime& dtDefaultValue ) {
		int y = 0, m = 0, d = 0;

		dtDefaultValue.get( y, m, d );

		m_iDefaultYear	= ( udUInt ) y;
		m_iDefaultMonth	= ( udUInt ) m;
		m_iDefaultDay	= ( udUInt ) d;
	} // void SetDefaultValue

/*
	CWgtDatePick::CWgtDatePick( ) :
		Gtk::HBox( ),
		m_fDay( 0.0f ),
		m_fMonth( 0.0f ),
		m_fYear( 0.0f ),
		m_fDefaultDay( 0.0f ),
		m_fDefaultMonth( 0.0f ),
		m_fDefaultYear( 0.0f )
	{
		Init( );
		InitValue( );
	} // CWgtDatePick

	CWgtDatePick::CWgtDatePick( const CWgtDatePick& objCopy ) : Gtk::HBox( ) {
	} // CWgtDatePick

	CWgtDatePick::CWgtDatePick( udDouble fYear, udDouble fMonth, udDouble fDay, udDouble fDefaultYear, udDouble fDefaultMonth, udDouble fDefaultDay ) :
		Gtk::HBox( ),
		m_fDay( fYear ),
		m_fMonth( fMonth ),
		m_fYear( fYear ),
		m_fDefaultDay( fDefaultDay ),
		m_fDefaultMonth( fDefaultMonth ),
		m_fDefaultYear( fDefaultYear )
	{
		Init( );
		InitValue( );
	}

	CWgtDatePick::CWgtDatePick( const string& szValue, const string& szDefaultValue ) :
		Gtk::HBox( ),
		m_fDay( 0.0f ),
		m_fMonth( 0.0f ),
		m_fYear( 0.0f ),
		m_fDefaultDay( 0.0f ),
		m_fDefaultMonth( 0.0f ),
		m_fDefaultYear( 0.0f )
	{
		Init( );
		SetValue( szValue );
		SetDefaultValue( szDefaultValue );
	}

	CWgtDatePick::CWgtDatePick( const wstring& szValue, const wstring& szDefaultValue ) :
		Gtk::HBox( ),
		m_fDay( 0.0f ),
		m_fMonth( 0.0f ),
		m_fYear( 0.0f ),
		m_fDefaultDay( 0.0f ),
		m_fDefaultMonth( 0.0f ),
		m_fDefaultYear( 0.0f )
	{
		Init( );
		SetValue( szValue );
		SetDefaultValue( szDefaultValue );
	}

	CWgtDatePick::CWgtDatePick( const udDatetime& dtValue, const udDatetime& dtDefaultValue ) :
		Gtk::HBox( ),
		m_fDay( 0.0f ),
		m_fMonth( 0.0f ),
		m_fYear( 0.0f ),
		m_fDefaultDay( 0.0f ),
		m_fDefaultMonth( 0.0f ),
		m_fDefaultYear( 0.0f )
	{
		Init( );
		SetValue( dtValue );
		SetDefaultValue( dtDefaultValue );
	}

	CWgtDatePick::~CWgtDatePick( ) {
	} // ~CWgtDatePick

	CWgtDatePick& CWgtDatePick::operator =( const CWgtDatePick& objCopy ) {
		return *this;
	} // CWgtDatePick& operator =
	
	void CWgtDatePick::Reset( ) {
		SetValue( m_fDefaultYear, m_fDefaultMonth, m_fDefaultDay );
	} // void Reset

	void CWgtDatePick::SetValue( const string& szValue ) {
		if ( szValue.size( ) >= 10 ) {
			SetValue(
				toDouble( szValue.substr( 0, 4 ) ),
				toDouble( szValue.substr( 5, 2 ) ),
				toDouble( szValue.substr( 8, 2 ) )
			);
		}
	} // void SetValue

	void CWgtDatePick::SetValue( const wstring& szValue ) {
		if ( szValue.size( ) >= 10 ) {
			SetValue(
				toDouble( szValue.substr( 0, 4 ) ),
				toDouble( szValue.substr( 5, 2 ) ),
				toDouble( szValue.substr( 8, 2 ) )
			);
		}
	} // void SetValue

	void CWgtDatePick::SetValue( const udDatetime& dtDate ) {
		udInt year, month, day;
		dtDate.get( year, month, day );
		SetValue( year, month, day );
	} // void SetValue

	void CWgtDatePick::SetValue( udDouble fYear, udDouble fMonth, udDouble fDay ) {
		m_fYear = fYear;
		m_fMonth = fMonth;
		m_fDay = fDay;

		InitValue( );
	} // void SetValue

	void CWgtDatePick::SetDefaultValue( const string& szValue ) {
		if ( szValue.size( ) >= 10 ) {
			SetDefaultValue(
				toDouble( szValue.substr( 0, 4 ) ),
				toDouble( szValue.substr( 5, 2 ) ),
				toDouble( szValue.substr( 7, 2 ) )
			);
		}
	} // void SetDefaultValue

	void CWgtDatePick::SetDefaultValue( const wstring& szValue ) {
		if ( szValue.size( ) >= 10 ) {
			SetDefaultValue(
				toDouble( szValue.substr( 0, 4 ) ),
				toDouble( szValue.substr( 5, 2 ) ),
				toDouble( szValue.substr( 7, 2 ) )
			);
		}
	} // void SetValue

	void CWgtDatePick::SetDefaultValue( const udDatetime& dtDate ) {
		udInt year, month, day;
		dtDate.get( year, month, day );
		SetDefaultValue( year, month, day );
	} // void SetValue

	void CWgtDatePick::SetDefaultValue( udDouble fYear, udDouble fMonth, udDouble fDay ) {
		m_fDefaultYear = fYear;
		m_fDefaultMonth = fMonth;
		m_fDefaultDay = fDay;
	} // void SetValue

	CWgtDatePick& CWgtDatePick::LoadValue( ) {
		m_fYear		= m_inpYear.get_value( );
		m_fMonth	= m_inpMonth.get_value( );
		m_fDay		= m_inpDay.get_value( );

		return *this;
	} // CWgtDatePick& LoadValue

	string CWgtDatePick::GetValue( const string& ) const {
		return	toString( m_fYear, "%02.0f" ) + "-" +
				toString( m_fMonth, "%02.0f" ) + "-" +
				toString( m_fDay, "%02.0f" );
	} // string GetValue

	wstring CWgtDatePick::GetValue( const wstring& ) const {
		return	toWString( m_fYear, L"%02.0f" ) + L"-" +
				toWString( m_fMonth, L"%02.0f" ) + L"-" +
				toWString( m_fDay, L"%02.0f" );
	} // wstring GetValue

	udDatetime CWgtDatePick::GetValue( const udDatetime& ) const {
		return	udDatetime( GetValue( string( "" ) ) );
	} // udDatetime GetValue

	void CWgtDatePick::Init( ) {
		set_size_request( 140, -1 );

		m_inpDay.set_update_policy( Gtk::UPDATE_IF_VALID );
		m_inpDay.set_size_request( 40 );
		m_inpDay.set_range( 1, 31 );
		m_inpDay.set_increments( 1, 7 );
		m_inpDay.set_numeric( true );
		m_inpDay.set_wrap( true );
		//m_inpDay.signal_output( ).connect( sigc::mem_fun( this, &CWgtDatePick::OnOutputDay
		m_inpMonth.set_update_policy( Gtk::UPDATE_IF_VALID );
		m_inpMonth.set_size_request( 40 );
		m_inpMonth.set_range( 1, 12 );
		m_inpMonth.set_increments( 1, 4 );
		m_inpMonth.set_numeric( true );
		m_inpMonth.set_wrap( true );
		m_inpYear.set_update_policy( Gtk::UPDATE_IF_VALID );
		m_inpYear.set_size_request( 55 );
		m_inpYear.set_range( 1900, 2100 );
		m_inpYear.set_increments( 1, 10 );
		m_inpYear.set_numeric( true );
		m_inpYear.set_wrap( true );

		pack_start( m_inpDay, Gtk::PACK_SHRINK );
		pack_start( m_inpMonth, Gtk::PACK_SHRINK, 2 );
		pack_start( m_inpYear, Gtk::PACK_SHRINK );
		show_all_children( );
	} // void Init

	void CWgtDatePick::InitValue( ) {
		m_inpDay.set_value( m_fDay );
		m_inpMonth.set_value( m_fMonth );
		m_inpYear.set_value( m_fYear );
	} // void InitValue
//*/
} // namespace UdSDK
