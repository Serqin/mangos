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
SDName: boss_devourer_of_souls
SD%Complete: 0%
SDComment:
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"

enum
{
	SPELL_PHANTOM_BLAST = 68982,
	H_SPELL_PHANTOM_BLAST = 70322,
	SPELL_WAILING_SOULS = 68873
};

struct MANGOS_DLL_DECL boss_devourer_of_souls : public ScriptedAI
{
	
	boss_devourer_of_souls(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
		Reset();
    }

	bool m_bIsRegularMode;

	uint32 m_phantomBlast_Timer;
	uint32 m_wailingSouls_Timer;

	void Reset()
    {
		m_phantomBlast_Timer = urand(5000, 9000);
		m_wailingSouls_Timer = urand(5000, 9000);
    }
	
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_phantomBlast_Timer < uiDiff)
        {
			if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_PHANTOM_BLAST : H_SPELL_PHANTOM_BLAST) == CAST_OK){
				m_phantomBlast_Timer = urand(5000, 9000);
			}
        }
        else m_phantomBlast_Timer -= uiDiff;

		if (m_wailingSouls_Timer < uiDiff)
        {
			if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_WAILING_SOULS) == CAST_OK){
				m_wailingSouls_Timer = urand(5000, 9000);
			}
        }
        else m_wailingSouls_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }

};



CreatureAI* GetAI_boss_devourer_of_souls(Creature* pCreature)
{
    return new boss_devourer_of_souls(pCreature);
}

void AddSC_boss_devourer_of_souls()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_devourer_of_souls";
    NewScript->GetAI = &GetAI_boss_devourer_of_souls;
    NewScript->RegisterSelf();
}