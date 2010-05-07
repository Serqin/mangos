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
#include "def_forge.h"

enum BossSpells
{
        //common
        SPELL_BERSERK                           = 47008,
        //yells
        //summons
        NPC_SOUL_FRAGMENT                       = 36535,
        //Abilities
        SPELL_MAGIC_BANE                        = 68793,
        SPELL_CORRUPT_SOUL                      = 68839,
        SPELL_CONSUME_SOUL                      = 68858,
        SPELL_TELEPORT                          = 68988,
        SPELL_SOULSTORM                         = 68872,
        SPELL_SOULSTORM_2                       = 68921,
        SPELL_FEAR                              = 68950,
        SPELL_SHADOW_BOLT                       = 70043,
};

struct MANGOS_DLL_DECL boss_bronjahmAI : public ScriptedAI
{
    boss_bronjahmAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance *pInstance;
    uint8 stage;
	uint32 m_corruptSoul_Timer;
	uint32 m_fear_Timer;
	uint32 m_shadowBolt_Timer;
	uint32 m_magicBane_Timer;

    void Reset()
    {
		if(pInstance) pInstance->SetData(TYPE_BRONJAHM, NOT_STARTED);
        stage = 0;
		m_corruptSoul_Timer = urand(15000, 25000);
		m_fear_Timer = urand(8000, 12000);
		m_shadowBolt_Timer = urand(8000, 12000);
		m_magicBane_Timer = urand(3000, 8000);

    }

    void Aggro(Unit *who) 
    {
		if(pInstance) pInstance->SetData(TYPE_BRONJAHM, IN_PROGRESS);
        DoScriptText(-1632001,m_creature,who);
        SetCombatMovement(true);
    }

    void JustDied(Unit *killer)
    {
		if(pInstance) pInstance->SetData(TYPE_BRONJAHM, DONE);
        DoScriptText(-1632004,m_creature,killer);
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1632002,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1632003,m_creature,pVictim);
               break;
        };
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: 

				if (m_corruptSoul_Timer < uiDiff){

					if (Unit * pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0)){
						if (DoCastSpellIfCan(pTarget, SPELL_CORRUPT_SOUL, CAST_TRIGGERED) == CAST_OK){
							DoScriptText(-1632006,m_creature,pTarget);
							m_creature->SummonCreature(
								NPC_SOUL_FRAGMENT, 
								pTarget->GetPositionX(), 
								pTarget->GetPositionY(), 
								pTarget->GetPositionZ(), 
								pTarget->GetOrientation(), 
								TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 
								30 * IN_MILLISECONDS
							);
						}
					}

					m_corruptSoul_Timer = urand(15000, 25000);

				} else m_corruptSoul_Timer -= uiDiff;

				break;

            case 1:
					
				if (DoCastSpellIfCan(m_creature, SPELL_TELEPORT) == CAST_OK) stage = 2;

				break;

            case 2:
				if (DoCastSpellIfCan(m_creature, SPELL_SOULSTORM, CAST_TRIGGERED) == CAST_OK){
					DoScriptText(-1632005,m_creature);
					SetCombatMovement(false);
					stage = 3;
				}
				break;
            case 3:
				if (m_fear_Timer < uiDiff){
					DoCastSpellIfCan(m_creature, SPELL_FEAR);
					m_fear_Timer = urand(8000,12000);
				} else m_fear_Timer -= uiDiff;
        }

		if (m_shadowBolt_Timer < uiDiff){
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOW_BOLT);
			m_shadowBolt_Timer = urand(8000,12000);
		} else m_shadowBolt_Timer -= uiDiff;

		if (m_magicBane_Timer < uiDiff){
			DoCastSpellIfCan(m_creature, SPELL_MAGIC_BANE);
			m_magicBane_Timer = urand(3000, 8000);
		} else m_magicBane_Timer -= uiDiff;

        if (m_creature->GetHealthPercent() <= 30.0f && stage == 0) stage = 1;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_soul_fragmentAI : public ScriptedAI
{
    mob_soul_fragmentAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;
    Creature* pBoss;
    uint32 m_uiRangeCheck_Timer;

    void Reset()
    {
        m_uiRangeCheck_Timer = 1000;
        if (!m_pInstance) return;
        pBoss = (Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_BRONJAHM));
		if (pBoss){
			m_creature->SetSpeedRate(MOVE_RUN, 0.3f);
			m_creature->GetMotionMaster()->MoveChase(pBoss);
			m_creature->SetRespawnDelay(DAY);
		}
    }

    void AttackStart(Unit* pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_BRONJAHM) != IN_PROGRESS ) m_creature->ForcedDespawn();

        if (m_uiRangeCheck_Timer < uiDiff)
        {
            if (pBoss->IsWithinDistInMap(m_creature, 2.0f))
            {
                pBoss->CastSpell(pBoss, SPELL_CONSUME_SOUL, false);
                m_creature->ForcedDespawn();
            } else m_creature->GetMotionMaster()->MoveChase(pBoss);

            m_uiRangeCheck_Timer = 1000;
        }
        else m_uiRangeCheck_Timer -= uiDiff;
    }

};

CreatureAI* GetAI_boss_bronjahm(Creature* pCreature)
{
    return new boss_bronjahmAI(pCreature);
}

CreatureAI* GetAI_mob_soul_fragment(Creature* pCreature)
{
    return new mob_soul_fragmentAI (pCreature);
}


void AddSC_boss_bronjahm()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_bronjahm";
    newscript->GetAI = &GetAI_boss_bronjahm;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_soul_fragment";
    newscript->GetAI = &GetAI_mob_soul_fragment;
    newscript->RegisterSelf();

}