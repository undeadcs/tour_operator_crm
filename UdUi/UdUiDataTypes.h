/**
	UndeadCS SDK
	Библиотека интерфейса пользователя
	
	типы данных
*/
#ifndef UNDEADCS_SDK_USER_INTERFACE_DATA_TYPES_H
#define UNDEADCS_SDK_USER_INTERFACE_DATA_TYPES_H

namespace UdSDK {

	typedef class CUdWindow		udWindow, *udPWindow, **udPPWindow;

	typedef WNDCLASSEX			udWndClass, *udPWndClass;
	typedef Graphics			udGdi, *udPGdi, **udPPGdi;
	typedef Image				udImage, *udPImage;
	typedef Bitmap				udBitmap, *udPBitmap;
	typedef Font				udFont, *udPFont;
	typedef CachedBitmap		udCachedBitmap, *udPCachedBitmap;

	typedef class CHMsgLoop		udHMsgLoop, *udPHMsgLoop, **udPPHMsgLoop;

	typedef map< string, udPImage			>	udAscImage, *udPAscImage;
	typedef map< string, udPBitmap			>	udAscBitmap, *udPAscBitmap;
	typedef map< string, udPFont			>	udAscFont, *udPAscFont;
	typedef map< string, HCURSOR			>	udAscHCursor, udAscHCursor;

} // namespace UdSDK

#endif
