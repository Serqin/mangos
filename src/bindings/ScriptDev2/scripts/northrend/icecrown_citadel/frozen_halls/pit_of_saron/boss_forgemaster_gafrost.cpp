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
SDName: boss_forgemaster_gafrost
SD%Complete: 0%
SDComment:
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_AGGRO                                   = -1658001,
    SAY_SLAY_1                                  = -1658002,
    SAY_SLAY_2                                  = -1658003,
    SAY_DEATH                                   = -1658004,
    SAY_PHASE2                                  = -1658005,
    SAY_PHASE3                                  = -1658006,

    SPELL_PERMAFROST                            = 70326,
    SPELL_PERMAFROST_TRIGGER                    = 68786, // triggered by PERMAFROST. Used to check aura
    SPELL_THROW_SARONITE                        = 68788,
    SPELL_THUNDERING_STOMP                      = 68771,
    SPELL_CHILLING_WAVE                         = 68778,
    H_SPELL_CHILLING_WAVE                       = 70333,
    SPELL_DEEP_FREEZE                           = 70381,
    H_SPELL_DEEP_FREEZE                         = 72930,
    SPELL_FORGE_MACE                            = 68785,
    H_SPELL_FORGE_MACE                          = 70335,
    SPELL_FORGE_BLADE                           = 68774,
    H_SPELL_FORGE_BLADE                         = 70334,

    EQUIP_ID_SWORD                              = 49345,
    EQUIP_ID_MACE                               = 49344,
    ACHIEV_DOESNT_GO_TO_ELEVEN                  = 4524,
};

struct MANGOS_DLL_DECL boss_forgemaster_gafrost : public ScriptedAI
{
	
	boss_forgemaster_gafrost(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
		Reset();
    }

    uint8 EventPhase;

	uint32 m_uiThrowSaroniteTimer;
	uint32 m_uiChillingWaveTimer;
	uint32 m_uiDeepFreezeTimer;

	bool m_bIsRegularMode;

	void Reset()
    {
		EventPhase = 0;
		m_uiThrowSaroniteTimer = 45000;
		m_uiChillingWaveTimer = 10000;
		m_uiDeepFreezeTimer = 10000;
		SetEquipmentSlots(true);
    }

	void Aggro(Unit* pWho)
	{
		DoScriptText(SAY_AGGRO, m_creature);
		DoCastSpellIfCan(m_creature, SPELL_PERMAFROST, CAST_TRIGGERED);
	}

	void DamageTaken(Unit *done_by, uint32 &damage)
    {
		if (EventPhase == 0 && m_creature->GetHealthPercent() < 66){
			DoCastSpellIfCan(m_creature, SPELL_THUNDERING_STOMP, CAST_TRIGGERED);
			if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FORGE_BLADE : H_SPELL_FORGE_BLADE, CAST_TRIGGERED) == CAST_OK){
				SetEquipmentSlots(false, EQUIP_ID_SWORD, -1, -1);
				//m_creature->SetByteValue(UNIT_FIELD_BYTES_2, 0, SHEATH_STATE_MELEE);
				EventPhase = 1;
				DoScriptText(SAY_PHASE2, m_creature);
			}
		}

		if (EventPhase == 1 && m_creature->GetHealthPercent() < 33){
			DoCastSpellIfCan(m_creature, SPELL_THUNDERING_STOMP, CAST_TRIGGERED);
			if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FORGE_MACE : H_SPELL_FORGE_MACE, CAST_TRIGGERED) == CAST_OK){
				SetEquipmentSlots(false, EQUIP_ID_MACE, -1, -1);
				//m_creature->SetByteValue(UNIT_FIELD_BYTES_2, 0, SHEATH_STATE_MELEE);
				EventPhase = 2;
				DoScriptText(SAY_PHASE3, m_creature);
			}
		}
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

		switch(EventPhase){
			case 0:

				if (m_uiThrowSaroniteTimer < uiDiff)
				{
					if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
						if (DoCastSpellIfCan(pTarget, SPELL_THROW_SARONITE) == CAST_OK)
							m_uiThrowSaroniteTimer = 35000;
				}
				else
					m_uiThrowSaroniteTimer -= uiDiff;

				break;

			case 1:

				if (m_uiChillingWaveTimer < uiDiff)
				{
					if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
						if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_CHILLING_WAVE : H_SPELL_CHILLING_WAVE) == CAST_OK)
							m_uiChillingWaveTimer = 40000;
				}
				else
					m_uiChillingWaveTimer -= uiDiff;


				break;

			case 2:

				if (m_uiDeepFreezeTimer < uiDiff)
				{
					if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
						if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_DEEP_FREEZE : H_SPELL_DEEP_FREEZE) == CAST_OK)
							m_uiDeepFreezeTimer = 35000;
				}
				else
					m_uiDeepFreezeTimer -= uiDiff;

				break;
		}
        

        DoMeleeAttackIfReady();
    }

};



CreatureAI* GetAI_boss_forgemaster_gafrost(Creature* pCreature)
{
    return new boss_forgemaster_gafrost(pCreature);
}

void AddSC_boss_forgemaster_gafrost()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_forgemaster_gafrost";
    NewScript->GetAI = &GetAI_boss_forgemaster_gafrost;
    NewScript->RegisterSelf();
}