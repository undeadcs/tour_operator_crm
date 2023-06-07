
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWndFilePick::CWndFilePick( const wstring& folder ) : udWndGraphic( ) {
		m_szFolder		= folder;
		m_hWndDlgParent	= NULL;
	}

	CWndFilePick::~CWndFilePick( ) {
	}

	LRESULT CWndFilePick::msgSetCursor( WPARAM wParam, LPARAM lParam ) {
		::SetCursor( LoadCursor( NULL, IDC_HAND ) );
		return TRUE;
	} // LRESULT msgSetCursor

	LRESULT CWndFilePick::msgLButtonDown( WPARAM wParam, LPARAM lParam ) {
		//HWND hWnd = ( m_hWndDlgParent ? m_hWndDlgParent : g_pApp->GetModel( )->GetWndWork( )->GetHWnd( ) );
		//////////////////////////////////////////////////////////////////////////////////////////
		TCHAR filepath[ 1024 ], filename[ 1024 ];
		memset( filepath, 0, sizeof( filepath ) );
		memset( filename, 0, sizeof( filename ) );
		OPENFILENAME of;
		ZeroMemory( &of, sizeof( OPENFILENAME ) );
		of.lStructSize = sizeof( OPENFILENAME );
		//of.hwndOwner = hWnd;
		of.hwndOwner = m_hWndDlgParent;
		of.lpstrFilter = L"All\0*.*\0";
		of.lpstrFile = filepath;
		of.nMaxFile = 1024;
		of.lpstrFileTitle = filename;
		of.nMaxFileTitle = 1024;
		of.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
		if ( GetOpenFileName( &of ) == TRUE ) {
			m_szFile = wstring( filename );
			wstring wFileTmpPath = m_szFolder + L"\\" + m_szFile;
			if ( CopyFile( filepath, wFileTmpPath.data( ), FALSE ) == FALSE ) {
				WriteUdUiLog( "CWndFilePick, CopyFile failed\n" );
			}
			this->Render( );
			this->Flush( );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		return 0;
	} // LRESULT msgLButtonDown

	void CWndFilePick::Render( ) {
		udPGdi	gdi	= this->GetGdi( );
		udPFont fntLabel	= g_pApp->GetFont( );
		wstring	title		= L"";
		udInt	w			= this->get( "width" )->toInt( ),
				h			= this->get( "height" )->toInt( ),
				padLeft		= 7,
				padTop		= 7;
		SolidBrush	brhLabel( Color( 0, 0, 0 ) );
		RectF	rctLayout(
			( float ) padLeft, ( float ) padTop,
			( float ) ( w - padLeft ), ( float ) ( h - padTop )
		);
		gdi->Clear( Color( 255, 255, 255 ) );
		if ( m_szFile.empty( ) ) {
			title = this->get( "title" )->toWString( );
			brhLabel.SetColor( Color( 200, 200, 200 ) );
		} else {
			title = m_szFile;
		}
		
		gdi->DrawString( title.data( ), title.size( ), fntLabel,
			rctLayout, NULL, &brhLabel
		);
		Pen penBorder( Color( 201, 202, 204 ), 2 );
		gdi->DrawLine( &penBorder, 0, 1, w, 0 );
		gdi->DrawLine( &penBorder, 0, h - 1, w, h - 1 );
		gdi->DrawLine( &penBorder, 1, 1, 1, h - 1 );
		gdi->DrawLine( &penBorder, w - 1, 0, w - 1, h - 1 );
	} // void Render

	void CWndFilePick::Clear( ) {
		if ( !m_szFile.empty( ) ) { // снести файл
			wstring path = m_szFolder + L"\\" + m_szFile;
			DeleteFile( path.data( ) );
		}
		m_szFile = L"";
		this->Render( );
		this->Flush( );
	} // void ClearSelection

} // namespace UdSDK
