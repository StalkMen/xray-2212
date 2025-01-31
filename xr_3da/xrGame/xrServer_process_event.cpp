#include "stdafx.h"
#include "xrServer.h"
#include "game_sv_single.h"
#include "alife_simulator.h"
#include "xrserver_objects.h"
#include "game_base.h"
#include "game_cl_base.h"
#include "ai_space.h"
#include "alife_object_registry.h"
#include "xrServer_Objects_ALife_Items.h"

void xrServer::Process_event	(NET_Packet& P, ClientID sender)
{
	u32			timestamp;
	u16			type;
	u16			destination;
	u32			MODE			= net_flags(TRUE,TRUE);

//	xrClientData *l_pC = ID_to_client(sender);

	// correct timestamp with server-unique-time (note: direct message correction)
	P.r_u32		(timestamp	);
	/*
	xrClientData*	c_sender	= ID_to_client	(sender);
	if (c_sender)
	{
		u32			sv_timestamp	= Device.TimerAsync	() - (c_sender->stats.getPing()/2);		// approximate time this message travels
		timestamp					= (timestamp+sv_timestamp)/2;								// approximate timestamp with both client and server time
		CopyMemory	(&P.B.data[P.r_pos-4], &timestamp, 4);
	}
	*/

	// read generic info
	P.r_u16		(type		);
	P.r_u16		(destination);

	CSE_Abstract*	receiver	= game->get_entity_from_eid	(destination);
	if (receiver)	
		receiver->OnEvent						(P,type,timestamp,sender);

	switch		(type)
	{
	case GE_GAME_EVENT:
		{
			u16		game_event_type;
			P.r_u16(game_event_type);
//			game->OnEvent(P,game_event_type,timestamp,sender);
			game->AddDelayedEvent(P,game_event_type,timestamp,sender);
		}
/* // moved to game_sv_teamdeathmatch
	case GEG_PLAYER_CHANGE_TEAM: //cs & TDM
		{
			xrClientData *l_pC = ID_to_client(sender);
			s16 l_team; P.r_s16(l_team);
			game->OnPlayerChangeTeam(l_pC->ID, l_team);
//			VERIFY					(verify_entities());
		}break;
*/

/* // moved to game_sv_deathmatch
	case GEG_PLAYER_CHANGE_SKIN: //dm only
		{
			xrClientData *l_pC = ID_to_client(sender);
			u8 l_skin; P.r_u8(l_skin);
			game->OnPlayerChangeSkin(l_pC->ID, l_skin);
//			VERIFY					(verify_entities());
		}break;
	case GEG_PLAYER_KILL: //dm only
		{
			xrClientData *l_pC = ID_to_client(sender);
			game->OnPlayerWantsDie(l_pC->ID);
//			VERIFY					(verify_entities());
		}break;
*/

/* // moved to game_sv_deathmatch
	case GEG_PLAYER_READY:// cs & dm
		{
			CSE_Abstract*		E			= game->get_entity_from_eid	(destination);
			if (E) {
				xrClientData*	C			= E->owner;
				if (C && (C->owner == E))
				{
					game->OnPlayerReady		(C->ID);
				}
			}
//			VERIFY					(verify_entities());
		}break;
*/

/*	// moved to game_sv_deathmatch
	case GEG_PLAYER_BUY_FINISHED: // dm only
		{
			xrClientData *l_pC = ID_to_client(sender);
			game->OnPlayerBuyFinished(l_pC->ID, P);
//			VERIFY					(verify_entities());
		}break;
*/
	case GE_INFO_TRANSFER:{
		ClientID clientID;clientID.setBroadcast();
		SendBroadcast			(clientID,P,MODE);
//		VERIFY					(verify_entities());
		}break;
	case GE_PDA:{
		ClientID clientID;clientID.setBroadcast();
		SendBroadcast			(clientID,P,MODE);
//		VERIFY					(verify_entities());
		}break;
	case GE_INV_ACTION:
		{
			xrClientData* CL		= ID_to_client(sender);
			if (CL)	CL->net_Ready	= TRUE;
			if (SV_Client) SendTo(SV_Client->ID, P, net_flags(TRUE, TRUE));
//			VERIFY					(verify_entities());
		}break;
/*	case GE_BUY: //cs & dm
		{
			string64			i_name;
			P.r_stringZ			(i_name);
			CSE_Abstract*		E			= game->get_entity_from_eid	(destination);
			if (E) {
				xrClientData*		C			= E->owner;
				if (C && (C->owner == E))
				{
					game->OnPlayerBuy		(C->ID,destination,i_name);
				}
			}
//			VERIFY					(verify_entities());
		}
		break;*/
	case GE_RESPAWN:
		{
			CSE_Abstract*		E	= game->get_entity_from_eid	(destination);
			if (E) 
			{
				R_ASSERT			(E->s_flags.is(M_SPAWN_OBJECT_PHANTOM));

				svs_respawn			R;
				R.timestamp			= timestamp	+ E->RespawnTime*1000;
				R.phantom			= destination;
				q_respawn.insert	(R);
			}
//			VERIFY					(verify_entities());
		}
		break;
	case GE_WPN_STATE_CHANGE:
	case GE_ZONE_STATE_CHANGE:{
		ClientID clientID;clientID.setBroadcast();
		SendBroadcast			(clientID,P,MODE);
//		VERIFY					(verify_entities());
		}break;
	case GE_TRADE_BUY:
	case GE_OWNERSHIP_TAKE:
		{
			Process_event_ownership	(P,sender,timestamp,destination);
			VERIFY					(verify_entities());
		}break;
	case GE_TRADE_SELL:
	case GE_OWNERSHIP_REJECT:
		{
			Process_event_reject	(P,sender,timestamp,destination,P.r_u16());
			VERIFY					(verify_entities());
		}break;
	case GE_DESTROY:
		{
			Process_event_destroy	(P,sender,timestamp,destination, NULL);
			VERIFY					(verify_entities());
		}
		break;
	case GE_TRANSFER_AMMO:
		{
			u16					id_parent=destination,id_entity;
			P.r_u16				(id_entity);
			CSE_Abstract*		e_parent	= game->get_entity_from_eid	(id_parent);	// ��� �������� (��� ����� ����)
			CSE_Abstract*		e_entity	= game->get_entity_from_eid	(id_entity);	// ��� ������
			if (!e_entity)		break;
			if (0xffff != e_entity->ID_Parent)	break;						// this item already taken
			xrClientData*		c_parent	= e_parent->owner;
			xrClientData*		c_from		= ID_to_client	(sender);
			R_ASSERT			(c_from == c_parent);						// assure client ownership of event

			// Signal to everyone (including sender)
			ClientID clientID;clientID.setBroadcast();
			SendBroadcast		(clientID,P,MODE);

			// Perfrom real destroy
			entity_Destroy		(e_entity	);
			VERIFY				(verify_entities());
		}
		break;
	case GE_HIT:
		{
			// Parse message
//last	u16					id_dest		=	destination, id_src;
//last	P.r_u16				(id_src);
			/*CSE_Abstract*	e_dest		= game->get_entity_from_eid	(id_dest);*/	// ��� ����������
			
//last		CSE_Abstract*		e_src		= game->get_entity_from_eid	(id_src	); if(!e_src) break; // @@@ WT		// ��������� ����

//			xrClientData*		c_src		= e_src->owner;
//			xrClientData*		c_from		= ID_to_client	(sender);
//			R_ASSERT			(c_src == c_from);							// assure client ownership of event

//			CSE_Abstract*		e_hitter = e_src;
//			CSE_Abstract*		e_hitted = receiver;

//last	game->OnHit(id_src, id_dest, P);*/
			P.r_pos -=2;
			game->AddDelayedEvent(P,GAME_EVENT_ON_HIT, 0, ClientID() );
			
			// Signal just to destination (����, ��� ����������)
//last	ClientID clientID;clientID.setBroadcast();
//last	SendBroadcast		(clientID,P,MODE);
		}
		break;
	case GE_DIE:
		{
			// Parse message
			u16					id_dest		=	destination, id_src;
			P.r_u16				(id_src);


			xrClientData *l_pC	= ID_to_client(sender);
//			VERIFY				(game && l_pC && l_pC->owner);
			VERIFY				(game && l_pC);
			if ((game->Type() != GAME_SINGLE) && l_pC && l_pC->owner)
			{
				Msg					("* [%2d] killed by [%2d] - sended by [%s:%2d]", id_dest, id_src, game->get_option_s(*l_pC->Name,"name","Player"), l_pC->owner->ID);
			}

			CSE_Abstract*		e_dest		= game->get_entity_from_eid	(id_dest);	// ��� ����
			VERIFY				(e_dest);
			if (game->Type() != GAME_SINGLE)
				Msg				("* [%2d] is [%s:%s]", id_dest, *e_dest->s_name, e_dest->name_replace());
			CSE_Abstract*		e_src		= game->get_entity_from_eid	(id_src	);	// ��� ����
			VERIFY				(e_src);
			R_ASSERT2			(e_dest && e_src, "Killer or/and being killed are offline or not exist at all :(");
			if (game->Type() != GAME_SINGLE)
				Msg				("* [%2d] is [%s:%s]", id_src, *e_src->s_name, e_src->name_replace());

			xrClientData*		c_dest		= e_dest->owner;			// ������, ��� ���� ����
			xrClientData*		c_src		= e_src->owner;				// ������, ��� ���� ����
//			xrClientData*		c_from		= ID_to_client	(sender);	// ������, ������ ������ �������
//			R_ASSERT2			(c_dest == c_from, "Security error (SSU :)");// assure client ownership of event

			//
			
			if (/*e_src->s_flags.is(M_SPAWN_OBJECT_ASPLAYER) && */e_dest->s_flags.is(M_SPAWN_OBJECT_ASPLAYER)) {
				//game->OnPlayerKillPlayer	(c_src->ID,c_dest->ID);

				NET_Packet P_;
				P_.B.count = 0;
				P_.w_clientID(c_src->ID);
				P_.w_clientID(c_dest->ID);
				P_.r_pos = 0;
				ClientID clientID;clientID.set(0);
//				game->OnEvent(P_,GAME_EVENT_PLAYER_KILLED, 0, clientID);
				game->AddDelayedEvent(P_,GAME_EVENT_PLAYER_KILLED, 0, clientID);
			}

			if (c_src->owner->ID == id_src) {
				// Main unit
				P.w_begin			(M_EVENT);
				P.w_u32				(timestamp);
				P.w_u16				(type);
				P.w_u16				(destination);
				P.w_u16				(id_src);
				P.w_clientID		(c_src->ID);
			}
			ClientID clientID;clientID.setBroadcast();
			SendBroadcast			(clientID,P,MODE);
			VERIFY					(verify_entities());
		}
		break;
	case GE_GRENADE_EXPLODE:
		{
			ClientID clientID;clientID.setBroadcast();
			SendBroadcast		(clientID,P,MODE);
//			VERIFY				(verify_entities());
		}break;
	case GE_ADDON_ATTACH:
	case GE_ADDON_DETACH:
		{			
			SendTo(SV_Client->ID, P, net_flags(TRUE, TRUE));
//			VERIFY					(verify_entities());
		}break;
	case GEG_PLAYER_ITEM2SLOT:
	case GEG_PLAYER_ITEM2BELT:
	case GEG_PLAYER_ITEM2RUCK:
		{
			u16 id = P.r_u16();
			CSE_Abstract*		E	= game->get_entity_from_eid	(id);
			if (E)
			{
				CSE_ALifeItemCustomOutfit* pOutfit = smart_cast<CSE_ALifeItemCustomOutfit*>(E);
				if (pOutfit)
				{
					SendBroadcast		(sender,P,MODE);
					break;
				};
			};
			SendTo(SV_Client->ID, P, net_flags(TRUE, TRUE));
		}break;
	case GEG_PLAYER_BUYMENU_OPEN:
	case GEG_PLAYER_BUYMENU_CLOSE:
	case GEG_PLAYER_INVENTORYMENU_OPEN:
	case GEG_PLAYER_INVENTORYMENU_CLOSE:
	case GEG_PLAYER_DEACTIVATE_CURRENT_SLOT:
	case GEG_PLAYER_RESTORE_CURRENT_SLOT:
	case GEG_PLAYER_SPRINT_START:
	case GEG_PLAYER_SPRINT_END:
		{
//			SendBroadcast		(sender,P,MODE);
			SendTo(SV_Client->ID, P, net_flags(TRUE, TRUE));

//			VERIFY					(verify_entities());
		}break;
	case GEG_PLAYER_ITEMDROP:
	case GEG_PLAYER_ITEM_EAT:
		{
			SendTo(SV_Client->ID, P, net_flags(TRUE, TRUE));
//			VERIFY					(verify_entities());
		}break;		
	case GE_TELEPORT_OBJECT:
		{
			game->teleport_object	(P,destination);
		}break;
	case GE_ADD_RESTRICTION:
		{
			game->add_restriction	(P,destination);
		}break;
	case GE_REMOVE_RESTRICTION:
		{
			game->remove_restriction(P,destination);
		}break;
	case GE_REMOVE_ALL_RESTRICTIONS:
		{
			game->remove_all_restrictions(P,destination);
		}break;
	default:
		R_ASSERT2	(0,"Game Event not implemented!!!");
		break;
	}
}
