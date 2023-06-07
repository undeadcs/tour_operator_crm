
#include "includes.h"

namespace UdSDK {

	void CenterOneWindowAboveParent( HWND hWnd ) {
		HWND hwndOwner, hwndDlg = hWnd; 
		RECT rc, rcDlg, rcOwner;
	if ((hwndOwner = GetParent(hwndDlg)) == NULL) 
    {
        hwndOwner = GetDesktopWindow(); 
    }

		GetWindowRect(hwndOwner, &rcOwner); 
		GetWindowRect(hwndDlg, &rcDlg); 
		CopyRect(&rc, &rcOwner); 

		// Offset the owner and dialog box rectangles so that right and bottom 
		// values represent the width and height, and then offset the owner again 
		// to discard space taken up by the dialog box. 

		OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top); 
		OffsetRect(&rc, -rc.left, -rc.top); 
		OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom); 

		// The new position is the sum of half the remaining space and the owner's 
		// original position. 

		SetWindowPos(hwndDlg, 
					 HWND_TOP, 
					 rcOwner.left + (rc.right / 2), 
					 rcOwner.top + (rc.bottom / 2), 
					 0, 0,          // Ignores size arguments. 
					 SWP_NOSIZE);
	} // void CenterOneWindowAboveParent

	void ExtOpenFile( const wstring& path, bool open, bool maximized ) {
		ShellExecute( NULL, ( open ? L"open" : L"edit" ), path.data( ), NULL, NULL, ( maximized ? SW_SHOWMAXIMIZED : SW_SHOW ) );
	} // void ExtOpenFile

	void ExtOpenFolder( const wstring& path, bool maximized ) {
		ShellExecute( NULL, L"explore", path.data( ), NULL, NULL, ( maximized ? SW_SHOWMAXIMIZED : SW_SHOW ) );
	} // void ExtOpenFolder

} // namespace UdSDK
