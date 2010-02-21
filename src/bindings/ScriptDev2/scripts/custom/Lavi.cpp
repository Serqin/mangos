#include "precompiled.h"
#include "Lavi.h"
#include "../system/system.h"

//This function is called when the player clicks an option on the gossip menu
bool GossipSelect_webshop_items(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	if(!pPlayer->getAttackers().empty())
    {
		pCreature->MonsterWhisper("You are in combat", pPlayer->GetGUID());
        pPlayer->CLOSE_GOSSIP_MENU();
        return false;
    }

	uint32 webshopItemId = uiAction - GOSSIP_ACTION_INFO_DEF;
	pPlayer->CLOSE_GOSSIP_MENU();
	//													0		 1		2
	QueryResult* pResult = SD2Database.PQuery("SELECT item_id, count, account_id FROM webshop_items WHERE id = '%u'", webshopItemId);
	
	if (pResult)
	{
		Field* pFields = pResult->Fetch();
		if (pPlayer->GetSession()->GetAccountId() != pFields[2].GetUInt32()){
			return false;
		}

		Item* item = pPlayer->StoreNewItemInInventorySlot(pFields[0].GetUInt32(),  pFields[1].GetInt32());
		if (item)
		{
			pPlayer->SendNewItem(item, pFields[1].GetInt32(), true, false);
			pPlayer->SaveInventoryAndGoldToDB();
			SD2Database.PExecute("DELETE FROM webshop_items WHERE id = '%u'", webshopItemId);
		}
		else
		{
			pCreature->MonsterWhisper("Unable to send the item. Your bag is full.", pPlayer->GetGUID());
		}

	}
	pPlayer->CLOSE_GOSSIP_MENU();
    return true;
}

//This function is called when the player opens the gossip menu
bool GossipHello_webshop_items(Player* pPlayer, Creature* pCreature)
{
	QueryResult* pResult = SD2Database.PQuery("SELECT id, item_id, item_name, count FROM webshop_items WHERE account_id = '%u' LIMIT 15", pPlayer->GetSession()->GetAccountId());
	
	if (pResult)
	{
		do
		{
			Field* pFields = pResult->Fetch();
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pFields[2].GetString(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + pFields[0].GetUInt32());
		} while (pResult->NextRow());
	}

    pPlayer->SEND_GOSSIP_MENU(907, pCreature->GetGUID());

    return true;
}

void AddSC_Lavi()
{
	Script* newscript;

    newscript = new Script;
    newscript->Name = "webshop_items";
    newscript->pGossipHello = &GossipHello_webshop_items;
    newscript->pGossipSelect = &GossipSelect_webshop_items;
    newscript->RegisterSelf();
}