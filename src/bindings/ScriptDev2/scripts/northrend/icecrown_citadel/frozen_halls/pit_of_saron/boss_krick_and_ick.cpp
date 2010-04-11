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
SDName: boss_krick_and_ick
SD%Complete: 0%
SDComment:
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"

enum
{
	SPELL_MIGHTY_KICK = 69021,
	SPELL_POISON_NOVA = 68989,
    H_SPELL_POISON_NOVA = 70434,
};


struct MANGOS_DLL_DECL boss_ick : public ScriptedAI
{
	
	boss_ick(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
		Reset();
    }

	bool m_bIsRegularMode;

	uint32 m_uiMightyKickTimer;
	uint32 m_uiPoisonNovaTimer;

	void Reset()
    {
		m_uiMightyKickTimer = urand(5000, 10000);
		m_uiPoisonNovaTimer = urand(20000, 30000);
    }
	
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiMightyKickTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIGHTY_KICK);
			m_uiMightyKickTimer = urand(5000, 10000);
		}
		else
			m_uiMightyKickTimer -= uiDiff;

		if (m_uiPoisonNovaTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_POISON_NOVA : H_SPELL_POISON_NOVA);
			m_uiPoisonNovaTimer = urand(20000, 30000);
		}
		else
			m_uiPoisonNovaTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }

};


CreatureAI* GetAI_boss_ick(Creature* pCreature)
{
    return new boss_ick(pCreature);
}

void AddSC_boss_ick()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_ick";
    NewScript->GetAI = &GetAI_boss_ick;
    NewScript->RegisterSelf();
}