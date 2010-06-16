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
SDName: boss_lady_deathwhisper
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"
enum BossSpells
{
        //common
        SPELL_BERSERK                           = 47008,
        //yells
        //summons
        NPC_VENGEFUL_SHADE                      = 38222,
        NPC_FANATIC                             = 37890,
        NPC_REANIMATED_FANATIC                  = 38009,
        NPC_ADHERENT                            = 37949,
        NPC_REANIMATED_ADHERENT                 = 38010,
        //Abilities
        SPELL_MANA_BARRIER                      = 70842,
        SPELL_SHADOW_BOLT                       = 71254,
        SPELL_DEATH_AND_DECAY                   = 71001,
        SPELL_DARK_EMPOWERMENT                  = 70901,
        SPELL_FROSTBOLT                         = 71420,
        SPELL_INSIGNIFICANCE                    = 71204,

        SPELL_DOMINATE_MIND                     = 71289,

        SPELL_VENGEFUL_BLAST                    = 71494,
        SPELL_VENGEFUL_BLAST_0                  = 71544,
        // summons
        SPELL_FROST_FEVER                       = 71129,
        SPELL_DEATHCHILL_BOLT                   = 70594,
        SPELL_DEATHCHILL_BLAST                  = 70906,
        SPELL_DARK_MARTYRDROM                   = 70903,
        SPELL_CURSE_OF_TOPOR                    = 71237,
        SPELL_SHORUD_OF_THE_OCCULUT             = 70768,
        SPELL_ADHERENTS_DETERMINIATION          = 71234,
        SPELL_SUMMON_VISUAL                     = 41236,

        SPELL_NECROTIC_STRIKE                   = 70659,
        SPELL_SHADOW_CLEAVE                     = 70670,
        SPELL_VAMPIRIC_MIGHT                    = 70674,
};

static Locations SpawnLoc[]=
{
    {-623.055481f, 2211.326660f, 51.764259f},  // 0 Lady's stay point
    {-620.197449f, 2272.062256f, 50.848679f},  // 1 Right Door 1
    {-598.636353f, 2272.062256f, 50.848679f},  // 2 Right Door 2
    {-578.495728f, 2272.062256f, 50.848679f},  // 3 Right Door 3
    {-578.495728f, 2149.211182f, 50.848679f},  // 4 Left Door 1
    {-598.636353f, 2149.211182f, 50.848679f},  // 5 Left Door 2
    {-620.197449f, 2149.211182f, 50.848679f},  // 6 Left Door 3
    {-517.652466f, 2216.611328f, 62.823681f},  // 7 Upper marsh 1
    {-517.652466f, 2211.611328f, 62.823681f},  // 8 Upper marsh 2
    {-517.652466f, 2206.611328f, 62.823681f},  // 9 Upper marsh 3
};

struct MANGOS_DLL_DECL boss_lady_deathwhisperAI : public ScriptedAI
{
    boss_lady_deathwhisperAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    uint8 stage;
    uint8 Difficulty;
    bool MovementStarted;
    bool intro;

	uint32 uiShadowBoltTimer;
	uint32 uiNpcFanaticTimer;
	uint32 uiDarkEmpowermentTimer;
	uint32 uiDeathAndDecayTimer;
	uint32 uiFrostBoltTimer;
	uint32 uiInsignificanceTimer;

