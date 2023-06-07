
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CHReportLanding::CHReportLanding( ) {
		m_pObjFilter	= new udMorph;

		m_pObjFilter->get( "date1" )->ini( datetime( ) );
		m_pObjFilter->get( "date2" )->ini( datetime( ) );
		m_pObjFilter->get( "group" )->ini( ( udUInt ) 0 );
		m_pObjFilter->get( "direct" )->ini( ( udUInt ) 0 );
	}

	CHReportLanding::CHReportLanding( const CHReportLanding& objCopy ) {
	}

	CHReportLanding::~CHReportLanding( ) {
		udDELETE( m_pObjFilter );
	}

	CHReportLanding& CHReportLanding::operator =( const CHReportLanding& objCopy ) {
		return *this;
	}

	void CHReportLanding::Init( ) {
	} // void Init

	void CHReportLanding::InitSync( udPDatabaseSync sync ) {
	} // void InitSync

	void CHReportLanding::ProcRequest( udPSocket sock ) {
		m_pObjFilter->read( sock );

		wstring text = L"отправка отчета\r\n";
		//////////////////////////////////////////////////////////////////////////////////
		string szQuery;
		//////////////////////////////////////////////////////////////////////////////////////////
		datetime date;
		string ldate1, rdate1;
		udUInt group, direct;
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date1" )->toTime( );
		if ( date.get( ) > 0 ) {
			ldate1 = date.toString( "%Y-%m-%d" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		date = m_pObjFilter->get( "date2" )->toTime( );
		if ( date.get( ) > 0 ) {
			date.inc( 1, datetime::datetimeDay );
			rdate1 = date.toString( "%Y-%m-%d" );
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		group = m_pObjFilter->get( "group" )->toUInt( );
		direct = m_pObjFilter->get( "direct" )->toUInt( );
		//////////////////////////////////////////////////////////////////////////////////////////
		szQuery = "SELECT "
				"* "
				"FROM `ud_client` c, `ud_tourclient` tc, `ud_tour` t, `ud_grouptour` gt, `ud_groupperiod` gp, `ud_group` g, `ud_stour` st "
				"WHERE gp.`groupperiod_date1`>='" + ( ldate1.empty( ) ? "1970-01-01" : ldate1 ) + "' AND " +
				"gp.`groupperiod_date1`<='" + ( rdate1.empty( ) ? "2030-01-01" : rdate1 ) + "' AND "
				"g.`group_id`=gp.`groupperiod_group` AND "
				"gt.`grouptour_group`=g.`group_id` AND "
				"gt.`grouptour_tour`=t.`tour_id` AND "
				"gt.`grouptour_period`=gp.`groupperiod_index` AND "
				"tc.`tourclient_tour`=t.`tour_id` AND "
				"tc.`tourclient_client`=c.`client_id` AND "
				"t.`tour_state`=st.`stour_id` ";
		if ( group > 0 ) {
			szQuery += "AND g.`group_id`=" + toString( group ) + " ";
		}
		if ( direct > 0 ) {
			szQuery += "AND g.`group_direct`=" + toString( direct ) + " ";
		}
		szQuery += "ORDER BY g.`group_id`, t.`tour_id`";
		//////////////////////////////////////////////////////////////////////////////////////////
		g_pApp->GetLog( )->Write( szQuery + "\n" );
		//////////////////////////////////////////////////////////////////////////////////
		udPDatabase db = g_pApp->GetDatabase( );
		udArrMorph arr;
		if ( db->GetAssoc( szQuery, arr ) ) {
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 80 );
			
			udArrBase arr1;
			udPMorph morph = NULL;
			udPBase add = NULL, grp = NULL;
			udArrString arrString;
			map< udUInt, udUInt > mapTours;
			udDWord dwGroupId;
			udDwAscBase ascGroup;
			udDwAscBase::iterator itGroup;
			datetime date1, date2;

			text += L"найдено " + toWString( arr.size( ) ) + L" записей\r\n";
			g_pApp->GetLog( )->Write( "rows=" + toString( arr.size( ) ) + "\n" );
			
			for( udArrMorph::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
				morph = *i;

				mapTours[ morph->get( "tour_id" )->toUInt( ) ] = 1;

				dwGroupId = morph->get( "group_id" )->toDWord( );
				itGroup = ascGroup.find( dwGroupId );
				if ( itGroup == ascGroup.end( ) ) { // группа не была занесена в список
					grp = new udProjectReportGroup;
					grp->get( "id" )->ini( morph->get( "group_id" )->toUInt( ) );
					grp->get( "name" )->ini( morph->get( "group_name" )->toWString( ) );
					grp->get( "date1" )->ini( morph->get( "tour_date1" )->toTime( ) );
					grp->get( "date2" )->ini( morph->get( "tour_date2" )->toTime( ) );
					grp->get( "min" )->ini( morph->get( "groupperiod_min" )->toUInt( ) );
					grp->get( "max" )->ini( morph->get( "groupperiod_max" )->toUInt( ) );
					ascGroup[ dwGroupId ] = grp;
				} else {
					grp = itGroup->second;
					if ( grp->get( "min" )->toUInt( ) > morph->get( "groupperiod_min" )->toUInt( ) ) {
						grp->get( "min" )->ini( morph->get( "groupperiod_min" )->toUInt( ) );
					}
					if ( grp->get( "max" )->toUInt( ) < morph->get( "groupperiod_max" )->toUInt( ) ) {
						grp->get( "max" )->ini( morph->get( "groupperiod_max" )->toUInt( ) );
					}
					if ( grp->get( "date1" )->toTime( ).get( ) > morph->get( "tour_date1" )->toTime( ).get( ) ) {
						grp->get( "date1" )->ini( morph->get( "tour_date1" )->toTime( ) );
					}
					if ( grp->get( "date2" )->toTime( ).get( ) < morph->get( "tour_date2" )->toTime( ).get( ) ) {
						grp->get( "date2" )->ini( morph->get( "tour_date2" )->toTime( ) );
					}
				}

				add = new udProjectReportLanding;
				add->get( "group" )->ini( morph->get( "group_id" )->toUInt( ) );
				add->get( "tour" )->ini( morph->get( "tour_id" )->toUInt( ) );
				add->get( "period" )->ini( morph->get( "groupperiod_index" )->toUInt( ) );
				add->get( "client" )->ini( morph->get( "client_id" )->toUInt( ) );
				add->get( "direct" )->ini( morph->get( "group_direct" )->toUInt( ) );
				add->get( "date1" )->ini( morph->get( "groupperiod_date1" )->toTime( ) );
				add->get( "fio" )->ini( morph->get( "client_fio" )->toWString( ) );
				add->get( "phone" )->ini( morph->get( "client_lphone" )->toWString( ) );
				add->get( "state" )->ini( morph->get( "stour_name" )->toWString( ) );
				add->get( "comment" )->ini( morph->get( "tour_comment" )->toWString( ) );
				arr1.push_back( add );
			}

			ClearArrMorph( arr );

			for( map< udUInt, udUInt >::iterator i = mapTours.begin( ); i != mapTours.end( ); ++i ) {
				arrString.push_back( toString( i->first ) );
			}

			string szLike;
			string szStr = ConvertString( wstring( L"Пакет" ), CP_UTF8 );
			if ( szStr.size( ) ) {
				udDWord	dwLen	= szStr.size( ) * 2;
				udPChar szTmp	= new udChar[ dwLen + 1 ];

				memset( szTmp, 0, dwLen + 1 );
				UdBaseEscapeString( szTmp, szStr.c_str( ), szStr.size( ) );

				szStr = szTmp;

				delete [ ] szTmp;
			}
			szLike = "t.`tservice_name` LIKE '%" + szStr + "%' ";

			szQuery = "SELECT "
				"ts.`tourservice_tour`, ns.`nameservice_name` "
				"FROM `ud_tourservice` ts, `ud_service` s, `ud_nameservice` ns, `ud_tservice` t "
				"WHERE ts.`tourservice_tour` IN(" + Join( ",", arrString ) + ") AND "
				"s.`service_type`=t.`tservice_id` AND "
				"ts.`tourservice_service`=s.`service_id` AND "
				"s.`service_name`=ns.`nameservice_id` AND " +
				szLike +
				"GROUP BY ts.`tourservice_tour`";
			if ( db->GetAssoc( szQuery, arr ) ) {
				udUInt iTourId1, iTourId2;
				for( udArrMorph::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					iTourId1 = ( *i )->get( "tourservice_tour" )->toUInt( );
					for( udArrBase::iterator j = arr1.begin( ); j != arr1.end( ); ++j ) {
						iTourId2 = ( *j )->get( "tour" )->toUInt( );
						if ( iTourId1 == iTourId2 ) {
							( *j )->get( "has_packet" )->ini( ( *i )->get( "nameservice_name" )->toWString( ) );
						}
					}
				}
				ClearArrMorph( arr );
			} else {
				text += L"услуг, содержащих в названии Пакет не найдено\r\n";
			}

			szStr = ConvertString( wstring( L"Посадка" ), CP_UTF8 );
			if ( szStr.size( ) ) {
				udDWord	dwLen	= szStr.size( ) * 2;
				udPChar szTmp	= new udChar[ dwLen + 1 ];

				memset( szTmp, 0, dwLen + 1 );
				UdBaseEscapeString( szTmp, szStr.c_str( ), szStr.size( ) );

				szStr = szTmp;

				delete [ ] szTmp;
			}
			szLike = "t.`tservice_name` LIKE '%" + szStr + "%' ";

			szQuery = "SELECT "
				"ts.`tourservice_tour`, ns.`nameservice_name` "
				"FROM `ud_tourservice` ts, `ud_service` s, `ud_nameservice` ns, `ud_tservice` t "
				"WHERE ts.`tourservice_tour` IN(" + Join( ",", arrString ) + ") AND "
				"s.`service_type`=t.`tservice_id` AND "
				"ts.`tourservice_service`=s.`service_id` AND "
				"s.`service_name`=ns.`nameservice_id` AND " +
				szLike +
				"GROUP BY ts.`tourservice_tour`";
			if ( db->GetAssoc( szQuery, arr ) ) {
				udUInt iTourId1, iTourId2;
				for( udArrMorph::iterator i = arr.begin( ); i != arr.end( ); ++i ) {
					iTourId1 = ( *i )->get( "tourservice_tour" )->toUInt( );
					for( udArrBase::iterator j = arr1.begin( ); j != arr1.end( ); ++j ) {
						iTourId2 = ( *j )->get( "tour" )->toUInt( );
						if ( iTourId1 == iTourId2 ) {
							( *j )->get( "has_landing" )->ini( ( *i )->get( "nameservice_name" )->toWString( ) );
						}
					}
				}
				ClearArrMorph( arr );
			} else {
				text += L"услуг, содержащих в названии Посадка не найдено\r\n";
			}

			udBinaryData data;
			WriteInstances( ascGroup, data );
			WriteInstances( arr1, data );
			sock->Send( data );
			data.clear( );
			ClearDwAscBase( ascGroup );
			ClearArrBase( arr1 );
		} else {
			sock->Wait( udSocket::UDSOCKET_SELECT_WRITE, 1000 );
			sock->SendByte( ( char ) 0 );
			text += L"ничего не найдено";
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		sock->Wait( udSocket::UDSOCKET_SELECT_READ, 1000 );
		char code = 0;
		sock->ReadByte( &code );
		//////////////////////////////////////////////////////////////////////////////////////////
	} // void ProcRequest

} // namespace UdSDK
