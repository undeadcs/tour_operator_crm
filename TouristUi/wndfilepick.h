/**
	Tourist application
	Библиотека инетерфейса пользователя

	контрол выбора файла
*/
#ifndef TOURIST_APPLICATION_TOURIST_UI_FILE_PICK_H
#define TOURIST_APPLICATION_TOURIST_UI_FILE_PICK_H

namespace UdSDK {

	class CWndFilePick : public udWndGraphic {
		wstring	m_szFolder,			// папка хранения файла
				m_szFile;			// файла
		HWND	m_hWndDlgParent;	// хэндл родительского окна

	public:
		CWndFilePick( const wstring& folder = L"" );

		virtual ~CWndFilePick( );

		void	Clear( );

		virtual void	Render( );
		virtual LRESULT msgLButtonDown( WPARAM wParam, LPARAM lParam );
		virtual LRESULT msgSetCursor( WPARAM wParam, LPARAM lParam );

		void	SetDialogParent( HWND hWnd ) { m_hWndDlgParent = hWnd; }
		void	SetFolder( const wstring& folder ) { m_szFolder = folder; }
		void	SetFile( const wstring& file ) { m_szFile = file; }

		wstring	GetFolder( )	const { return m_szFolder;	}
		wstring	GetFile( )		const { return m_szFile;	}

	};

} // namespace UdSDK

#endif