    void Reset()
    {
        if(!pInstance) return;
        Difficulty = pInstance->GetData(TYPE_DIFFICULTY);
        pInstance->SetData(TYPE_DEATHWHISPER, NOT_STARTED);
        stage = 0;
        MovementStarted = false;
        intro = false;

		uiShadowBoltTimer = urand(4000, 7000);
		uiNpcFanaticTimer = urand(42000, 65000);
		uiDarkEmpowermentTimer = urand(10000, 20000);
		uiDeathAndDecayTimer = urand(10000, 15000);
		uiFrostBoltTimer = urand(15000, 20000);
		uiInsignificanceTimer = urand(8000, 15000);
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
		ScriptedAI::MoveInLineOfSight(pWho);
        if (stage) return;
        else intro = true;
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631029,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631030,m_creature,pVictim);
               break;
        };
    }

    void JustReachedHome()
    {
        if (pInstance) pInstance->SetData(TYPE_DEATHWHISPER, FAIL);
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(!pInstance) return;
        if(type != POINT_MOTION_TYPE) return;
        if(MovementStarted && id != 1)
        {
			m_creature->GetMotionMaster()->MovePoint(1, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z);
        }
        else
		{
			m_creature->GetMotionMaster()->MovementExpired();
			MovementStarted = false;
			SetCombatMovement(false);
		}
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_DEATHWHISPER, IN_PROGRESS);
		DoCastSpellIfCan(m_creature, SPELL_MANA_BARRIER);
        MovementStarted = true;
        SetCombatMovement(false);
        DoScriptText(-1631023,m_creature);
        m_creature->GetMotionMaster()->MovePoint(1, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_DEATHWHISPER, DONE);
        DoScriptText(-1631032,m_creature,killer);
    }

    void JustSummoned(Creature* summoned)
    {
        if(!pInstance || !summoned) return;

        if (Unit* pTarget= m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
		{
			summoned->AddThreat(pTarget, 100.0f);
		}
    }

    void CallGuard(uint8 place)
    {
		if (place < 2) 
		{
			m_creature->SummonCreature(urand(0,1) ? NPC_FANATIC : NPC_ADHERENT, SpawnLoc[3*place+1].x, SpawnLoc[3*place+1].y, SpawnLoc[3*place+1].z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 300*IN_MILLISECONDS);
			m_creature->SummonCreature(urand(0,1) ? NPC_FANATIC : NPC_ADHERENT, SpawnLoc[3*place+3].x, SpawnLoc[3*place+3].y, SpawnLoc[3*place+3].z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 300*IN_MILLISECONDS);
		}
		m_creature->SummonCreature(urand(0,1) ? NPC_FANATIC : NPC_ADHERENT, SpawnLoc[3*place+2].x, SpawnLoc[3*place+2].y, SpawnLoc[3*place+2].z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 300*IN_MILLISECONDS);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_creature || !m_creature->isAlive())
            return;

		if (m_creature->HasAura(SPELL_MANA_BARRIER))
		{
			if (m_creature->GetPower(POWER_MANA) > uiDamage)
			{
				m_creature->SetPower(POWER_MANA,m_creature->GetPower(POWER_MANA)-uiDamage);
				uiDamage = 0;
			}
			else
			{
				m_creature->SetPower(POWER_MANA,0);
				m_creature->RemoveAurasDueToSpell(SPELL_MANA_BARRIER);
			}
		}
    }

    void UpdateAI(const uint32 diff)
    {
		
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (intro)
		{
			if (uiShadowBoltTimer < diff)
			{
				DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOW_BOLT);
				switch (stage)
				{
					case 0:
						DoScriptText(-1631020,m_creature);
                        stage = 1;
                        break;
					case 1:
						DoScriptText(-1631021,m_creature);
						stage = 2;
						break;
					case 2:
						DoScriptText(-1631022,m_creature);
                        stage = 3;
						intro = false;
                        break;
					default:
						break;
				}
				uiShadowBoltTimer = urand(5000, 8000);
			}
			else uiShadowBoltTimer -= diff;
		}

		if (m_creature->HasAura(SPELL_MANA_BARRIER)) {
             if(m_creature->GetHealth() <= m_creature->GetMaxHealth()) {
                  if (m_creature->GetPower(POWER_MANA) > (m_creature->GetMaxHealth() - m_creature->GetHealth()))
                        {
                         m_creature->SetPower(POWER_MANA,m_creature->GetPower(POWER_MANA)-(m_creature->GetMaxHealth() - m_creature->GetHealth()));
                         m_creature->SetHealth(m_creature->GetMaxHealth());
                        }
                        else m_creature->SetPower(POWER_MANA,0);
            }
        }

        if (MovementStarted) return;

        switch(stage)
		{
			case 3:
				if (IsCombatMovement())
					SetCombatMovement(false);
				
				if (uiShadowBoltTimer < diff)
				{
					DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOW_BOLT);
					uiShadowBoltTimer = urand(5000, 8000);
				}
				else uiShadowBoltTimer -= diff;
				
				if (uiNpcFanaticTimer < diff)
				{
					DoScriptText(-1631028,m_creature);
					CallGuard(urand(0,1));
					uiNpcFanaticTimer = urand(42000, 65000);
				}
				else uiNpcFanaticTimer -= diff;
				
				if (uiDarkEmpowermentTimer < diff)
				{
					switch (urand(0,1))
					{
					case 0:
						if(Creature *pGuard = GetClosestCreatureWithEntry(m_creature, NPC_FANATIC, 100.0f))
						{
							DoCastSpellIfCan(pGuard, SPELL_DARK_EMPOWERMENT);
							DoScriptText(-1631026, m_creature);
						}
						break;
					case 1:
						if(Creature *pGuard = GetClosestCreatureWithEntry(m_creature, NPC_ADHERENT, 100.0f))
						{
							DoCastSpellIfCan(pGuard, SPELL_DARK_EMPOWERMENT);
							DoScriptText(-1631027,m_creature);
						}
						break;
					}
					uiDarkEmpowermentTimer = urand(10000, 20000);
				}
				else uiDarkEmpowermentTimer -= diff;
				break;

            case 4:
				if (uiFrostBoltTimer < diff)
				{
					DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROSTBOLT);
					uiFrostBoltTimer = urand(15000, 20000);
				}
				else uiFrostBoltTimer -= diff;
				if (uiInsignificanceTimer < diff)
				{
					DoCastSpellIfCan(m_creature->getVictim(), SPELL_INSIGNIFICANCE);
					uiInsignificanceTimer = urand(8000, 15000);
				}
				else uiInsignificanceTimer -= diff;

                    //bsw->timedCast(NPC_VENGEFUL_SHADE, diff);

                    //bsw->timedCast(SPELL_DOMINATE_MIND, diff);
				
				break;
        }

		if (uiDeathAndDecayTimer < diff)
		{
			DoCastSpellIfCan(m_creature, SPELL_DEATH_AND_DECAY, CAST_TRIGGERED & CAST_INTERRUPT_PREVIOUS);
			uiDeathAndDecayTimer = urand(10000, 15000);
		}
		else uiDeathAndDecayTimer -= diff;

		if (stage == 3 && !m_creature->HasAura(SPELL_MANA_BARRIER)) 
		{
			stage = 4;
			DoScriptText(-1631024,m_creature);
			SetCombatMovement(true);
			m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
		}
		
		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lady_deathwhisper(Creature* pCreature)
{
    return new boss_lady_deathwhisperAI(pCreature);
}

