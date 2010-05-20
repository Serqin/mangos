#include "precompiled.h"
#include "ulduar.h"

enum
{
	SPELL_FLAME_JETS = 62680,
	SPELL_SCORCH = 62546,
	SPELL_SLAG_POT = 62717
};

struct MANGOS_DLL_DECL boss_ignisAI : public ScriptedAI
{
    boss_ignisAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *pInstance;
    bool Regular;
    //Timers
    uint32 ScorchTimer;
    uint32 SlagPotTimer;
    uint32 FlameJetsTimer;
    uint32 SummonTimer;

    void Reset()
    {
        ScorchTimer = urand(20000, 30000);
        SlagPotTimer = urand(10000, 20000);
		FlameJetsTimer = urand(15000, 25000);

        if(pInstance) pInstance->SetData(TYPE_IGNIS, NOT_STARTED);
    }

    void Aggro(Unit* who) 
    {
        if(pInstance) pInstance->SetData(TYPE_IGNIS, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_IGNIS, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (ScorchTimer < diff)
        {
			DoCastSpellIfCan(m_creature, SPELL_SCORCH);
            ScorchTimer = urand(10000, 20000);
        }
        else ScorchTimer -= diff;

        if (FlameJetsTimer < diff)
        {
			DoCastSpellIfCan(m_creature, SPELL_FLAME_JETS);
            FlameJetsTimer = urand(15000, 25000);
        }
        else FlameJetsTimer -= diff;

        /*if (SlagPotTimer < diff)
        {
            Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if(target && target->isAlive())
				DoCastSpellIfCan(target, SPELL_SLAG_POT);
            SlagPotTimer =  urand(10000, 20000);
        }
        else SlagPotTimer -= diff;*/

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ignisAI(Creature* pCreature)
{
    return new boss_ignisAI(pCreature);
}

void AddSC_boss_ignis()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ignis";
    newscript->GetAI = &GetAI_boss_ignisAI;
    newscript->RegisterSelf();
}