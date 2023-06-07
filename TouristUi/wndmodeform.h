/**
	Окно для формы данных

	информация о контролах формы.
	поле ввода
		атрибут сущности
		положение
		размеры
		шрифт
	многострочное поле ввода
		атрибут сущности
		положение
		размеры
		шрифт
	выпадающий список
		атрибут сущности
		положение
		размеры
		набор пунктов
	дата
		атрибут сущности
		положение
		размеры
	дата+время
		атрибут сущности
		положение
		размеры
	extlist
		указатель на обработчик состояния
		атрибут сущности
		положение
		размеры
		настройки списка сущностей (при выборе)
		пейджер списка сущностей
		индекс контрола (для обращения к обработчику состояния, который наполняет список сущностей)
	formlist
		указатель на обработчик состояния
		положение
		размеры
		тип { выбор из списка сущностей, форма сущности, приложение }

	выбор из списка сущностей
		создаем диалоговое окно для выбора из списка (при этом по индексу списка загружаем нужные сущности)
	форма сущности
		создаем форму
		создаем диалоговое окно и наполняем его элементами формы
	приложение
		показываем диалоговое выбора файла
*/
#ifndef WNDMODEFORM_H
#define WNDMODEFORM_H

namespace UdSDK {
	// коробка
	struct ModeFormControlBox {
		udInt	x,				// позиция по оси X
				y,				// позиция по оси Y
				w,				// ширина (суммируется с отступами)
				h,				// высота (суммируется с отступами)
				padL,			// отступ слева
				padT,			// отступ сверху
				padR,			// отступ справа
				padB,			// отступ снизу
				border;			// ширина бордюра в пикселях
		udDWord	border_color,	// цвет бордюра
				color,			// цвет текста
				background;		// цвет фона
	};
	// тип
	enum ModeFormControlType {
		ctrlInput,
		ctrlText,
		ctrlDate,
		ctrlDatetime,
		ctrlSelect,
		ctrlExtlist,
		ctrlFormlist,
		ctrlFilepick,
		ctrlStatic,
	};
	// информация о поле ввода
	struct ModeFormInputInfo {
		HWND hwnd;
		udPFont font;
		HFONT hfont;
		udDWord addstyle;

		ModeFormInputInfo( ) {
			hwnd = NULL;
			font = NULL;
			hfont = NULL;
			addstyle = 0;
		}

		~ModeFormInputInfo( ) {
			hwnd = NULL;
			font = NULL;
			addstyle = 0;
			if ( hfont ) {
				DeleteFont( hfont );
			}
		}
	};
	// информация о многострочном поле ввода
	struct ModeFormTextInfo {
		HWND hwnd;
		udPFont font;
		HFONT hfont;

		ModeFormTextInfo( ) {
			hwnd = NULL;
			font = NULL;
			hfont = NULL;
		}

		~ModeFormTextInfo( ) {
			hwnd = NULL;
			font = NULL;
			if ( hfont ) {
				DeleteFont( hfont );
			}
		}
	};
	// информация о дате
	struct ModeFormDateInfo {
		HWND hwnd;
		udPFont font;
		HFONT hfont;
		udInt bString;

		ModeFormDateInfo( ) {
			hwnd = NULL;
			font = NULL;
			hfont = NULL;
			bString = FALSE;
		}

		~ModeFormDateInfo( ) {
			hwnd = NULL;
			font = NULL;
			if ( hfont ) {
				DeleteFont( hfont );
			}
		}
	};
	// информация о селекте
	struct ModeFormSelectInfo {
		HWND hwnd;
		udPFont font;
		HFONT hfont;
		udArrWString arr;

		ModeFormSelectInfo( ) {
			hwnd = NULL;
			font = NULL;
			hfont = NULL;
		}

		~ModeFormSelectInfo( ) {
			hwnd = NULL;
			font = NULL;
			if ( hfont ) {
				DeleteFont( font );
			}
			if ( !arr.empty( ) ) {
				arr.clear( );
			}
		}
	};
	// информация об ExtList
	struct ModeFormExtlistInfo {
		udPExtList extlst;
		//pfnGetObject fnGetObject;
		//string table, attr;
		wstring title;
		//bool auto_load;
		//pfnGetListHandler fnGetListHandler;
		//udPMorph opt;

		ModeFormExtlistInfo( ) {
			extlst = NULL;
			//fnGetObject = NULL;
			//fnGetListHandler = NULL;
			//opt = NULL;
			//auto_load = true;
		}

		~ModeFormExtlistInfo( ) {
			if ( extlst ) {
				delete extlst;
			}
		}
	};
	// информация об элементе формы
	struct ModeFormControlInfo {
		udDWord id;
		string attr;
		ModeFormControlBox* box;
		ModeFormControlType type;
		ModeFormInputInfo* input;
		ModeFormTextInfo* textarea;
		ModeFormSelectInfo* select;
		//udPFont font;
		//HFONT hfont;
		//HWND hwnd;
		udPWndFormList frmlst;
		//udArrWString select;
		ModeFormExtlistInfo* extlst;
		ModeFormDateInfo* date;
		udPWndFilePick filepick;
		udPWndStatic staticw;

		ModeFormControlInfo( ) {
			id = 0;
			attr = "";
			box = NULL;
			type = ctrlInput;
			//font = NULL;
			//hfont = NULL;
			//hwnd = NULL;
			frmlst = NULL;
			extlst = NULL;
			input = NULL;
			textarea = NULL;
			select = NULL;
			date = NULL;
			filepick = NULL;
		}