struct MANGOS_DLL_DECL mob_vengeful_shadeAI : public ScriptedAI
{
    mob_vengeful_shadeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;
	uint32 uiVengefulBlastTimer;

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetInCombatWithZone();
        if (Unit* pTarget= m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0) ) {
                m_creature->AddThreat(pTarget, 1000.0f);
                m_creature->GetMotionMaster()->MoveChase(pTarget);
                m_creature->SetSpeedRate(MOVE_RUN, 0.5);
                }
		DoCastSpellIfCan(m_creature, SPELL_VENGEFUL_BLAST);
		uiVengefulBlastTimer = 1000;
    }


    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_DEATHWHISPER) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (uiVengefulBlastTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_VENGEFUL_BLAST_0);
			if (m_creature->IsWithinDist(m_creature->getVictim(), 3.0f, false))
			{
				m_creature->ForcedDespawn();
			}
			else
			{
				m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
				m_creature->SetSpeedRate(MOVE_RUN, 0.5);
			}
			uiVengefulBlastTimer = 1000;
		}
		else uiVengefulBlastTimer -= uiDiff;
    }

};

CreatureAI* GetAI_mob_vengeful_shade(Creature* pCreature)
{
    return new mob_vengeful_shadeAI(pCreature);
}

struct MANGOS_DLL_DECL  mob_cult_adherentAI : public ScriptedAI
{
    mob_cult_adherentAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    bool bone;

