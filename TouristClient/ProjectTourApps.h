/**
	Tourist application
	Клиентская часть

	приложение 1 к туру для распечатки группового договора
*/
#ifndef TOURIST_APPLICATION_TOURIST_CLIENT_TOUR_APPS_H
#define TOURIST_APPLICATION_TOURIST_CLIENT_TOUR_APPS_H

namespace UdSDK {

	class CHGroupContractApp1 : public udFormHandler {
	public:
		enum {
			ctrlidPay1,			// оплата 1
			ctrlidPay2,			// оплата 2
			ctrlidPay1Date,		// дата оплаты 1
			ctrlidPay2Date,		// дата оплаты 2
			ctrlidDays1,		// За сколько дней до поездки предоставить документы (не позднее)
			ctrlidDays2,		// В течение скольки дней после бронирования подтвердить тур
			ctrlidDays3,		// В течение скольки дней после бронирования оплатить тур (не позднее)
			/*ctrlidCountry,		// страна
			ctrlidDate1,		// срок поездки с
			ctrlidDate2,		// срок поездки по
			ctrlidPrice1,		// цена у.е.
			ctrlidPrice2,		// цена в рублях
			ctrlidAdvance,		// аванс
			ctrlidAdvDate,		// дата аванса
			ctrlidSurCharge,	// доплата
			ctrlidSurDate,		// дата доплаты*/
			ctrlidProduct,		// туристский продукт
			//ctrlidTouristInfo,	// сведения о туристе
			ctrlidOccupancy,	// средства размещения
			ctrlidProgram,		// экскурсионная программа
			ctrlidTransfer,		// услуги по перевозке
			ctrlidCover,		// страхование
			ctrlidVisa,			// визовое оформление
			ctrlidService,		// другие услуги
			ctrlidTransfer2,	// трансфер
		};

	public:
		CHGroupContractApp1( );

		virtual ~CHGroupContractApp1( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectGroupContractApp1; }

		virtual void	FormListOnAdd( udPWndFormList wnd );
		virtual void	FormListOnEdit( udPWndFormList wnd, udPBase obj );
		virtual bool	FormListOnDelete( udPWndFormList wnd, udPBase obj );
		virtual void	LoadAfterSetObject( );

	}; // class CHGroupContractApp1F

	class CHGCTouristInfo : public udFormHandler {
	public:
		enum {
			ctrlidFio,		// ФИО
			ctrlidSex,		// Пол
			ctrlidBorn,		// дата рождения
			ctrlidPassport,	// пасспорт
		};

	public:
		CHGCTouristInfo( );

		virtual ~CHGCTouristInfo( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectGCTouristInfo; }

	}; // class CHGCTouristInfo

	class CHGCOccupancy : public udFormHandler {
	public:
		enum {
			ctrlidName,		// наименование
			ctrlidCategory,	// категория номера
			ctrlidAmount,	// количество проживающих
			ctrlidFeed,		// питание
		};

	public:
		CHGCOccupancy( );

		virtual ~CHGCOccupancy( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectGCOccupancy; }

	}; // class CHGCOccupancy

	class CHGCProgram : public udFormHandler {
	public:
		enum {
			ctrlidText,	// текст
		};

	public:
		CHGCProgram( );

		virtual ~CHGCProgram( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectGCProgram; }

	}; // class CHGCProgram

	class CHGCTransfer : public udFormHandler {
	public:
		enum {
			ctrlidFio,		// ФИО
			ctrlidTrans,	// перевозчик
			ctrlidLevel,	// уровень сервиса
			ctrlidDate1,	// дата отбытия
			ctrlidDate2,	// дата прибытия
		};

	public:
		CHGCTransfer( );

		virtual ~CHGCTransfer( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectGCTransfer; }

	}; // class CHGCTransfer

	class CHGCCover : public udFormHandler {
	public:
		enum {
			ctrlidType,	// тип страховки
			ctrlidName,	// наименование страховщика
		};

	public:
		CHGCCover( );

		virtual ~CHGCCover( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectGCCover; }

	}; // class CHGCCover

	class CHGCVisa : public udFormHandler {
	public:
		enum {
			ctrlidType,	// тип услуги
		};

	public:
		CHGCVisa( );

		virtual ~CHGCVisa( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectGCVisa; }

	}; // class CHGCVisa

	class CHGCService : public udFormHandler {
	public:
		enum {
			ctrlidName,	// наименование услуги
			ctrlidChar,	// характеристики
		};

	public:
		CHGCService( );

		virtual ~CHGCService( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectGCService; }

	}; // class CHGCService

	class CHGCTransfer2 : public udFormHandler {
	public:
		enum {
			ctrlidRoute,	// маршрут
			ctrlidType,		// тип трансфера
		};

	public:
		CHGCTransfer2( );

		virtual ~CHGCTransfer2( );

		virtual udInt	GetFormModel( );
		virtual udInt	GetFormView( );
		virtual udInt	ValidateForm( );
		virtual udInt	FormOk( );
		virtual udInt	FormCancel( );
		virtual udInt	ProcForm( bool modal = false );

		virtual pfnGetObject	GetObjectCallback( ) { return GetProjectGCTransfer2; }

	}; // class CHGCTransfer2

} // namespace UdSDK

#endif
