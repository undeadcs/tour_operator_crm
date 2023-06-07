
#ifndef RENDERER_H
#define RENDERER_H

namespace UdSDK {

	/**
	 *	Отрисовщик
	 */
	class CRenderer {
	public:
		// тип повторения бэкграунда
		typedef enum repType {
			UDSDK_REPEAT_XY,	// повтор будет делаться пока не заполнится весь фон
			UDSDK_REPEAT_X,		//
			UDSDK_REPEAT_Y,
			UDSDK_REPEAT_NONE
		} repType;

	public:
		CRenderer( ) { }

		static void	RenderBackground( Graphics* pRender, Bitmap* imgSprite, const Rect& rctObj, const Rect& rctBackground, repType type = CRenderer::UDSDK_REPEAT_XY );
		static void RenderHeaderRect(
			udPGdi gdi,
			const wstring& title, udUInt iFontSize,
			udInt x, udInt y, udInt w, udInt h,
			udInt iBorderWidth, udDWord dwBrdColor,
			udDWord dwColor, udDWord dwBckColor,
			udInt padL = 0, udInt padT = 0,
			udInt padR = 0, udInt padB = 0
		);

	}; // class CRenderer

} // namespace UdSDK

#endif