/**
	Tourist application
	Набор классов объектов

	пользователь
*/
#ifndef TOURIST_APPLICATION_OBJECTS_USER_H
#define TOURIST_APPLICATION_OBJECTS_USER_H

namespace UdSDK {

	class CProjectUser : public udBase {
	public:
		enum {
			USERRANK_INVALID,		// не допустимый тип пользователя
			USERRANK_ADMINISTRATOR,	// администратор
			USERRANK_MANAGER,		// менеджер
			USERRANK_CACHIER,		// кассир
			USERRANK_ACCOUNTANT,	// бухгалтер
			USERRANK_CHIEF,			// руководитель
			USERRANK_SUPERADMIN		// суперадминистратор
		};

	public:
		CProjectUser( );

		virtual ~CProjectUser( );

		virtual void	GetConfig( udBaseConfig& objConfig );

	}; // class CProjectUser

} // namespace UdSDK

#endif