	uint32 uiFrostFeverTimer;
	uint32 uiDeathchillBoltTimer;
	uint32 uiDeathchillBlastTimer;

    void Reset()
    {
		DoCastSpellIfCan(m_creature, SPELL_SUMMON_VISUAL);
        bone = false;
		uiFrostFeverTimer = urand(10000, 20000);
		uiDeathchillBoltTimer = urand(5000, 10000);
		uiDeathchillBlastTimer = urand(10000, 15000);
    }

    void Aggro(Unit *who) 
    {
		DoCastSpellIfCan(m_creature, SPELL_SHORUD_OF_THE_OCCULUT);
        DoStartMovement(who, 20.0f);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_DEATHWHISPER) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (uiFrostFeverTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROST_FEVER);
			uiFrostFeverTimer = urand(10000, 20000);
		}
		else uiFrostFeverTimer -= uiDiff;

		if (uiDeathchillBoltTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROST_FEVER);
			uiDeathchillBoltTimer = urand(10000, 20000);
		}
		else uiDeathchillBoltTimer -= uiDiff;

		if (uiDeathchillBlastTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_DEATHCHILL_BLAST);
			uiDeathchillBlastTimer = urand(10000, 15000);
		}
		else uiDeathchillBlastTimer -= uiDiff;
		
		if (m_creature->GetHealthPercent() < 15.0f && !bone)
		{
			if (!urand(0,3)) DoCastSpellIfCan(m_creature, SPELL_DARK_MARTYRDROM);//30%
			bone = true;
		}
    }
};

CreatureAI* GetAI_mob_cult_adherent(Creature* pCreature)
{
    return new mob_cult_adherentAI(pCreature);
}

struct MANGOS_DLL_DECL  mob_cult_fanaticAI : public ScriptedAI
{
    mob_cult_fanaticAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    bool bone;

	uint32 uiNecroticStrikeTimer;
	uint32 uiShadowCleaveTimer;

    void Reset()
    {
        bone = false;
		uiNecroticStrikeTimer = urand(5000, 10000);
		uiShadowCleaveTimer = urand(5000, 10000);
    }

    void Aggro(Unit *who) 
    {
		DoCastSpellIfCan(m_creature, SPELL_VAMPIRIC_MIGHT);
        DoStartMovement(who);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_DEATHWHISPER) != IN_PROGRESS) 
			m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (uiNecroticStrikeTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_NECROTIC_STRIKE);
			uiNecroticStrikeTimer = urand(5000, 10000);
		}
		else uiNecroticStrikeTimer -= uiDiff;

		if (uiShadowCleaveTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_NECROTIC_STRIKE);
			uiShadowCleaveTimer = urand(5000, 10000);
		}
		else uiShadowCleaveTimer -= uiDiff;

		if (m_creature->GetHealthPercent() < 15.0f && !bone)
		{
			if (!urand(0,3)) DoCastSpellIfCan(m_creature, SPELL_DARK_MARTYRDROM); //30%
			bone = true;
		}
    }
};

CreatureAI* GetAI_mob_cult_fanatic(Creature* pCreature)
{
    return new mob_cult_fanaticAI(pCreature);
}

void AddSC_boss_lady_deathwhisper()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lady_deathwhisper";
    newscript->GetAI = &GetAI_boss_lady_deathwhisper;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_vengeful_shade";
    newscript->GetAI = &GetAI_mob_vengeful_shade;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_cult_adherent";
    newscript->GetAI = &GetAI_mob_cult_adherent;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_cult_fanatic";
    newscript->GetAI = &GetAI_mob_cult_fanatic;
    newscript->RegisterSelf();
}