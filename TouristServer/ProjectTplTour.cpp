
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	static void TplTourFilterRoute( udArrBase& arr ) {
		LoadListDataByAttr( arr, g_pApp->GetHandler( ), GetProjectCity, "ud_city", "city", "name", "city_" );
	} // void TplTourFilterRoute

	static void TplTourFilterService( udArrBase& arr ) {
		if ( arr.empty( ) ) {
			return;
		}
		/*for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->get( "#name" )->ini( wstring( L"" ) );
		}*/

		udPHandler handler = g_pApp->GetHandler( );

		LoadListDataByAttr( arr, handler, GetProjectService, "ud_service", "service", "name", "id_name" );
		//LoadListDataByAttr( arr, handler, GetProjectNameService, "ud_nameservice", "id_name", "name", "#name" );
		LoadListDataByAttr( arr, handler, GetProjectNameService, "ud_nameservice", "id_name", "name", "service_" );

		udPBase obj = NULL;
		/*wstring name = L"";
		wstring date1 = L"", date2 = L"";
		datetime date;*/

		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			obj = *i;
			obj->unset( "id_name" );

			/*date = obj->get( "date1" )->toTime( );
			date1 = date.toWString( "%Y-%m-%d" );
			date = obj->get( "date2" )->toTime( );
			date2 = date.toWString( "%Y-%m-%d" );
			name = obj->get( "#name" )->toWString( );
			obj->get( "#name" )->ini(
				date1 + L"-" + date2 + L", " +
				obj->get( "num" )->toWString( ) + L", " +
				name
			);*/
		}
	} // void TplTourFilterService

	static void TplTourFilterAttach( udArrBase& arrAdd, udArrBase& arrUpd, udArrBase& arrDel, udPBase obj ) {
		udPAppConfig pAppConfig = g_pApp->GetConfig( );
		wstring tmp_dir = pAppConfig->get( "temporary_dir" )->toWString( ),
			client_dir = pAppConfig->get( "files_dir" )->toWString( ) + L"\\tpltour",
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
	} // void TplTourFilterAttach

	udPBase GetProjectTplTour2( ) {
		udPBase ret = GetProjectTplTour( );
		ret->get( "direct_" )->ini( wstring( L"" ) );
		return ret;
	} // udPBase GetProjectTplTour2

	CHTplTour::CHTplTour( ) : udStateHandlerSimple( ) {
		m_szTableName	= "ud_tpltour";
		m_szUpdIndex	= "id";
		m_szName		= "tpltour";
		m_fnGetObject	= GetProjectTplTour;

		this->AddManagedTable( "ud_tpltourroute", "id", GetProjectTplTourRoute );
		this->AddManagedTable( "ud_tpltourservice", "id", GetProjectTplTourService );
		this->AddManagedTable( "ud_attach", "id", GetProjectAttach );

		this->AddAppendData( ( udInt ) edit_append_foreignkey, "ud_direct", "direct", "id", GetProjectDirect );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_tpltourroute", "tpltour", "id", GetProjectTplTourRoute, "`tpltourroute_id` ASC", "", TplTourFilterRoute );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_tpltourservice", "tpltour", "id", GetProjectTplTourService, "`tpltourservice_id` ASC", "", TplTourFilterService );
		this->AddAppendData( ( udInt ) edit_append_array, "ud_attach", "obj", "id", GetProjectAttach, "`attach_id` ASC", "`attach_type`=" + toString( udProjectAttach::ATTACH_TPLTOUR ) );//,
			//NULL, NULL, TplTourFilterAttach );

		m_pObjFilter->get( "name" )->ini( wstring( L"" ) );
		m_pObjFilter->get( "direct" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "tour" )->ini( ( udUInt ) 0 );
	}

	CHTplTour::CHTplTour( const CHTplTour& objCopy ) {
	}

	CHTplTour::~CHTplTour( ) {
	}

	CHTplTour& CHTplTour::operator =( const CHTplTour& objCopy ) {
		return *this;
	}

	void CHTplTour::Init( ) {
		udStateHandlerSimple::Init( );
		wstring folder = L"";
		udPAppConfig appConfig = g_pApp->GetConfig( );
		//folder = appConfig->get( "temporary_dir" )->toWString( ) + L"\\tpltour";
		//CreateDirectory( folder.data( ), NULL );
		folder = appConfig->get( "files_dir" )->toWString( ) + L"\\tpltour";
		CreateDirectory( folder.data( ), NULL );
	} // void Init

	udInt CHTplTour::FindObj( udPBase obj, const string& attr, bool apply ) {
		if ( apply ) {
			return 0;
		}
		// при обновлении пожалуйста, ищем по ID и пох
		return udStateHandlerSimple::FindObj( obj, attr, apply );
	} // bool FindObj

	udInt CHTplTour::AddObj( udPBase obj, bool save_info ) {
		if ( udStateHandlerSimple::AddObj( obj, save_info ) == 1 ) {
			// дополнительно создаем папку для приложений
			wstring dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
				L"\\tpltour\\" + obj->get( "id" )->toWString( );
			CreateDirectory( dir.data( ), NULL );
			g_pApp->GetLog( )->Write( "'" + toString( dir ) + "'\n" );
			return 1;
		}
		return 0;
	} // udInt AddObj

	void CHTplTour::DeleteLinkedObjects( udPBase obj ) {
		udPDatabase db = g_pApp->GetDatabase( );
		string query = "", szWhere = "";
		udBaseConfig config;
		udPBase obj1;
		udPType type = NULL;

		// маршрут
		obj1 = new udProjectTplTourRoute;
		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "tpltour" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );

		// услуги
		obj1 = new udProjectTplTourService;
		obj1->GetConfig( config );
		delete obj1;
		szWhere = config.GetAttrIndex( "tpltour" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );

		// приложения
		// приложение
		obj1 = new udProjectAttach;
		obj1->GetConfig( config );
		delete obj1;
		type = new udType( ( udInt ) ( udProjectAttach::ATTACH_TPLTOUR ) );
		szWhere = config.GetAttrIndex( "type" ) + "=" + type->toString( ) +
			config.GetAttrIndex( "obj" ) + "=" + obj->get( "id" )->toString( );
		query = "DELETE FROM `" + config.GetSQLTableName( ) + "` WHERE " + szWhere;
		db->Query( query );
		config.Clear( );
		wstring folder = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) +
			L"\\tpltour\\" + obj->get( "id" )->toWString( );
		// папку удалить
		DirectoryClear( folder );
		RemoveDirectory( folder.data( ) );

		// шаблоны документов
	} // void DeleteLinkedObjects

	void CHTplTour::ExtraDataRecieve( udInt opId, udPSocket sock, udPBase obj ) {
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
				wstring dst_dir = g_pApp->GetConfig( )->get( "files_dir" )->toWString( ) + L"\\tpltour\\" + obj->get( "id" )->toWString( );
				CreateDirectory( dst_dir.data( ), NULL );
				DirectoryCopyFiles( tmp_dir, dst_dir );
				DirectoryClear( tmp_dir );
				//////////////////////////////////////////////////////////////////////////////////
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		}
	} // void ExtraDataRecieve

	void CHTplTour::ExtraDataSending( udPSocket sock, udPBase obj ) {
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
			config.GetAttrIndex( "type" ) + "=" + toString( udProjectAttach::ATTACH_TPLTOUR );
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
				L"\\tpltour\\" + obj->get( "id" )->toWString( );
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

	bool CHTplTour::CustomLoadObjects( udPPager pager, udArrBase& arr, udArrError& err, const string& orderby, udInt ordertype, udLongLong& cnt ) {
		udPHandler handler = g_pApp->GetHandler( );
		string szOrderType = ( ordertype == 1 ) ? "ASC" : "DESC", szOrder = "";
		udHandlerOption option;

		option.SetTable( "ud_tpltour" );
		option.SetClass( GetProjectTplTour2 );
		option.SetLimit( pager->GetSQLLimit( ) );
		if ( orderby == "direct" ) {
			szOrder = "`tpltour_direct_` ";
		} else {
			szOrder = "`tpltour_" + orderby + "` ";
		}
		if ( !szOrder.empty( ) ) {
			szOrder += szOrderType;
		}
		option.SetOrder( szOrder );
		option.SetWhere( this->GetWhereCondition( ) );
		option.AddLeftJoin(
			"name", "tpltour_direct_", "ud_direct",
			"`ud_direct`.`direct_id`=`ud_tpltour`.`tpltour_direct`", GetProjectDirect
		);
		option.SetSqlCalcFoundRows( true );
		//////////////////////////////////////////////////////////////////////////////////////////
		udUInt id = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		string log = "\nфильтр шаблона тура\n";
		m_pObjFilter->dump( log );
		g_pApp->GetLog( )->Write( log + "\n" );

		id = m_pObjFilter->get( "tour" )->toUInt( );
		if ( id > 0 ) {
			option.AddLeftJoin( "",
				"CROSS JOIN `ud_objlnk` ON `ud_objlnk`.`objlnk_id_src`=`ud_tpltour`.`tpltour_id` AND "
				"`ud_objlnk`.`objlnk_id_dst`=" + toString( id ) + " AND "
				"`ud_objlnk`.`objlnk_type_src`=" + toString( udProjectObjectLink::objlnkTplTour ) + " AND "
				"`ud_objlnk`.`objlnk_type_dst`=" + toString( udProjectObjectLink::objlnkTour )
			);
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		option.SetIgnoreAttr( "direct_" );
		handler->GetUdObject( arr, option, &err );
		//////////////////////////////////////////////////////////////////////////////////////////
		udPDatabase db = g_pApp->GetDatabase( );
		udArrMorph arr1;
		if ( db->GetAssoc( "SELECT FOUND_ROWS( ) cnt", arr1 ) ) {
			cnt = arr1[ 0 ]->get( "cnt" )->toLLong( );
			ClearArrMorph( arr1 );
		}
		g_pApp->GetLog( )->Write( "cnt=" + toString( cnt ) + "\n" );
		log = "\n";
		for( udArrBase::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
			( *i )->dump( log );
			log += "\n";
		}
		g_pApp->GetLog( )->Write( log );
		//////////////////////////////////////////////////////////////////////////////////////////
		return true;
	} // bool CustomLoadObjects

	string CHTplTour::GetWhereCondition( ) {
		string ret = "";
		udBaseConfig config;
		//////////////////////////////////////////////////////////////////////////////////////////
		udPBase obj = new udProjectTplTour;
		obj->GetConfig( config );
		udDELETE( obj );
		//////////////////////////////////////////////////////////////////////////////////////////
		udArrString arrString;
		wstring str = L"";
		udUInt id = 0;
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
		if ( !arrString.empty( ) ) {
			ret = Join( " AND ", arrString );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		//g_pApp->GetWndServer( )->UpdateText( L"ret='" + toWString( ret ) + L"'" );
		//////////////////////////////////////////////////////////////////////////////////////////
		return ret;
	} // string GetWhereCondition

} // namespace UdSDK
