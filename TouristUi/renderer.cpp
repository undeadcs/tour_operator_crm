
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	// повторяет фон по горизонтали
	static void RenderHorizontalBackground( Graphics* pRender, Bitmap* imgSprite, const Rect& rctObj, const Rect& rctBackground ) {
		udInt	iWidthRemains	= rctObj.Width,
				iCurrentX		= rctObj.X;
		while( iWidthRemains > rctBackground.Width ) {
			pRender->DrawImage(
				imgSprite, Rect( iCurrentX, rctObj.Y, rctBackground.Width, rctBackground.Height ),
				rctBackground.X, rctBackground.Y, rctBackground.Width, rctBackground.Height, UnitPixel
			);
			iWidthRemains -= rctBackground.Width;
			iCurrentX += rctBackground.Width;
		}
		if ( iWidthRemains ) {
			pRender->DrawImage(
				imgSprite, Rect( iCurrentX, rctObj.Y, rctBackground.Width, rctBackground.Height ),
				rctBackground.X, rctBackground.Y, rctBackground.Width, rctBackground.Height, UnitPixel
			);
		}
	} // void RenderHorizontalBackground

	// повторяет фон по вертикали
	static void RenderVerticalBackground( Graphics* pRender, Bitmap* imgSprite, const Rect& rctObj, const Rect& rctBackground ) {
		udInt	iHeightRemains	= rctObj.Height,
				iCurrentY		= rctObj.Y;
		while( iHeightRemains > rctBackground.Height ) {
			pRender->DrawImage(
				imgSprite, Rect( rctObj.X, iCurrentY, rctBackground.Width, rctBackground.Height ),
				rctBackground.X, rctBackground.Y, rctBackground.Width, rctBackground.Height, UnitPixel
			);
			iHeightRemains -= rctBackground.Height;
			iCurrentY += rctBackground.Height;
		}
		if ( iHeightRemains ) {
			pRender->DrawImage(
				imgSprite, Rect( rctObj.X, iCurrentY, rctBackground.Width, rctBackground.Height ),
				rctBackground.X, rctBackground.Y, rctBackground.Width, rctBackground.Height, UnitPixel
			);
		}
	} // void RenderVerticalBackground 

	void CRenderer::RenderBackground( Graphics* pRender, Bitmap* imgSprite, const Rect& rctObj, const Rect& rctBackground, repType type ) {
		if ( rctObj.Width < rctBackground.Width && rctObj.Height < rctBackground.Height ) {
			pRender->DrawImage(
				imgSprite, rctObj,
				rctObj.X, rctObj.Y, rctObj.Width, rctObj.Height, UnitPixel
			);
		} else if ( rctObj.Width < rctBackground.Width ) {
			RenderVerticalBackground( pRender, imgSprite, rctObj,
				Rect( rctBackground.X, rctBackground.Y, rctObj.Width, rctBackground.Height )
			);
		} else if ( rctObj.Height < rctBackground.Height ) {
			RenderHorizontalBackground( pRender, imgSprite, rctObj,
				Rect( rctBackground.X, rctBackground.Y, rctBackground.Width, rctObj.Height )
			);
		} else {
			if ( type != CRenderer::UDSDK_REPEAT_NONE ) { // если требуется повтор бэкгарунда
				if ( type == CRenderer::UDSDK_REPEAT_X ) {
					RenderHorizontalBackground( pRender, imgSprite, rctObj, rctBackground );
				} else if ( type == CRenderer::UDSDK_REPEAT_Y ) {
					RenderVerticalBackground( pRender, imgSprite, rctObj, rctBackground );
				} else { // повтор в обе стороны
					udInt h = rctObj.Height, y = rctObj.Y;
					while( h > rctBackground.Height ) {
						RenderHorizontalBackground( pRender, imgSprite,
							Rect( rctObj.X, y, rctObj.Width, rctObj.Height - y ),
							rctBackground
						);
						h -= rctBackground.Height;
						y += rctBackground.Height;
					}
					if ( h ) {
						RenderHorizontalBackground( pRender, imgSprite,
							Rect( rctObj.X, y, rctObj.Width, rctObj.Height - y ),
							Rect( rctBackground.X, rctBackground.Y, rctBackground.Width, rctBackground.Height - h )
						);
					}
				}
			} else {
				pRender->DrawImage(
					imgSprite, Rect( rctObj.X, rctObj.Y, rctBackground.Width, rctBackground.Height ),
					rctBackground.X, rctBackground.Y, rctBackground.Width, rctBackground.Height, UnitPixel
				);
			}
		}
	} // void RenderBackground

	void CRenderer::RenderHeaderRect(
			udPGdi gdi,
			const wstring& title, udUInt iFontSize,
			udInt x, udInt y, udInt w, udInt h,
			udInt iBorderWidth, udDWord dwBrdColor,
			udDWord dwColor, udDWord dwBckColor,
			udInt padL, udInt padT,
			udInt padR, udInt padB
	) {
		SolidBrush	brhBackground( dwBckColor ),
					brhLabel( dwColor );
		Pen			penBorder( Color( dwBrdColor ), ( float ) iBorderWidth );
		udPFont		fnt = g_pApp->GetFont( L"Tahoma", iFontSize, FontStyleBold );
		// рисуем фон
		gdi->FillRectangle( &brhBackground, x, y, w, h );
		// рисуем рамку
		gdi->DrawRectangle( &penBorder, x, y, w, h );
		// рисуем текст
		gdi->DrawString( title.data( ), title.size( ), fnt,
			RectF(
				( float ) ( x + padL ), ( float ) ( y + padT ),
				( float ) ( x + w - padL - padR ),
				( float ) ( y + h - padT - padB )
			), NULL, &brhLabel
		);
	}

} // namespace UdSDK
