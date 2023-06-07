
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udPBase GetProjectServicePeriodForCost( ) {
		udPBase ret = GetProjectServicePeriod( );
		ret->get( "partner" )->ini( ( udUInt ) 0 );
		return ret;
	} // udPBase GetProjectServicePeriodForCost

	static void TourFilterService( udArrBase& arr ) {
		if ( arr.empty( ) ) {
			return;
		}
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "service_" )->ini( wstring( L"" ) );
		}

		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udPBase obj1 = NULL, obj2 = NULL;
		string szWhere;
		udArrString arrString;
		udArrBase arr1;
		udArrError err;
		// подгружаем ID названий услуг
		LoadListDataByAttr( arr, handler, GetProjectService, "ud_service", "service", "name", "id_name" );
		// подгружаем названия услуг
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj1 = *i;
			arrString.push_back( obj1->get( "id_name" )->toString( ) );
		}
		obj1 = new udProjectNameService;
		obj1->GetConfig( config );
		szWhere = config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrString ) + ")";
		config.Clear( );
		udDELETE( obj1 );
		// подгружаем названия услуг
		option.SetTable( "ud_nameservice" );
		option.SetClass( GetProjectNameService );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr1, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
			ClearArrBase( arr1 );
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				( *i )->unset( "id_name" );
			}
			return;
		} else if ( arr1.empty( ) ) {
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				( *i )->unset( "id_name" );
			}
			return;
		}
		udUInt id = 0;
		datetime date;
		wstring date1, date2, ready;
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj1 = *i;
			id = obj1->get( "id_name" )->toUInt( );
			obj1->unset( "id_name" );
			for( udArrBase::iterator j = arr1.begin( ); j != arr1.end( ); ++j ) {
				obj2 = *j;
				if ( id == obj2->get( "id" )->toUInt( ) ) {
					obj1->get( "service_" )->ini( obj2->get( "name" )->toWString( ) );
					/*date = obj1->get( "date1" )->toTime( );
					date1 = date.toWString( "%Y-%m-%d" );
					date = obj1->get( "date2" )->toTime( );
					date2 = date.toWString( "%Y-%m-%d" );
					if ( obj1->get( "ready" )->toUInt( ) == 1 ) {
						ready = L"Не готова";
					} else {
						ready = L"Готова";
					}

					obj1->get( "name" )->ini( obj2->get( "name" )->toWString( ) );

					obj1->get( "#name" )->ini(
						ready + L"," + date1 + L"-" + date2 + L", " +
						obj1->get( "num" )->toWString( ) + L", " +
						obj2->get( "name" )->toWString( )
					);*/
					break;
				}
			}
		}
		ClearArrBase( arr1 );
	} // void TourFilterService

	static void TourFilterClient( udArrBase& arr ) {
		if ( arr.empty( ) ) {
			return;
		}
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "fio" )->ini( wstring( L"" ) );
		}
		LoadListDataByAttr( arr, g_pApp->GetHandler( ), GetProjectClient, "ud_client", "client", "fio", "fio" );
	} // void TourFilterClient

	static void TourFilterTourLink( udArrBase& arr ) {
		if ( arr.empty( ) ) {
			return;
		}
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "tour2_" )->ini( wstring( L"" ) );
		}
		LoadListDataByAttr( arr, g_pApp->GetHandler( ), GetProjectTour, "ud_tour", "tour2", "name", "tour2_" );
	} // void TourFilterTourLink

	static void TourFilterRoute( udArrBase& arr ) {
		if ( arr.empty( ) ) {
			return;
		}
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "city_" )->ini( wstring( L"" ) );
		}
		LoadListDataByAttr( arr, g_pApp->GetHandler( ), GetProjectCity, "ud_city", "city", "name", "city_" );
	} // void TourFilterRoute

	udPBase TourSpecAppendEmployer( udBinaryData& data, udPBase obj ) {
		if ( obj->get( "emptype" )->toInt( ) == 1 ) {
			return LoadInstanceByForeignKey( g_pApp->GetHandler( ), obj, GetProjectClientUr, "ud_clientur", "employer", "id" );
		}
		return LoadInstanceByForeignKey( g_pApp->GetHandler( ), obj, GetProjectClient, "ud_client", "employer", "id" );
	} // udPBase TourSpecAppendEmployer

	static void TourFilterAttach( udArrBase& arrAdd, udArrBase& arrUpd, udArrBase& arrDel, udPBase obj ) {
		udPAppConfig pAppConfig = g_pApp->GetConfig( );
		wstring tmp_dir = pAppConfig->get( "temporary_dir" )->toWString( ),
			client_dir = pAppConfig->get( "files_dir" )->toWString( ) + L"\\tour",
			dst_dir, filename, src;
		HANDLE hFile = NULL;
		udPLog __log = g_pApp->GetLog( );
		//string log = "\nClientFilterAttach\n";
		udPBase app = NULL;
		/*log += "сам объект, ";
		obj->dump( log );*/

		dst_dir = client_dir + L"\\" + obj->get( "id" )->toWString( );
		CreateDirectory( dst_dir.data( ), NULL );
		//__log->Write( "создаем папку '" + toString( dst_dir ) + "'\n" );

		//log += "на удаление:\n";
		for( udArrBase::iterator i = arrDel.begin( ); i != arrDel.end( ); ++i ) {
			app = *i;
			/*app->dump( log );
			log += "\n";*/

			filename = dst_dir + L"\\" + app->get( "name" )->toWString( );
			__log->Write( "удаляем файл '" + toString( filename ) + "'\n" );
			DeleteFile( filename.data( ) );
		}
		//log += "\nна добавление:\n";
		for( udArrBase::iterator i = arrAdd.begin( ); i != arrAdd.end( ); ++i ) {
			app = *i;
			/*app->dump( log );
			log += "\n";*/

			src = tmp_dir + L"\\" + app->get( "name" )->toWString( );
			filename = dst_dir + L"\\" + app->get( "name" )->toWString( );
			/*__log->Write( "добавляем файл '" + toString( filename ) + "', копируем из временного '" +
				toString( src ) + "'\n" );*/
			/*if ( CopyFile( src.data( ), filename.data( ), FALSE ) == 0 ) {
				__log->Write( "CopyFile, error code=" + toString( GetLastError( ) ) + "\n" );
			}*/
		}
		//log += "на обновление:\n";
		for( udArrBase::iterator i = arrUpd.begin( ); i != arrUpd.end( ); ++i ) {
			app = *i;
			/*app->dump( log );
			log += "\n";*/

			src = tmp_dir + L"\\" + app->get( "name" )->toWString( );
			filename = dst_dir + L"\\" + app->get( "name" )->toWString( );
			//__log->Write( "удаляем файл '" + toString( filename ) + "'\n" );
			//DeleteFile( filename.data( ) );
			/*if ( CopyFile( src.data( ), filename.data( ), FALSE ) == 0 ) {
				__log->Write( "CopyFile, error code=" + toString( GetLastError( ) ) + "\n" );
			}*/
		}
		
		//__log->Write( log );
	} // void TourFilterAttach

	udPBase GetProjectTour2( ) {
		udPBase ret = GetProjectTour( );
		ret->get( "employer_" )->ini( wstring( L"" ) );
		ret->get( "direct_" )->ini( wstring( L"" ) );
		ret->get( "status_" )->ini( wstring( L"" ) );
		ret->get( "employer2_" )->ini( wstring( L"" ) );
		ret->get( "manager_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTour2

	CHTour::CHTour( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_tour";
		m_szUpdIndex	= "id";
		m_szName		= "tour";
		m_fnGetObject	= GetProjectTour;

		this->AddManagedTable( "ud_tourroute", "id", GetProjectTourRoute );
		this->AddManagedTable( "ud_tourservice", "id", GetProjectTourService );
		this->AddManagedTable( "ud_tourclient", "id", GetProjectTourClient );
		this->AddManagedTable( "ud_tourtour", "id", GetProjectTourTour );
		this->AddManagedTable( "ud_attach", "id", GetProjectAttach );
		// приложение 1
		this->AddManagedTable( "ud_gcapp1", "id", GetProjectGroupContractApp1 );
		this->AddManagedTable( "ud_gctouristinfo", "id", GetProjectGCTouristInfo );
		this->AddManagedTable( "ud_gcoccupancy", "id", GetProjectGCOccupancy );
		this->AddManagedTable( "ud_gcprogram", "id", GetProjectGCProgram );
		this->AddManagedTable( "ud_gctransfer", "id", GetProjectGCTransfer );
		this->AddManagedTable( "ud_gccover", "id", GetProjectGCCover );
		this->AddManagedTable( "ud_gcvisa", "id", GetProjectGCVisa );
		this->AddManagedTable( "ud_gcservice", "id", GetProjectGCService );
		this->AddManagedTable( "ud_gctransfer2", "id", GetProjectGCTransfer2 );

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_company", "company", "id", GetProjectCompany );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_client", "employer", "id", GetProjectClient,
			"", "", NULL, NULL, NULL, TourSpecAppendEmployer );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_cldoc", "document", "id", GetProjectClDoc, "`cldoc_id` ASC" );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_user", "manager2", "id", GetProjectUser );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_direct", "direct", "id", GetProjectDirect );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_stour", "state", "id", GetProjectSTour );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_tourroute", "tour", "id", GetProjectTourRoute, "`tourroute_id` ASC", "", TourFilterRoute );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_tourservice", "tour", "id", GetProjectTourService, "`tourservice_id` ASC", "", TourFilterService );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_tourclient", "tour", "id", GetProjectTourClient, "`tourclient_id` ASC", "", TourFilterClient );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_tourtour", "tour1", "id", GetProjectTourTour, "`tourtour_id` ASC", "", TourFilterTourLink );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_attach", "obj", "id", GetProjectAttach, "`attach_id` ASC", "`attach_type`=" + toString( udProjectAttach::ATTACH_TOUR ) );//,
			//NULL, NULL, TourFilterAttach );

		m_pObjFilter->get( "name" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "direct" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "date1" )->ini( datetime( ) );
		m_pObjFilter->get( "date2" )->ini( datetime( ) );
		m_pObjFilter->get( "status" )->ini( ( udInt ) 0 );
		m_pObjFilter->get( "employer" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "emptype" )->ini( ( udInt ) udProjectTour::TOUR_EMPLOYER_CLIENT );
		m_pObjFilter->get( "order" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "tpltour" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "manager" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "client" )->ini( ( udUInt ) 0 );
	}

	CHTour::CHTour( const CHTour& objCopy ) {
	}

	CHTour::~CHTour( ) {
	}

	CHTour& CHTour::operator =( const CHTour& objCopy ) {
		return *this;
	}

	void CHTour::Init( ) {
		udStateHandlerSimple::Init( );
		wstring folder = L"";
		udPAppConfig appConfig = g_pApp->GetConfig( );
		//folder = appConfig->get( "temporary_dir" )->toWString( ) + L"\\tour";
		//CreateDirectory( folder.data( ), NULL );
		folder = appConfig->get( "files_dir" )->toWString( ) + L"\\tour";
		CreateDirectory( folder.data( ), NULL );
	} // void Init

	udInt CHTour::FindObj( udPBase obj, const string& attr, bool apply ) {
		if ( apply ) {
			return 0;
		}
		// при обновлении пожалуйста, ищем по ID и пох
		return udStateHandlerSimple::FindObj( obj, attr, apply );
	} // bool FindObj

	udInt CHTour::AddObj( udPBase obj, bool save_info ) {
		if ( udStateHandlerSimple::AddObj( obj, save_info ) == 1 ) {
			// дополнительно создаем папку для приложений
			wstring dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
				L"\\tour\\" + obj->get( "id" )->toWString( );
			CreateDirectory( dir.data( ), NULL );
			g_pApp->GetLog( )->Write( "'" + toString( dir ) + "'\n" );
			return 1;
		}
		return 0;
	} // udInt AddObj

	void CHTour::DeleteLinkedObjects( udPBase obj ) {
		udPDatabase db = g_pApp->GetDatabase( );
		string query = "", szWhere = "";
		udBaseConfig config;
		udPBase obj1;
		udPType type = NULL;

		// маршрут
		obj1 = new udProjectTourRoute;
		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "tour" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );

		// услуги
		obj1 = new udProjectTourService;
		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "tour" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );

		// приложение
		obj1 = new udProjectAttach;
		obj1->GetConfig( config );
		delete obj1;
		type = new udType( ( udInt ) ( udProjectAttach::ATTACH_CLIENT ) );
		szWhere = config.GetAttrIndex( "type" ) + "=" + type->toString( ) +
			config.GetAttrIndex( "obj" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );
		wstring folder = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
			L"\\tour\\" + obj->get( "id" )->toWString( );
		// папку удалить
		DirectoryClear( folder );
		RemoveDirectory( folder.data( ) );

		// туристы
		obj1 = new udProjectTourClient;
		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "tour" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );

		// связи с турами
		obj1 = new udProjectTourTour;
		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "tour1" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );
	} // void DeleteLinkedObjects

	udInt CHTour::DelObj( udPBase obj, bool save_info ) {
		if ( udStateHandlerSimple::DelObj( obj, save_info ) == 1 ) {
			// удаляем связанные объекты
			this->DeleteLinkedObjects( obj );
			return 1;
		}
		return 0;
	} // udInt DelObj

	void CHTour::ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj ) {
		if ( ( opId == opidAdd1 ) || ( opId == opidUpd1 ) ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		// приложение 1
		udPLog __log = g_pApp->GetLog( );
		
		__log->Write( "CHTour::ExtraDataRecieve\n" );

		string log = "\nApp1:\n";
		udPBase pApp1 = NULL, obj1 = NULL;
		udArrBase arrLoaded, arrCurrent;
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udArrError err;
		udUInt id = 0;
		udPHObjectLock hLock = g_pApp->GetModel( )->GetLockHandler( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// общие данные приложения
		option.SetTable( "ud_gcapp1" );
		pApp1 = GetProjectGroupContractApp1( );
		pApp1->GetConfig( config );
		pApp1->read( sock );
		pApp1->get( "tour" )->ini( obj->get( "id" )->toUInt( ) );
		pApp1->dump( log );
		__log->Write( log + "\n" );
		if ( pApp1->get( "id" )->toUInt( ) > 0 ) {
			//bLoadDel = true;
			option.SetIndexAttr( "id" );
			handler->UpdUdObject( pApp1, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		} else {
			handler->AddUdObject( pApp1, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
			udUInt id = ( udUInt )(  ( udPMySql ) g_pApp->GetDatabase( ) )->GetInsertId( );
			pApp1->get( "id" )->ini( id );
		}
		option.Clear( );
		id = pApp1->get( "id" )->toUInt( );
		//////////////////////////////////////////////////////////////////////////////////////////
		// Средства размещения
		ReadInstances( sock, arrLoaded, GetProjectGCOccupancy );
		LoadInstanceByForeignKey( handler, pApp1, arrCurrent, GetProjectGCOccupancy, "ud_gcoccupancy", "app", "id" );
		if ( arrLoaded.empty( ) ) { // с клиента ничего не прилетело, вдруг их удалили
			if ( !arrCurrent.empty( ) ) { // если в прилетевших нет, то все на удаление
				for( udArrBase::iterator j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "app" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_gcoccupancy", true );
						hLock->UnlockObject( obj1, GetProjectGCOccupancy );
					}
				}
			}
		} else if ( arrCurrent.empty( ) ) { // у объекта не было потомков
			for( udArrBase::iterator j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				obj1 = *j;
				obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
				obj1->get( "app" )->ini( id );
				udStateHandlerSimple::AddObj( obj1, "ud_gcoccupancy", true );
			}
		} else { // оба не пустые, необходимо сравнить и составить списки add/upd/del
			udArrBase arrAdd, arrUpd, arrDel;
			udPBase objNew = NULL, objOld = NULL;
			udArrBase::iterator j, k;
			bool bFound = false;

			// добавление и обновление
			for( j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				objNew = *j;
				bFound = false;

				for( k = arrCurrent.begin( ); k != arrCurrent.end( ); ++k ) {
					objOld = *k;
					if ( objNew->get( "id" )->toUInt( ) == objOld->get( "id" )->toUInt( ) ) {
						bFound = true;
						arrUpd.push_back( objNew );
						break;
					}
				}

				if ( !bFound ) {
					arrAdd.push_back( objNew );
				}
			}

			// удаление
			for( j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
				objOld = *j;
				bFound = false;

				for( k = arrLoaded.begin( ); k != arrLoaded.end( ); ++k ) {
					objNew = *k;
					if ( objOld->get( "id" )->toUInt( ) == objNew->get( "id" )->toUInt( ) ) {
						bFound = true;
						break;
					}
				}

				if ( !bFound ) {
					arrDel.push_back( objOld );
				}
			}

			// выполняем операции
			if ( !arrAdd.empty( ) ) {
				for( udArrBase::iterator j = arrAdd.begin( ); j != arrAdd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
					obj1->get( "app" )->ini( id );
					udStateHandlerSimple::AddObj( obj1, "ud_gcoccupancy", true );
				}
			}
			if ( !arrUpd.empty( ) ) {
				for( udArrBase::iterator j = arrUpd.begin( ); j != arrUpd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "app" )->ini( id ); // вдруг прилетит херня
					udStateHandlerSimple::UpdObj( obj1, "ud_gcoccupancy", "id", true );
					hLock->UnlockObject( obj1, GetProjectGCOccupancy );
				}
			}
			if ( !arrDel.empty( ) ) {
				for( udArrBase::iterator j = arrDel.begin( ); j != arrDel.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "app" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_gcoccupancy", true );
						hLock->UnlockObject( obj1, GetProjectGCOccupancy );
					}
				}
			}
		}
		ClearArrBase( arrLoaded );
		ClearArrBase( arrCurrent );
		//////////////////////////////////////////////////////////////////////////////////////////
		// Экскурсионная программа
		ReadInstances( sock, arrLoaded, GetProjectGCProgram );
		LoadInstanceByForeignKey( handler, pApp1, arrCurrent, GetProjectGCProgram, "ud_gcprogram", "app", "id" );
		if ( arrLoaded.empty( ) ) { // с клиента ничего не прилетело, вдруг их удалили
			if ( !arrCurrent.empty( ) ) { // если в прилетевших нет, то все на удаление
				for( udArrBase::iterator j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "app" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_gcprogram", true );
						hLock->UnlockObject( obj1, GetProjectGCProgram );
					}
				}
			}
		} else if ( arrCurrent.empty( ) ) { // у объекта не было потомков
			for( udArrBase::iterator j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				obj1 = *j;
				obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
				obj1->get( "app" )->ini( id );
				udStateHandlerSimple::AddObj( obj1, "ud_gcprogram", true );
			}
		} else { // оба не пустые, необходимо сравнить и составить списки add/upd/del
			udArrBase arrAdd, arrUpd, arrDel;
			udPBase objNew = NULL, objOld = NULL;
			udArrBase::iterator j, k;
			bool bFound = false;

			// добавление и обновление
			for( j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				objNew = *j;
				bFound = false;

				for( k = arrCurrent.begin( ); k != arrCurrent.end( ); ++k ) {
					objOld = *k;
					if ( objNew->get( "id" )->toUInt( ) == objOld->get( "id" )->toUInt( ) ) {
						bFound = true;
						arrUpd.push_back( objNew );
						break;
					}
				}

				if ( !bFound ) {
					arrAdd.push_back( objNew );
				}
			}

			// удаление
			for( j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
				objOld = *j;
				bFound = false;

				for( k = arrLoaded.begin( ); k != arrLoaded.end( ); ++k ) {
					objNew = *k;
					if ( objOld->get( "id" )->toUInt( ) == objNew->get( "id" )->toUInt( ) ) {
						bFound = true;
						break;
					}
				}

				if ( !bFound ) {
					arrDel.push_back( objOld );
				}
			}

			// выполняем операции
			if ( !arrAdd.empty( ) ) {
				for( udArrBase::iterator j = arrAdd.begin( ); j != arrAdd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
					obj1->get( "app" )->ini( id );
					udStateHandlerSimple::AddObj( obj1, "ud_gcprogram", true );
				}
			}
			if ( !arrUpd.empty( ) ) {
				for( udArrBase::iterator j = arrUpd.begin( ); j != arrUpd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "app" )->ini( id ); // вдруг прилетит херня
					udStateHandlerSimple::UpdObj( obj1, "ud_gcprogram", "id", true );
					hLock->UnlockObject( obj1, GetProjectGCProgram );
				}
			}
			if ( !arrDel.empty( ) ) {
				for( udArrBase::iterator j = arrDel.begin( ); j != arrDel.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "app" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_gcprogram", true );
						hLock->UnlockObject( obj1, GetProjectGCProgram );
					}
				}
			}
		}
		ClearArrBase( arrLoaded );
		ClearArrBase( arrCurrent );
		//////////////////////////////////////////////////////////////////////////////////////////
		// Услуги по перевозке туристов
		ReadInstances( sock, arrLoaded, GetProjectGCTransfer );
		LoadInstanceByForeignKey( handler, pApp1, arrCurrent, GetProjectGCTransfer, "ud_gctransfer", "app", "id" );
		if ( arrLoaded.empty( ) ) { // с клиента ничего не прилетело, вдруг их удалили
			if ( !arrCurrent.empty( ) ) { // если в прилетевших нет, то все на удаление
				for( udArrBase::iterator j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "app" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_gctransfer", true );
						hLock->UnlockObject( obj1, GetProjectGCTransfer );
					}
				}
			}
		} else if ( arrCurrent.empty( ) ) { // у объекта не было потомков
			for( udArrBase::iterator j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				obj1 = *j;
				obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
				obj1->get( "app" )->ini( id );
				udStateHandlerSimple::AddObj( obj1, "ud_gctransfer", true );
			}
		} else { // оба не пустые, необходимо сравнить и составить списки add/upd/del
			udArrBase arrAdd, arrUpd, arrDel;
			udPBase objNew = NULL, objOld = NULL;
			udArrBase::iterator j, k;
			bool bFound = false;

			// добавление и обновление
			for( j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				objNew = *j;
				bFound = false;

				for( k = arrCurrent.begin( ); k != arrCurrent.end( ); ++k ) {
					objOld = *k;
					if ( objNew->get( "id" )->toUInt( ) == objOld->get( "id" )->toUInt( ) ) {
						bFound = true;
						arrUpd.push_back( objNew );
						break;
					}
				}

				if ( !bFound ) {
					arrAdd.push_back( objNew );
				}
			}

			// удаление
			for( j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
				objOld = *j;
				bFound = false;

				for( k = arrLoaded.begin( ); k != arrLoaded.end( ); ++k ) {
					objNew = *k;
					if ( objOld->get( "id" )->toUInt( ) == objNew->get( "id" )->toUInt( ) ) {
						bFound = true;
						break;
					}
				}

				if ( !bFound ) {
					arrDel.push_back( objOld );
				}
			}

			// выполняем операции
			if ( !arrAdd.empty( ) ) {
				for( udArrBase::iterator j = arrAdd.begin( ); j != arrAdd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
					obj1->get( "app" )->ini( id );
					udStateHandlerSimple::AddObj( obj1, "ud_gctransfer", true );
				}
			}
			if ( !arrUpd.empty( ) ) {
				for( udArrBase::iterator j = arrUpd.begin( ); j != arrUpd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "app" )->ini( id ); // вдруг прилетит херня
					udStateHandlerSimple::UpdObj( obj1, "ud_gctransfer", "id", true );
					hLock->UnlockObject( obj1, GetProjectGCTransfer );
				}
			}
			if ( !arrDel.empty( ) ) {
				for( udArrBase::iterator j = arrDel.begin( ); j != arrDel.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "app" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_gctransfer", true );
						hLock->UnlockObject( obj1, GetProjectGCTransfer );
					}
				}
			}
		}
		ClearArrBase( arrLoaded );
		ClearArrBase( arrCurrent );
		//////////////////////////////////////////////////////////////////////////////////////////
		// Страхование
		ReadInstances( sock, arrLoaded, GetProjectGCCover );
		LoadInstanceByForeignKey( handler, pApp1, arrCurrent, GetProjectGCCover, "ud_gccover", "app", "id" );
		if ( arrLoaded.empty( ) ) { // с клиента ничего не прилетело, вдруг их удалили
			if ( !arrCurrent.empty( ) ) { // если в прилетевших нет, то все на удаление
				for( udArrBase::iterator j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "app" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_gccover", true );
						hLock->UnlockObject( obj1, GetProjectGCCover );
					}
				}
			}
		} else if ( arrCurrent.empty( ) ) { // у объекта не было потомков
			for( udArrBase::iterator j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				obj1 = *j;
				obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
				obj1->get( "app" )->ini( id );
				udStateHandlerSimple::AddObj( obj1, "ud_gccover", true );
			}
		} else { // оба не пустые, необходимо сравнить и составить списки add/upd/del
			udArrBase arrAdd, arrUpd, arrDel;
			udPBase objNew = NULL, objOld = NULL;
			udArrBase::iterator j, k;
			bool bFound = false;

			// добавление и обновление
			for( j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				objNew = *j;
				bFound = false;

				for( k = arrCurrent.begin( ); k != arrCurrent.end( ); ++k ) {
					objOld = *k;
					if ( objNew->get( "id" )->toUInt( ) == objOld->get( "id" )->toUInt( ) ) {
						bFound = true;
						arrUpd.push_back( objNew );
						break;
					}
				}

				if ( !bFound ) {
					arrAdd.push_back( objNew );
				}
			}

			// удаление
			for( j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
				objOld = *j;
				bFound = false;

				for( k = arrLoaded.begin( ); k != arrLoaded.end( ); ++k ) {
					objNew = *k;
					if ( objOld->get( "id" )->toUInt( ) == objNew->get( "id" )->toUInt( ) ) {
						bFound = true;
						break;
					}
				}

				if ( !bFound ) {
					arrDel.push_back( objOld );
				}
			}

			// выполняем операции
			if ( !arrAdd.empty( ) ) {
				for( udArrBase::iterator j = arrAdd.begin( ); j != arrAdd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
					obj1->get( "app" )->ini( id );
					udStateHandlerSimple::AddObj( obj1, "ud_gccover", true );
				}
			}
			if ( !arrUpd.empty( ) ) {
				for( udArrBase::iterator j = arrUpd.begin( ); j != arrUpd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "app" )->ini( id ); // вдруг прилетит херня
					udStateHandlerSimple::UpdObj( obj1, "ud_gccover", "id", true );
					hLock->UnlockObject( obj1, GetProjectGCCover );
				}
			}
			if ( !arrDel.empty( ) ) {
				for( udArrBase::iterator j = arrDel.begin( ); j != arrDel.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "app" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_gccover", true );
						hLock->UnlockObject( obj1, GetProjectGCCover );
					}
				}
			}
		}
		ClearArrBase( arrLoaded );
		ClearArrBase( arrCurrent );
		//////////////////////////////////////////////////////////////////////////////////////////
		// Визовое оформление
		ReadInstances( sock, arrLoaded, GetProjectGCVisa );
		LoadInstanceByForeignKey( handler, pApp1, arrCurrent, GetProjectGCVisa, "ud_gcvisa", "app", "id" );
		if ( arrLoaded.empty( ) ) { // с клиента ничего не прилетело, вдруг их удалили
			if ( !arrCurrent.empty( ) ) { // если в прилетевших нет, то все на удаление
				for( udArrBase::iterator j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "app" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_gcvisa", true );
						hLock->UnlockObject( obj1, GetProjectGCVisa );
					}
				}
			}
		} else if ( arrCurrent.empty( ) ) { // у объекта не было потомков
			for( udArrBase::iterator j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				obj1 = *j;
				obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
				obj1->get( "app" )->ini( id );
				udStateHandlerSimple::AddObj( obj1, "ud_gcvisa", true );
			}
		} else { // оба не пустые, необходимо сравнить и составить списки add/upd/del
			udArrBase arrAdd, arrUpd, arrDel;
			udPBase objNew = NULL, objOld = NULL;
			udArrBase::iterator j, k;
			bool bFound = false;

			// добавление и обновление
			for( j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				objNew = *j;
				bFound = false;

				for( k = arrCurrent.begin( ); k != arrCurrent.end( ); ++k ) {
					objOld = *k;
					if ( objNew->get( "id" )->toUInt( ) == objOld->get( "id" )->toUInt( ) ) {
						bFound = true;
						arrUpd.push_back( objNew );
						break;
					}
				}

				if ( !bFound ) {
					arrAdd.push_back( objNew );
				}
			}

			// удаление
			for( j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
				objOld = *j;
				bFound = false;

				for( k = arrLoaded.begin( ); k != arrLoaded.end( ); ++k ) {
					objNew = *k;
					if ( objOld->get( "id" )->toUInt( ) == objNew->get( "id" )->toUInt( ) ) {
						bFound = true;
						break;
					}
				}

				if ( !bFound ) {
					arrDel.push_back( objOld );
				}
			}

			// выполняем операции
			if ( !arrAdd.empty( ) ) {
				for( udArrBase::iterator j = arrAdd.begin( ); j != arrAdd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
					obj1->get( "app" )->ini( id );
					udStateHandlerSimple::AddObj( obj1, "ud_gcvisa", true );
				}
			}
			if ( !arrUpd.empty( ) ) {
				for( udArrBase::iterator j = arrUpd.begin( ); j != arrUpd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "app" )->ini( id ); // вдруг прилетит херня
					udStateHandlerSimple::UpdObj( obj1, "ud_gcvisa", "id", true );
					hLock->UnlockObject( obj1, GetProjectGCVisa );
				}
			}
			if ( !arrDel.empty( ) ) {
				for( udArrBase::iterator j = arrDel.begin( ); j != arrDel.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "app" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_gcvisa", true );
						hLock->UnlockObject( obj1, GetProjectGCVisa );
					}
				}
			}
		}
		ClearArrBase( arrLoaded );
		ClearArrBase( arrCurrent );
		//////////////////////////////////////////////////////////////////////////////////////////
		// Другие услуги
		ReadInstances( sock, arrLoaded, GetProjectGCService );
		LoadInstanceByForeignKey( handler, pApp1, arrCurrent, GetProjectGCService, "ud_gcservice", "app", "id" );
		if ( arrLoaded.empty( ) ) { // с клиента ничего не прилетело, вдруг их удалили
			if ( !arrCurrent.empty( ) ) { // если в прилетевших нет, то все на удаление
				for( udArrBase::iterator j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "app" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_gcservice", true );
						hLock->UnlockObject( obj1, GetProjectGCService );
					}
				}
			}
		} else if ( arrCurrent.empty( ) ) { // у объекта не было потомков
			for( udArrBase::iterator j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				obj1 = *j;
				obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
				obj1->get( "app" )->ini( id );
				udStateHandlerSimple::AddObj( obj1, "ud_gcservice", true );
			}
		} else { // оба не пустые, необходимо сравнить и составить списки add/upd/del
			udArrBase arrAdd, arrUpd, arrDel;
			udPBase objNew = NULL, objOld = NULL;
			udArrBase::iterator j, k;
			bool bFound = false;

			// добавление и обновление
			for( j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				objNew = *j;
				bFound = false;

				for( k = arrCurrent.begin( ); k != arrCurrent.end( ); ++k ) {
					objOld = *k;
					if ( objNew->get( "id" )->toUInt( ) == objOld->get( "id" )->toUInt( ) ) {
						bFound = true;
						arrUpd.push_back( objNew );
						break;
					}
				}

				if ( !bFound ) {
					arrAdd.push_back( objNew );
				}
			}

			// удаление
			for( j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
				objOld = *j;
				bFound = false;

				for( k = arrLoaded.begin( ); k != arrLoaded.end( ); ++k ) {
					objNew = *k;
					if ( objOld->get( "id" )->toUInt( ) == objNew->get( "id" )->toUInt( ) ) {
						bFound = true;
						break;
					}
				}

				if ( !bFound ) {
					arrDel.push_back( objOld );
				}
			}

			// выполняем операции
			if ( !arrAdd.empty( ) ) {
				for( udArrBase::iterator j = arrAdd.begin( ); j != arrAdd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
					obj1->get( "app" )->ini( id );
					udStateHandlerSimple::AddObj( obj1, "ud_gcservice", true );
				}
			}
			if ( !arrUpd.empty( ) ) {
				for( udArrBase::iterator j = arrUpd.begin( ); j != arrUpd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "app" )->ini( id ); // вдруг прилетит херня
					udStateHandlerSimple::UpdObj( obj1, "ud_gcservice", "id", true );
					hLock->UnlockObject( obj1, GetProjectGCService );
				}
			}
			if ( !arrDel.empty( ) ) {
				for( udArrBase::iterator j = arrDel.begin( ); j != arrDel.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "app" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_gcservice", true );
						hLock->UnlockObject( obj1, GetProjectGCService );
					}
				}
			}
		}
		ClearArrBase( arrLoaded );
		ClearArrBase( arrCurrent );
		//////////////////////////////////////////////////////////////////////////////////////////
		// Трансфер
		ReadInstances( sock, arrLoaded, GetProjectGCTransfer2 );
		LoadInstanceByForeignKey( handler, pApp1, arrCurrent, GetProjectGCTransfer2, "ud_gctransfer2", "app", "id" );
		if ( arrLoaded.empty( ) ) { // с клиента ничего не прилетело, вдруг их удалили
			if ( !arrCurrent.empty( ) ) { // если в прилетевших нет, то все на удаление
				for( udArrBase::iterator j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "app" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_gctransfer2", true );
						hLock->UnlockObject( obj1, GetProjectGCTransfer2 );
					}
				}
			}
		} else if ( arrCurrent.empty( ) ) { // у объекта не было потомков
			for( udArrBase::iterator j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				obj1 = *j;
				obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
				obj1->get( "app" )->ini( id );
				udStateHandlerSimple::AddObj( obj1, "ud_gctransfer2", true );
			}
		} else { // оба не пустые, необходимо сравнить и составить списки add/upd/del
			udArrBase arrAdd, arrUpd, arrDel;
			udPBase objNew = NULL, objOld = NULL;
			udArrBase::iterator j, k;
			bool bFound = false;

			// добавление и обновление
			for( j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
				objNew = *j;
				bFound = false;

				for( k = arrCurrent.begin( ); k != arrCurrent.end( ); ++k ) {
					objOld = *k;
					if ( objNew->get( "id" )->toUInt( ) == objOld->get( "id" )->toUInt( ) ) {
						bFound = true;
						arrUpd.push_back( objNew );
						break;
					}
				}

				if ( !bFound ) {
					arrAdd.push_back( objNew );
				}
			}

			// удаление
			for( j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
				objOld = *j;
				bFound = false;

				for( k = arrLoaded.begin( ); k != arrLoaded.end( ); ++k ) {
					objNew = *k;
					if ( objOld->get( "id" )->toUInt( ) == objNew->get( "id" )->toUInt( ) ) {
						bFound = true;
						break;
					}
				}

				if ( !bFound ) {
					arrDel.push_back( objOld );
				}
			}

			// выполняем операции
			if ( !arrAdd.empty( ) ) {
				for( udArrBase::iterator j = arrAdd.begin( ); j != arrAdd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
					obj1->get( "app" )->ini( id );
					udStateHandlerSimple::AddObj( obj1, "ud_gctransfer2", true );
				}
			}
			if ( !arrUpd.empty( ) ) {
				for( udArrBase::iterator j = arrUpd.begin( ); j != arrUpd.end( ); ++j ) {
					obj1 = *j;
					obj1->get( "app" )->ini( id ); // вдруг прилетит херня
					udStateHandlerSimple::UpdObj( obj1, "ud_gctransfer2", "id", true );
					hLock->UnlockObject( obj1, GetProjectGCTransfer2 );
				}
			}
			if ( !arrDel.empty( ) ) {
				for( udArrBase::iterator j = arrDel.begin( ); j != arrDel.end( ); ++j ) {
					obj1 = *j;
					if ( obj1->get( "app" )->toUInt( ) == id ) { // вдруг прилетит херня
						udStateHandlerSimple::DelObj( obj1, "ud_gctransfer2", true );
						hLock->UnlockObject( obj1, GetProjectGCTransfer2 );
					}
				}
			}
		}
		ClearArrBase( arrLoaded );
		ClearArrBase( arrCurrent );
		//////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( "отсылаем подтверждающий байт\n" );
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->SendByte( ( char ) 80 );
		//////////////////////////////////////////////////////////////////////////////////////////
		// СТАРЫЙ КОД
		/*bool bLoadDel = false;
		//////////////////////////////////////////////////////////////////////////////////////////
		option.SetTable( "ud_gcapp1" );
		pApp1 = GetProjectGroupContractApp1( );
		pApp1->GetConfig( config );
		pApp1->read( sock );
		pApp1->get( "tour" )->ini( obj->get( "id" )->toUInt( ) );
		pApp1->dump( log );
		__log->Write( log + "\n" );
		if ( pApp1->get( "id" )->toUInt( ) > 0 ) {
			bLoadDel = true;
			option.SetIndexAttr( "id" );
			handler->UpdUdObject( pApp1, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		} else {
			handler->AddUdObject( pApp1, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
			udUInt id = ( udUInt )(  ( udPMySql ) g_pApp->GetDatabase( ) )->GetInsertId( );
			pApp1->get( "id" )->ini( id );
		}
		option.Clear( );
		udArrBase arrAdd, arrUpd;
		//////////////////////////////////////////////////////////////////////////////////////////
		ReadInstances( sock, arr, GetProjectGCOccupancy );
		option.Clear( );
		option.SetTable( "ud_gcoccupancy" );
		option.SetIndexAttr( "id" );
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "app" )->ini( pApp1->get( "id" )->toUInt( ) );
			if ( ( *i )->get( "id" )->toUInt( ) > 0 ) {
				arrUpd.push_back( *i );
			} else {
				arrAdd.push_back( *i );
			}
		}
		if ( !arrAdd.empty( ) ) {
			handler->AddUdObject( arrAdd, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		}
		if ( !arrUpd.empty( ) ) {
			handler->UpdUdObject( arrUpd, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		}
		arrAdd.clear( );
		arrUpd.clear( );
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////
		ReadInstances( sock, arr, GetProjectGCProgram );
		option.Clear( );
		option.SetTable( "ud_gcprogram" );
		option.SetIndexAttr( "id" );
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "app" )->ini( pApp1->get( "id" )->toUInt( ) );
			if ( ( *i )->get( "id" )->toUInt( ) > 0 ) {
				arrUpd.push_back( *i );
			} else {
				arrAdd.push_back( *i );
			}
		}
		if ( !arrAdd.empty( ) ) {
			handler->AddUdObject( arrAdd, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		}
		if ( !arrUpd.empty( ) ) {
			handler->UpdUdObject( arrUpd, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		}
		arrAdd.clear( );
		arrUpd.clear( );
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////
		ReadInstances( sock, arr, GetProjectGCTransfer );
		option.Clear( );
		option.SetTable( "ud_gctransfer" );
		option.SetIndexAttr( "id" );
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "app" )->ini( pApp1->get( "id" )->toUInt( ) );
			if ( ( *i )->get( "id" )->toUInt( ) > 0 ) {
				arrUpd.push_back( *i );
			} else {
				arrAdd.push_back( *i );
			}
		}
		if ( !arrAdd.empty( ) ) {
			handler->AddUdObject( arrAdd, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		}
		if ( !arrUpd.empty( ) ) {
			handler->UpdUdObject( arrUpd, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		}
		arrAdd.clear( );
		arrUpd.clear( );
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////
		ReadInstances( sock, arr, GetProjectGCCover );
		option.Clear( );
		option.SetTable( "ud_gccover" );
		option.SetIndexAttr( "id" );
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "app" )->ini( pApp1->get( "id" )->toUInt( ) );
			if ( ( *i )->get( "id" )->toUInt( ) > 0 ) {
				arrUpd.push_back( *i );
			} else {
				arrAdd.push_back( *i );
			}
		}
		if ( !arrAdd.empty( ) ) {
			handler->AddUdObject( arrAdd, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		}
		if ( !arrUpd.empty( ) ) {
			handler->UpdUdObject( arrUpd, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		}
		arrAdd.clear( );
		arrUpd.clear( );
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////
		ReadInstances( sock, arr, GetProjectGCVisa );
		option.Clear( );
		option.SetTable( "ud_gcvisa" );
		option.SetIndexAttr( "id" );
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "app" )->ini( pApp1->get( "id" )->toUInt( ) );
			if ( ( *i )->get( "id" )->toUInt( ) > 0 ) {
				arrUpd.push_back( *i );
			} else {
				arrAdd.push_back( *i );
			}
		}
		if ( !arrAdd.empty( ) ) {
			handler->AddUdObject( arrAdd, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		}
		if ( !arrUpd.empty( ) ) {
			handler->UpdUdObject( arrUpd, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		}
		arrAdd.clear( );
		arrUpd.clear( );
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////
		ReadInstances( sock, arr, GetProjectGCService );
		option.Clear( );
		option.SetTable( "ud_gcservice" );
		option.SetIndexAttr( "id" );
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "app" )->ini( pApp1->get( "id" )->toUInt( ) );
			if ( ( *i )->get( "id" )->toUInt( ) > 0 ) {
				arrUpd.push_back( *i );
			} else {
				arrAdd.push_back( *i );
			}
		}
		if ( !arrAdd.empty( ) ) {
			handler->AddUdObject( arrAdd, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		}
		if ( !arrUpd.empty( ) ) {
			handler->UpdUdObject( arrUpd, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		}
		arrAdd.clear( );
		arrUpd.clear( );
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////
		ReadInstances( sock, arr, GetProjectGCTransfer2 );
		option.Clear( );
		option.SetTable( "ud_gctransfer2" );
		option.SetIndexAttr( "id" );
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "app" )->ini( pApp1->get( "id" )->toUInt( ) );
			if ( ( *i )->get( "id" )->toUInt( ) > 0 ) {
				arrUpd.push_back( *i );
			} else {
				arrAdd.push_back( *i );
			}
		}
		if ( !arrAdd.empty( ) ) {
			handler->AddUdObject( arrAdd, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		}
		if ( !arrUpd.empty( ) ) {
			handler->UpdUdObject( arrUpd, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
		}
		arrAdd.clear( );
		arrUpd.clear( );
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( bLoadDel ) {
			ReadInstances( sock, arr, GetProjectGCOccupancy );
			if ( !arr.empty( ) ) {
				option.Clear( );
				option.SetTable( "ud_gcoccupancy" );
				option.SetIndexAttr( "id" );
				handler->DelUdObject( arr, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( __log, err );
					err.clear( );
				}
			}
			ClearArrBase( arr );
			ReadInstances( sock, arr, GetProjectGCProgram );
			if ( !arr.empty( ) ) {
				option.Clear( );
				option.SetTable( "ud_gcprogram" );
				option.SetIndexAttr( "id" );
				handler->DelUdObject( arr, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( __log, err );
					err.clear( );
				}
			}
			ClearArrBase( arr );
			ReadInstances( sock, arr, GetProjectGCTransfer );
			if ( !arr.empty( ) ) {
				option.Clear( );
				option.SetTable( "ud_gctransfer" );
				option.SetIndexAttr( "id" );
				handler->DelUdObject( arr, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( __log, err );
					err.clear( );
				}
			}
			ClearArrBase( arr );
			ReadInstances( sock, arr, GetProjectGCCover );
			if ( !arr.empty( ) ) {
				option.Clear( );
				option.SetTable( "ud_gccover" );
				option.SetIndexAttr( "id" );
				handler->DelUdObject( arr, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( __log, err );
					err.clear( );
				}
			}
			ClearArrBase( arr );
			ReadInstances( sock, arr, GetProjectGCVisa );
			if ( !arr.empty( ) ) {
				option.Clear( );
				option.SetTable( "ud_gcvisa" );
				option.SetIndexAttr( "id" );
				handler->DelUdObject( arr, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( __log, err );
					err.clear( );
				}
			}
			ClearArrBase( arr );
			ReadInstances( sock, arr, GetProjectGCService );
			if ( !arr.empty( ) ) {
				option.Clear( );
				option.SetTable( "ud_gcservice" );
				option.SetIndexAttr( "id" );
				handler->DelUdObject( arr, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( __log, err );
					err.clear( );
				}
			}
			ClearArrBase( arr );
			ReadInstances( sock, arr, GetProjectGCTransfer2 );
			if ( !arr.empty( ) ) {
				option.Clear( );
				option.SetTable( "ud_gctransfer2" );
				option.SetIndexAttr( "id" );
				handler->DelUdObject( arr, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( __log, err );
					err.clear( );
				}
			}
			ClearArrBase( arr );
		}
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->SendByte( ( char ) 80 );*/
		//////////////////////////////////////////////////////////////////////////////////////////
		char code = 0;
		sock->ReadByte( &code );
		if ( code == 80 ) {
			udType num( ( udUInt ) 0 );
			if ( num.read( sock ) != 1 ) {
				return;
			}
			if ( num.toUInt( ) > 0 ) {
				//////////////////////////////////////////////////////////////////////////////////
				udPLog __log = g_pApp->GetLog( );
				udUInt cnt = 0, num1 = num.toUInt( );
				string log;
				wstring tmp_dir = g_pApp->GetConfig( )->get( "temporary_dir" )->toWString( );
				udInt err = 0;
				udFileTransfer objFileTransfer;
				// очищаем временную папку от левых файлов
				DirectoryClear( tmp_dir );
				objFileTransfer.SetTmpDir( tmp_dir );
				while( cnt < num1 ) {
					err = objFileTransfer.Recv( sock );
					log = udFileTransfer::ErrorExplain( err ) + "\n";
					__log->Write( log );
					if ( err != udFileTransfer::FileTransferErrorOk ) {
						break;
					}
					++cnt;
				}
				//////////////////////////////////////////////////////////////////////////////////
				// копируем файлы
				wstring dst_dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) + L"\\tour\\" + obj->get( "id" )->toWString( );
				CreateDirectory( dst_dir.data( ), NULL );
				DirectoryCopyFiles( tmp_dir, dst_dir );
				DirectoryClear( tmp_dir );
				//////////////////////////////////////////////////////////////////////////////////
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		/*code = 0;
		sock->ReadByte( &code );
		if ( code == 80 ) {
			udUInt id = 0, type = 0;
			udType num( ( udUInt ) 0 );
			if ( num.read( sock ) != 1 ) {
				return;
			}
			type = num.toUInt( );
			if ( num.read( sock ) != 1 ) {
				return;
			}
			id = num.toUInt( );
			udPBase add = new udProjectObjectLink;
			add->get( "id_src" )->ini( id );
			add->get( "id_dst" )->ini( obj->get( "id" )->toUInt( ) );
			add->get( "type_src" )->ini( type );
			add->get( "type_dst" )->ini( udProjectObjectLink::objlnkTour );
			datetime date;
			date.load( );
			add->get( "created" )->ini( date );
			udHandlerOption option;
			option.SetTable( "ud_objlnk" );
			udArrError err;
			g_pApp->GetHandler( )->AddUdObject( add, option, &err );
			if ( !err.empty( ) ) {
				WriteErrors( g_pApp->GetLog( ), err );
			}
			
			string log = "\nobject link\n";
			add->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			
			udDELETE( add );
		}*/
		}
	} // void ExtraDataRecieve

	void CHTour::ExtraDataSending( udPSocket sock, udPBase obj ) {
		//////////////////////////////////////////////////////////////////////////////////////////
		udPHandler handler = g_pApp->GetHandler( );
		udPLog __log = g_pApp->GetLog( );
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr;
		udArrError err;
		string szWhere;
		udPBase obj1 = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////
		config.Clear( );
		option.Clear( );
		obj1 = new udProjectGroupContractApp1;
		obj1->GetConfig( config );
		udDELETE( obj1 );
		option.SetTable( "ud_gcapp1" );
		option.SetClass( GetProjectGroupContractApp1 );
		option.SetWhere( config.GetAttrIndex( "tour" ) + "=" + obj->get( "id" )->toString( ) );
		handler->GetUdObject( arr, option, &err );
		option.Clear( );
		config.Clear( );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
			err.clear( );
		}
		if ( arr.empty( ) ) {
			sock->SendByte( ( char ) 0 );
		} else {
			sock->SendByte( ( char ) 80 );
			udPBase pApp1 = arr[ 0 ], obj1 = NULL;
			udBinaryData data;
			udArrBase arr1;
			//////////////////////////////////////////////////////////////////////////////////////
			pApp1->dump( data );
			//////////////////////////////////////////////////////////////////////////////////////
			obj1 = new udProjectGCOccupancy;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			option.SetTable( "ud_gcoccupancy" );
			option.SetClass( GetProjectGCOccupancy );
			option.SetWhere( config.GetAttrIndex( "app" ) + "=" + pApp1->get( "id" )->toString( ) );
			option.SetOrder( config.GetAttrIndex( "id" ) );
			handler->GetUdObject( arr1, option, &err );
			option.Clear( );
			config.Clear( );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
			WriteInstances( arr1, data );
			ClearArrBase( arr1 );
			//////////////////////////////////////////////////////////////////////////////////////
			obj1 = new udProjectGCProgram;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			option.SetTable( "ud_gcprogram" );
			option.SetClass( GetProjectGCProgram );
			option.SetWhere( config.GetAttrIndex( "app" ) + "=" + pApp1->get( "id" )->toString( ) );
			option.SetOrder( config.GetAttrIndex( "id" ) );
			handler->GetUdObject( arr1, option, &err );
			option.Clear( );
			config.Clear( );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
			WriteInstances( arr1, data );
			ClearArrBase( arr1 );
			//////////////////////////////////////////////////////////////////////////////////////
			obj1 = new udProjectGCTransfer;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			option.SetTable( "ud_gctransfer" );
			option.SetClass( GetProjectGCTransfer );
			option.SetWhere( config.GetAttrIndex( "app" ) + "=" + pApp1->get( "id" )->toString( ) );
			option.SetOrder( config.GetAttrIndex( "id" ) );
			handler->GetUdObject( arr1, option, &err );
			option.Clear( );
			config.Clear( );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
			WriteInstances( arr1, data );
			ClearArrBase( arr1 );
			//////////////////////////////////////////////////////////////////////////////////////
			obj1 = new udProjectGCCover;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			option.SetTable( "ud_gccover" );
			option.SetClass( GetProjectGCCover );
			option.SetWhere( config.GetAttrIndex( "app" ) + "=" + pApp1->get( "id" )->toString( ) );
			option.SetOrder( config.GetAttrIndex( "id" ) );
			handler->GetUdObject( arr1, option, &err );
			option.Clear( );
			config.Clear( );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
			WriteInstances( arr1, data );
			ClearArrBase( arr1 );
			//////////////////////////////////////////////////////////////////////////////////////
			obj1 = new udProjectGCVisa;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			option.SetTable( "ud_gcvisa" );
			option.SetClass( GetProjectGCVisa );
			option.SetWhere( config.GetAttrIndex( "app" ) + "=" + pApp1->get( "id" )->toString( ) );
			option.SetOrder( config.GetAttrIndex( "id" ) );
			handler->GetUdObject( arr1, option, &err );
			option.Clear( );
			config.Clear( );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
			WriteInstances( arr1, data );
			ClearArrBase( arr1 );
			//////////////////////////////////////////////////////////////////////////////////////
			obj1 = new udProjectGCService;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			option.SetTable( "ud_gcservice" );
			option.SetClass( GetProjectGCService );
			option.SetWhere( config.GetAttrIndex( "app" ) + "=" + pApp1->get( "id" )->toString( ) );
			option.SetOrder( config.GetAttrIndex( "id" ) );
			handler->GetUdObject( arr1, option, &err );
			option.Clear( );
			config.Clear( );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
			WriteInstances( arr1, data );
			ClearArrBase( arr1 );
			//////////////////////////////////////////////////////////////////////////////////////
			obj1 = new udProjectGCTransfer2;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			option.SetTable( "ud_gctransfer2" );
			option.SetClass( GetProjectGCTransfer2 );
			option.SetWhere( config.GetAttrIndex( "app" ) + "=" + pApp1->get( "id" )->toString( ) );
			option.SetOrder( config.GetAttrIndex( "id" ) );
			handler->GetUdObject( arr1, option, &err );
			option.Clear( );
			config.Clear( );
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
				err.clear( );
			}
			WriteInstances( arr1, data );
			ClearArrBase( arr1 );
			//////////////////////////////////////////////////////////////////////////////////////
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			//////////////////////////////////////////////////////////////////////////////////////
		}
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////
		// финансы
		udBinaryData data;
		LoadSumm( arr, obj, udProjectFinanceSumm::SummAtTourPrice	); // итоговая цена
		data.add( ( char ) 1 );
		WriteInstances( arr, data );
		ClearArrBase( arr );
		LoadSumm( arr, obj, udProjectFinanceSumm::SummAtTour		); // оплата за тур
		data.add( ( char ) 1 );
		WriteInstances( arr, data );
		ClearArrBase( arr );
		LoadSumm( arr, obj, udProjectFinanceSumm::SummAtTourCost	); // себестоимость
		data.add( ( char ) 1 );
		WriteInstances( arr, data );
		ClearArrBase( arr );
		LoadSumm( arr, obj, udProjectFinanceSumm::SummAtService		); // оплата услуг
		data.add( ( char ) 1 );
		WriteInstances( arr, data );
		ClearArrBase( arr );
		LoadOperations( arr, obj->get( "id" )->toUInt( ) ); // операции по оплате за тур
		data.add( ( char ) 1 );
		WriteInstances( arr, data );
		ClearArrBase( arr );
		// отсылаем данные
		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->Send( data );
		//////////////////////////////////////////////////////////////////////////////////////////
		obj1 = new udProjectAttach;
		obj1->GetConfig( config );
		udDELETE( obj1 );
		szWhere = config.GetAttrIndex( "obj" ) + "=" + obj->get( "id" )->toString( ) + " AND " +
			config.GetAttrIndex( "type" ) + "=" + toString( udProjectAttach::ATTACH_TOUR );
		option.SetTable( "ud_attach" );
		option.SetClass( GetProjectAttach );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
			sock->SendByte( ( char ) 0 );
		} else if ( arr.empty( ) ) {
			sock->SendByte( ( char ) 0 );
			g_pApp->GetLog( )->Write( "не найдено приложений\n" );
		} else {
			sock->SendByte( ( char ) 1 ); // есть файлы

			udInt err = 0;
			udFileTransfer objFileTransfer;
			udPBase obj1 = NULL;
			string log = "\n";
			wstring filename, path,
				client_dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
				L"\\tour\\" + obj->get( "id" )->toWString( );
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj1 = *i;
				obj1->dump( log );
				log += "\n";
				filename = obj1->get( "name" )->toWString( );
				path = client_dir + L"\\" + filename;
				objFileTransfer.SetFileName( filename );
				objFileTransfer.SetFilePath( path );
				err = objFileTransfer.Send( sock );
				log += udFileTransfer::ErrorExplain( err ) + "\n";
			}
			__log->Write( log );
		}
		ClearArrBase( arr );
		option.Clear( );
		config.Clear( );
		data.clear( );
		//////////////////////////////////////////////////////////////////////////////////////////
		/*obj1 = new udProjectFinanceOperation;
		datetime date;
		date.load( );
		obj1->get( "created" )->ini( date );
		//obj1->get( "acc" )->ini( 0 );
		obj1->get( "tour" )->ini( id );
		obj1->get( "type" )->ini( ( udInt ) udProjectFinanceOperation::fotPayTour );
		obj1->get( "money" )->ini( obj->get( "how" )->toInt( ) );
		obj1->get( "comment" )->ini( obj->get( "comment" )->toWString( ) );
		option.Clear( );
		option.SetTable( "ud_fop" );
		handler->AddUdObject( obj1, option );
		udDELETE( obj1 );
		// сохраняем информацию об операции (сумма)
		obj1 = new udProjectFinanceSumm;
		obj1->get( "summ" )->ini( obj->get( "summ" )->toFloat( ) );
		obj1->get( "currency" )->ini( obj->get( "currency" )->toUInt( ) );
		obj1->get( "type" )->ini( obj->get( "type" )->toInt( ) );
		obj1->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtOperation );
		obj1->get( "obj" )->ini( ( udUInt )(  ( udPMySql ) g_pApp->GetDatabase( ) )->GetInsertId( ) );
		option.SetTable( "ud_fsumm" );
		handler->AddUdObject( obj1, option );
		udDELETE( obj1 );*/
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ExtraDataSending

	bool CHTour::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_tour" );
		option.SetClass( GetProjectTour2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "direct" ) {
			szOrder = "`tour_direct_` ";
		} else if ( orderby == "employer" ) {
			szOrder = "`tour_employer_` " + szOrderType + ", `tour_employer2_` " + szOrderType;
		} else if ( orderby == "manager" ) {
			szOrder = "`tour_manager_` " + szOrderType;
		} else {
			szOrder = "`tour_" + orderby + "` ";
		}
		if ( !szOrder.empty( ) && ( orderby != "employer" ) ) {
			szOrder += szOrderType;
		}
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		option.AddLeftJoin(
			"name", "tour_direct_", "ud_direct",
			"`ud_direct`.`direct_id`=`ud_tour`.`tour_direct`", GetProjectDirect
		);
		option.AddLeftJoin(
			"fio", "tour_employer_", "ud_client",
			"`ud_client`.`client_id`=`ud_tour`.`tour_employer` AND `ud_tour`.`tour_emptype`=" + toString( udProjectTour::TOUR_EMPLOYER_CLIENT ),
			GetProjectClient
		);
		option.AddLeftJoin(
			"name", "tour_employer2_", "ud_clientur",
			"`ud_clientur`.`clientur_id`=`ud_tour`.`tour_employer` AND `ud_tour`.`tour_emptype`=" + toString( udProjectTour::TOUR_EMPLOYER_CLIENTUR ),
			GetProjectClientUr
		);
		option.AddLeftJoin(
			"fio", "tour_manager_", "ud_user",
			"`ud_user`.`user_id`=`ud_tour`.`tour_manager2`", GetProjectUser
		);
		option.SetSqlCalcFoundRows( true );
		//////////////////////////////////////////////////////////////////////////////////////////
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "order" )->toUInt( );
		if ( id > 0 ) {
			option.AddLeftJoin( "",
				"CROSS JOIN `ud_objlnk` tbllnk1 ON tbllnk1.`objlnk_id_src`=" + toString( id ) + " AND "
				"tbllnk1.`objlnk_id_dst`=`ud_tour`.`tour_id` AND "
				"tbllnk1.`objlnk_type_src`=" + toString( udProjectObjectLink::objlnkOrder ) + " AND "
				"tbllnk1.`objlnk_type_dst`=" + toString( udProjectObjectLink::objlnkTour )
			);
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "tpltour" )->toUInt( );
		if ( id > 0 ) {
			option.AddLeftJoin( "",
				"CROSS JOIN `ud_objlnk` tbllnk2 ON tbllnk2.`objlnk_id_src`=" + toString( id ) + " AND "
				"tbllnk2.`objlnk_id_dst`=`ud_tour`.`tour_id` AND "
				"tbllnk2.`objlnk_type_src`=" + toString( udProjectObjectLink::objlnkTplTour ) + " AND "
				"tbllnk2.`objlnk_type_dst`=" + toString( udProjectObjectLink::objlnkTour )
			);
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "client" )->toUInt( );
		if ( id ) {
			option.AddLeftJoin( "",
				"CROSS JOIN `ud_tourclient` tc ON tc.`tourclient_client`=" + toString( id ) + " AND tc.`tourclient_tour`=`ud_tour`.`tour_id`"
			);
			/*string szWhere = option.GetWhere( );
			szWhere += " `ud_tour`.`tour_id` IN( SELECT tc.`tour` FROM `ud_tourclient` tc LEFT JOIN `ud_client` c ON "
				"c.`id`=" + toString( id ) + " AND c.`id`=tc.`client` WHERE tc.`client`=" + toString( id ) + " )";
			option.SetWhere( szWhere );*/
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		option.SetIgnoreAttr( "direct_" );
		option.SetIgnoreAttr( "status_" );
		option.SetIgnoreAttr( "employer_" );
		option.SetIgnoreAttr( "employer2_" );
		option.SetIgnoreAttr( "manager_" );
		handler->GetUdObject( arr, option, &err );
		if ( err.empty( ) ) {
			map< udInt, wstring > mapStatus;
			mapStatus[ udProjectTour::TOUR_STATUS_NONE		] = L"";
			mapStatus[ udProjectTour::TOUR_STATUS_OPENED	] = L"Открыт";
			mapStatus[ udProjectTour::TOUR_STATUS_CLOSED	] = L"Закрыт";
			mapStatus[ udProjectTour::TOUR_STATUS_REMIND	] = L"Требует внимания";
			udInt		status	= 0;
			datetime	date;
			wstring		date1, str1, str2;
			udPBase		obj		= NULL;

			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				status = obj->get( "status" )->toInt( );
				obj->get( "status_" )->ini( mapStatus[ status ] );
				date = obj->get( "date1" )->toTime( );
				str1 = obj->get( "employer_" )->toWString( );
				str2 = obj->get( "employer2_" )->toWString( );
				if ( str1.empty( ) ) {
					obj->get( "employer_" )->ini( str2 );
				}
				obj->unset( "employer2_" );
			}
		} else {
			WriteErrors( g_pApp->GetLog( ), err );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		udPDatabase db = g_pApp->GetDatabase( );
		udArrMorph arr1;
		if ( db->GetAssoc( "SELECT FOUND_ROWS( ) cnt", arr1 ) ) {
			cnt = arr1[ 0 ]->get( "cnt" )->toLLong( );
			ClearArrMorph( arr1 );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		return true;
	} // bool CustomLoadObjects

	string CHTour::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectTour;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		wstring str = L"";
		udUInt id = 0;
		udInt status = 0;
		datetime date;
		//////////////////////////////////////////////////////////////////////////////////////////
		str = m_pObjFilter->get( "name" )->toWString( );
		if ( !str.empty( ) ) {
			string szStr = ConvertString( str, CP_UTF8 );
			if ( szStr.size( ) ) {
				udDWord	dwLen	= szStr.size( ) * 2;
				udPChar szTmp	= new udChar[ dwLen + 1 ];

				memset( szTmp, 0, dwLen + 1 );
				UdBaseEscapeString( szTmp, szStr.c_str( ), szStr.size( ) );

				szStr = szTmp;

				delete [ ] szTmp;
			}

			string add = config.GetAttrIndex( "name" ) + " LIKE '";

			int pos = 0;
			for( string::iterator i = szStr.begin( ); i != szStr.end( ); ++i, ++pos ) {
				if ( *i == '*' ) {
					add += '%';
				} else if ( *i == '%' ) {
				} else {
					add += *i;
				}
			}

			add += "'";
			arrString.push_back( add );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "direct" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "direct" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date1" )->toTime( );
		if ( date.get( ) > 0 ) {
			arrString.push_back( config.GetAttrIndex( "date1" ) + ">='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date2" )->toTime( );
		if ( date.get( ) > 0 ) {
			date.inc( 1, datetime::datetimeDay );
			arrString.push_back( config.GetAttrIndex( "date1" ) + "<='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		status = m_pObjFilter->get( "status" )->toInt( );
		if ( status > 0 ) {
			arrString.push_back( config.GetAttrIndex( "status" ) + "=" + toString( status ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "employer" )->toUInt( );
		if ( id > 0 ) {
			udInt type = m_pObjFilter->get( "emptype" )->toInt( );
			arrString.push_back( config.GetAttrIndex( "emptype" ) + "=" + toString( type ) );
			arrString.push_back( config.GetAttrIndex( "employer" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "manager" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "manager2" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !arrString.empty( ) ) {
			ret = Join( " AND ", arrString );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		//g_pApp->GetWndServer( )->UpdateText( L"ret='" + toWString( ret ) + L"'" );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ret;
	} // string GetWhereCondition

	void CHTour::SchemeCenter( udInt opId, udPSocket sock ) {
		if ( opId > opidGet ) {
			switch( opId ) {
				case opidSummAdd:			this->OpSummAdd( sock );			break;
				case opidSummBack:			this->OpSummBack( sock );			break;
				case opidSummConvert:		this->OpSummConvert( sock );		break;
				case opidSummLoad:			this->OpSummLoad( sock );			break;
				case opidCalculateFinal:	this->OpCalculateFinal( sock );		break;
				case opidAddToGroup:		this->OpAddToGroup( sock );			break;
				case opidGetPersAcc:		this->OpGetPersAcc( sock );			break;
				case opidGetServicePrice:	this->OpGetServicePrice( sock );	break;
				default:
					break;
			}
		} else {
			udStateHandlerSimple::SchemeCenter( opId, sock );
		}
	} // void SchemeCenter

	void CHTour::OpSummAdd( udPSocket sock ) {
		/**
			последовательность данных:
			ID тура
			тип окна
			сумма
		*/
		udType id( ( udUInt ) 0 );
		if ( id.read( sock ) == 1 ) { // считали ID тура
			udType type( ( udInt ) 0 );
			if ( type.read( sock ) == 1 ) {
				g_pApp->GetLog( )->Write( "считали ID тура, ID=" + id.toString( ) + "\n" );
				udPBase obj = GetProjectFinanceSummAdd( );
				if ( type.toInt( ) == udProjectFinanceSumm::SummAtService ) {
					obj->get( "partner" )->ini( ( udUInt ) 0 );
				}
				if ( obj->read( sock ) == 1 ) {
					string log = "\nсчитали сумму на добавление\n";
					obj->dump( log );
					g_pApp->GetLog( )->Write( log + "\n" );

					udArrBase arr;
					char ret = AccumulateAdd( arr, id.toUInt( ), type.toInt( ), obj );
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ret );
					udBinaryData data;
					data.add( ( char ) 1 );
					WriteInstances( arr, data );

					if ( type.toInt( ) == udProjectFinanceSumm::SummAtTour ) {
						udArrBase arrOperation;
						LoadOperations( arrOperation, id.toUInt( ) );
						data.add( ( char ) 1 );
						WriteInstances( arrOperation, data );
					}

					// отсылаем данные
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->Send( data );
					// очищаем данные
					ClearArrBase( arr );
				}
				udDELETE( obj );
			}
		}
	} // void OpSummAdd

	char CHTour::AccumulateAdd( udArrBase& arr, udUInt id, udInt type, udPBase obj ) {
		/**
			type=SummAtTourPrice
				суммы, которые наполняет менеджер и ничего более
			type=SummAtTour
				суммы, которые снимаются с лицевого счета заказчика тура
			type=SummAtTourCost
				суммы, которые наполняются автоматически, на основе набора услуг
			type=SummAtService
				суммы, которые снимаются с лицевого счета компании продавца
		*/
		char ret = 80;
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrError err;
		udBaseConfig confSumm;
		udPBase obj1 = NULL;
		string szWhere = "", log = "";
		// наполняем конфиг
		obj1 = new udProjectFinanceSumm;
		obj1->GetConfig( confSumm );
		udDELETE( obj1 );
		// наполняем условие выборки
		szWhere = confSumm.GetAttrIndex( "at" ) + "=" + toString( type ) + " AND " +
			confSumm.GetAttrIndex( "obj" ) + "=" + toString( id );
		// если сумма добавляется к партнеру, то необходимо ограничить их по партнеру
		if ( type == udProjectFinanceSumm::SummAtService ) {
			g_pApp->GetLog( )->Write( "udProjectFinanceSumm::SummAtService\n" );
			udArrBase arrSummToPartner;
			udBaseConfig config;
			obj1 = new udProjectFinanceSummToPartner;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			option.SetTable( "ud_fsummtopartner" );
			option.SetClass( GetProjectFinanceSummToPartner );
			option.SetWhere( config.GetAttrIndex( "partner_id" ) + "=" + obj->get( "partner" )->toString( ) );
			config.Clear( );
			handler->GetUdObject( arrSummToPartner, option );
			if ( arrSummToPartner.empty( ) ) {
				g_pApp->GetLog( )->Write( "udProjectFinanceSumm::SummAtService, arrSummToPartner is empty\n" );
			} else {
				g_pApp->GetLog( )->Write( "udProjectFinanceSumm::SummAtService, arrSummToPartner size=" + toString( arrSummToPartner.size( ) ) + "\n" );
				map< udUInt, udUInt > ids; // ID связей сумм и партнеров
				for( udArrBase::iterator i = arrSummToPartner.begin( ); i != arrSummToPartner.end( ); ++i ) {
					ids[ ( *i )->get( "summ_id" )->toUInt( ) ] = 1;
				}
				udArrString arrString;
				for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
					arrString.push_back( toString( i->first ) );
				}
				// все таки необходимо ограничить, так как суммы себестоимости тоже вяжутся к партнерам
				szWhere += " AND " + confSumm.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrString ) + ")";
				g_pApp->GetLog( )->Write( "where=\"" + szWhere + "\"\n" );
				option.SetTable( "ud_fsumm" );
				option.SetClass( GetProjectFinanceSumm );
				option.SetWhere( szWhere );
				handler->GetUdObject( arr, option, &err );
				option.Clear( );
			}
			ClearArrBase( arrSummToPartner );
			option.Clear( );
		} else {
			option.SetTable( "ud_fsumm" );
			option.SetClass( GetProjectFinanceSumm );
			option.SetWhere( szWhere );
			handler->GetUdObject( arr, option, &err );
			option.Clear( );
		}
		if ( err.empty( ) ) {
			// необходимо снять со счетов
			if ( type == udProjectFinanceSumm::SummAtTour ) {
				ret = this->ClientPay( id, obj );
			} else if ( type == udProjectFinanceSumm::SummAtService ) {
				ret = this->CompanyPay( id, obj );
			}
			if ( ret == 80 ) {
				if ( arr.empty( ) ) { // нет сумм
					g_pApp->GetLog( )->Write( "CHTour::AccumulateAdd, отсутствуют суммы, добавляем\n" );
					obj1 = new udProjectFinanceSumm;
					obj1->get( "summ" )->ini( obj->get( "summ" )->toFloat( ) );
					obj1->get( "currency" )->ini( obj->get( "currency" )->toUInt( ) );
					obj1->get( "type" )->ini( obj->get( "type" )->toInt( ) );
					obj1->get( "at" )->ini( type );
					obj1->get( "obj" )->ini( id );
					option.SetTable( "ud_fsumm" );
					handler->AddUdObject( obj1, option );
					udDELETE( obj1 );
					if ( type == udProjectFinanceSumm::SummAtService ) {
						udPMySql db = ( udPMySql ) g_pApp->GetDatabase( );
						obj1 = new udProjectFinanceSummToPartner;
						obj1->get( "summ_id" )->ini( ( udUInt ) db->GetInsertId( ) );
						obj1->get( "partner_id" )->ini( obj->get( "partner" )->toUInt( ) );
						option.SetTable( "ud_fsummtopartner" );
						log = "\nfsummtopartner\n";
						obj1->dump( log );
						g_pApp->GetLog( )->Write( log + "\n" );
						handler->AddUdObject( obj1, option );
					}
				} else { // есть суммы, ищем похожую
					bool found = false;
					udUInt c1 = 0, c2 = obj->get( "currency" )->toUInt( );
					udInt t1 = 0, t2 = obj->get( "type" )->toInt( );
					udFloat s1 = 0.0f, s2 = obj->get( "summ" )->toFloat( );
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						obj1 = *i;
						c1 = obj1->get( "currency" )->toUInt( );
						t1 = obj1->get( "type" )->toInt( );
						if ( ( c1 == c2 ) && ( t1 == t2 ) ) {
							s1 = obj1->get( "summ" )->toFloat( );
							obj1->get( "summ" )->ini( s1 + s2 );
							found = true;
							break;
						}
					}
					obj1 = NULL;
					if ( found ) { // если найдена, то обновляем
						g_pApp->GetLog( )->Write( "CHTour::AccumulateAdd, сумма найдена, обновляем\n" );
						option.SetTable( "ud_fsumm" );
						option.SetClass( GetProjectFinanceSumm );
						option.SetIndexAttr( "id" );
						handler->UpdUdObject( arr, option );
						option.SetIndexAttr( "" );
					} else { // если не найдена, то добавляем
						g_pApp->GetLog( )->Write( "CHTour::AccumulateAdd, сумма не найдена, добавляем\n" );
						obj1 = new udProjectFinanceSumm;
						obj1->get( "summ" )->ini( obj->get( "summ" )->toFloat( ) );
						obj1->get( "currency" )->ini( obj->get( "currency" )->toUInt( ) );
						obj1->get( "type" )->ini( obj->get( "type" )->toInt( ) );
						obj1->get( "at" )->ini( type );
						obj1->get( "obj" )->ini( id );
						option.SetTable( "ud_fsumm" );
						handler->AddUdObject( obj1, option );
						udDELETE( obj1 );
						if ( type == udProjectFinanceSumm::SummAtService ) {
							udPMySql db = ( udPMySql ) g_pApp->GetDatabase( );
							obj1 = new udProjectFinanceSummToPartner;
							obj1->get( "summ_id" )->ini( ( udUInt ) db->GetInsertId( ) );
							obj1->get( "partner_id" )->ini( obj->get( "partner" )->toUInt( ) );
							option.SetTable( "ud_fsummtopartner" );
							log = "\nfsummtopartner\n";
							obj1->dump( log );
							g_pApp->GetLog( )->Write( log + "\n" );
							handler->AddUdObject( obj1, option );
						}
					}
					log = "\nсуммы на счету, ID=" + toString( id ) + "\n";
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						( *i )->dump( log );
						log += "\n";
					}
					g_pApp->GetLog( )->Write( log + "\n" );
				}
				if ( ret == 80 ) {
					// сохраняем информацию об операции (операция)
					obj1 = new udProjectFinanceOperation;
					datetime date;
					date.load( );
					obj1->get( "created" )->ini( date );
					//obj1->get( "acc" )->ini( 0 );
					obj1->get( "tour" )->ini( id );
					obj1->get( "type" )->ini( ( udInt ) udProjectFinanceOperation::fotPayTour );
					obj1->get( "money" )->ini( obj->get( "how" )->toInt( ) );
					obj1->get( "comment" )->ini( obj->get( "comment" )->toWString( ) );
					option.Clear( );
					option.SetTable( "ud_fop" );
					handler->AddUdObject( obj1, option );
					udDELETE( obj1 );
					// сохраняем информацию об операции (сумма)
					obj1 = new udProjectFinanceSumm;
					obj1->get( "summ" )->ini( obj->get( "summ" )->toFloat( ) );
					obj1->get( "currency" )->ini( obj->get( "currency" )->toUInt( ) );
					obj1->get( "type" )->ini( obj->get( "type" )->toInt( ) );
					obj1->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtOperation );
					obj1->get( "obj" )->ini( ( udUInt )(  ( udPMySql ) g_pApp->GetDatabase( ) )->GetInsertId( ) );
					option.SetTable( "ud_fsumm" );
					handler->AddUdObject( obj1, option );
					udDELETE( obj1 );
				}
			} else {
				g_pApp->GetLog( )->Write(
					"CHTour::AccumulateAdd, возникли ошибки при оплате, "
					"type=" + toString( type ) + ", code=" + toString( ( udInt ) ret ) + "\n"
				);
			}
			// подгружаем итоговые данные
			ClearArrBase( arr );
			option.Clear( );
			LoadSumm( arr, id, type );
		} else {
			g_pApp->GetLog( )->Write( "возникли ошибки при выборке данных\n" );
			WriteErrors( g_pApp->GetLog( ), err );
		}
		return ret;
	} // char AccumulateAdd

	char CHTour::ClientPay( udUInt iTourId, udPBase obj ) {
		char ret = 80;
		udPBase pAcc = this->GetPersAcc( iTourId, udProjectFinanceSumm::SummAtTour );
		if ( pAcc ) { // снимаем деньги со счета
			udPHandler handler = g_pApp->GetHandler( );
			udHandlerOption option;
			udArrError err;
			udBaseConfig confSumm;
			udPBase obj1 = NULL;
			udArrBase arr;
			string szWhere = "", log = "";
			// наполняем конфиг
			obj1 = new udProjectFinanceSumm;
			obj1->GetConfig( confSumm );
			udDELETE( obj1 );
			// наполняем условие выборки
			szWhere = confSumm.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtAcc ) + " AND " +
				confSumm.GetAttrIndex( "obj" ) + "=" + pAcc->get( "id" )->toString( );
			confSumm.Clear( );
			//
			option.SetTable( "ud_fsumm" );
			option.SetClass( GetProjectFinanceSumm );
			option.SetWhere( szWhere );
			handler->GetUdObject( arr, option, &err );
			option.SetWhere( "" );
			if ( err.empty( ) ) {
				if ( arr.empty( ) ) { // нет сумм
					ret = 0;
				} else { // есть суммы, ищем похожую
					udPBase pFound = NULL;
					udUInt c1 = 0, c2 = obj->get( "currency" )->toUInt( );
					udInt t1 = 0, t2 = obj->get( "type" )->toInt( );
					udFloat s1 = 0.0f, s2 = obj->get( "summ" )->toFloat( );
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						obj1 = *i;
						c1 = obj1->get( "currency" )->toUInt( );
						t1 = obj1->get( "type" )->toInt( );
						if ( ( c1 == c2 ) && ( t1 == t2 ) ) {
							pFound = obj1;
							break;
						}
					}
					obj1 = NULL;
					if ( pFound ) { // если найдена, то обновляем
						s1 = pFound->get( "summ" )->toFloat( );
						if ( s1 < s2 ) { // не достаточно средств для снятия
							ret = 0;
						} else {
							pFound->get( "summ" )->ini( s1 - s2 );
							option.SetIndexAttr( "id" );
							handler->UpdUdObject( arr, option );
							option.SetIndexAttr( "" );
						}
					} else { // сумма не найдена, снять нельзя
						ret = 0;
					}
					log = "\nсуммы на счету, ID=" + pAcc->get( "id" )->toString( ) + "\n";
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						( *i )->dump( log );
						log += "\n";
					}
					g_pApp->GetLog( )->Write( log + "\n" );
				}
			} else {
				g_pApp->GetLog( )->Write( "возникли ошибки при выборке данных\n" );
				WriteErrors( g_pApp->GetLog( ), err );
			}
		} else {
			ret = 1;
		}
		return ret;
	} // char ClientPay

	char CHTour::CompanyPay( udUInt iTourId, udPBase obj ) {
		char ret = 80;
		udPBase pAcc = this->GetPersAcc( iTourId, udProjectFinanceSumm::SummAtService, obj->get( "partner" )->toUInt( ) );
		if ( pAcc ) { // снимаем деньги со счета
			udPHandler handler = g_pApp->GetHandler( );
			udHandlerOption option;
			udArrError err;
			udBaseConfig confSumm;
			udPBase obj1 = NULL;
			udArrBase arr;
			string szWhere = "", log = "";
			// наполняем конфиг
			obj1 = new udProjectFinanceSumm;
			obj1->GetConfig( confSumm );
			udDELETE( obj1 );
			// наполняем условие выборки
			szWhere = confSumm.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtAcc ) + " AND " +
				confSumm.GetAttrIndex( "obj" ) + "=" + pAcc->get( "id" )->toString( );
			confSumm.Clear( );
			//
			option.SetTable( "ud_fsumm" );
			option.SetClass( GetProjectFinanceSumm );
			option.SetWhere( szWhere );
			handler->GetUdObject( arr, option, &err );
			option.SetWhere( "" );
			if ( err.empty( ) ) {
				if ( arr.empty( ) ) { // нет сумм
					g_pApp->GetLog( )->Write( "CHTour::CompanyPay, нет сумм\n" );
					ret = 0;
				} else { // есть суммы, ищем похожую
					udPBase pFound = NULL;
					udUInt c1 = 0, c2 = obj->get( "currency" )->toUInt( );
					udInt t1 = 0, t2 = obj->get( "type" )->toInt( );
					udFloat s1 = 0.0f, s2 = obj->get( "summ" )->toFloat( );
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						obj1 = *i;
						c1 = obj1->get( "currency" )->toUInt( );
						t1 = obj1->get( "type" )->toInt( );
						if ( ( c1 == c2 ) && ( t1 == t2 ) ) {
							pFound = obj1;
							break;
						}
					}
					obj1 = NULL;
					if ( pFound ) { // если найдена, то обновляем
						s1 = pFound->get( "summ" )->toFloat( );
						if ( s1 < s2 ) { // не достаточно средств для снятия
							g_pApp->GetLog( )->Write( "CHTour::CompanyPay, не достаточно средств для снятия\n" );
							ret = 0;
						} else {
							pFound->get( "summ" )->ini( s1 - s2 );
							option.SetIndexAttr( "id" );
							handler->UpdUdObject( arr, option );
							option.SetIndexAttr( "" );
						}
					} else { // сумма не найдена, снять нельзя
						g_pApp->GetLog( )->Write( "CHTour::CompanyPay, сумма не найдена\n" );
						ret = 0;
					}
					log = "\nсуммы на счету, ID=" + pAcc->get( "id" )->toString( ) + "\n";
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						( *i )->dump( log );
						log += "\n";
					}
					g_pApp->GetLog( )->Write( log + "\n" );
				}
			} else {
				g_pApp->GetLog( )->Write( "возникли ошибки при выборке данных\n" );
				WriteErrors( g_pApp->GetLog( ), err );
			}
		} else {
			g_pApp->GetLog( )->Write( "CHTour::CompanyPay, не найден лицевой счет партнера\n" );
			ret = 1;
		}
		return ret;
	} // char CompanyPay

	void CHTour::OpSummBack( udPSocket sock ) {
		udType id( ( udUInt ) 0 );
		if ( id.read( sock ) == 1 ) { // считали ID лицевого счета
			udType type( ( udInt ) 0 );
			if ( type.read( sock ) == 1 ) {
				udPBase obj = GetProjectFinanceSummBack( );
				if ( type.toInt( ) == udProjectFinanceSumm::SummAtService ) {
					obj->get( "partner" )->ini( ( udUInt ) 0 );
				}
				if ( obj->read( sock ) == 1 ) {
					string log = "\nсчитали сумму на снятие\n";
					obj->dump( log );
					g_pApp->GetLog( )->Write( log + "\n" );

					udArrBase arr;
					char ret = this->AccumulateBack( arr, id.toUInt( ), type.toInt( ), obj );
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ret );
					udBinaryData data;
					data.add( ( char ) 1 );
					WriteInstances( arr, data );

					if ( type.toInt( ) == udProjectFinanceSumm::SummAtTour ) {
						udArrBase arrOperation;
						LoadOperations( arrOperation, id.toUInt( ) );
						data.add( ( char ) 1 );
						WriteInstances( arrOperation, data );
					}

					// отсылаем данные
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->Send( data );
					// очищаем данные
					ClearArrBase( arr );
				}
				udDELETE( obj );
			}
		}
	} // void OpSummBack

	char CHTour::AccumulateBack( udArrBase& arr, udUInt id, udInt type, udPBase obj ) {
		char ret = 80;
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrError err;
		udBaseConfig confSumm;
		udPBase obj1 = NULL;
		string szWhere = "", log = "";
		// наполняем конфиг
		obj1 = new udProjectFinanceSumm;
		obj1->GetConfig( confSumm );
		udDELETE( obj1 );
		// наполняем условие выборки
		szWhere = confSumm.GetAttrIndex( "at" ) + "=" + toString( type ) + " AND " +
			confSumm.GetAttrIndex( "obj" ) + "=" + toString( id );
		// если сумма добавляется к партнеру, то необходимо ограничить их по партнеру
		if ( type == udProjectFinanceSumm::SummAtService ) {
			g_pApp->GetLog( )->Write( "udProjectFinanceSumm::SummAtService\n" );
			udArrBase arrSummToPartner;
			udBaseConfig config;
			obj1 = new udProjectFinanceSummToPartner;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			option.SetTable( "ud_fsummtopartner" );
			option.SetClass( GetProjectFinanceSummToPartner );
			option.SetWhere( config.GetAttrIndex( "partner_id" ) + "=" + obj->get( "partner" )->toString( ) );
			config.Clear( );
			handler->GetUdObject( arrSummToPartner, option );
			if ( !arrSummToPartner.empty( ) ) {
				map< udUInt, udUInt > ids; // ID связей сумм и партнеров
				for( udArrBase::iterator i = arrSummToPartner.begin( ); i != arrSummToPartner.end( ); ++i ) {
					ids[ ( *i )->get( "summ_id" )->toUInt( ) ] = 1;
				}
				udArrString arrString;
				for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
					arrString.push_back( toString( i->first ) );
				}
				szWhere += " AND " + confSumm.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrString ) + ")";
				g_pApp->GetLog( )->Write( "where=\"" + szWhere + "\"\n" );
				option.SetTable( "ud_fsumm" );
				option.SetClass( GetProjectFinanceSumm );
				option.SetWhere( szWhere );
				handler->GetUdObject( arr, option, &err );
				option.Clear( );
			}
			ClearArrBase( arrSummToPartner );
			option.Clear( );
		} else {
			option.SetTable( "ud_fsumm" );
			option.SetClass( GetProjectFinanceSumm );
			option.SetWhere( szWhere );
			handler->GetUdObject( arr, option, &err );
			option.Clear( );
		}
		if ( err.empty( ) ) {
			if ( arr.empty( ) ) { // не откуда списывать
				g_pApp->GetLog( )->Write( "CHTour::AccumulateBack, не откуда списывать\n" );
				ret = 0;
			} else {
				// необходимо зачислить на счета
				if ( type == udProjectFinanceSumm::SummAtTour ) {
					ret = this->ClientRet( id, obj );
				} else if ( type == udProjectFinanceSumm::SummAtService ) {
					ret = this->CompanyRet( id, obj );
				}
				if ( ret == 80 ) {
					udPBase pFound = NULL;
					udUInt c1 = 0, c2 = obj->get( "currency" )->toUInt( );
					udInt t1 = 0, t2 = obj->get( "type" )->toInt( );
					udFloat s1 = 0.0f, s2 = obj->get( "summ" )->toFloat( );
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						obj1 = *i;
						c1 = obj1->get( "currency" )->toUInt( );
						t1 = obj1->get( "type" )->toInt( );
						if ( ( c1 == c2 ) && ( t1 == t2 ) ) {
							pFound = obj1;
							break;
						}
					}
					obj1 = NULL;
					if ( pFound ) { // если найдена, то обновляем
						s1 = pFound->get( "summ" )->toFloat( );
						if ( s1 < s2 ) { // не достаточно средств для снятия
							g_pApp->GetLog( )->Write( "CHTour::AccumulateBack, не достаточно средств для снятия\n" );
							ret = 0;
						} else {
							g_pApp->GetLog( )->Write( "CHTour::AccumulateBack, достаточно средств для снятия\n" );
							pFound->get( "summ" )->ini( s1 - s2 );
							option.SetTable( "ud_fsumm" );
							option.SetClass( GetProjectFinanceSumm );
							option.SetIndexAttr( "id" );
							handler->UpdUdObject( arr, option );
							option.Clear( );
						}
					} else { // сумма не найдена, снять нельзя
						g_pApp->GetLog( )->Write( "CHTour::AccumulateBack, сумма не найдена, снять нельзя\n" );
						ret = 0;
					}
					log = "\nсуммы на счету, ID=" + toString( id ) + "\n";
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						( *i )->dump( log );
						log += "\n";
					}
					g_pApp->GetLog( )->Write( log + "\n" );
					if ( ret == 80 ) { // если все нормально
						// сохраняем информацию об операции (операция)
						obj1 = new udProjectFinanceOperation;
						datetime date;
						date.load( );
						obj1->get( "created" )->ini( date );
						//obj->get( "acc" )->ini( id );
						obj1->get( "tour" )->ini( id );
						obj1->get( "type" )->ini( ( udInt ) udProjectFinanceOperation::fotBackTour );
						obj1->get( "money" )->ini( obj->get( "how" )->toInt( ) );
						obj1->get( "comment" )->ini( obj->get( "comment" )->toWString( ) );
						option.Clear( );
						option.SetTable( "ud_fop" );
						handler->AddUdObject( obj1, option );
						udDELETE( obj1 );
						// сохраняем информацию об операции (сумма)
						obj1 = new udProjectFinanceSumm;
						obj1->get( "summ" )->ini( obj->get( "summ" )->toFloat( ) );
						obj1->get( "currency" )->ini( obj->get( "currency" )->toUInt( ) );
						obj1->get( "type" )->ini( obj->get( "type" )->toInt( ) );
						obj1->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtOperation );
						obj1->get( "obj" )->ini( ( udUInt )(  ( udPMySql ) g_pApp->GetDatabase( ) )->GetInsertId( ) );
						option.SetTable( "ud_fsumm" );
						handler->AddUdObject( obj1, option );
						option.Clear( );
						udDELETE( obj1 );
						// зачисляем сумму обратно на счет
					}
				} else {
					g_pApp->GetLog( )->Write(
						"CHTour::AccumulateBack, возникли ошибки при снятии, "
						"type=" + toString( type ) + ", code=" + toString( ( udInt ) ret ) + "\n"
					);
				}
				// подгружаем итоговые данные
				ClearArrBase( arr );
			}
			// подгружаем итоговые данные
			ClearArrBase( arr );
			option.Clear( );
			LoadSumm( arr, id, type );
		} else {
			g_pApp->GetLog( )->Write( "возникли ошибки при выборке данных\n" );
			WriteErrors( g_pApp->GetLog( ), err );
		}
		return ret;
	} // char AccumulateBack

	char CHTour::ClientRet( udUInt iTourId, udPBase obj ) {
		char ret = 80;
		udPBase pAcc = this->GetPersAcc( iTourId, udProjectFinanceSumm::SummAtTour );
		if ( pAcc ) {
			udPHandler handler = g_pApp->GetHandler( );
			udHandlerOption option;
			udArrError err;
			udBaseConfig confSumm;
			udPBase obj1 = NULL;
			string szWhere = "", log = "";
			udArrBase arr;
			// наполняем конфиг
			obj1 = new udProjectFinanceSumm;
			obj1->GetConfig( confSumm );
			udDELETE( obj1 );
			// наполняем условие выборки
			szWhere = confSumm.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtAcc ) + " AND " +
				confSumm.GetAttrIndex( "obj" ) + "=" + pAcc->get( "id" )->toString( );
			confSumm.Clear( );
			//
			option.SetTable( "ud_fsumm" );
			option.SetClass( GetProjectFinanceSumm );
			option.SetWhere( szWhere );
			handler->GetUdObject( arr, option, &err );
			option.SetWhere( "" );
			if ( err.empty( ) ) {
				if ( arr.empty( ) ) { // нет сумм
					obj1 = new udProjectFinanceSumm;
					obj1->get( "summ" )->ini( obj->get( "summ" )->toFloat( ) );
					obj1->get( "currency" )->ini( obj->get( "currency" )->toUInt( ) );
					obj1->get( "type" )->ini( obj->get( "type" )->toInt( ) );
					obj1->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtAcc );
					obj1->get( "obj" )->ini( pAcc->get( "id" )->toUInt( ) );
					handler->AddUdObject( obj1, option, &err );
					if ( !err.empty( ) ) {
						WriteErrors( g_pApp->GetLog( ), err );
						err.clear( );
					}
					udDELETE( obj1 );
				} else { // есть суммы, ищем похожую
					bool found = false;
					udUInt c1 = 0, c2 = obj->get( "currency" )->toUInt( );
					udInt t1 = 0, t2 = obj->get( "type" )->toInt( );
					udFloat s1 = 0.0f, s2 = obj->get( "summ" )->toFloat( );
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						obj1 = *i;
						c1 = obj1->get( "currency" )->toUInt( );
						t1 = obj1->get( "type" )->toInt( );
						if ( ( c1 == c2 ) && ( t1 == t2 ) ) {
							s1 = obj1->get( "summ" )->toFloat( );
							obj1->get( "summ" )->ini( s1 + s2 );
							found = true;
							break;
						}
					}
					obj1 = NULL;
					if ( found ) { // если найдена, то обновляем
						option.SetIndexAttr( "id" );
						handler->UpdUdObject( arr, option );
						option.SetIndexAttr( "" );
					} else { // если не найдена, то добавляем
						obj1 = new udProjectFinanceSumm;
						obj1->get( "summ" )->ini( obj->get( "summ" )->toFloat( ) );
						obj1->get( "currency" )->ini( obj->get( "currency" )->toUInt( ) );
						obj1->get( "type" )->ini( obj->get( "type" )->toInt( ) );
						obj1->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtAcc );
						obj1->get( "obj" )->ini( pAcc->get( "id" )->toUInt( ) );
						handler->AddUdObject( obj1, option );
						udDELETE( obj1 );
					}
					log = "\nсуммы на счету, ID=" + pAcc->get( "id" )->toString( ) + "\n";
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						( *i )->dump( log );
						log += "\n";
					}
					g_pApp->GetLog( )->Write( log + "\n" );
				}
			} else {
				g_pApp->GetLog( )->Write( "возникли ошибки при выборке данных\n" );
				WriteErrors( g_pApp->GetLog( ), err );
			}
		} else {
			ret = 1;
		}
		return ret;
	} // char ClientRet

	char CHTour::CompanyRet( udUInt iTourId, udPBase obj ) {
		char ret = 80;
		udPBase pAcc = this->GetPersAcc( iTourId, udProjectFinanceSumm::SummAtService, obj->get( "partner" )->toUInt( ) );
		if ( pAcc ) {
			udPHandler handler = g_pApp->GetHandler( );
			udHandlerOption option;
			udArrError err;
			udBaseConfig confSumm;
			udPBase obj1 = NULL;
			string szWhere = "", log = "";
			udArrBase arr;
			//
			log = "\npersacc:\n";
			pAcc->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			// наполняем конфиг
			obj1 = new udProjectFinanceSumm;
			obj1->GetConfig( confSumm );
			udDELETE( obj1 );
			// наполняем условие выборки
			szWhere = confSumm.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtAcc ) + " AND " +
				confSumm.GetAttrIndex( "obj" ) + "=" + pAcc->get( "id" )->toString( );
			confSumm.Clear( );
			//
			option.SetTable( "ud_fsumm" );
			option.SetClass( GetProjectFinanceSumm );
			option.SetWhere( szWhere );
			handler->GetUdObject( arr, option, &err );
			option.Clear( );
			if ( err.empty( ) ) {
				if ( arr.empty( ) ) { // нет сумм
					obj1 = new udProjectFinanceSumm;
					obj1->get( "summ" )->ini( obj->get( "summ" )->toFloat( ) );
					obj1->get( "currency" )->ini( obj->get( "currency" )->toUInt( ) );
					obj1->get( "type" )->ini( obj->get( "type" )->toInt( ) );
					obj1->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtAcc );
					obj1->get( "obj" )->ini( pAcc->get( "id" )->toUInt( ) );
					option.SetTable( "ud_fsumm" );
					handler->AddUdObject( obj1, option, &err );
					if ( !err.empty( ) ) {
						WriteErrors( g_pApp->GetLog( ), err );
						err.clear( );
					}
					udDELETE( obj1 );
				} else { // есть суммы, ищем похожую
					bool found = false;
					udUInt c1 = 0, c2 = obj->get( "currency" )->toUInt( );
					udInt t1 = 0, t2 = obj->get( "type" )->toInt( );
					udFloat s1 = 0.0f, s2 = obj->get( "summ" )->toFloat( );
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						obj1 = *i;
						c1 = obj1->get( "currency" )->toUInt( );
						t1 = obj1->get( "type" )->toInt( );
						if ( ( c1 == c2 ) && ( t1 == t2 ) ) {
							s1 = obj1->get( "summ" )->toFloat( );
							obj1->get( "summ" )->ini( s1 + s2 );
							found = true;
							break;
						}
					}
					obj1 = NULL;
					if ( found ) { // если найдена, то обновляем
						option.SetTable( "ud_fsumm" );
						option.SetIndexAttr( "id" );
						handler->UpdUdObject( arr, option );
						option.SetIndexAttr( "" );
					} else { // если не найдена, то добавляем
						obj1 = new udProjectFinanceSumm;
						obj1->get( "summ" )->ini( obj->get( "summ" )->toFloat( ) );
						obj1->get( "currency" )->ini( obj->get( "currency" )->toUInt( ) );
						obj1->get( "type" )->ini( obj->get( "type" )->toInt( ) );
						obj1->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtAcc );
						obj1->get( "obj" )->ini( pAcc->get( "id" )->toUInt( ) );
						option.SetTable( "ud_fsumm" );
						handler->AddUdObject( obj1, option );
						udDELETE( obj1 );
					}
					log = "\nсуммы на счету, ID=" + pAcc->get( "id" )->toString( ) + "\n";
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						( *i )->dump( log );
						log += "\n";
					}
					g_pApp->GetLog( )->Write( log + "\n" );
				}
			} else {
				g_pApp->GetLog( )->Write( "возникли ошибки при выборке данных\n" );
				WriteErrors( g_pApp->GetLog( ), err );
			}
		} else {
			ret = 1;
		}
		return ret;
	} // char CompanyRet

	void CHTour::OpSummConvert( udPSocket sock ) {
		udType id( ( udUInt ) 0 );
		if ( id.read( sock ) == 1 ) { // считали ID тура
			udType type( ( udInt ) 0 );
			if ( type.read( sock ) == 1 ) {
				udPBase obj = GetProjectFinanceSummConvert( );
				if ( type.toInt( ) == udProjectFinanceSumm::SummAtService ) {
					obj->get( "partner" )->ini( ( udUInt ) 0 );
				}
				if ( obj->read( sock ) == 1 ) {
					string log = "\nсчитали сумму на конвертацию\n";
					obj->dump( log );
					g_pApp->GetLog( )->Write( log + "\n" );

					udArrBase arr;
					char ret = this->AccumulateConvert( arr, id.toUInt( ), type.toInt( ), obj );
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ret );
					udBinaryData data;
					data.add( ( char ) 1 );
					WriteInstances( arr, data );

					if ( type.toInt( ) == udProjectFinanceSumm::SummAtTour ) {
						udArrBase arrOperation;
						LoadOperations( arrOperation, id.toUInt( ) );
						data.add( ( char ) 1 );
						WriteInstances( arrOperation, data );
					}

					// отсылаем данные
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->Send( data );
					// очищаем данные
					ClearArrBase( arr );
				}
				udDELETE( obj );
			}
		}
	} // void OpSummConvert

	char CHTour::AccumulateConvert( udArrBase& arr, udUInt id, udInt type, udPBase obj ) {
		char ret = 80;
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrError err;
		udBaseConfig confSumm;
		udPBase obj1 = NULL;
		string szWhere = "", log = "";
		// наполняем конфиг
		obj1 = new udProjectFinanceSumm;
		obj1->GetConfig( confSumm );
		udDELETE( obj1 );
		// наполняем условие выборки
		szWhere = confSumm.GetAttrIndex( "at" ) + "=" + toString( type ) + " AND " +
			confSumm.GetAttrIndex( "obj" ) + "=" + toString( id );
		if ( type == udProjectFinanceSumm::SummAtService ) {
			g_pApp->GetLog( )->Write( "udProjectFinanceSumm::SummAtService\n" );
			udArrBase arrSummToPartner;
			udBaseConfig config;
			obj1 = new udProjectFinanceSummToPartner;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			option.SetTable( "ud_fsummtopartner" );
			option.SetClass( GetProjectFinanceSummToPartner );
			option.SetWhere( config.GetAttrIndex( "partner_id" ) + "=" + obj->get( "partner" )->toString( ) );
			config.Clear( );
			handler->GetUdObject( arrSummToPartner, option );
			if ( !arrSummToPartner.empty( ) ) {
				map< udUInt, udUInt > ids; // ID связей сумм и партнеров
				for( udArrBase::iterator i = arrSummToPartner.begin( ); i != arrSummToPartner.end( ); ++i ) {
					ids[ ( *i )->get( "summ_id" )->toUInt( ) ] = 1;
				}
				udArrString arrString;
				for( map< udUInt, udUInt >::iterator i = ids.begin( ); i != ids.end( ); ++i ) {
					arrString.push_back( toString( i->first ) );
				}
				szWhere += " AND " + confSumm.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrString ) + ")";
				g_pApp->GetLog( )->Write( "where=\"" + szWhere + "\"\n" );
				option.SetTable( "ud_fsumm" );
				option.SetClass( GetProjectFinanceSumm );
				option.SetWhere( szWhere );
				handler->GetUdObject( arr, option, &err );
				option.Clear( );
			}
			ClearArrBase( arrSummToPartner );
			option.Clear( );
		} else {
			option.SetTable( "ud_fsumm" );
			option.SetClass( GetProjectFinanceSumm );
			option.SetWhere( szWhere );
			handler->GetUdObject( arr, option, &err );
			option.Clear( );
		}
		if ( err.empty( ) ) {
			udFloat fConvResult = 0.0f; // сумма результат конвертации
			// ID валют
			udUInt	c	= 0,
					c1	= obj->get( "currency1" )->toUInt( ),
					c2	= obj->get( "currency2" )->toUInt( );
			if ( c2 == 0 ) {
				c2 = c1;
			}
			// типы
			udInt	t	= 0,
					t1	= obj->get( "type1" )->toInt( ),
					t2	= obj->get( "type2" )->toInt( );
			if ( t2 == 0 ) {
				t2 = t1;
			}
			log = "\nconvert:\n"
				"c1=" + toString( c1 ) + ", c2=" + toString( c2 ) + ", "
				"obj->c1=" + obj->get( "currency1" )->toString( ) + ", "
				"obj->c2=" + obj->get( "currency2" )->toString( ) + "\n";
			g_pApp->GetLog( )->Write( log );
			if ( arr.empty( ) ) { // нет сумм
				ret = 0;
			} else { // есть суммы, ищем похожую
				udPBase pFrom = NULL, pTo = NULL;
				// ищем сумму из
				for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					obj1 = *i;
					c = obj1->get( "currency" )->toUInt( );
					t = obj1->get( "type" )->toInt( );
					if ( ( c == c1 ) && ( t == t1 ) ) {
						pFrom = obj1;
						break;
					}
				}
				// исходная сумма обязательна при любых раскладах
				if ( pFrom ) { // если найдена сумма источник
					// ищем сумму в
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						obj1 = *i;
						c = obj1->get( "currency" )->toUInt( );
						t = obj1->get( "type" )->toInt( );
						if ( ( c == c2 ) && ( t == t2 ) ) {
							pTo = obj1;
							break;
						}
					}
					/**
						1. обязательно должна быть сумма "из".
						2. сумма "из" должна быть больше, чем требуется для конвертации.
						3. если не найдена сумма "в", то создаем новую.
						4. если найдена сумма "в", то плюсуем.
						5. ищем валютную пару для двух валют.
					*/
					obj1 = NULL;
					udFloat s1 = pFrom->get( "summ" )->toFloat( ), s2 = obj->get( "summ" )->toFloat( );
					if ( s1 < s2 ) { // не достаточно средств для конвертации
						ret = 0;
					} else {
						if ( c1 == c2 ) { // при равенстве валют размен 1 к 1
							udFloat fNew = s2;
							fConvResult = fNew;
							// минусуем сумму источник
							pFrom->get( "summ" )->ini( s1 - s2 );
							if ( pTo ) { // плюсуем сумму результат
								fNew += pTo->get( "summ" )->toFloat( );
								pTo->get( "summ" )->ini( fNew );
							} else { // создаем новую сумму
								obj1 = new udProjectFinanceSumm;
								obj1->get( "summ" )->ini( fNew );
								obj1->get( "currency" )->ini( c2 );
								obj1->get( "type" )->ini( t2 );
								obj1->get( "at" )->ini( type );
								obj1->get( "obj" )->ini( id );
								option.SetTable( "ud_fsumm" );
								handler->AddUdObject( obj1, option );
								udDELETE( obj1 );
								if ( type == udProjectFinanceSumm::SummAtService ) {
									udPMySql db = ( udPMySql ) g_pApp->GetDatabase( );
									obj1 = new udProjectFinanceSummToPartner;
									obj1->get( "summ_id" )->ini( ( udUInt ) db->GetInsertId( ) );
									obj1->get( "partner_id" )->ini( obj->get( "partner" )->toUInt( ) );
									option.SetTable( "ud_fsummtopartner" );
									log = "\nfsummtopartner\n";
									obj1->dump( log );
									g_pApp->GetLog( )->Write( log + "\n" );
									handler->AddUdObject( obj1, option );
								}
							}
							// обновляем суммы
							option.SetTable( "ud_fsumm" );
							option.SetIndexAttr( "id" );
							handler->UpdUdObject( arr, option );
							option.SetIndexAttr( "" );
						} else {
							// ищем валютную пару для конвертации
							udPBase pPair = this->FindCurrencyPair( c1, c2 );
							if ( pPair ) { // валютная пара есть
								log = "\nнайденная валютная пара\n";
								pPair->dump( log );
								g_pApp->GetLog( )->Write( log + "\n" );
								udFloat base = pPair->get( "internal" )->toFloat( );
								if ( base == 0.0f ) {
									base = pPair->get( "course" )->toFloat( );
								}
								if ( base > 0.0f ) {
									udFloat fNew = s2 / base;
									fConvResult = fNew;
									// минусуем сумму источник
									pFrom->get( "summ" )->ini( s1 - s2 );
									if ( pTo ) { // плюсуем сумму результат
										fNew += pTo->get( "summ" )->toFloat( );
										pTo->get( "summ" )->ini( fNew );
									} else { // создаем новую сумму
										obj1 = new udProjectFinanceSumm;
										obj1->get( "summ" )->ini( fNew );
										obj1->get( "currency" )->ini( c2 );
										obj1->get( "type" )->ini( t2 );
										obj1->get( "at" )->ini( type );
										obj1->get( "obj" )->ini( id );
										option.SetTable( "ud_fsumm" );
										handler->AddUdObject( obj1, option );
										udDELETE( obj1 );
										if ( type == udProjectFinanceSumm::SummAtService ) {
											udPMySql db = ( udPMySql ) g_pApp->GetDatabase( );
											obj1 = new udProjectFinanceSummToPartner;
											obj1->get( "summ_id" )->ini( ( udUInt ) db->GetInsertId( ) );
											obj1->get( "partner_id" )->ini( obj->get( "partner" )->toUInt( ) );
											option.SetTable( "ud_fsummtopartner" );
											log = "\nfsummtopartner\n";
											obj1->dump( log );
											g_pApp->GetLog( )->Write( log + "\n" );
											handler->AddUdObject( obj1, option );
										}
									}
									// обновляем суммы
									option.SetTable( "ud_fsumm" );
									option.SetIndexAttr( "id" );
									handler->UpdUdObject( arr, option );
									option.SetIndexAttr( "" );
								} else { // не установлен курс
									ret = 2;
								}
								udDELETE( pPair );
							} else { // отсутствует валютная пара для конвертации
								ret = 1;
							}
						}
					}
					log = "\nсуммы на счету, ID=" + toString( id ) + "\n";
					for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
						( *i )->dump( log );
						log += "\n";
					}
					g_pApp->GetLog( )->Write( log + "\n" );
				} else { // нет суммы - нет средств
					ret = 0;
				}
			}
			if ( ret == 80 ) {
				// сохраняем информацию об операции (операция)
				obj1 = new udProjectFinanceOperation;
				datetime date;
				date.load( );
				obj1->get( "created" )->ini( date );
				//obj1->get( "acc" )->ini( id );
				obj1->get( "tour" )->ini( id );
				obj1->get( "type" )->ini( ( udInt ) udProjectFinanceOperation::fotConv );
				obj1->get( "comment" )->ini( obj->get( "comment" )->toWString( ) );
				option.Clear( );
				option.SetTable( "ud_fop" );
				handler->AddUdObject( obj1, option );
				udDELETE( obj1 );
				// id операции
				udUInt iOperationId = ( udUInt )(  ( udPMySql ) g_pApp->GetDatabase( ) )->GetInsertId( );
				// сохраняем информацию об операции (сумма из)
				obj1 = new udProjectFinanceSumm;
				obj1->get( "summ" )->ini( obj->get( "summ" )->toFloat( ) );
				obj1->get( "currency" )->ini( c1 );
				obj1->get( "type" )->ini( t1 );
				obj1->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtOperation );
				obj1->get( "obj" )->ini( iOperationId );
				option.SetTable( "ud_fsumm" );
				handler->AddUdObject( obj1, option );
				option.Clear( );
				udDELETE( obj1 );
				// сохраняем информацию об операции (сумма в)
				obj1 = new udProjectFinanceSumm;
				obj1->get( "summ" )->ini( fConvResult );
				obj1->get( "currency" )->ini( c2 );
				obj1->get( "type" )->ini( t2 );
				obj1->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtOperation );
				obj1->get( "obj" )->ini( iOperationId );
				option.SetTable( "ud_fsumm" );
				handler->AddUdObject( obj1, option );
				option.Clear( );
				udDELETE( obj1 );
			}
			// подгружаем итоговые данные
			ClearArrBase( arr );
			option.Clear( );
			LoadSumm( arr, id, type );
		} else {
			g_pApp->GetLog( )->Write( "возникли ошибки при выборке данных\n" );
			WriteErrors( g_pApp->GetLog( ), err );
		}
		return ret;
	} // char AccumulateConvert

	void CHTour::OpSummLoad( udPSocket sock ) {
		udType id( ( udUInt ) 0 );
		if ( id.read( sock ) == 1 ) { // считали ID тура
			udPHandler handler = g_pApp->GetHandler( );
			udHandlerOption option;
			udBaseConfig config;
			udPBase obj = NULL;
			string szWhere = "";
			udArrBase arr;
			udBinaryData data;
			char code = 0;
			udInt type = 0;
			// наполняем конфиг
			obj = new udProjectFinanceSumm;
			obj->GetConfig( config );
			udDELETE( obj );
			// наполняем опции
			option.SetTable( "ud_fsumm" );
			option.SetClass( GetProjectFinanceSumm2 );
			option.SetIgnoreAttr( "currency_" );
			option.SetIgnoreAttr( "type_" );
			option.AddLeftJoin(
				"name", "fsumm_currency_", "ud_currency",
				"`ud_currency`.`currency_id`=`ud_fsumm`.`fsumm_currency`", GetProjectCurrency
			);
			///////////////////////////////////////////////////////////////////////////////////////
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );
			///////////////////////////////////////////////////////////////////////////////////////
			// итоговая цена
			szWhere = config.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtTourPrice ) + " AND " +
				config.GetAttrIndex( "obj" ) + "=" + id.toString( );
			option.SetWhere( szWhere );
			handler->GetUdObject( arr, option );
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				type = obj->get( "type" )->toInt( );
				obj->get( "type_" )->ini( udProjectFinanceSumm::GetTypeName( type ) );
			}
			// отправляем ответ
			WriteInstances( arr, data );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			// ждем подтверждающий байт
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			// очищаем данные
			data.clear( );
			ClearArrBase( arr );
			///////////////////////////////////////////////////////////////////////////////////////
			// оплата тура
			szWhere = config.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtTour ) + " AND " +
				config.GetAttrIndex( "obj" ) + "=" + id.toString( );
			option.SetWhere( szWhere );
			handler->GetUdObject( arr, option );
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				type = obj->get( "type" )->toInt( );
				obj->get( "type_" )->ini( udProjectFinanceSumm::GetTypeName( type ) );
			}
			// отправляем ответ
			WriteInstances( arr, data );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			// ждем подтверждающий байт
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			// очищаем данные
			data.clear( );
			ClearArrBase( arr );
			///////////////////////////////////////////////////////////////////////////////////////
			// себестоимость
			szWhere = config.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtTourCost ) + " AND " +
				config.GetAttrIndex( "obj" ) + "=" + id.toString( );
			option.SetWhere( szWhere );
			option.SetClass( GetProjectFinanceSumm3 );
			option.SetIgnoreAttr( "partner" );
			option.SetIgnoreAttr( "partner_" );
			option.AddLeftJoin(
				"partner_id", "fsumm_partner", "ud_fsummtopartner",
				"`ud_fsumm`.`fsumm_id`=`ud_fsummtopartner`.`fsummtopartner_summ_id`", GetProjectFinanceSummToPartner
			);
			option.AddLeftJoin(
				"name", "fsumm_partner_", "ud_partner",
				"`ud_partner`.`partner_id`=`ud_fsummtopartner`.`fsummtopartner_partner_id`", GetProjectPartner
			);
			handler->GetUdObject( arr, option );
			string log = "\nsummcost: суммы себестоимости\n";
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				type = obj->get( "type" )->toInt( );
				obj->get( "type_" )->ini( udProjectFinanceSumm::GetTypeName( type ) );
				obj->dump( log );
				log += "\n";
			}
			g_pApp->GetLog( )->Write( log );
			// отправляем ответ
			WriteInstances( arr, data );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			// ждем подтверждающий байт
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			// очищаем данные
			data.clear( );
			ClearArrBase( arr );
			///////////////////////////////////////////////////////////////////////////////////////
			// оплата услуг
			szWhere = config.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtService ) + " AND " +
				config.GetAttrIndex( "obj" ) + "=" + id.toString( );
			option.SetWhere( szWhere );
			option.SetClass( GetProjectFinanceSumm3 );
			option.SetIgnoreAttr( "partner" );
			option.SetIgnoreAttr( "partner_" );
			option.AddLeftJoin(
				"partner_id", "fsumm_partner", "ud_fsummtopartner",
				"`ud_fsumm`.`fsumm_id`=`ud_fsummtopartner`.`fsummtopartner_summ_id`", GetProjectFinanceSummToPartner
			);
			option.AddLeftJoin(
				"name", "fsumm_partner_", "ud_partner",
				"`ud_partner`.`partner_id`=`ud_fsummtopartner`.`fsummtopartner_partner_id`", GetProjectPartner
			);
			handler->GetUdObject( arr, option );
			log = "\nsumm: суммы оплаты услуг\n";
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				type = obj->get( "type" )->toInt( );
				obj->get( "type_" )->ini( udProjectFinanceSumm::GetTypeName( type ) );
				obj->dump( log );
				log += "\n";
			}
			g_pApp->GetLog( )->Write( log );
			// отправляем ответ
			WriteInstances( arr, data );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			// ждем подтверждающий байт
			sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
			sock->ReadByte( &code );
			// очищаем данные
			data.clear( );
			ClearArrBase( arr );
			///////////////////////////////////////////////////////////////////////////////////////
		}
	} // void OpSummLoad

	udPBase CHTour::GetPersAcc( udUInt iTourId, udInt type, udUInt iPartnerId ) {
		udPBase ret = NULL;
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr;
		udPBase pTour = NULL, obj = NULL;
		obj = new udProjectTour;
		obj->GetConfig( config );
		udDELETE( obj );
		option.SetTable( "ud_tour" );
		option.SetClass( GetProjectTour );
		option.SetWhere( config.GetAttrIndex( "id" ) + "=" + toString( iTourId ) );
		config.Clear( );
		handler->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			pTour = arr[ 0 ];
			arr.clear( );
			obj = new udProjectPersAcc;
			obj->GetConfig( config );
			udDELETE( obj );
			option.Clear( );
			if ( type == udProjectFinanceSumm::SummAtTour ) { // заказчик (клиент) - продавец (компания)
				string szWhere = "";
				if ( pTour->get( "emptype" )->toInt( ) == udProjectTour::TOUR_EMPLOYER_CLIENT ) {
					szWhere += config.GetAttrIndex( "type" ) + "=" + toString( udProjectPersAcc::PERS_ACC_CLIENT_COMPANY );
				} else {
					szWhere += config.GetAttrIndex( "type" ) + "=" + toString( udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY );
				}
				szWhere += string( " AND " ) +
					config.GetAttrIndex( "obj1" ) + "=" + pTour->get( "employer" )->toString( ) + " AND " +
					config.GetAttrIndex( "obj2" ) + "=" + pTour->get( "company" )->toString( );
				option.SetWhere( szWhere );
			} else if ( type == udProjectFinanceSumm::SummAtService ) { // продавец (компания) - партнер
				option.SetWhere(
					config.GetAttrIndex( "type" ) + "=" + toString( udProjectPersAcc::PERS_ACC_COMPANY_PARTNER ) + " AND " +
					config.GetAttrIndex( "obj1" ) + "=" + pTour->get( "company" )->toString( ) + " AND " +
					config.GetAttrIndex( "obj2" ) + "=" + toString( iPartnerId )
				 );
			}
			udDELETE( pTour );
			option.SetTable( "ud_persacc" );
			option.SetClass( GetProjectPersAcc );
			handler->GetUdObject( arr, option );
			if ( !arr.empty( ) ) {
				ret = arr[ 0 ];
				arr.clear( );
			}
		}
		return ret;
	} // udPBase GetPersAcc

	udPBase CHTour::FindCurrencyPair( udUInt c1, udUInt c2 ) {
		g_pApp->GetLog( )->Write(
			"CHTour::FindCurrencyPair, c1=" + toString( c1 ) + ", c2=" + toString( c2 ) + "\n"
		);
		udPBase ret = NULL;
		udPBase obj = NULL;
		udArrBase arr;
		udHandlerOption option;
		udBaseConfig config;
		string szWhere = "";
		obj = new udProjectCurrencyPair;
		obj->GetConfig( config );
		udDELETE( obj );
		szWhere = config.GetAttrIndex( "currency1" ) + "=" + toString( c1 ) + " AND " +
			config.GetAttrIndex( "currency2" ) + "=" + toString( c2 );
		g_pApp->GetLog( )->Write(
			"CHTour::FindCurrencyPair, where=\"" + szWhere + "\"\n"
		);
		config.Clear( );
		option.SetTable( "ud_currencypair" );
		option.SetClass( GetProjectCurrencyPair );
		option.SetWhere( szWhere );
		g_pApp->GetHandler( )->GetUdObject( arr, option );
		if ( !arr.empty( ) ) {
			ret = arr[ 0 ];
			arr.clear( );

			string log = "\nCHTour::FindCurrencyPair, ret=";
			ret->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
		}
		return ret;
	} // udPBase FindCurrencyPair

	void CHTour::HandleAddRequest( udPSocket sock ) {
		udPBase obj = m_fnGetObject( );
		if ( ( obj->read( sock ) == 1 ) && // скачали
			( this->FindObj( obj, "name" ) == 0 ) && // не нашли клона
			( this->AddObj( obj, true ) == 1 ) // добавили основу
		) {
			udPLog __log = g_pApp->GetLog( );
			string log = "\ngot object\n";
			obj->dump( log );
			__log->Write( log + "\n" );
			udPBase tmp = NULL;
			//////////////////////////////////////////////////////////////////////////////////////////
			if ( !m_arrAppend.empty( ) ) { // проверяем наличие доп сущностей из списков
				udUInt id = obj->get( "id" )->toUInt( );
				udArrBase arr;
				udPBase obj1 = NULL;
				for( vctAppend::iterator i = m_arrAppend.begin( ); i != m_arrAppend.end( ); ++i ) {
					if ( i->type == ( udInt ) edit_append_array ) {
						log = "\nwait object\n";
						tmp = i->fnGetObject( );
						tmp->dump( log );
						__log->Write( log + "\n" );
						udDELETE( tmp );

						ReadInstances( sock, arr, i->fnGetObject );
						if ( !arr.empty( ) ) {
							if ( i->fnBeforeProc ) {
								udArrBase arr2, arr3;
								i->fnBeforeProc( arr, arr2, arr3, obj );
							}
							for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
								obj1 = *j;
								obj1->get( i->fkey )->ini( id );
								udStateHandlerSimple::AddObj( obj1, i->table, true );
							}
							if ( i->fnAfterProc ) {
								udArrBase arr2, arr3;
								i->fnAfterProc( arr, arr2, arr3, obj );
							}
						}
						ClearArrBase( arr );
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			this->ExtraDataRecieve( opidUpd1, sock, obj );
			//////////////////////////////////////////////////////////////////////////////////////////
			/*if ( !m_arrAppend.empty( ) ) { // проверяем наличие доп сущностей из списков
				udUInt id = obj->get( "id" )->toUInt( );
				udArrBase arr;
				udPBase obj1 = NULL;
				for( vctAppend::iterator i = m_arrAppend.begin( ); i != m_arrAppend.end( ); ++i ) {
					if ( i->type == ( udInt ) edit_append_array ) {
						ReadInstances( sock, arr, i->fnGetObject );
						if ( !arr.empty( ) ) {
							if ( i->fnBeforeProc ) {
								udArrBase arr2, arr3;
								i->fnBeforeProc( arr, arr2, arr3, obj );
							}
							for( udArrBase::iterator j = arr.begin( ); j != arr.end( ); ++j ) {
								obj1 = *j;
								obj1->get( i->fkey )->ini( id );
								udStateHandlerSimple::AddObj( obj1, i->table, true );
							}
							if ( i->fnAfterProc ) {
								udArrBase arr2, arr3;
								i->fnAfterProc( arr, arr2, arr3, obj );
							}
						}
						ClearArrBase( arr );
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			this->ExtraDataRecieve( opidUpd1, sock, obj );//*/
			//////////////////////////////////////////////////////////////////////////////////////////
			udHTour::Cost( obj );
			//////////////////////////////////////////////////////////////////////////////////////////
			udBinaryData data;
			data.add( obj->get( "id" )->toUInt( ) );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			data.clear( );
			//////////////////////////////////////////////////////////////////////////////////////////
		}
		delete obj;
	} // void HandleAddRequest

	void CHTour::HandleUpdRequest( udPSocket sock ) {
		udPLog __log = g_pApp->GetLog( );
		string log = "\nwait object:\n";
		udPBase obj = m_fnGetObject( );

		obj->dump( log );
		__log->Write( log + "\n" );

		if ( ( obj->read( sock ) == 1 ) && // считали объект
			( obj->get( "id" )->toUInt( ) > 0 ) && // объект на обновление
			( this->FindObj( obj, "id", false ) == 1 ) && // объект существует
			( this->UpdObj( obj, true ) == 1 ) // обновили объект
		) {
			log = "\ngot object\n";
			obj->dump( log );
			__log->Write( log + "\n" );
			udPBase tmp = NULL;
			//////////////////////////////////////////////////////////////////////////////////////////
			g_pApp->GetModel( )->GetLockHandler( )->UnlockObject( obj, m_fnGetObject );
			//////////////////////////////////////////////////////////////////////////////////////////
			if ( !m_arrAppend.empty( ) ) { // проверяем наличие доп сущностей из списков
				udPHandler handler = g_pApp->GetHandler( );
				udPHObjectLock hLock = g_pApp->GetModel( )->GetLockHandler( );
				udUInt id = obj->get( "id" )->toUInt( );
				udArrBase arrLoaded, arrCurrent;
				udPBase obj1 = NULL;
				for( vctAppend::iterator i = m_arrAppend.begin( ); i != m_arrAppend.end( ); ++i ) {
					if ( i->type == ( udInt ) edit_append_array ) {
						//////////////////////////////////////////////////////////////////////////////
						log = "\nwait object\n";
						tmp = i->fnGetObject( );
						tmp->dump( log );
						__log->Write( log + "\n" );
						udDELETE( tmp );
						//////////////////////////////////////////////////////////////////////////////
						ReadInstances( sock, arrLoaded, i->fnGetObject );
						LoadInstanceByForeignKey( handler, obj, arrCurrent, i->fnGetObject, i->table, i->fkey, i->key, i->order, i->where1 );
						//////////////////////////////////////////////////////////////////////////////
						if ( arrLoaded.empty( ) ) { // с клиента ничего не прилетело, вдруг их удалили
							if ( !arrCurrent.empty( ) ) { // если в прилетевших нет, то все на удаление
								for( udArrBase::iterator j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
									obj1 = *j;
									if ( obj1->get( i->fkey )->toUInt( ) == id ) { // вдруг прилетит херня
										udStateHandlerSimple::DelObj( obj1, i->table, true );
										hLock->UnlockObject( obj1, i->fnGetObject );
									}
								}
							}
						} else if ( arrCurrent.empty( ) ) { // у объекта не было потомков
							for( udArrBase::iterator j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
								obj1 = *j;
								obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
								obj1->get( i->fkey )->ini( id );
								udStateHandlerSimple::AddObj( obj1, i->table, true );
							}
						} else { // оба не пустые, необходимо сравнить и составить списки add/upd/del
							udArrBase arrAdd, arrUpd, arrDel;
							udPBase objNew = NULL, objOld = NULL;
							udArrBase::iterator j, k;
							bool bFound = false;

							// добавление и обновление
							for( j = arrLoaded.begin( ); j != arrLoaded.end( ); ++j ) {
								objNew = *j;
								bFound = false;

								for( k = arrCurrent.begin( ); k != arrCurrent.end( ); ++k ) {
									objOld = *k;
									if ( objNew->get( "id" )->toUInt( ) == objOld->get( "id" )->toUInt( ) ) {
										bFound = true;
										arrUpd.push_back( objNew );
										break;
									}
								}

								if ( !bFound ) {
									arrAdd.push_back( objNew );
								}
							}

							// удаление
							for( j = arrCurrent.begin( ); j != arrCurrent.end( ); ++j ) {
								objOld = *j;
								bFound = false;

								for( k = arrLoaded.begin( ); k != arrLoaded.end( ); ++k ) {
									objNew = *k;
									if ( objOld->get( "id" )->toUInt( ) == objNew->get( "id" )->toUInt( ) ) {
										bFound = true;
										break;
									}
								}

								if ( !bFound ) {
									arrDel.push_back( objOld );
								}
							}

							// выполняем операции
							if ( i->fnBeforeProc ) {
								i->fnBeforeProc( arrAdd, arrUpd, arrDel, obj );
							}
							if ( !arrAdd.empty( ) ) {
								for( udArrBase::iterator j = arrAdd.begin( ); j != arrAdd.end( ); ++j ) {
									obj1 = *j;
									obj1->get( "id" )->ini( ( udUInt ) 0 ); // вдруг прилетит херня
									obj1->get( i->fkey )->ini( id );
									udStateHandlerSimple::AddObj( obj1, i->table, true );
								}
							}
							if ( !arrUpd.empty( ) ) {
								for( udArrBase::iterator j = arrUpd.begin( ); j != arrUpd.end( ); ++j ) {
									obj1 = *j;
									obj1->get( i->fkey )->ini( id ); // вдруг прилетит херня
									udStateHandlerSimple::UpdObj( obj1, i->table, "id", true );
									hLock->UnlockObject( obj1, i->fnGetObject );
								}
							}
							if ( !arrDel.empty( ) ) {
								for( udArrBase::iterator j = arrDel.begin( ); j != arrDel.end( ); ++j ) {
									obj1 = *j;
									if ( obj1->get( i->fkey )->toUInt( ) == id ) { // вдруг прилетит херня
										udStateHandlerSimple::DelObj( obj1, i->table, true );
										hLock->UnlockObject( obj1, i->fnGetObject );
									}
								}
							}
							if ( i->fnAfterProc ) {
								i->fnAfterProc( arrAdd, arrUpd, arrDel, obj );
							}
							//ClearArrBase( arrAdd );
							//ClearArrBase( arrUpd );
							//ClearArrBase( arrDel );
						}
						ClearArrBase( arrLoaded );
						ClearArrBase( arrCurrent );
						//////////////////////////////////////////////////////////////////////////////
						/*ReadInstances( sock, arrAdd, i->fnGetObject );
						ReadInstances( sock, arrUpd, i->fnGetObject );
						ReadInstances( sock, arrDel, i->fnGetObject );
						if ( i->fnBeforeProc ) {
							i->fnBeforeProc( arrAdd, arrUpd, arrDel, obj );
						}
						if ( !arrAdd.empty( ) ) {
							for( udArrBase::iterator j = arrAdd.begin( ); j != arrAdd.end( ); ++j ) {
								obj1 = *j;
								obj1->get( i->fkey )->ini( id );
								this->AddObj( obj1, i->table, true );
							}
						}
						if ( !arrUpd.empty( ) ) {
							for( udArrBase::iterator j = arrUpd.begin( ); j != arrUpd.end( ); ++j ) {
								obj1 = *j;
								this->UpdObj( obj1, i->table, "id", true );
								g_pApp->GetModel( )->GetLockHandler( )->UnlockObject( obj1, i->fnGetObject );
							}
						}
						if ( !arrDel.empty( ) ) {
							for( udArrBase::iterator j = arrDel.begin( ); j != arrDel.end( ); ++j ) {
								obj1 = *j;
								this->DelObj( obj1, i->table, true );
								g_pApp->GetModel( )->GetLockHandler( )->UnlockObject( obj1, i->fnGetObject );
							}
						}
						if ( i->fnAfterProc ) {
							i->fnAfterProc( arrAdd, arrUpd, arrDel, obj );
						}
						ClearArrBase( arrAdd );
						ClearArrBase( arrUpd );
						ClearArrBase( arrDel );*/
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			this->ExtraDataRecieve( opidAdd1, sock, obj );
			//////////////////////////////////////////////////////////////////////////////////////////
			/*g_pApp->GetModel( )->GetLockHandler( )->UnlockObject( obj, m_fnGetObject );
			//////////////////////////////////////////////////////////////////////////////////////////
			if ( !m_arrAppend.empty( ) ) { // проверяем наличие доп сущностей из списков
				udUInt id = obj->get( "id" )->toUInt( );
				udArrBase arrAdd, arrUpd, arrDel;
				udPBase obj1 = NULL;
				for( vctAppend::iterator i = m_arrAppend.begin( ); i != m_arrAppend.end( ); ++i ) {
					if ( i->type == ( udInt ) edit_append_array ) {
						ReadInstances( sock, arrAdd, i->fnGetObject );
						ReadInstances( sock, arrUpd, i->fnGetObject );
						ReadInstances( sock, arrDel, i->fnGetObject );
						if ( i->fnBeforeProc ) {
							i->fnBeforeProc( arrAdd, arrUpd, arrDel, obj );
						}
						if ( !arrAdd.empty( ) ) {
							for( udArrBase::iterator j = arrAdd.begin( ); j != arrAdd.end( ); ++j ) {
								obj1 = *j;
								obj1->get( i->fkey )->ini( id );
								udStateHandlerSimple::AddObj( obj1, i->table, true );
							}
						}
						if ( !arrUpd.empty( ) ) {
							for( udArrBase::iterator j = arrUpd.begin( ); j != arrUpd.end( ); ++j ) {
								obj1 = *j;
								this->UpdObj( obj1, i->table, "id", true );
								g_pApp->GetModel( )->GetLockHandler( )->UnlockObject( obj1, i->fnGetObject );
							}
						}
						if ( !arrDel.empty( ) ) {
							for( udArrBase::iterator j = arrDel.begin( ); j != arrDel.end( ); ++j ) {
								obj1 = *j;
								udStateHandlerSimple::DelObj( obj1, i->table, true );
								g_pApp->GetModel( )->GetLockHandler( )->UnlockObject( obj1, i->fnGetObject );
							}
						}
						if ( i->fnAfterProc ) {
							i->fnAfterProc( arrAdd, arrUpd, arrDel, obj );
						}
						ClearArrBase( arrAdd );
						ClearArrBase( arrUpd );
						ClearArrBase( arrDel );
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			this->ExtraDataRecieve( opidAdd1, sock, obj );*/
			//////////////////////////////////////////////////////////////////////////////////////////
			// пересчитываем себестоимость тура
			udHTour::Cost( obj );
			//////////////////////////////////////////////////////////////////////////////////////////
			udBinaryData data;
			data.add( obj->get( "id" )->toUInt( ) );
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );
			data.clear( );
			//////////////////////////////////////////////////////////////////////////////////////////
		}
		delete obj;
	} // void HandleUpdRequest

	static udInt FindPeriod( udArrBase& arrTourService, udArrBase& arrServicePeriod, udArrBase& arrResult ) {
		udInt		found	= 0,		// количество найденных
					k		= -1;		// индекс найденного периода
		udPBase		s		= NULL,		// указатель на услугу тура
					p		= NULL,		// указатель на период услуги
					add		= NULL;		// новый объект
		datetime	sd1, sd2,			// даты оказания услуги
					pd1, pd2,			// даты периода
					tmp;				// максимальная (минимальная) дата слева (справа)
		udUInt		id		= 0;		// ID услуги
		udPLog		__log	= g_pApp->GetLog( );
		string		log		= "";
		// для каждой услуги
		//log = "\nуслуги в туре:\n";
		for( size_t i = 0; i < arrTourService.size( ); ++i ) {
			//__log->Write( "s\n" );
			s	= arrTourService[ i ];

			/*log = "\n";
			s->dump( log );
			log += "\n";
			__log->Write( log );*/

			id	= s->get( "service" )->toUInt( );
			k	= -1;
			sd1	= s->get( "date1" )->toTime( );
			sd2	= s->get( "date2" )->toTime( );
			tmp.set( 0 ); // зануляем дату
			// пробегаемся по периодам
			for( size_t j = 0; j < arrServicePeriod.size( ); ++j ) {
				//__log->Write( "p\n" );
				p = arrServicePeriod[ j ];
				if ( p->get( "service" )->toUInt( ) == id ) { // период текущей услуги
					pd1 = p->get( "date1" )->toTime( );
					pd2 = p->get( "date2" )->toTime( );
					if ( ( ( pd1 <= sd1 ) && ( pd1 > tmp ) ) || ( tmp.get( ) == 0 ) ) {
						tmp = pd1;
						k = j;
					}
				}
			}
			// подводим итог пробежки по периодам
			if ( tmp.get( ) == 0 ) { // дат слева не найдено, ищем справа
				for( size_t j = 0; j < arrServicePeriod.size( ); ++j ) {
					//__log->Write( "p\n" );
					p = arrServicePeriod[ j ];
					if ( p->get( "service" )->toUInt( ) == id ) { // период текущей услуги
						pd1 = p->get( "date1" )->toTime( );
						pd2 = p->get( "date2" )->toTime( );
						if ( ( ( pd1 >= sd1 ) && ( pd1 < tmp ) ) || ( tmp.get( ) == 0 ) ) {
							tmp = pd1;
							k = j;
						}
					}
				}
			}
			if ( tmp.get( ) != 0 ) { // в итоге был найден период
				//__log->Write( "k=" + toString( k ) + "\n" );
				p = arrServicePeriod[ k ];
				add = new udProjectServicePeriod;
				p->apply( add );
				arrResult.push_back( add );
				++found;
			}
		}
		//__log->Write( log );
		log = "\nполученные периоды (n=" + toString( arrResult.size( ) ) + "):\n";
		for( udArrBase::iterator i = arrResult.begin( ); i != arrResult.end( ); ++i ) {
			( *i )->dump( log );
			log += "\n";
		}
		__log->Write( log );
		//
		return found;
	} // udInt FindPeriod

	/*
		Если есть количество человек в цене, которое меньше num, но больше чем left или left равно 0,
		то заменяем left

		Если есть количество человек в цене, которое больше num, но меньше right или right равно 0,
		то заменяем right
		
		В конце, если left не равно 0, то левая цена на выбор,
		иначе, если right не равно 0, то правая цена на выбор,
		иначе фэйл
	*/
	static udPBase FindPriceByNum( udUInt num, udArrBase& arrPrice ) {
		udUInt	left	= 0,	// цена слева
				right	= 0,	// цена справа
				cur		= 0;	// текущее количество человек
		udPBase	pLeft	= NULL,	// указатель на цену слева
				pRight	= NULL,	// указатель на цену справа
				pPrice	= NULL;	// текущая цена на осмотре

		string log = "\nFindPriceByNum, num=" + toString( num ) + "\n";

		for( udArrBase::iterator i = arrPrice.begin( ); i != arrPrice.end( ); ++i ) {
			pPrice = *i;
			cur = pPrice->get( "num" )->toUInt( );

			log += "cur=" + toString( cur ) + ", "
				"left=" + toString( left ) + ", "
				"right=" + toString( right ) + "\n";

			if ( cur <= num ) {
				if ( ( cur > left ) || !left ) {
					left = cur;
					pLeft = pPrice;
				}
			} else { // cur > num
				if ( ( cur < right ) || !right ) {
					right = cur;
					pRight = pPrice;
				}
			}
		}

		log += "\npicked: ";
		log += pLeft ? "left" : "right";
		g_pApp->GetLog( )->Write( log + "\n" );

		return pLeft ? pLeft : pRight;
	} // udPBase FindPriceByNum

	static udInt FindPrice( udUInt num, udArrBase& arrServicePeriod, udArrBase& arrServicePrice, udArrBase& arrResult ) {
		udInt		found		= 0,	// количество найденных
					k			= -1;	// индекс найденной цены
		udPBase		p			= NULL,	// указатель на период
					r			= NULL,	// указатель на цену
					add			= NULL;	// указатель на объект для добавления
		udUInt		rn			= 0,	// количество людей в цене
					tmp			= 0,	// временное искомое количество
					iPeriod		= 0,	// индекс периода
					iService	= 0;	// ID услуги
		udPLog		__log		= g_pApp->GetLog( );
		string		log			= "";
		udArrBase	arrPrice;			// набор сумм по услуге и периоду

		__log->Write(
			"\nFindPrice"
			"\nnum=" + toString( num ) +
			"\narrServicePeriod.size=" + toString( arrServicePeriod.size( ) ) +
			"\narrServicePrice.size=" + toString( arrServicePrice.size( ) ) +
			"\n"
		);
		
		//__log->Write( "num=" + toString( num ) + "\n" );
		// пробегаемся по периодам
		for( size_t i = 0; i < arrServicePeriod.size( ); ++i ) {
			p = arrServicePeriod[ i ];

			/*log = "\n\nпериод:\n";
			p->dump( log );
			__log->Write( log + "\n\n" );*/

			k = -1;
			tmp = 0;
			iPeriod = p->get( "index" )->toUInt( );
			iService = p->get( "service" )->toUInt( );
			// пробегаемся по ценам
			for( size_t j = 0; j < arrServicePrice.size( ); ++j ) {
				r = arrServicePrice[ j ];

				if ( ( iPeriod == r->get( "period" )->toUInt( ) ) && ( iService == r->get( "service" )->toUInt( ) ) ) {
					/*log = "\nцена:\n";
					r->dump( log );
					__log->Write( log + "\n" );*/

					/*rn = r->get( "num" )->toUInt( );
					if ( ( ( rn <= num ) && ( rn > tmp ) ) || ( tmp == 0 ) ) {
						tmp = rn;
						k = j;
					}*/
					arrPrice.push_back( r );
				}
			}

			if ( r = FindPriceByNum( num, arrPrice ) ) {
				add = new udProjectPriceTableItem;
				r->apply( add );
				add->get( "partner" )->ini( p->get( "partner" )->toUInt( ) );
				arrResult.push_back( add );
				++found;
			}

			arrPrice.clear( );
			// подводим итоги
			/*if ( tmp == 0 ) {
				for( size_t j = 0; j < arrServicePrice.size( ); ++j ) {
					r = arrServicePrice[ j ];
					if ( ( iPeriod == r->get( "period" )->toUInt( ) ) && ( iService == r->get( "service" )->toUInt( ) ) ) {
						rn = r->get( "num" )->toUInt( );
						if ( ( ( rn >= num ) && ( rn < tmp ) ) || ( tmp == 0 ) ) {
							tmp = rn;
							k = j;
						}
					}
				}
			}*/
			/*if ( tmp != 0 ) {
				r = arrServicePrice[ k ];
				add = new udProjectPriceTableItem;
				r->apply( add );
				add->get( "partner" )->ini( p->get( "partner" )->toUInt( ) );
				arrResult.push_back( add );
				++found;
			}*/
			//__log->Write( "завершили работу с периодом\n\n\n" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////
		log = "\nFindPrice: 1, полученные цены (n=" + toString( arrResult.size( ) ) + "):\n";
		for( udArrBase::iterator i = arrResult.begin( ); i != arrResult.end( ); ++i ) {
			( *i )->dump( log );
			log += "\n";
		}
		__log->Write( log );
		//////////////////////////////////////////////////////////////////////////////////////////////
		return found;
	} // udInt FindPrice

	static udInt FindPrice( map< udUInt, udUInt > mapServiceAmount, udArrBase& arrServicePeriod, udArrBase& arrServicePrice, udArrBase& arrResult ) {
		udInt	found		= 0,	// количество найденных
				k			= -1;	// индекс найденной цены
		udPBase	p			= NULL,	// указатель на период
				r			= NULL,	// указатель на цену
				add			= NULL;	// указатель на объект для добавления
		udUInt	rn			= 0,	// количество людей в цене
				tmp			= 0,	// временное искомое количество
				iPeriod		= 0,	// индекс периода
				iService	= 0,	// ID услуги
				num			= 0;
		udPLog	__log		= g_pApp->GetLog( );
		string	log			= "";
		map< udUInt, udUInt >::iterator itAmount;
		udArrBase arrPrice;
		
		//__log->Write( "num=" + toString( num ) + "\n" );
		// пробегаемся по периодам
		for( size_t i = 0; i < arrServicePeriod.size( ); ++i ) {
			p = arrServicePeriod[ i ];

			/*log = "\n\nпериод:\n";
			p->dump( log );
			__log->Write( log + "\n\n" );*/

			k = -1;
			tmp = 0;
			iPeriod = p->get( "index" )->toUInt( );
			iService = p->get( "service" )->toUInt( );
			itAmount = mapServiceAmount.find( iService );
			if ( itAmount != mapServiceAmount.end( ) ) {
				num = itAmount->second;
				//__log->Write( "Поиск слева\n" );
				// пробегаемся по ценам
				for( size_t j = 0; j < arrServicePrice.size( ); ++j ) {
					r = arrServicePrice[ j ];

					if ( ( iPeriod == r->get( "period" )->toUInt( ) ) && ( iService == r->get( "service" )->toUInt( ) ) ) {
						/*log = "\nцена:\n";
						r->dump( log );
						__log->Write( log + "\n" );*/

						/*rn = r->get( "num" )->toUInt( );
						if ( ( ( rn <= num ) && ( rn > tmp ) ) || ( tmp == 0 ) ) {
							tmp = rn;
							k = j;
						}*/
						arrPrice.push_back( r );
					}
				}

				if ( r = FindPriceByNum( num, arrPrice ) ) {
					add = new udProjectPriceTableItem;
					r->apply( add );
					add->get( "partner" )->ini( p->get( "partner" )->toUInt( ) );
					arrResult.push_back( add );
					++found;
				}

				arrPrice.clear( );
				/*
				// подводим итоги
				if ( tmp == 0 ) {
					__log->Write( "Поиск справа\n" );
					for( size_t j = 0; j < arrServicePrice.size( ); ++j ) {
						r = arrServicePrice[ j ];
						
						log = "\nцена:\n";
						r->dump( log );
						__log->Write( log + "\n" );

						if ( ( iPeriod == r->get( "period" )->toUInt( ) ) && ( iService == r->get( "service" )->toUInt( ) ) ) {
							rn = r->get( "num" )->toUInt( );
							if ( ( ( rn >= num ) && ( rn < tmp ) ) || ( tmp == 0 ) ) {
								tmp = rn;
								k = j;
							}
						}
					}
				}
				if ( tmp != 0 ) {
					r = arrServicePrice[ k ];
					add = new udProjectPriceTableItem;
					r->apply( add );
					add->get( "partner" )->ini( p->get( "partner" )->toUInt( ) );
					arrResult.push_back( add );
					++found;
				}//*/
			}
			//__log->Write( "завершили работу с периодом\n\n\n" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////
		log = "\nFindPrice: 2, полученные цены (n=" + toString( arrResult.size( ) ) + "):\n";
		for( udArrBase::iterator i = arrResult.begin( ); i != arrResult.end( ); ++i ) {
			( *i )->dump( log );
			log += "\n";
		}
		__log->Write( log );
		//////////////////////////////////////////////////////////////////////////////////////////////
		return found;
	} // udInt FindPrice

	static void LoadPriceByPeriod( udArrBase& arrPeriod, udArrBase& arrResult, udPHandler handler ) {
		udHandlerOption option;
		udBaseConfig config;
		udPBase obj = NULL;
		udArrError err;
		udArrString arrString;
		string szWhere = "", log = "";
		udPLog __log = g_pApp->GetLog( );
		//////////////////////////////////////////////////////////////////////////////////////////////
		obj = new udProjectPriceTableItem;
		obj->GetConfig( config );
		udDELETE( obj );
		for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
			obj = *i;
			arrString.push_back(
				"(" +
				config.GetAttrIndex( "service" ) + "=" + obj->get( "service" )->toString( ) +
				" AND " +
				config.GetAttrIndex( "period" ) + "=" + obj->get( "index" )->toString( ) +
				")"
			);
		}
		szWhere = Join( " OR ", arrString );
		option.SetTable( "ud_pricetableitem" );
		option.SetClass( GetProjectPriceTableItem );
		option.SetWhere( szWhere );
		handler->GetUdObject( arrResult, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////
	} // void LoadPriceByPeriod

	// глупо, но сделаем пока так
	static udUInt CountTourTourist( udUInt id ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr;
		udPBase obj = NULL;
		udUInt ret = 0;
		obj = new udProjectTourClient;
		obj->GetConfig( config );
		udDELETE( obj );
		option.SetTable( "ud_tourclient" );
		option.SetClass( GetProjectTourClient );
		option.SetWhere( config.GetAttrIndex( "tour" ) + "=" + toString( id ) );
		handler->GetUdObject( arr, option );
		ret = arr.size( );
		ClearArrBase( arr );
		return ret;
	} // udUInt CountTourTourist

	static udUInt CollectTourCost( udPBase tour, udArrBase& arrPrice, udArrBase& arrResult, udInt iPriceType = CHTour::priceCost );

	// собирает цены услуг в единый набор сумм
	static udUInt CollectTourCost( udPBase tour, udArrBase& arrPrice, udArrBase& arrResult, udInt iPriceType ) {
		udPLog __log = g_pApp->GetLog( );
		string log = "";
		__log->Write( "CollectTourCost, enter\n" );

		udUInt currency_num = 0, iCurrency = 0, iPartner = 0;
		udPBase obj = NULL, add = NULL;
		string szAttr = "amount", szCurrencyAttr = "currency";
		udFloat s1 = 0.0f, s2 = 0.0f;
		map< udUInt, map< udUInt, udPBase > > mapSumm;
		map< udUInt, udPBase >::iterator itSumm;
		map< udUInt, map< udUInt, udPBase > >::iterator itPartner;

		if ( iPriceType == udHTour::priceRecommend ) {
			szAttr = "amount_recommend";
			szCurrencyAttr = "currency_recommend";
		}
		//
		log = "\n";
		for( udArrBase::iterator i = arrPrice.begin( ); i != arrPrice.end( ); ++i ) {
			obj = *i;
			obj->dump( log );
			log += "\n";
			iPartner = obj->get( "partner" )->toUInt( );
			iCurrency = obj->get( szCurrencyAttr )->toUInt( );
			itPartner = mapSumm.find( iPartner );
			if ( itPartner == mapSumm.end( ) ) { // партнера нет, добавим и сумму сразу
				add = new udProjectFinanceSumm;
				add->get( "summ" )->ini( obj->get( szAttr )->toFloat( ) );
				add->get( "currency" )->ini( iCurrency );
				add->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtTourCost );
				add->get( "obj" )->ini( tour->get( "id" )->toUInt( ) );
				add->get( "partner" )->ini( iPartner );
				mapSumm[ iPartner ][ iCurrency ] = add;
				++currency_num;
			} else {
				itSumm = itPartner->second.find( iCurrency );
				if ( itSumm == itPartner->second.end( ) ) {
					add = new udProjectFinanceSumm;
					add->get( "summ" )->ini( obj->get( szAttr )->toFloat( ) );
					add->get( "currency" )->ini( iCurrency );
					add->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtTourCost );
					add->get( "obj" )->ini( tour->get( "id" )->toUInt( ) );
					add->get( "partner" )->ini( iPartner );
					mapSumm[ iPartner ][ iCurrency ] = add;
					++currency_num;
				} else {
					s1 = itSumm->second->get( "summ" )->toFloat( );
					s2 = obj->get( szAttr )->toFloat( );
					itSumm->second->get( "summ" )->ini( s1 + s2 );
				}
			}
		}
		__log->Write( log );
		//
		for( map< udUInt, map< udUInt, udPBase > >::iterator i = mapSumm.begin( ); i != mapSumm.end( ); ++i ) {
			for( map< udUInt, udPBase >::iterator j = i->second.begin( ); j != i->second.end( ); ++j ) {
				arrResult.push_back( j->second );
			}
		}
		mapSumm.clear( );
		//
		__log->Write( "CollectTourCost, exit\n" );
		return currency_num;
	} // udUInt CollectTourCost

	static void CountServiceAmount( udUInt iGroupId, udPBase tour, udPBase pGroupTour, udArrBase& arrPeriod, map< udUInt, udUInt >& mapServiceAmount ) {
		//////////////////////////////////////////////////////////////////////////////////////////////
		udPLog __log = g_pApp->GetLog( );
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr;
		udArrError err;
		udPBase obj = NULL;
		udUInt iServiceId = 0;
		string szWhere = "", log = "";
		//////////////////////////////////////////////////////////////////////////////////////////////
		log = "\nCountServiceAmount\n";
		//////////////////////////////////////////////////////////////////////////////////////////////
		obj = new udProjectGroupTour;
		obj->GetConfig( config );
		udDELETE( obj );
		// входят в ту же группу и в тот же период
		option.SetWhere(
			config.GetAttrIndex( "group" ) + "=" + toString( iGroupId ) + " AND " +
			config.GetAttrIndex( "period" ) + "=" + pGroupTour->get( "period" )->toString( )
		);
		option.SetTable( "ud_grouptour" );
		option.SetClass( GetProjectGroupTour );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
		} else if ( !arr.empty( ) ) {
			/////////////////////////////////////////////////////////////////////////////////////////
			udArrString arrString;
			// наполняем спектр услуг нулями
			for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
				obj = *i;
				iServiceId = obj->get( "service" )->toUInt( );
				mapServiceAmount[ iServiceId ] = 0;
				arrString.push_back( toString( iServiceId ) );
			}
			string szIn = "IN (" + Join( ",", arrString ) + ")";
			arrString.clear( );
			/////////////////////////////////////////////////////////////////////////////////////////
			// выгребаем услуги, которые вошли в другие туры
			udUInt iTourId = 0;
			map< udUInt, udUInt > mapTourId; // ID туров, а вдруг кто-то воткнет их несколько
			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				iTourId = obj->get( "tour" )->toUInt( );
				mapTourId[ iTourId ] = iTourId;
			}
			udArrBase arrService;
			// для каждого тура выгребаем услуги, которые попадают в спект и принадлежат туру
			option.Clear( );
			option.SetTable( "ud_tourservice" );
			option.SetClass( GetProjectTourService );
			config.Clear( );
			obj = new udProjectTourService;
			obj->GetConfig( config );
			udDELETE( obj );
			udUInt iTouristNum = 0;//CountTourTourist( tour->get( "id" )->toUInt( ) );

			log += "iTouristNum=" + toString( iTouristNum ) + "\n"
				"mapTourId.size=" + toString( mapTourId.size( ) ) + "\n";

			for( map< udUInt, udUInt >::iterator i = mapTourId.begin( ); i != mapTourId.end( ); ++i ) {
				iTouristNum += CountTourTourist( i->first );
			}

			log += "iTouristNum=" + toString( iTouristNum ) + "\n";

			for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
				obj = *i;
				iServiceId = obj->get( "service" )->toUInt( );
				mapServiceAmount[ iServiceId ] = iTouristNum;
			}
			/*
			for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
				obj = *i;
				iServiceId = obj->get( "service" )->toUInt( );
				mapServiceAmount[ iServiceId ] = iTouristNum;
			}
			map< udUInt, udUInt >::iterator itService;
			for( map< udUInt, udUInt >::iterator i = mapTourId.begin( ); i != mapTourId.end( ); ++i ) {
				szWhere = config.GetAttrIndex( "tour" ) + "=" + toString( i->first ) + " AND " +
					config.GetAttrIndex( "service" ) + " " + szIn;
				option.SetWhere( szWhere );
				handler->GetUdObject( arrService, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( __log, err );
				} else if ( !arrService.empty( ) ) {
					iTouristNum = CountTourTourist( i->first );
					for( udArrBase::iterator j = arrService.begin( ); j != arrService.end( ); ++j ) {
						obj = *j;
						iServiceId = obj->get( "service" )->toUInt( );
						itService = mapServiceAmount.find( iServiceId );
						if ( itService != mapServiceAmount.end( ) ) {
							itService->second += iTouristNum;
						}
					}
				}
				ClearArrBase( arrService );
			}
			//*/
			/////////////////////////////////////////////////////////////////////////////////////////
		} else { // тур в группе один, для каждой услуги втыкаем количество человек данного тура
			udUInt iTouristNum = CountTourTourist( tour->get( "id" )->toUInt( ) );

			log += "iTouristNum=" + toString( iTouristNum ) + "\n";

			for( udArrBase::iterator i = arrPeriod.begin( ); i != arrPeriod.end( ); ++i ) {
				obj = *i;
				iServiceId = obj->get( "service" )->toUInt( );
				mapServiceAmount[ iServiceId ] = iTouristNum;
			}
		}
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////////
		__log->Write( log + "\n" );
	} // void CountServiceAmount

	static void BindServiceNumToPrice( udArrBase& arrPriceToTake, udArrBase& arrTourService ) {
		string log = "";
		udPLog __log = g_pApp->GetLog( );
		udUInt idService = 0;
		udFloat amount = 0.0f;
		udPBase pService = NULL, pPrice = NULL;
		map< udUInt, udUInt > mapAlreadyProc;
		map< udUInt, udUInt >::iterator itAlreadyProc;

		__log->Write( "BindServiceNumToPrice\n" );

		log += "\narrPriceToTake:\n";
		for( udArrBase::iterator i = arrPriceToTake.begin( ); i != arrPriceToTake.end( ); ++i ) {
			( *i )->dump( log );
			log += "\n";
		}

		log += "\narrTourService:\n";
		for( udArrBase::iterator i = arrTourService.begin( ); i != arrTourService.end( ); ++i ) {
			( *i )->dump( log );
			log += "\n";
		}

		__log->Write( log );
		log = "";

		for( udArrBase::iterator i = arrPriceToTake.begin( ); i != arrPriceToTake.end( ); ++i ) {
			pPrice = *i;
			idService = pPrice->get( "service" )->toUInt( );

			for( udArrBase::iterator j = arrTourService.begin( ); j != arrTourService.end( ); ++j ) {
				pService = *j;

				if ( idService == pService->get( "service" )->toUInt( ) ) {
					amount = pPrice->get( "amount_recommend" )->toFloat( );
					pPrice->get( "amount_recommend" )->ini( amount * pService->get( "num" )->toUInt( ) );

					amount = pPrice->get( "amount" )->toFloat( );
					pPrice->get( "amount" )->ini( amount * pService->get( "num" )->toUInt( ) );
					break;
				}
			}
		}

		log += "\narrPriceToTake:\n";
		for( udArrBase::iterator i = arrPriceToTake.begin( ); i != arrPriceToTake.end( ); ++i ) {
			( *i )->dump( log );
			log += "\n";
		}
		__log->Write( log );
		__log->Write( "BindServiceNumToPrice exit\n" );
	} // void BindServiceNumToPrice

	void CHTour::Cost( udPBase obj ) {
		//////////////////////////////////////////////////////////////////////////////////////////////
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udPBase obj1 = NULL;
		udPLog __log = g_pApp->GetLog( );
		udArrBase arr;
		udArrError err;
		string szWhere = "", log = "";
		udArrString arrString;
		//////////////////////////////////////////////////////////////////////////////////////////////
		__log->Write( "Расчет себестоимости тура\n" );
		// проверяем, что тур входит в какую то группу
		obj1 = new udProjectGroupTour;
		obj1->GetConfig( config );
		udDELETE( obj1 );
		szWhere = config.GetAttrIndex( "tour" ) + "=" + obj->get( "id" )->toString( );
		option.SetTable( "ud_grouptour" );
		option.SetClass( GetProjectGroupTour );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
		} else if ( arr.empty( ) ) { // не состоит в группе
			__log->Write( "тур не состоит в группе\n" );
			udArrBase arrTourService;
			config.Clear( );
			obj1 = new udProjectTourService;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			szWhere = config.GetAttrIndex( "tour" ) + "=" + obj->get( "id" )->toString( );
			option.SetTable( "ud_tourservice" );
			option.SetClass( GetProjectTourService );
			option.SetWhere( szWhere );
			handler->GetUdObject( arrTourService, option, &err );
			if ( !err.empty( ) ) {
				__log->Write( "при выборке связей тура и услуг произошли ошибки\n" );
				WriteErrors( __log, err );
			} else if ( !arrTourService.empty( ) ) { // есть услуги (иначе аномалия)
				udArrBase arrServicePeriod; // периоды услуг
				for( udArrBase::iterator i = arrTourService.begin( ); i != arrTourService.end( ); ++i ) {
					arrString.push_back( ( *i )->get( "service" )->toString( ) );
				}
				config.Clear( );
				obj1 = new udProjectServicePeriod;
				obj1->GetConfig( config );
				udDELETE( obj1 );
				szWhere = config.GetAttrIndex( "service" ) + " IN(" + Join( ",", arrString ) + ")";
				option.SetTable( "ud_serviceperiod" );
				option.SetClass( GetProjectServicePeriodForCost );
				option.SetWhere( szWhere );
				option.SetIgnoreAttr( "partner" );
				option.AddLeftJoin(
					"partner", "serviceperiod_partner", "ud_service",
					"`ud_service`.`service_id`=`ud_serviceperiod`.`serviceperiod_service`", GetProjectService
				);
				handler->GetUdObject( arrServicePeriod, option, &err );
				if ( !err.empty( ) ) {
					__log->Write( "при выборке периодов услуг произошли ошибки\n" );
					WriteErrors( __log, err );
				} else if ( !arrServicePeriod.empty( ) ) { // нашлись периоды
					udArrBase arrPeriodToTake; // множество периодов, которые совпали с услугами тура
					if ( FindPeriod( arrTourService, arrServicePeriod, arrPeriodToTake ) > 0 ) {
						// выгребаем цены по периодам
						udUInt num = CountTourTourist( obj->get( "id" )->toUInt( ) );
						udArrBase arrPriceToTake, arrPrice;
						// подгружаем цены по услугам
						LoadPriceByPeriod( arrPeriodToTake, arrPriceToTake, handler );
						// дополнительно умножаем цены на количество услуги в турах
						BindServiceNumToPrice( arrPriceToTake, arrTourService );
						// находим цены по каждой услуге (период+количество человек)
						if ( FindPrice( num, arrPeriodToTake, arrPriceToTake, arrPrice ) > 0 ) {
							udArrBase arrSumm;
							if ( CollectTourCost( obj, arrPrice, arrSumm ) > 0 ) {
								// сносим старые
								udPMySql db = ( udPMySql ) g_pApp->GetDatabase( );
								udPBase pPartnerSumm = NULL, summ = NULL;
								udUInt id = 0;
								db->Query(
									"DELETE FROM `ud_fsumm` WHERE "
									"`fsumm_at`=" + toString( udProjectFinanceSumm::SummAtTourCost ) + " AND "
									"`fsumm_obj`=" + obj->get( "id" )->toString( )
								);
								// добавляем новые
								udHandlerOption option1;
								option.Clear( );
								option.SetTable( "ud_fsumm" );
								option.SetIgnoreAttr( "partner" );
								option1.SetTable( "ud_fsummtopartner" );
								log = "\n\nfinalsumm:\n";
								for( udArrBase::iterator i = arrSumm.begin( ); i != arrSumm.end( ); ++i ) {
									summ = *i;
									handler->AddUdObject( summ, option );
									id = ( udUInt ) db->GetInsertId( );
									pPartnerSumm = new udProjectFinanceSummToPartner;
									pPartnerSumm->get( "summ_id" )->ini( id );
									pPartnerSumm->get( "partner_id" )->ini( summ->get( "partner" )->toUInt( ) );
									handler->AddUdObject( pPartnerSumm, option1 );
									udDELETE( pPartnerSumm );
								}
							} else {
								__log->Write( "не удалось собрать цены в набор сумм\n" );
							}
							ClearArrBase( arrSumm );
						} else {
							__log->Write( "не удалось найти цену по каждой услуге\n" );
						}
						ClearArrBase( arrPriceToTake );
						ClearArrBase( arrPrice );
					} else {
						__log->Write( "не удалось найти период по каждой услуге\n" );
					}
					ClearArrBase( arrPeriodToTake );
				} else {
					__log->Write( "не найдены периоды услуг\n" );
				}
				ClearArrBase( arrServicePeriod );
			} else {
				__log->Write( "не найдены услуги тура\n" );
			}
			ClearArrBase( arrTourService );
		} else { // состоит в какой-то группе
			__log->Write( "тур состоит в группе\n" );
			udPBase pGroupTour = arr[ 0 ];
			udUInt iGroupId = pGroupTour->get( "group" )->toUInt( );
			//////////////////////////////////////////////////////////////////////////////////////////
			udArrBase arrTourService;
			config.Clear( );
			obj1 = new udProjectTourService;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			szWhere = config.GetAttrIndex( "tour" ) + "=" + obj->get( "id" )->toString( );
			option.SetTable( "ud_tourservice" );
			option.SetClass( GetProjectTourService );
			option.SetWhere( szWhere );
			handler->GetUdObject( arrTourService, option, &err );
			if ( !err.empty( ) ) {
				__log->Write( "при выборке связей тура и услуг произошли ошибки\n" );
				WriteErrors( __log, err );
			} else if ( !arrTourService.empty( ) ) { // есть услуги (иначе аномалия)
				udArrBase arrServicePeriod; // периоды услуг
				for( udArrBase::iterator i = arrTourService.begin( ); i != arrTourService.end( ); ++i ) {
					arrString.push_back( ( *i )->get( "service" )->toString( ) );
				}
				config.Clear( );
				obj1 = new udProjectServicePeriod;
				obj1->GetConfig( config );
				udDELETE( obj1 );
				szWhere = config.GetAttrIndex( "service" ) + " IN(" + Join( ",", arrString ) + ")";
				option.SetTable( "ud_serviceperiod" );
				option.SetClass( GetProjectServicePeriod );
				option.SetWhere( szWhere );
				handler->GetUdObject( arrServicePeriod, option, &err );
				if ( !err.empty( ) ) {
					__log->Write( "при выборке периодов услуг произошли ошибки\n" );
					WriteErrors( __log, err );
				} else if ( !arrServicePeriod.empty( ) ) { // нашлись периоды
					udArrBase arrPeriodToTake; // множество периодов, которые совпали с услугами тура
					if ( FindPeriod( arrTourService, arrServicePeriod, arrPeriodToTake ) > 0 ) {
						// выгребаем цены по периодам
						map< udUInt, udUInt > mapServiceAmount;
						udUInt num = CountTourTourist( obj->get( "id" )->toUInt( ) );
						udArrBase arrPriceToTake, arrPrice;
						// подгружаем цены по услугам
						LoadPriceByPeriod( arrPeriodToTake, arrPriceToTake, handler );
						// дополнительно умножаем цены на количество услуги в турах
						BindServiceNumToPrice( arrPriceToTake, arrTourService );
						//////////////////////////////////////////////////////////////////////////
						// необходимо по каждой услуге просчитать количество человек пертендующих на нее
						CountServiceAmount( iGroupId, obj, pGroupTour, arrPeriodToTake, mapServiceAmount );
						//////////////////////////////////////////////////////////////////////////
						// находим цены по каждой услуге (период+количество человек)
						if ( FindPrice( mapServiceAmount, arrPeriodToTake, arrPriceToTake, arrPrice ) > 0 ) {
							udArrBase arrSumm;
							if ( CollectTourCost( obj, arrPrice, arrSumm ) > 0 ) {
								// сносим старые
								udPMySql db = ( udPMySql ) g_pApp->GetDatabase( );
								udPBase pPartnerSumm = NULL, summ = NULL;
								udUInt id = 0;
								db->Query(
									"DELETE FROM `ud_fsumm` WHERE "
									"`fsumm_at`=" + toString( udProjectFinanceSumm::SummAtTourCost ) + " AND "
									"`fsumm_obj`=" + obj->get( "id" )->toString( )
								);
								// добавляем новые
								udHandlerOption option1;
								option.Clear( );
								option.SetTable( "ud_fsumm" );
								option.SetIgnoreAttr( "partner" );
								option1.SetTable( "ud_fsummtopartner" );
								log = "\n\nfinalsumm:\n";
								for( udArrBase::iterator i = arrSumm.begin( ); i != arrSumm.end( ); ++i ) {
									summ = *i;
									handler->AddUdObject( summ, option );
									id = ( udUInt ) db->GetInsertId( );
									pPartnerSumm = new udProjectFinanceSummToPartner;
									pPartnerSumm->get( "summ_id" )->ini( id );
									pPartnerSumm->get( "partner_id" )->ini( summ->get( "partner" )->toUInt( ) );
									handler->AddUdObject( pPartnerSumm, option1 );
									udDELETE( pPartnerSumm );
								}
							} else {
								__log->Write( "не удалось собрать цены в набор сумм\n" );
							}
							ClearArrBase( arrSumm );
						} else {
							__log->Write( "не удалось найти цену по каждой услуге\n" );
						}
						ClearArrBase( arrPriceToTake );
						ClearArrBase( arrPrice );
					} else {
						__log->Write( "не удалось найти период по каждой услуге\n" );
					}
					ClearArrBase( arrPeriodToTake );
				} else {
					__log->Write( "не найдены периоды услуг\n" );
				}
				ClearArrBase( arrServicePeriod );
			} else {
				__log->Write( "не найдены услуги тура\n" );
			}
			ClearArrBase( arrTourService );
			//////////////////////////////////////////////////////////////////////////////////////////
		}
		ClearArrBase( arr );
		//////////////////////////////////////////////////////////////////////////////////////////////
	} // void Cost

	void CHTour::LoadSumm( udArrBase& arr, udPBase obj, udInt type ) {
		LoadSumm( arr, obj->get( "id" )->toUInt( ), type );
	} // void LoadSumm

	void CHTour::LoadSumm( udArrBase& arr, udUInt id, udInt type ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udPBase obj1 = NULL;
		
		// наполняем конфиг
		obj1 = new udProjectFinanceSumm;
		obj1->GetConfig( config );
		udDELETE( obj1 );
		// наполняем опции
		option.SetTable( "ud_fsumm" );
		option.SetClass( GetProjectFinanceSumm2 );
		option.SetIgnoreAttr( "currency_" );
		option.AddLeftJoin(
			"name", "fsumm_currency_", "ud_currency",
			"`ud_currency`.`currency_id`=`ud_fsumm`.`fsumm_currency`", GetProjectCurrency
		);
		option.SetWhere(
			config.GetAttrIndex( "at" ) + "=" + toString( type ) + " AND " +
			config.GetAttrIndex( "obj" ) + "=" + toString( id )
		);

		if ( ( type == udProjectFinanceSumm::SummAtTourCost ) || ( type == udProjectFinanceSumm::SummAtService ) ) {
			option.SetClass( GetProjectFinanceSumm3 );
			option.SetIgnoreAttr( "partner" );
			option.SetIgnoreAttr( "partner_" );
			option.AddLeftJoin(
				"partner_id", "fsumm_partner", "ud_fsummtopartner",
				"`ud_fsumm`.`fsumm_id`=`ud_fsummtopartner`.`fsummtopartner_summ_id`", GetProjectFinanceSummToPartner
			);
			option.AddLeftJoin(
				"name", "fsumm_partner_", "ud_partner",
				"`ud_partner`.`partner_id`=`ud_fsummtopartner`.`fsummtopartner_partner_id`", GetProjectPartner
			);
		}

		handler->GetUdObject( arr, option );
	} // void LoadSumm

	void CHTour::OpCalculateFinal( udPSocket sock ) {
		/**
		 * 1. считываем ID тура, для которого необходимо просчитать итоговую цену
		 * 2. составляем набор сумм
		 * 3. отправляем суммы обратно
		 */
		//////////////////////////////////////////////////////////////////////////////////////////////
		udPLog __log = g_pApp->GetLog( );
		udType id( ( udUInt ) 0 );
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		id.read( sock );

		if ( !id.toUInt( ) ) {
			__log->Write( "CHTour::OpCalculateFinal, введен неверный ID тура\n" );
			return;
		}

		__log->Write( "CHTour::OpCalculateFinal, введен ID тура: " + id.toString( ) + "\n" );
		//////////////////////////////////////////////////////////////////////////////////////////////
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arr, arrRet;
		udArrError err;
		string szWhere = "", log = "";
		udArrString arrString;
		udPBase obj = NULL, pTour = NULL;
		//////////////////////////////////////////////////////////////////////////////////////////////
		// выгребаем тур
		obj = new udProjectTour;
		obj->GetConfig( config );
		udDELETE( obj );
		option.SetTable( "ud_tour" );
		option.SetClass( GetProjectTour );
		option.SetWhere( config.GetAttrIndex( "id" ) + "=" + id.toString( ) );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			__log->Write( "CHTour::OpCalculateFinal, ошибки при выборке тура\n" );
			WriteErrors( __log, err );
			return;
		} else if ( arr.empty( ) || !arr[ 0 ] ) {
			__log->Write( "CHTour::OpCalculateFinal, тур не найден\n" );
			return;
		}

		pTour = arr[ 0 ];
		__log->Write( "CHTour::OpCalculateFinal, найден тур ID: " + pTour->get( "id" )->toString( ) + "\n" );
		if ( id.toUInt( ) != pTour->get( "id" )->toUInt( ) ) {
			__log->Write( "CHTour::OpCalculateFinal, найден другой тур\n" );
			return;
		}
		arr.clear( );
		config.Clear( );
		//////////////////////////////////////////////////////////////////////////////////////////////
		__log->Write( "CHTour::OpCalculateFinal, составление набора сумм\n" );
		CHTour::CalculateFinal( arrRet, pTour );
		__log->Write( "CHTour::OpCalculateFinal, составление набора сумм завершен\n" );
		//////////////////////////////////////////////////////////////////////////////////////////////
		if ( arrRet.empty( ) ) {
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 0 );
		} else {
			__log->Write( "got result, size=" + toString( arrRet.size( ) ) + "\n" );

			log = "\n";
			for( udArrBase::iterator i = arrRet.begin( ); i != arrRet.end( ); ++i ) {
				( *i )->dump( log );
				log += "\n";
			}
			__log->Write( log );

			__log->Write( "deleting old summ\n" );
			// сносим старые
			( ( udPMySql ) g_pApp->GetDatabase( ) )->Query(
				"DELETE FROM `ud_fsumm` WHERE "
				"`fsumm_at`=" + toString( udProjectFinanceSumm::SummAtTourPrice ) + " AND "
				"`fsumm_obj`=" + pTour->get( "id" )->toString( )
			);
			// добавляем новые
			option.Clear( );
			option.SetTable( "ud_fsumm" );

			handler->AddUdObject( arrRet, option );

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );

			udBinaryData data;

			LoadListDataByAttr( arrRet, handler, GetProjectCurrency, "ud_currency", "currency", "name", "currency_" );
			WriteInstances( arrRet, data );

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->Send( data );

			data.clear( );
			ClearArrBase( arrRet );
		}

		udDELETE( pTour );

		char code = 0;
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		sock->ReadByte( &code );
		//////////////////////////////////////////////////////////////////////////////////////////////
	} // void OpCalculateFinal

	void CHTour::OpAddToGroup( udPSocket sock ) {
		udType	iIdTour( ( udUInt ) 0 ),
				iIdGroup( ( udUInt ) 0 ),
				iPeriodIndex( ( udUInt ) 0 );

		if ( ( iIdTour.read( sock ) != 1 ) || ( iIdGroup.read( sock ) != 1 ) || ( iPeriodIndex.read( sock ) != 1 ) ) {
			return;
		}

		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udArrError err;
		udPBase obj = NULL;

		obj = new udProjectGroupTour;
		obj->get( "tour" )->ini( iIdTour.toUInt( ) );
		obj->get( "group" )->ini( iIdGroup.toUInt( ) );
		obj->get( "period" )->ini( iPeriodIndex.toUInt( ) );

		option.SetTable( "ud_grouptour" );
		handler->AddUdObject( obj, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( g_pApp->GetLog( ), err );
		}

		g_pApp->GetLog( )->Write(
			"CHTour::OpAddToGroup, "
			"tour_id=" + iIdTour.toString( ) + ", "
			"group_id=" + iIdGroup.toString( ) + ", "
			"period_index=" + iPeriodIndex.toString( ) + "\n"
		);

		sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
		sock->SendByte( ( char ) 80 );
	} // void OpAddToGroup

	void CHTour::LoadOperations( udArrBase& arr, udUInt id ) {
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udArrBase arrOperation;
		udPBase obj1 = NULL;

		// операции по оплате тура
		obj1 = new udProjectFinanceOperation;
		obj1->GetConfig( config );
		udDELETE( obj1 );
		option.SetTable( "ud_fop" );
		option.SetClass( GetProjectFinanceOperation );
		option.SetOrder( config.GetAttrIndex( "created" ) + " DESC" );
		option.SetWhere(
			config.GetAttrIndex( "type" ) + " IN(" +
			toString( ( udInt ) udProjectFinanceOperation::fotPayTour ) + "," +
			toString( ( udInt ) udProjectFinanceOperation::fotBackTour ) + "," +
			toString( ( udInt ) udProjectFinanceOperation::fotConv ) +
			") AND " + config.GetAttrIndex( "tour" ) + "=" + toString( id )
		);
		handler->GetUdObject( arrOperation, option );
		option.Clear( );
		config.Clear( );

		if ( !arrOperation.empty( ) ) {
			udArrString arrId;
			udArrBase arrSumm;

			for( udArrBase::iterator i = arrOperation.begin( ); i != arrOperation.end( ); ++i ) {
				arrId.push_back( ( *i )->get( "id" )->toString( ) );
			}

			// наполняем конфиг
			obj1 = new udProjectFinanceSumm;
			obj1->GetConfig( config );
			udDELETE( obj1 );
			// наполняем опции
			option.SetTable( "ud_fsumm" );
			option.SetClass( GetProjectFinanceSumm2 );
			option.SetIgnoreAttr( "currency_" );
			option.AddLeftJoin(
				"name", "fsumm_currency_", "ud_currency",
				"`ud_currency`.`currency_id`=`ud_fsumm`.`fsumm_currency`", GetProjectCurrency
			);
			option.SetWhere(
				config.GetAttrIndex( "at" ) + "=" + toString( udProjectFinanceSumm::SummAtOperation ) + " AND " +
				config.GetAttrIndex( "obj" ) + " IN(" + Join( ",", arrId ) + ")"
			);

			handler->GetUdObject( arrSumm, option );
			if ( !arrSumm.empty( ) ) {
				udPBase pOperation		= NULL,
						pSumm			= NULL,
						pAdd			= NULL;
				udUInt	iIdOperation	= 0;

				for( udArrBase::iterator i = arrOperation.begin( ); i != arrOperation.end( ); ++i ) {
					pOperation = *i;
					pAdd = new udBase;
					pAdd->get( "created" )->ini( pOperation->get( "created" )->toTime( ) );
					pAdd->get( "type" )->ini( pOperation->get( "type" )->toInt( ) );
					iIdOperation = pOperation->get( "id" )->toUInt( );
					
					for( udArrBase::iterator j = arrSumm.begin( ); j != arrSumm.end( ); ++j ) {
						pSumm = *j;

						if ( iIdOperation == pSumm->get( "obj" )->toUInt( ) ) {
							pAdd->get( "summ" )->ini( pSumm->get( "summ" )->toFloat( ) );
							pAdd->get( "currency" )->ini( pSumm->get( "currency" )->toUInt( ) );
							pAdd->get( "summ_type" )->ini( pSumm->get( "type" )->toInt( ) );
							pAdd->get( "currency_" )->ini( pSumm->get( "currency_" )->toWString( ) );
							break;
						}
					}

					arr.push_back( pAdd );
				}

				ClearArrBase( arrSumm );
			}

			option.Clear( );
			config.Clear( );
			arrId.clear( );
			ClearArrBase( arrOperation );
		}
	} // void LoadOperations

	void CHTour::CalculateFinal( udArrBase& arrRet, udPBase pTour ) {
		udPLog __log = g_pApp->GetLog( );
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udPBase obj = NULL;
		udArrBase arr;
		udArrError err;
		string szWhere = "", log = "";
		udArrString arrString;
		//////////////////////////////////////////////////////////////////////////////////////////////
		// проверяем, что тур входит в какую то группу
		obj = new udProjectGroupTour;
		obj->GetConfig( config );
		udDELETE( obj );
		szWhere = config.GetAttrIndex( "tour" ) + "=" + pTour->get( "id" )->toString( );
		option.SetTable( "ud_grouptour" );
		option.SetClass( GetProjectGroupTour );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
		} else if ( arr.empty( ) ) { // не состоит в группе
			__log->Write( "тур не состоит в группе\n" );
			udArrBase arrTourService;
			config.Clear( );
			obj = new udProjectTourService;
			obj->GetConfig( config );
			udDELETE( obj );
			option.SetTable( "ud_tourservice" );
			option.SetClass( GetProjectTourService );
			option.SetWhere( config.GetAttrIndex( "tour" ) + "=" + pTour->get( "id" )->toString( ) );
			handler->GetUdObject( arrTourService, option, &err );
			if ( !err.empty( ) ) {
				__log->Write( "при выборке связей тура и услуг произошли ошибки\n" );
				WriteErrors( __log, err );
			} else if ( !arrTourService.empty( ) ) { // есть услуги (иначе аномалия)
				udArrBase arrServicePeriod; // периоды услуг
				for( udArrBase::iterator i = arrTourService.begin( ); i != arrTourService.end( ); ++i ) {
					arrString.push_back( ( *i )->get( "service" )->toString( ) );
				}
				config.Clear( );
				obj = new udProjectServicePeriod;
				obj->GetConfig( config );
				udDELETE( obj );
				szWhere = config.GetAttrIndex( "service" ) + " IN(" + Join( ",", arrString ) + ")";
				option.SetTable( "ud_serviceperiod" );
				option.SetClass( GetProjectServicePeriodForCost );
				option.SetWhere( szWhere );
				option.SetIgnoreAttr( "partner" );
				option.AddLeftJoin(
					"partner", "serviceperiod_partner", "ud_service",
					"`ud_service`.`service_id`=`ud_serviceperiod`.`serviceperiod_service`", GetProjectService
				);
				handler->GetUdObject( arrServicePeriod, option, &err );
				if ( !err.empty( ) ) {
					__log->Write( "при выборке периодов услуг произошли ошибки\n" );
					WriteErrors( __log, err );
				} else if ( !arrServicePeriod.empty( ) ) { // нашлись периоды
					udArrBase arrPeriodToTake; // множество периодов, которые совпали с услугами тура
					if ( FindPeriod( arrTourService, arrServicePeriod, arrPeriodToTake ) > 0 ) {
						// выгребаем цены по периодам
						udUInt num = CountTourTourist( pTour->get( "id" )->toUInt( ) );
						udArrBase arrPriceToTake, arrPrice;
						// подгружаем цены по услугам
						LoadPriceByPeriod( arrPeriodToTake, arrPriceToTake, handler );
						// дополнительно умножаем цены на количество услуги в турах
						BindServiceNumToPrice( arrPriceToTake, arrTourService );
						// находим цены по каждой услуге (период+количество человек)
						if ( FindPrice( num, arrPeriodToTake, arrPriceToTake, arrPrice ) > 0 ) {
							udArrBase arrSumm;
							if ( CollectTourCost( pTour, arrPrice, arrSumm, udHTour::priceRecommend ) > 0 ) {
								log = "\nCHTour::OpCalculateFinal, summ:\n";
								for( udArrBase::iterator i = arrSumm.begin( ); i != arrSumm.end( ); ++i ) {
									( *i )->dump( log );
									log += "\n";
								}
								__log->Write( log );

								if ( !arrSumm.empty( ) ) {
									// собираем суммы, Тип и партнер игнорируются, все завязывается на валюту
									map< udUInt, udPBase > mapCurrencyToSumm; // чтобы не перебирать набор сумм
									map< udUInt, udPBase >::iterator itCurrencyToSumm;
									udPBase summ = NULL, add = NULL;
									udUInt iCurrency = 0;
									udFloat fSumm = 0.0f, fTmp = 0.0f;

									for( udArrBase::iterator i = arrSumm.begin( ); i != arrSumm.end( ); ++i ) {
										summ = *i;
										iCurrency = summ->get( "currency" )->toUInt( );
										itCurrencyToSumm = mapCurrencyToSumm.find( iCurrency );

										if ( itCurrencyToSumm == mapCurrencyToSumm.end( ) ) {
											add = new udProjectFinanceSumm;
											add->get( "summ" )->ini( summ->get( "summ" )->toFloat( ) );
											add->get( "currency" )->ini( iCurrency );
											add->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtTourPrice );
											add->get( "obj" )->ini( pTour->get( "id" )->toUInt( ) );
											arrRet.push_back( add );
											mapCurrencyToSumm[ iCurrency ] = add;
										} else {
											fTmp = itCurrencyToSumm->second->get( "summ" )->toFloat( );
											itCurrencyToSumm->second->get( "summ" )->ini( fTmp + summ->get( "summ" )->toFloat( ) );
										}
									}
								}
							} else {
								__log->Write( "не удалось собрать цены в набор сумм\n" );
							}
							ClearArrBase( arrSumm );
						} else {
							__log->Write( "не удалось найти цену по каждой услуге\n" );
						}
						ClearArrBase( arrPriceToTake );
						ClearArrBase( arrPrice );
					} else {
						__log->Write( "не удалось найти период по каждой услуге\n" );
					}
					ClearArrBase( arrPeriodToTake );
				} else {
					__log->Write( "не найдены периоды услуг\n" );
				}
				ClearArrBase( arrServicePeriod );
			} else {
				__log->Write( "не найдены услуги тура\n" );
			}
			ClearArrBase( arrTourService );
		} else { // состоит в какой-то группе
			__log->Write( "тур состоит в группе\n" );
			udPBase pGroupTour = arr[ 0 ];
			udUInt iGroupId = pGroupTour->get( "group" )->toUInt( );
			//////////////////////////////////////////////////////////////////////////////////////////
			udArrBase arrTourService;
			config.Clear( );
			obj = new udProjectTourService;
			obj->GetConfig( config );
			udDELETE( obj );
			szWhere = config.GetAttrIndex( "tour" ) + "=" + pTour->get( "id" )->toString( );
			option.SetTable( "ud_tourservice" );
			option.SetClass( GetProjectTourService );
			option.SetWhere( szWhere );
			handler->GetUdObject( arrTourService, option, &err );
			if ( !err.empty( ) ) {
				__log->Write( "при выборке связей тура и услуг произошли ошибки\n" );
				WriteErrors( __log, err );
			} else if ( !arrTourService.empty( ) ) { // есть услуги (иначе аномалия)
				udArrBase arrServicePeriod; // периоды услуг
				for( udArrBase::iterator i = arrTourService.begin( ); i != arrTourService.end( ); ++i ) {
					arrString.push_back( ( *i )->get( "service" )->toString( ) );
				}
				config.Clear( );
				obj = new udProjectServicePeriod;
				obj->GetConfig( config );
				udDELETE( obj );
				szWhere = config.GetAttrIndex( "service" ) + " IN(" + Join( ",", arrString ) + ")";
				option.SetTable( "ud_serviceperiod" );
				option.SetClass( GetProjectServicePeriod );
				option.SetWhere( szWhere );
				handler->GetUdObject( arrServicePeriod, option, &err );
				if ( !err.empty( ) ) {
					__log->Write( "при выборке периодов услуг произошли ошибки\n" );
					WriteErrors( __log, err );
				} else if ( !arrServicePeriod.empty( ) ) { // нашлись периоды
					udArrBase arrPeriodToTake; // множество периодов, которые совпали с услугами тура
					if ( FindPeriod( arrTourService, arrServicePeriod, arrPeriodToTake ) > 0 ) {
						// выгребаем цены по периодам
						map< udUInt, udUInt > mapServiceAmount;
						udUInt num = CountTourTourist( pTour->get( "id" )->toUInt( ) );
						udArrBase arrPriceToTake, arrPrice;
						// подгружаем цены по услугам
						LoadPriceByPeriod( arrPeriodToTake, arrPriceToTake, handler );
						// дополнительно умножаем цены на количество услуги в турах
						BindServiceNumToPrice( arrPriceToTake, arrTourService );
						//////////////////////////////////////////////////////////////////////////
						// необходимо по каждой услуге просчитать количество человек пертендующих на нее
						CountServiceAmount( iGroupId, pTour, pGroupTour, arrPeriodToTake, mapServiceAmount );
						//////////////////////////////////////////////////////////////////////////
						// находим цены по каждой услуге (период+количество человек)
						if ( FindPrice( mapServiceAmount, arrPeriodToTake, arrPriceToTake, arrPrice ) > 0 ) {
							udArrBase arrSumm;
							if ( CollectTourCost( pTour, arrPrice, arrSumm, udHTour::priceRecommend ) > 0 ) {
								log = "\nCHTour::OpCalculateFinal, summ:\n";
								for( udArrBase::iterator i = arrSumm.begin( ); i != arrSumm.end( ); ++i ) {
									( *i )->dump( log );
									log += "\n";
								}
								__log->Write( log );

								if ( !arrSumm.empty( ) ) {
									// собираем суммы, Тип и партнер игнорируются, все завязывается на валюту
									map< udUInt, udPBase > mapCurrencyToSumm; // чтобы не перебирать набор сумм
									map< udUInt, udPBase >::iterator itCurrencyToSumm;
									udPBase summ = NULL, add = NULL;
									udUInt iCurrency = 0;
									udFloat fSumm = 0.0f, fTmp = 0.0f;

									for( udArrBase::iterator i = arrSumm.begin( ); i != arrSumm.end( ); ++i ) {
										summ = *i;
										iCurrency = summ->get( "currency" )->toUInt( );
										itCurrencyToSumm = mapCurrencyToSumm.find( iCurrency );

										if ( itCurrencyToSumm == mapCurrencyToSumm.end( ) ) {
											add = new udProjectFinanceSumm;
											add->get( "summ" )->ini( summ->get( "summ" )->toFloat( ) );
											add->get( "currency" )->ini( iCurrency );
											add->get( "at" )->ini( ( udInt ) udProjectFinanceSumm::SummAtTourPrice );
											add->get( "obj" )->ini( pTour->get( "id" )->toUInt( ) );
											arrRet.push_back( add );
											mapCurrencyToSumm[ iCurrency ] = add;
										} else {
											fTmp = itCurrencyToSumm->second->get( "summ" )->toFloat( );
											itCurrencyToSumm->second->get( "summ" )->ini( fTmp + summ->get( "summ" )->toFloat( ) );
										}
									}
								}
							} else {
								__log->Write( "не удалось собрать цены в набор сумм\n" );
							}
							ClearArrBase( arrSumm );
						} else {
							__log->Write( "не удалось найти цену по каждой услуге\n" );
						}
						ClearArrBase( arrPriceToTake );
						ClearArrBase( arrPrice );
					} else {
						__log->Write( "не удалось найти период по каждой услуге\n" );
					}
					ClearArrBase( arrPeriodToTake );
				} else {
					__log->Write( "не найдены периоды услуг\n" );
				}
				ClearArrBase( arrServicePeriod );
			} else {
				__log->Write( "не найдены услуги тура\n" );
			}
			ClearArrBase( arrTourService );
			//////////////////////////////////////////////////////////////////////////////////////////
		}
		ClearArrBase( arr );
	} // void CalculateFinal

	void CHTour::CollectServicePriceFinal( udArrBase& arrRet, udPBase pTour ) {
		udPLog __log = g_pApp->GetLog( );
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udPBase obj = NULL;
		udArrBase arr;
		udArrError err;
		string szWhere = "", log = "";
		udArrString arrString;
		//////////////////////////////////////////////////////////////////////////////////////////////
		// проверяем, что тур входит в какую то группу
		obj = new udProjectGroupTour;
		obj->GetConfig( config );
		udDELETE( obj );
		szWhere = config.GetAttrIndex( "tour" ) + "=" + pTour->get( "id" )->toString( );
		option.SetTable( "ud_grouptour" );
		option.SetClass( GetProjectGroupTour );
		option.SetWhere( szWhere );
		handler->GetUdObject( arr, option, &err );
		if ( !err.empty( ) ) {
			WriteErrors( __log, err );
		} else if ( arr.empty( ) ) { // не состоит в группе
			__log->Write( "тур не состоит в группе\n" );
			udArrBase arrTourService;
			config.Clear( );
			obj = new udProjectTourService;
			obj->GetConfig( config );
			udDELETE( obj );
			option.SetTable( "ud_tourservice" );
			option.SetClass( GetProjectTourService );
			option.SetWhere( config.GetAttrIndex( "tour" ) + "=" + pTour->get( "id" )->toString( ) );
			handler->GetUdObject( arrTourService, option, &err );
			if ( !err.empty( ) ) {
				__log->Write( "при выборке связей тура и услуг произошли ошибки\n" );
				WriteErrors( __log, err );
			} else if ( !arrTourService.empty( ) ) { // есть услуги (иначе аномалия)
				udArrBase arrServicePeriod; // периоды услуг
				for( udArrBase::iterator i = arrTourService.begin( ); i != arrTourService.end( ); ++i ) {
					arrString.push_back( ( *i )->get( "service" )->toString( ) );
				}
				config.Clear( );
				obj = new udProjectServicePeriod;
				obj->GetConfig( config );
				udDELETE( obj );
				szWhere = config.GetAttrIndex( "service" ) + " IN(" + Join( ",", arrString ) + ")";
				option.SetTable( "ud_serviceperiod" );
				option.SetClass( GetProjectServicePeriodForCost );
				option.SetWhere( szWhere );
				option.SetIgnoreAttr( "partner" );
				option.AddLeftJoin(
					"partner", "serviceperiod_partner", "ud_service",
					"`ud_service`.`service_id`=`ud_serviceperiod`.`serviceperiod_service`", GetProjectService
				);
				handler->GetUdObject( arrServicePeriod, option, &err );
				if ( !err.empty( ) ) {
					__log->Write( "при выборке периодов услуг произошли ошибки\n" );
					WriteErrors( __log, err );
				} else if ( !arrServicePeriod.empty( ) ) { // нашлись периоды
					udArrBase arrPeriodToTake; // множество периодов, которые совпали с услугами тура
					if ( FindPeriod( arrTourService, arrServicePeriod, arrPeriodToTake ) > 0 ) {
						// выгребаем цены по периодам
						udUInt num = CountTourTourist( pTour->get( "id" )->toUInt( ) );
						udArrBase arrPriceToTake, arrPrice;
						// подгружаем цены по услугам
						LoadPriceByPeriod( arrPeriodToTake, arrPriceToTake, handler );
						// дополнительно умножаем цены на количество услуги в турах
						BindServiceNumToPrice( arrPriceToTake, arrTourService );
						// находим цены по каждой услуге (период+количество человек)
						FindPrice( num, arrPeriodToTake, arrPriceToTake, arrRet );
						ClearArrBase( arrPriceToTake );
					} else {
						__log->Write( "не удалось найти период по каждой услуге\n" );
					}
					ClearArrBase( arrPeriodToTake );
				} else {
					__log->Write( "не найдены периоды услуг\n" );
				}
				ClearArrBase( arrServicePeriod );
			} else {
				__log->Write( "не найдены услуги тура\n" );
			}
			ClearArrBase( arrTourService );
		} else { // состоит в какой-то группе
			__log->Write( "тур состоит в группе\n" );
			udPBase pGroupTour = arr[ 0 ];
			udUInt iGroupId = pGroupTour->get( "group" )->toUInt( );
			//////////////////////////////////////////////////////////////////////////////////////////
			udArrBase arrTourService;
			config.Clear( );
			obj = new udProjectTourService;
			obj->GetConfig( config );
			udDELETE( obj );
			szWhere = config.GetAttrIndex( "tour" ) + "=" + pTour->get( "id" )->toString( );
			option.SetTable( "ud_tourservice" );
			option.SetClass( GetProjectTourService );
			option.SetWhere( szWhere );
			handler->GetUdObject( arrTourService, option, &err );
			if ( !err.empty( ) ) {
				__log->Write( "при выборке связей тура и услуг произошли ошибки\n" );
				WriteErrors( __log, err );
			} else if ( !arrTourService.empty( ) ) { // есть услуги (иначе аномалия)
				udArrBase arrServicePeriod; // периоды услуг
				for( udArrBase::iterator i = arrTourService.begin( ); i != arrTourService.end( ); ++i ) {
					arrString.push_back( ( *i )->get( "service" )->toString( ) );
				}
				config.Clear( );
				obj = new udProjectServicePeriod;
				obj->GetConfig( config );
				udDELETE( obj );
				szWhere = config.GetAttrIndex( "service" ) + " IN(" + Join( ",", arrString ) + ")";
				option.SetTable( "ud_serviceperiod" );
				option.SetClass( GetProjectServicePeriod );
				option.SetWhere( szWhere );
				handler->GetUdObject( arrServicePeriod, option, &err );
				if ( !err.empty( ) ) {
					__log->Write( "при выборке периодов услуг произошли ошибки\n" );
					WriteErrors( __log, err );
				} else if ( !arrServicePeriod.empty( ) ) { // нашлись периоды
					udArrBase arrPeriodToTake; // множество периодов, которые совпали с услугами тура
					if ( FindPeriod( arrTourService, arrServicePeriod, arrPeriodToTake ) > 0 ) {
						// выгребаем цены по периодам
						map< udUInt, udUInt > mapServiceAmount;
						udUInt num = CountTourTourist( pTour->get( "id" )->toUInt( ) );
						udArrBase arrPriceToTake, arrPrice;
						// подгружаем цены по услугам
						LoadPriceByPeriod( arrPeriodToTake, arrPriceToTake, handler );
						// дополнительно умножаем цены на количество услуги в турах
						BindServiceNumToPrice( arrPriceToTake, arrTourService );
						//////////////////////////////////////////////////////////////////////////
						// необходимо по каждой услуге просчитать количество человек пертендующих на нее
						CountServiceAmount( iGroupId, pTour, pGroupTour, arrPeriodToTake, mapServiceAmount );
						//////////////////////////////////////////////////////////////////////////
						// находим цены по каждой услуге (период+количество человек)
						FindPrice( mapServiceAmount, arrPeriodToTake, arrPriceToTake, arrRet );
						ClearArrBase( arrPriceToTake );
					} else {
						__log->Write( "не удалось найти период по каждой услуге\n" );
					}
					ClearArrBase( arrPeriodToTake );
				} else {
					__log->Write( "не найдены периоды услуг\n" );
				}
				ClearArrBase( arrServicePeriod );
			} else {
				__log->Write( "не найдены услуги тура\n" );
			}
			ClearArrBase( arrTourService );
			//////////////////////////////////////////////////////////////////////////////////////////
		}
		ClearArrBase( arr );
	} // void CollectServicePriceFinal

	void CHTour::OpGetPersAcc( udPSocket sock ) {
		udType id( ( udUInt ) 0 );

		if ( id.read( sock ) ) {
			udPHandler handler = g_pApp->GetHandler( );
			udHandlerOption option;
			udBaseConfig config;
			udArrBase arr;
			udArrError err;
			udPBase obj = NULL;
			// 1 выгребаем тур
			obj = new udProjectTour;
			obj->GetConfig( config );
			udDELETE( obj );

			option.SetWhere( config.GetAttrIndex( "id" ) + "=" + id.toString( ) );
			option.SetTable( "ud_tour" );
			option.SetClass( GetProjectTour );
			option.SetLimit( "1" );
			handler->GetUdObject( arr, option, &err );
			option.Clear( );
			config.Clear( );
			if ( !err.empty( ) ) {
				WriteErrors( g_pApp->GetLog( ), err );
				// ошибки при выборке тура
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 1 );
			} else if ( arr.empty( ) ) {
				// отсутствует тур
				sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
				sock->SendByte( ( char ) 2 );
			} else { // тур найден, ищем счет
				udPBase pTour = arr[ 0 ];
				arr.clear( );

				obj = new udProjectPersAcc;
				obj->GetConfig( config );
				udDELETE( obj );

				udUInt type = 0;

				if ( pTour->get( "emptype" )->toUInt( ) == udProjectTour::TOUR_EMPLOYER_CLIENT ) {
					type = udProjectPersAcc::PERS_ACC_CLIENT_COMPANY;
				} else if ( pTour->get( "emptype" )->toUInt( ) == udProjectTour::TOUR_EMPLOYER_CLIENTUR ) {
					type = udProjectPersAcc::PERS_ACC_CLIENTUR_COMPANY;
				} else {
					udDELETE( pTour );
					g_pApp->GetLog( )->Write( "CHTour::OpGetPersAcc, unknown employer type '" + pTour->get( "emptype" )->toString( ) + "'\n" );
					// ошибка в коде типа заказчика
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 3 );
					return;
				}

				option.SetWhere(
					config.GetAttrIndex( "type" ) + "=" + toString( type ) + " AND " +
					config.GetAttrIndex( "obj1" ) + "=" + pTour->get( "employer" )->toString( ) + " AND " +
					config.GetAttrIndex( "obj2" ) + "=" + pTour->get( "company" )->toString( )
				);
				option.SetTable( "ud_persacc" );
				option.SetClass( GetProjectPersAcc );
				option.SetLimit( "1" );
				handler->GetUdObject( arr, option, &err );
				if ( !err.empty( ) ) {
					WriteErrors( g_pApp->GetLog( ), err );
					// ошибки при выборке лицевого счета
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 4 );
				} else if ( arr.empty( ) ) {
					// отсутствует лицевой счет
					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 5 );
				} else {
					udPBase pPersAcc = arr[ 0 ];

					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 80 );

					udBinaryData data;

					pPersAcc->dump( data );

					sock->Send( data );

					data.clear( );
					ClearArrBase( arr );
				}

				udDELETE( pTour );
			}
		}
	} // void OpGetPersAcc

	udPBase GetProjectServiceForPrint( ) {
		udPBase ret = GetProjectService( );
		// атрибуты с наименованиями
		ret->get( "name_" )->ini( wstring( L"" ) );
		ret->get( "type_" )->ini( wstring( L"" ) );
		ret->get( "partner_" )->ini( wstring( L"" ) );
		// атрибуты из цены
		ret->get( "num"					)->ini( ( udUInt )	0		);	// Количество людей
		ret->get( "amount"				)->ini( ( udFloat )	0.0f	);	// Сумма
		ret->get( "currency"			)->ini( ( udUInt )	0		);	// ID валюты
		ret->get( "amount_recommend"	)->ini( ( udFloat )	0.0f	);	// Сумма "рекомендованная"
		ret->get( "currency_recommend"	)->ini( ( udUInt )	0		);	// Сумма "рекомендованная"
		ret->get( "currency_" )->ini( wstring( L"" ) );
		ret->get( "currency_recommend_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectServiceForPrint

	void CHTour::OpGetServicePrice( udPSocket sock ) {
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udType id( ( udUInt ) 0 );
		if ( id.read( sock ) != 1 ) {
			return;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		udPLog __log = g_pApp->GetLog( );
		string log = "";
		udPHandler handler = g_pApp->GetHandler( );
		udHandlerOption option;
		udBaseConfig config;
		udPBase obj = NULL;
		udArrBase arr;
		udArrError err;

		obj = new udProjectTour;
		obj->GetConfig( config );
		udDELETE( obj );

		option.SetWhere( config.GetAttrIndex( "id" ) + "=" + id.toString( ) );
		option.SetTable( "ud_tour" );
		option.SetClass( GetProjectTour );
		handler->GetUdObject( arr, option, &err );
		option.Clear( );
		config.Clear( );
		if ( !err.empty( ) || arr.empty( ) ) {
			if ( !err.empty( ) ) {
				WriteErrors( __log, err );
			}

			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 0 );
		} else {
			udPBase pTour = *( arr.begin( ) );

			udArrBase arrPrice;
			udHTour::CollectServicePriceFinal( arrPrice, pTour );

			__log->Write( "CHTour::OpGetServicePrice, size=" + toString( arrPrice.size( ) ) + "\n" );

			if ( !arrPrice.empty( ) ) {
				udArrString arrString;
				udArrBase arrService;

				for( udArrBase::iterator i = arrPrice.begin( ); i != arrPrice.end( ); ++i ) {
					arrString.push_back( ( *i )->get( "service" )->toString( ) );
				}

				obj = new udProjectService;
				obj->GetConfig( config );
				udDELETE( obj );
				//////////////////////////////////////////////////////////////////////////////////////
				option.SetWhere( config.GetAttrIndex( "id" ) + " IN(" + Join( ",", arrString ) + ")" );
				option.SetTable( "ud_service" );
				option.SetClass( GetProjectServiceForPrint );
				//////////////////////////////////////////////////////////////////////////////////////
				option.AddLeftJoin(
					"name", "service_name_", "ud_nameservice",
					"`ud_nameservice`.`nameservice_id`=`ud_service`.`service_name`", GetProjectNameService
				);
				option.AddLeftJoin(
					"name", "service_type_", "ud_tservice",
					"`ud_tservice`.`tservice_id`=`ud_service`.`service_type`", GetProjectTService
				);
				option.AddLeftJoin(
					"name", "service_partner_", "ud_partner",
					"`ud_partner`.`partner_id`=`ud_service`.`service_partner`", GetProjectPartner
				);
				option.SetSqlCalcFoundRows( true );
				//////////////////////////////////////////////////////////////////////////////////////
				option.SetIgnoreAttr( "name_" );
				option.SetIgnoreAttr( "type_" );
				option.SetIgnoreAttr( "partner_" );
				option.SetIgnoreAttr( "num" );
				option.SetIgnoreAttr( "amount" );
				option.SetIgnoreAttr( "currency" );
				option.SetIgnoreAttr( "amount_recommend" );
				option.SetIgnoreAttr( "currency_recommend" );
				option.SetIgnoreAttr( "currency_" );
				option.SetIgnoreAttr( "currency_recommend_" );
				//////////////////////////////////////////////////////////////////////////////////////
				handler->GetUdObject( arrService, option, &err );
				if ( !err.empty( ) || arrService.empty( ) ) {
					if ( !err.empty( ) ) {
						WriteErrors( __log, err );
					}

					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->SendByte( ( char ) 0 );
				} else {
					log = "\nCHTour::OpGetServicePrice, services:\n";
					udPBase add = NULL, price = NULL, service = NULL;
					udArrBase arrWrite;

					for( udArrBase::iterator i = arrPrice.begin( ); i != arrPrice.end( ); ++i ) {
						price = *i;
						add = GetProjectServiceForPrint( );

						for( udArrBase::iterator j = arrService.begin( ); j != arrService.end( ); ++j ) {
							service = *j;

							if ( service->get( "id" )->toUInt( ) == price->get( "service" )->toUInt( ) ) {
								service->apply( add );
								break;
							}
						}

						add->get( "num" )->ini( price->get( "num" )->toUInt( ) );
						add->get( "amount" )->ini( price->get( "amount" )->toFloat( ) );
						add->get( "currency" )->ini( price->get( "currency" )->toUInt( ) );
						add->get( "amount_recommend" )->ini( price->get( "amount_recommend" )->toFloat( ) );
						add->get( "currency_recommend" )->ini( price->get( "currency_recommend" )->toUInt( ) );

						arrWrite.push_back( add );

						add->dump( log );
						log += "\n";
					}

					__log->Write( log );

					LoadListDataByAttr( arrWrite, handler, GetProjectCurrency, "ud_currency", "currency", "name", "currency_" );
					LoadListDataByAttr( arrWrite, handler, GetProjectCurrency, "ud_currency", "currency_recommend", "name", "currency_recommend_" );

					udBinaryData data;
					data.add( ( char ) 80 );
					WriteInstances( arrWrite, data );

					sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
					sock->Send( data );

					data.clear( );
					ClearArrBase( arrWrite );
				}
				//////////////////////////////////////////////////////////////////////////////////////
				ClearArrBase( arrService );
				ClearArrBase( arrPrice );
			}

			ClearArrBase( arr );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		char code = 0;
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		sock->ReadByte( &code );
	} // void OpGetServicePrice

} // namespace UdSDK
