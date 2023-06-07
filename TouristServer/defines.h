
#ifndef DEFINES_H
#define DEFINES_H

// макросы стандартных классов окон
#define STANDARD_BUTTON		L"BUTTON"
#define STANDARD_COMBOBOX	L"COMBOBOX"
#define STANDARD_EDIT		L"EDIT"
#define STANDARD_LISTBOX	L"LISTBOX"
#define STANDARD_MDICLIENT	L"MDICLIENT"
#define STANDARD_SCROLLBAR	L"SCROLLBAR"
#define STANDARD_STATIC		L"STATIC"

// заголовки для MessageBox
#define MB_TITLE_ERROR		L"ERROR"
#define MB_TITLE_EXCEPTION	L"EXCEPTION"
#define MB_TITLE_NOTE		L"NOTE"

// id элементов интерфейса
#define WND_ID_FORM_LOGIN	20001	// форма входа
#define WND_ID_FL_LOGIN		20002	// поле ввода логина
#define WND_ID_FL_PASSW		20003	// поле ввода пароля
#define WND_ID_FL_SUBMIT	20004	// кнопка отправки
#define WND_ID_FL_CANCEL	20005	// кнопка отмены входа
#define WND_ID_FORM_WORK	20006	// рабочая форма
#define WND_ID_AREA_MENU	20007	// область меню
#define WND_ID_AREA_WORK	20008	// рабочая область

#define WND_FW_MAX			1000	// максимальное количество
#define WND_ID_START		20009	// стартовое ID раздела
#define WND_ID_FW_REQ		20009	// обращения
#define WND_ID_FW_ORD		20010	// заявок
#define WND_ID_FW_TOURS		20011	// туры
#define WND_ID_FW_SERV		20012	// услуги
#define WND_ID_FW_GROUP		20013	// группы
#define WND_ID_FW_TMPL		20014	// шаблоны
#define WND_ID_FW_CLIENT	20015	// клиенты
#define WND_ID_FW_LOG		20016	// логи системы
#define WND_ID_FW_DOCS		20017	// документы клиента
#define WND_ID_FW_CRNCY		20018	// валюты
#define WND_ID_FW_CPAIR		20019	// валютная пара
#define WND_ID_FW_TSERV		20020	// тип услуги
#define WND_ID_FW_ORDST		20021	// статус заявки
#define WND_ID_FW_REQRES	20022	// результат обращения
#define WND_ID_FW_ROUTE		20023	// маршрут
#define WND_ID_FW_STTOUR	20024	// состояние тура
#define WND_ID_FW_COUNTRY	20025	// страна
#define WND_ID_FW_CITY		20026	// город
#define WND_ID_FW_TREQ		20027	// тип обращения
#define WND_ID_FW_COMPANY	20028	// компания продавец
#define WND_ID_FW_XSERV		20029	// единица измерения услуги
#define WND_ID_FW_PARTNER	20030	// партнер
#define WND_ID_FW_UCLIENT	20031	// клиент - юридическое лицо
#define WND_ID_FW_ACC		20032	// лицевой счет
#define WND_ID_FW_DIRECT	20033	// направление
#define WND_ID_FW_NSERV		20034	// название услуги
#define WND_ID_FW_RPARTNER	20035	// запрос партнеру
#define WND_ID_FW_APARNTER	20036	// ответ партнера
#define WND_ID_FW_USER		20037	// пользователи системы
#define WND_ID_FW_DOCTYPE	20038	// тип документа пользователя
#define WND_ID_FW_INFO		20039	// справочники
#define WND_ID_FW_REPORT	20040	// отчеты

#define WND_ID_BUTTON_DEL	24996	// кнопка удаления при выбранной строке
#define WND_ID_BUTTON_EDIT	24997	// кнопка редактирования при выбранной строке

// надписи
#define WND_ID_LBL_LOGIN	21001	// надпись логин
#define WND_ID_LBL_PASSW	21002	// надпись пароль

// для формы редактирования используется диапозон от 25000
#define WND_ID_EDIT_OK		24998
#define WND_ID_EDIT_CANCEL	24999
#define WND_ID_START_EDIT	25000

// для формы списка используется диапозон от 30000
#define WND_LIST_BUTTON_MAX	1000	// максимально количество кнопок в списке
#define WND_ID_LIST_ADD		29999	// кнопка добавления
#define WND_ID_LIST_ESTART	30000	// начальный ID для кнопок редактирования
#define WND_ID_LIST_DSTART	31000	// начальный ID для кнопок удаления

#ifndef FOR_WIN32
#define FOR_WIN32
#endif

#endif
