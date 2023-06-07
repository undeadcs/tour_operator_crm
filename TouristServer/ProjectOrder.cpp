
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	static void OrderFilterService( udArrBase& arr ) {
		udPHandler handler = g_pApp->GetHandler( );
		//////////////////////////////////////////////////////////////////////////////////////////
		LoadListDataByAttr( arr, handler, GetProjectTService, "ud_tservice", "type", "name", "type_" );
		LoadListDataByAttr( arr, handler, GetProjectCity, "ud_city", "city", "name", "city_" );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void OrderFilterService

	udPBase OrderSpecAppendEmployer( udBinaryData& data, udPBase obj ) {
		if ( obj->get( "emptype" )->toInt( ) == 1 ) {
			udPBase ret = LoadInstanceByForeignKey( g_pApp->GetHandler( ), obj, GetProjectClientUr, "ud_clientur", "employer", "id" );
			string log = "\nзаказчик: ";
			ret->dump( log );
			g_pApp->GetLog( )->Write( log + "\n" );
			return ret;
		}
		return LoadInstanceByForeignKey( g_pApp->GetHandler( ), obj, GetProjectClient, "ud_client", "employer", "id" );
	} // udPBase OrderSpecAppendEmployer

	static void OrderFilterAttach( udArrBase& arrAdd, udArrBase& arrUpd, udArrBase& arrDel, udPBase obj ) {
		udPAppConfig pAppConfig = g_pApp->GetConfig( );
		wstring tmp_dir = pAppConfig->get( "temporary_dir" )->toWString( ),
			client_dir = pAppConfig->get( "files_dir" )->toWString( ) + L"\\order",
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
	} // void OrderFilterAttach

	udPBase GetProjectOrder2( ) {
		udPBase ret = GetProjectOrder( );
		ret->get( "manager2_" )->ini( wstring( L"" ) );
		ret->get( "employer_" )->ini( wstring( L"" ) );
		ret->get( "direct_" )->ini( wstring( L"" ) );
		ret->get( "status_" )->ini( wstring( L"" ) );
		ret->get( "employer2_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectOrder2

	CHOrder::CHOrder( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_order";
		m_szUpdIndex	= "id";
		m_szName		= "order";
		m_fnGetObject	= GetProjectOrder;

		this->AddManagedTable( "ud_orderroute", "id", GetProjectOrderRoute );
		this->AddManagedTable( "ud_orderservice", "id", GetProjectOrderService );
		this->AddManagedTable( "ud_attach", "id", GetProjectAttach );

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_user", "manager2", "id", GetProjectUser );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_sorder", "state", "id", GetProjectSOrder );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_client", "employer", "id", GetProjectClient,
			"", "", NULL, NULL, NULL, OrderSpecAppendEmployer );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_direct", "direct", "id", GetProjectDirect );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_orderroute", "order", "id", GetProjectOrderRoute, "`orderroute_id` ASC" );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_orderservice", "order", "id", GetProjectOrderService, "`orderservice_id` ASC", "", OrderFilterService );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_attach", "obj", "id", GetProjectAttach, "`attach_id` ASC", "`attach_type`=" + toString( udProjectAttach::ATTACH_ORDER ) );//,
			//NULL, NULL, OrderFilterAttach );
		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_cldoc", "document", "id", GetProjectClDoc, "`cldoc_id` ASC" );

		m_pObjFilter->get( "date1" )->ini( datetime( ) );
		m_pObjFilter->get( "date2" )->ini( datetime( ) );
		m_pObjFilter->get( "manager" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "amount1" )->ini( ( udDWord ) 0 );
		m_pObjFilter->get( "amount2" )->ini( ( udDWord ) 0 );
		m_pObjFilter->get( "employer" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "emptype" )->ini( ( udInt ) udProjectOrder::ORDER_EMPLOYER_CLIENT );
		m_pObjFilter->get( "direct" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "request" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "tour" )->ini( ( udUInt ) 0 );
	}

	CHOrder::CHOrder( const CHOrder& objCopy ) {
	}

	CHOrder::~CHOrder( ) {
	}

	CHOrder& CHOrder::operator =( const CHOrder& objCopy ) {
		return *this;
	}

	void CHOrder::Init( ) {
		udStateHandlerSimple::Init( );
		wstring folder = L"";
		udPAppConfig appConfig = g_pApp->GetConfig( );
		//folder = appConfig->get( "temporary_dir" )->toWString( ) + L"\\order";
		//CreateDirectory( folder.data( ), NULL );
		folder = appConfig->get( "files_dir" )->toWString( ) + L"\\order";
		CreateDirectory( folder.data( ), NULL );
	} // void Init

	udInt CHOrder::AddObj( udPBase obj, bool save_info ) {
		if ( udStateHandlerSimple::AddObj( obj, save_info ) == 1 ) {
			// дополнительно создаем папку для приложений
			wstring dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
				L"\\order\\" + obj->get( "id" )->toWString( );
			CreateDirectory( dir.data( ), NULL );
			g_pApp->GetLog( )->Write( "'" + toString( dir ) + "'\n" );
			return 1;
		}
		return 0;
	} // udInt AddObj

	void CHOrder::DeleteLinkedObjects( udPBase obj ) {
		udPDatabase db = g_pApp->GetDatabase( );
		string query = "", szWhere = "";
		udBaseConfig config;
		udPBase obj1;
		udPType type = NULL;

		// приложение
		obj1 = new udProjectAttach;
		obj1->GetConfig( config );
		delete obj1;
		type = new udType( ( udInt ) ( udProjectAttach::ATTACH_ORDER ) );
		szWhere = config.GetAttrIndex( "type" ) + "=" + type->toString( ) +
			config.GetAttrIndex( "obj" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );
		wstring folder = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
			L"\\order\\" + obj->get( "id" )->toWString( );
		// папку удалить
		DirectoryClear( folder );
		RemoveDirectory( folder.data( ) );

		// муршрут
		obj1 = new udProjectOrderRoute;
		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "order" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );

		// услуги
		obj1 = new udProjectOrderService;
		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "order" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );
	} // void DeleteLinkedObjects

	udInt CHOrder::FindObj( udPBase obj, const string& attr, bool apply ) {
		if ( apply ) {
			return 0;
		}
		// при обновлении пожалуйста, ищем по ID и пох
		return udStateHandlerSimple::FindObj( obj, attr, apply );
	} // bool FindObj

	udInt CHOrder::DelObj( udPBase obj, bool save_info ) {
		if ( udStateHandlerSimple::DelObj( obj, save_info ) == 1 ) {
			// удаляем связанные объекты
			this->DeleteLinkedObjects( obj );
			return 1;
		}
		return 0;
	} // udInt DelObj

	void CHOrder::ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj ) {
		if ( ( opId == opidAdd1 ) || ( opId == opidUpd1 ) ) {
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
				wstring dst_dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) + L"\\order\\" + obj->get( "id" )->toWString( );
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
			add->get( "type_dst" )->ini( udProjectObjectLink::objlnkOrder );
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
			udDELETE( add );
		}*/
		//////////////////////////////////////////////////////////////////////////////////////////
		}
	} // void ExtraDataRecieve

	void CHOrder::ExtraDataSending( udPSocket sock, udPBase obj ) {
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
		obj1 = new udProjectAttach;
		obj1->GetConfig( config );
		udDELETE( obj1 );
		szWhere = config.GetAttrIndex( "obj" ) + "=" + obj->get( "id" )->toString( ) + " AND " +
			config.GetAttrIndex( "type" ) + "=" + toString( udProjectAttach::ATTACH_ORDER );
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
			udBinaryData data;
			data.add( arr.size( ) );
			sock->Send( data );
			udInt err = 0;
			udFileTransfer objFileTransfer;
			udPBase obj1 = NULL;
			string log = "\n";
			wstring filename, path,
				client_dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
				L"\\order\\" + obj->get( "id" )->toWString( );
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
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ExtraDataSending

	bool CHOrder::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_order" );
		option.SetClass( GetProjectOrder2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "manager2" ) {
			szOrder = "`order_manager2` ";
		} else if ( orderby == "employer" ) {
			szOrder = "`order_employer_` " + szOrderType + ", `order_employer2_` " + szOrderType;
		} else if ( orderby == "direct" ) {
			szOrder = "`order_direct_` ";
		} else {
			szOrder = "`order_" + orderby + "` ";
		}
		if ( !szOrder.empty( ) && ( orderby != "employer" ) ) {
			szOrder += szOrderType;
		}
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		option.AddLeftJoin(
			"fio", "order_manager2_", "ud_user",
			"`ud_user`.`user_id`=`ud_order`.`order_manager2`", GetProjectUser
		);
		option.AddLeftJoin(
			"fio", "order_employer_", "ud_client",
			"`ud_client`.`client_id`=`ud_order`.`order_employer` AND `ud_order`.`order_emptype`=" + toString( udProjectOrder::ORDER_EMPLOYER_CLIENT ),
			GetProjectClient
		);
		option.AddLeftJoin(
			"name", "order_employer2_", "ud_clientur",
			"`ud_clientur`.`clientur_id`=`ud_order`.`order_employer` AND `ud_order`.`order_emptype`=" + toString( udProjectOrder::ORDER_EMPLOYER_CLIENTUR ),
			GetProjectClientUr
		);
		option.AddLeftJoin(
			"name", "order_direct_", "ud_direct",
			"`ud_direct`.`direct_id`=`ud_order`.`order_direct`", GetProjectDirect
		);
		option.SetSqlCalcFoundRows( true );
		//////////////////////////////////////////////////////////////////////////////////////////
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "request" )->toUInt( );
		if ( id > 0 ) {
			option.AddLeftJoin( "",
				"CROSS JOIN `ud_objlnk` tbllnk1 ON tbllnk1.`objlnk_id_src`=" + toString( id ) + " AND "
				"tbllnk1.`objlnk_id_dst`=`ud_order`.`order_id` AND "
				"tbllnk1.`objlnk_type_src`=" + toString( udProjectObjectLink::objlnkRequest ) + " AND "
				"tbllnk1.`objlnk_type_dst`=" + toString( udProjectObjectLink::objlnkOrder )
			);
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "tour" )->toUInt( );
		if ( id > 0 ) {
			option.AddLeftJoin( "",
				"CROSS JOIN `ud_objlnk` tbllnk2 ON tbllnk2.`objlnk_id_src`=`ud_order`.`order_id` AND "
				"tbllnk2.`objlnk_id_dst`=" + toString( id ) + " AND "
				"tbllnk2.`objlnk_type_src`=" + toString( udProjectObjectLink::objlnkOrder ) + " AND "
				"tbllnk2.`objlnk_type_dst`=" + toString( udProjectObjectLink::objlnkTour )
			);
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		option.SetIgnoreAttr( "manager2_" );
		option.SetIgnoreAttr( "employer_" );
		option.SetIgnoreAttr( "direct_" );
		option.SetIgnoreAttr( "status_" );
		option.SetIgnoreAttr( "employer2_" );
		handler->GetUdObject( arr, option, &err );
		if ( err.empty( ) ) {
			map< udInt, wstring > mapStatus;
			mapStatus[ udProjectOrder::ORDER_STATUS_NONE	] = L"";
			mapStatus[ udProjectOrder::ORDER_STATUS_OPENED	] = L"Открыта";
			mapStatus[ udProjectOrder::ORDER_STATUS_CLOSED	] = L"Закрыта";
			mapStatus[ udProjectOrder::ORDER_STATUS_REMIND	] = L"Требует внимания";
			udInt	status	= 0;
			udPBase obj		= NULL;
			wstring	str1	= L"",
					str2	= L"";

			for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				obj = *i;
				status = obj->get( "status" )->toInt( );
				obj->get( "status_" )->ini( mapStatus[ status ] );

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

	string CHOrder::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectOrder;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		datetime date;
		udUInt id = 0;
		udDWord amount = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date1" )->toTime( );
		if ( date.get( ) > 0 ) {
			arrString.push_back( config.GetAttrIndex( "created" ) + ">='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date2" )->toTime( );
		if ( date.get( ) > 0 ) {
			date.inc( 1, datetime::datetimeDay );
			arrString.push_back( config.GetAttrIndex( "created" ) + "<='" + date.toString( "%Y-%m-%d" ) + "'" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "manager" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "manager2" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		amount = m_pObjFilter->get( "amount1" )->toDWord( );
		if ( amount > 0 ) {
			arrString.push_back( config.GetAttrIndex( "amount" ) + ">=" + toString( amount ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		amount = m_pObjFilter->get( "amount2" )->toDWord( );
		if ( amount > 0 ) {
			arrString.push_back( config.GetAttrIndex( "amount" ) + "<=" + toString( amount ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "employer" )->toUInt( );
		if ( id > 0 ) {
			udInt type = m_pObjFilter->get( "emptype" )->toInt( );
			arrString.push_back( config.GetAttrIndex( "emptype" ) + "=" + toString( type ) );
			arrString.push_back( config.GetAttrIndex( "employer" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		id = m_pObjFilter->get( "direct" )->toUInt( );
		if ( id > 0 ) {
			arrString.push_back( config.GetAttrIndex( "direct" ) + "=" + toString( id ) );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( !arrString.empty( ) ) {
			ret = Join( " AND ", arrString );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		//g_pApp->GetWndServer( )->UpdateText( L"ret='" + toWString( ret ) + L"'" );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ret;
	} // string GetWhereCondition( )

} // namespace UdSDK
