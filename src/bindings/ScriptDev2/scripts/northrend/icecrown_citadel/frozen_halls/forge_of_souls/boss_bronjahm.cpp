/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: boss_bronjahm
SD%Complete: 0%
SDComment:
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"

enum
{
	SPELL_FEAR = 68950,
	SPELL_MAGICS_BANE = 68793,
	H_SPELL_MAGICS_BANE = 69050,
	SPELL_SOULSTORM = 68872,
	SPELL_TELEPORT = 68988
};

struct MANGOS_DLL_DECL boss_bronjahm : public ScriptedAI
{
	
	boss_bronjahm(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
		Reset();
    }

	bool m_bIsRegularMode;
	bool soulStormCasted;
	bool teleported;
	bool fearCasted;

    uint32 MagicBane_Timer;

	void Reset()
    {
        MagicBane_Timer = 5000;
		soulStormCasted = false;
		teleported = false;
		fearCasted = false;
    }
	
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (MagicBane_Timer < uiDiff)
        {
			if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MAGICS_BANE : H_SPELL_MAGICS_BANE, CAST_TRIGGERED) == CAST_OK){
				MagicBane_Timer = urand(4000, 7000);
			}
        }
        else MagicBane_Timer -= uiDiff;

		if (!soulStormCasted && m_creature->GetHealthPercent() < 30.0f){
			if (!teleported){
				if (DoCastSpellIfCan(m_creature, SPELL_TELEPORT) == CAST_OK){
					teleported = true;
				}
			}

			if (teleported && !fearCasted){
				if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FEAR) == CAST_OK){
					fearCasted = true;
				}
			}

			if (fearCasted && !soulStormCasted){
				if (DoCastSpellIfCan(m_creature, SPELL_SOULSTORM) == CAST_OK){
					soulStormCasted = true;
				}
			}
		}

        DoMeleeAttackIfReady();
    }

};



CreatureAI* GetAI_boss_bronjahm(Creature* pCreature)
{
    return new boss_bronjahm(pCreature);
}

void AddSC_boss_bronjahm()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_bronjahm";
    NewScript->GetAI = &GetAI_boss_bronjahm;
    NewScript->RegisterSelf();
}