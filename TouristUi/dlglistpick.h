/**
	Диалоговое окно выбора из списка сущностей
*/
#ifndef DLGLISTPICK_H
#define DLGLISTPICK_H

namespace UdSDK {

	// создает диалоговое окно в памяти
	LRESULT		MakeDialog( HWND hWndParent, DLGPROC dlgProc, LPARAM lParam );

	// вызывает диалогове окно выбора из списка сущностей, возвращает указатель на объект или NULL,
	// если не было выбрано ничего. размеры диалогового окна подстраиваются под размеры окна
	// выбора из списка сущностей WndListPick
	udPMorph	DialogListPick( HWND hWndParent, udPListHandler handler );

	// создает диалоговое окно, потом создает окно формы и обращается к обработчику за заполнением формы
	// затем изменяет размер диалогового окна под размеры формы и центрует его относительно
	// родительского окна WndModeForm
	udPMorph	DialogForm( HWND hWndParent, udPFormHandler handler, udPBase obj = NULL );

} // namespace UdSDK

#endif
