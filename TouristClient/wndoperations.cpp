
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndOperations::CWndOperations( ) {
	}

	CWndOperations::~CWndOperations( ) {
		for( dwa_wrows::iterator i = m_mapRows.begin( ); i != m_mapRows.end( ); ++i ) {
			delete i->second;
		}
		ClearDwAscBase( m_ascRows );
	}

	void CWndOperations::Render( ) {
		udPGdi	gdi	= this->GetGdi( );

		gdi->Clear( Color( 255, 255, 255 ) );
	} // void Render

	void CWndOperations::SetRows( udArrBase& arr ) {
		// отступ между строками в 1 пиксел
		udInt cnt = 0, x = 0, y = 20, w = 0, h = 0, m = 1;
		udPWndStatic add = NULL; // вновь добавляемое окно строки
		udPBase obj = NULL; // текущий объект
		udDWord index = 0; // текущий индекс объекта
		udUInt size = 12;
		///////////////////////////////////////////////////////////////////////////////////////
		wstring title = L"", stype = L"";
		udInt type = 0;
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj = *i;
			// создаем окно
			add = new udWndStatic( size );
			add->get( "style" )->ini( ( udDWord ) ( WS_CHILD | WS_VISIBLE ) );
			add->get( "x" )->ini( x );
			add->get( "y" )->ini( y );
			type = obj->get( "type" )->toInt( );
			stype = udProjectFinanceOperation::GetOperationTypeName( type );
			title = obj->get( "created" )->toWString( ) + L", " +
				stype + L", \"" +
				obj->get( "comment" )->toWString( ) + L"\"";
			add->get( "title" )->ini( title );
			add->CalculateWidthHeight( m_hWnd );
			add->Create( m_hWnd );
			// сохраняем
			m_mapRows[ index ] = add;
			m_ascRows[ index ] = obj;
			// следующие значения
			y += add->get( "height" )->toInt( ) + m;
			++index;
		}
		///////////////////////////////////////////////////////////////////////////////////////
		arr.clear( );
		///////////////////////////////////////////////////////////////////////////////////////
	} // void SetRows

	void CWndOperations::ClearRows( ) {
		if ( !m_mapRows.empty( ) ) {
			for( dwa_wrows::iterator i = m_mapRows.begin( ); i != m_mapRows.end( ); ++i ) {
				i->second->Destroy( );
				delete i->second;
			}
			m_mapRows.clear( );
			ClearDwAscBase( m_ascRows );
		}
	} // void ClearRows

} // namespace UdSDK