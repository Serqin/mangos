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
SDName: boss_scourgelord_tyrannus
SD%Complete: 0%
SDComment:
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"

enum
{
	SAY_AGGRO                                   = -1658055,
    SAY_SLAY_1                                  = -1658056,
    SAY_SLAY_2                                  = -1658057,
    SAY_DEATH                                   = -1658058,
    SAY_DARK_MIGHT_1                            = -1658061,
    SAY_DARK_MIGHT_2                            = -1658062,

    SPELL_FORCEFUL_SMASH                        = 69155,
    H_SPELL_FORCEFUL_SMASH                      = 69627,
    SPELL_OVERLORDS_BRAND                       = 69172,
    SPELL_DARK_MIGHT                            = 69167,
    H_SPELL_DARK_MIGHT                          = 69629,
};


struct MANGOS_DLL_DECL boss_scourgelord_tyrannus : public ScriptedAI
{
	
	boss_scourgelord_tyrannus(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
		Reset();
    }

	uint32 m_uiForcefulSmashTimer;
	uint32 m_uiOverlordsBrandTimer;
	uint32 m_uiDarkMightTimer;

	bool m_bIsRegularMode;

	void Reset()
    {
		m_uiForcefulSmashTimer = urand(7000,11000);
		m_uiOverlordsBrandTimer = urand(30000,45000);
		m_uiDarkMightTimer = urand(40000,55000);
    }

	void Aggro(Unit* pWho)
	{
		DoScriptText(SAY_AGGRO, m_creature);
	}

	void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

	void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }
	
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiForcefulSmashTimer < uiDiff)
		{
			if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FORCEFUL_SMASH : H_SPELL_FORCEFUL_SMASH) == CAST_OK)
				m_uiForcefulSmashTimer = urand(7000,11000);
		}
		else
			m_uiForcefulSmashTimer -= uiDiff;

		if (m_uiOverlordsBrandTimer < uiDiff)
		{
			if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
				if (DoCastSpellIfCan(pTarget, SPELL_OVERLORDS_BRAND) == CAST_OK)
					m_uiOverlordsBrandTimer = urand(30000,45000);
		}
		else
			m_uiOverlordsBrandTimer -= uiDiff;

		if (m_uiDarkMightTimer < uiDiff)
		{
			if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_DARK_MIGHT : H_SPELL_DARK_MIGHT) == CAST_OK)
			{
				DoScriptText(SAY_DARK_MIGHT_1, m_creature);
                DoScriptText(SAY_DARK_MIGHT_2, m_creature);
				m_uiDarkMightTimer = urand(40000,55000);
			}
		}
		else
			m_uiDarkMightTimer -= uiDiff;


        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_boss_scourgelord_tyrannus(Creature* pCreature)
{
    return new boss_scourgelord_tyrannus(pCreature);
}


void AddSC_boss_scourgelord_tyrannus()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_scourgelord_tyrannus";
    NewScript->GetAI = &GetAI_boss_scourgelord_tyrannus;
    NewScript->RegisterSelf();
}