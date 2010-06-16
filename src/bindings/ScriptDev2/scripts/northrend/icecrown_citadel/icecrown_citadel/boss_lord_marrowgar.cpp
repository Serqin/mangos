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
SDName: boss_lord_marrowgar
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"

enum
{
        //common
        SPELL_BERSERK                           = 47008,
        //yells
        //summons
        NPC_BONE_SPIKE                          = 38711,
        NPC_COLDFLAME                           = 36672,
        //Abilities
        SPELL_BONE_SLICE                        = 69055,
        SPELL_CALL_COLD_FLAME                   = 69138,
        SPELL_CALL_COLD_FLAME_1                 = 71580,
        SPELL_COLD_FLAME                        = 69146,
        SPELL_COLD_FLAME_0                      = 69145,
        SPELL_BONE_STRIKE                       = 69057,
        SPELL_BONE_STORM                        = 69076,
        SPELL_BONE_STRIKE_IMPALE                = 69065,
        SPELL_BONE_STORM_STRIKE                 = 69075,
};

struct MANGOS_DLL_DECL boss_lord_marrowgarAI : public ScriptedAI
{
    boss_lord_marrowgarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    uint8 stage;
	uint32 uiBoneStrikeTimer;
	uint32 uiBoneSliceTimer;
	uint32 uiColdFlameTimer;

    bool intro;

    void Reset()
    {
        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, NOT_STARTED);
		uiBoneStrikeTimer = urand(17000, 27000);
		uiBoneSliceTimer = urand(5000, 10000);
		uiColdFlameTimer = urand(6000, 12000);
        stage = 0;
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
		ScriptedAI::MoveInLineOfSight(pWho);
        if (intro) return;
        DoScriptText(-1631000,m_creature);
        intro = true;
    }

    void JustReachedHome()
    {
        if (pInstance) pInstance->SetData(TYPE_MARROWGAR, FAIL);
    }

/*
    void JustSummoned(Creature* _summoned)
    {
        if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
            _summoned->AddThreat(target);
    }
*/
    void Aggro(Unit *who) 
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_MARROWGAR, IN_PROGRESS);
        DoScriptText(-1631001,m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631006,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631007,m_creature,pVictim);
               break;
        };
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, DONE);
        DoScriptText(-1631009,m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (uiBoneStrikeTimer < diff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
			{
				if (DoCastSpellIfCan(pTarget, SPELL_BONE_STRIKE) == CAST_OK)
				{
					switch (urand(0,2))
					{
					case 0:
						DoScriptText(-1631003,m_creature,pTarget);
						break;
					case 1:
						DoScriptText(-1631004,m_creature,pTarget);
						break;
					case 2:
						DoScriptText(-1631005,m_creature,pTarget);
						break;
					}
					Creature* pSpike = m_creature->SummonCreature(NPC_BONE_SPIKE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 180000);
					if (pSpike)
					{
						pSpike->AddThreat(pTarget, 100.0f);
					}
					uiBoneStrikeTimer = urand(17000, 27000);
				}
			}
		}
		else uiBoneStrikeTimer -= diff;

		if (uiBoneSliceTimer < diff)
		{
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_BONE_SLICE);
			uiBoneSliceTimer = urand(5000, 10000);
		}
		else uiBoneSliceTimer -= diff;

		if (uiColdFlameTimer < diff)
		{
			DoCastSpellIfCan(m_creature, SPELL_CALL_COLD_FLAME);
			uiColdFlameTimer = urand(6000, 12000);
		}
		else uiColdFlameTimer -= diff;

        DoMeleeAttackIfReady();

    }
};

struct MANGOS_DLL_DECL mob_coldflameAI : public ScriptedAI
{
    mob_coldflameAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 uiColdFlameTimer;

    float fPosX, fPosY, fPosZ;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->GetPosition(fPosX, fPosY, fPosZ);
        m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(150, 200), fPosX, fPosY, fPosZ);
        m_creature->GetMotionMaster()->MovePoint(1, fPosX, fPosY, fPosZ);
        SetCombatMovement(false);
        m_creature->SetSpeedRate(MOVE_RUN, 0.8f);
		DoCastSpellIfCan(m_creature, SPELL_COLD_FLAME);
		uiColdFlameTimer = 2000;
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(!m_pInstance) return;
        if(type != POINT_MOTION_TYPE) return;
		error_log("Movement type: %d, id: %d", type, id);
        if(id != 1)
             m_creature->GetMotionMaster()->MovePoint(1, fPosX, fPosY, fPosZ);
             else m_creature->ForcedDespawn();
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if (!m_creature)
			return;

		if (uiColdFlameTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature, SPELL_COLD_FLAME_0);
			uiColdFlameTimer = 2000;
		}
		else uiColdFlameTimer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};


struct MANGOS_DLL_DECL mob_bone_spikeAI : public ScriptedAI
{
    mob_bone_spikeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;
    Unit* pVictim;


    void Reset()
    {
        pVictim = NULL;
    }

    void Aggro(Unit* pWho)
    {
        if (!pVictim && pWho)  {
                        pVictim = pWho;
                        m_creature->SetInCombatWith(pVictim);
                        m_creature->SetSpeedRate(MOVE_RUN, 5.0f);
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
            if (pVictim) pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
    }

    void KilledUnit(Unit* _Victim)
    {
        if (pVictim) pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
    }

    void JustDied(Unit* Killer)
    {
        if (pVictim) pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_MARROWGAR) != IN_PROGRESS)
        {
            if (pVictim) pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
                m_creature->ForcedDespawn();
        }

        if (!pVictim) return;

		if(pVictim && !pVictim->isAlive())
        {
			pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
            m_creature->ForcedDespawn();
        }

		if(m_creature->IsWithinDistInMap(pVictim, 1.0f) &&
             !pVictim->HasAura(SPELL_BONE_STRIKE_IMPALE))
        {
			DoCastSpellIfCan(pVictim, SPELL_BONE_STRIKE_IMPALE);
            m_creature->GetMotionMaster()->Clear();
            SetCombatMovement(false);
        }

    }

};

CreatureAI* GetAI_mob_bone_spike(Creature* pCreature)
{
    return new mob_bone_spikeAI(pCreature);
}

CreatureAI* GetAI_mob_coldflame(Creature* pCreature)
{
    return new mob_coldflameAI(pCreature);
}

CreatureAI* GetAI_boss_lord_marrowgar(Creature* pCreature)
{
    return new boss_lord_marrowgarAI(pCreature);
}

void AddSC_boss_lord_marrowgar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lord_marrowgar";
    newscript->GetAI = &GetAI_boss_lord_marrowgar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_coldflame";
    newscript->GetAI = &GetAI_mob_coldflame;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_bone_spike";
    newscript->GetAI = &GetAI_mob_bone_spike;
    newscript->RegisterSelf();

}