		~ModeFormControlInfo( ) {
			if ( box ) {
				delete box;
			}
			if ( extlst ) {
				delete extlst;
			}
			if ( input ) {
				delete input;
			}
			if ( textarea ) {
				delete textarea;
			}
			if ( select ) {
				delete select;
			}
			if ( date ) {
				delete date;
			}
			if ( frmlst ) {
				delete frmlst;
			}
			if ( filepick ) {
				delete filepick;
			}
		}
	};

	class CWndModeForm : public udWndGraphic {
		typedef ModeFormControlInfo* _control;
		typedef map< udInt, _control > _int_asc_control;

		udPFormHandler		m_pHandler;		// обработчик
		pfnGetObject		m_fnGetObject;	// калбэк создания объекта
		udPBase				m_pObject;		// объект редактирования
		_int_asc_control	m_arrControl;	// набор элементов
		udPWndButton		m_btnOk,		// кнопка подтверждения
							m_btnCancel;	// кнопка отмены
		bool				m_bIsModal,		// модальное или нет родительское окно
							m_bIsWatch;		// режим просмотра

	public:
		CWndModeForm( udPFormHandler handler = NULL );

		~CWndModeForm( );

		void	MakeControls( );
		void	FillControls( );
		void	ShowControls( );
		void	LoadControlData( );
		void	SetButtonOkPosition( udInt x, udInt y );
		void	SetButtonCancelPosition( udInt x, udInt y );
		void	Clear( );
		void	SetObj( udPBase obj );
		void	AddInput( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font, udDWord addstyle );
		void	AddText( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font );
		void	AddDate( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font, udInt bString = FALSE );
		void	AddDatetime( udInt iIndex, string attr, ModeFormControlBox* box );
		void	AddSelect( udInt iIndex, string attr, ModeFormControlBox* box, udPFont font, const udArrWString& arr );
		// attr - атрибут сущности, по значению которого выбирать другой объект
		// title - заголовок контрола
		// table - таблица сущностей, которые надо выбирать при загрузке
		// fnGetObject - калбэк создания объекта сущности
		// attrpick - атрибут, значение которого будет показываться контролом
		// index - индекс контрола
		udPExtList	AddExtlist(
			udInt iIndex, string attr,
			ModeFormControlBox* box, const string& attrpick,
			//const string& index, const wstring& title,
			//const string& table, pfnGetObject fnGetObject,
			//bool auto_load = true
			const wstring& title,
			pfnGetListHandler fnGetListHandler,
			udPMorph opt = NULL,
			pfnExtLstAfterPick fnAfterPick = NULL
			);
		udPWndFormList	AddFormlist( udInt iIndex, string attr, ModeFormControlBox* box, const wstring& folder = L"" );
		udPWndFilePick	AddFilePick( udInt iIndex, const string& attr, ModeFormControlBox* box, const wstring& title, const wstring& folder );
		udPWndStatic	AddStatic( udInt iIndex, ModeFormControlBox* box, const wstring& title, udUInt size, udDWord color = 0 );

		virtual void	Render( );
		virtual bool	Create( HWND hParent = NULL, HMENU hMenu = NULL );

		virtual LRESULT	msgButtonClick( LPARAM lParam );
		virtual LRESULT msgDrawItem( WPARAM wParam, LPARAM lParam );

		virtual LRESULT	MessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void	SetIsModal( bool bIsModal ) { m_bIsModal = bIsModal; }
		void	SetIsWatch( bool bIsWatch );
		void	SetHandler( udPFormHandler handler ) { m_pHandler = handler; }
		void	SetGetObjectCallback( pfnGetObject fnGetObject ) { m_fnGetObject = fnGetObject; }

		udPBase			GetObj( )			const { return m_pObject;		}
		udPFormHandler	GetHandler( )		const { return m_pHandler;		}
		bool			IsModal( )			const { return m_bIsModal;		}
		bool			IsWatch( )			const { return m_bIsWatch;		}
		udPWndButton	GetButtonOk( )		const { return m_btnOk;			}
		udPWndButton	GetButtonCancel( )	const { return m_btnCancel;		}

		ModeFormControlInfo* GetInfo( udInt iIndex );

	private:
		// функции создания контролов
		void	MakeInput( udPGdi gdi, ModeFormControlInfo* info );
		void	MakeText( udPGdi gdi, ModeFormControlInfo* info );
		void	MakeExtlist( udPGdi gdi, ModeFormControlInfo* info );
		void	MakeSelect( udPGdi gdi, ModeFormControlInfo* info );
		void	MakeDate( udPGdi gdi, ModeFormControlInfo* info );
		void	MakeFormlist( udPGdi gdi, ModeFormControlInfo* info );
		void	MakeFilePick( udPGdi gdi, ModeFormControlInfo* info );
		void	MakeStatic( udPGdi gdi, ModeFormControlInfo* info );
		// функции наполнения контролов данными из атрибутов объекта
		void	FillInput( ModeFormControlInfo* info );
		void	FillText( ModeFormControlInfo* info );
		void	FillExtlst( ModeFormControlInfo* info );
		void	FillSelect( ModeFormControlInfo* info );
		void	FillDate( ModeFormControlInfo* info );
		void	FillFormlist( ModeFormControlInfo* info );
		void	FillFilePick( ModeFormControlInfo* info );
		// функции подгрузки данных из контролов в атрибуты объекта
		void	LoadInput( ModeFormControlInfo* info );
		void	LoadText( ModeFormControlInfo* info );
		void	LoadExtlst( ModeFormControlInfo* info );
		void	LoadSelect( ModeFormControlInfo* info );
		void	LoadDate( ModeFormControlInfo* info );
		void	LoadFormlist( ModeFormControlInfo* info );
		void	LoadFilePick( ModeFormControlInfo* info );
		// функции рисования контролов
		void	DrawSelect( LPDRAWITEMSTRUCT di );

	}; // class CWndModeForm

} // namespace UdSDK

#endif
