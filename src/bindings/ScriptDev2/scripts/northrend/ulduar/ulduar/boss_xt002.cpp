#include "precompiled.h"
#include "ulduar.h"

enum
{
	SPELL_TANTRUM =        62776,
	SPELL_SEARING_LIGHT =  63018,
	SPELL_GRAVITY =	       63024,

	SAY_AGGRO =			-1603000,
	SAY_DEATH =			-1603008,
	SAY_TANTRUM =		-1603001,
	SAY_SLAY_01 =		-1603002,
	SAY_SLAY_02 =		-1603003,
	SAY_BERSERK =		-1603007,
	SAY_ADDS =			-1603006,
	SAY_HEART_OPEN =	-1603004,
	SAY_HEART_CLOSE =	-1603005
};

struct MANGOS_DLL_DECL boss_xt002 : public ScriptedAI
{
    boss_xt002(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *pInstance;
    bool Regular;

    uint32 TantrumTimer;
    uint32 LightTimer;
    uint32 GravityTimer;

    void Reset()
    {
        TantrumTimer = urand(30000, 40000);
        LightTimer = urand(8000,10000);
        GravityTimer = urand(10000,15000);

        if(pInstance) pInstance->SetData(TYPE_XT002, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_XT002, IN_PROGRESS);
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_XT002, DONE);
        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit *who)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY_01, m_creature);
        else
            DoScriptText(SAY_SLAY_02, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(TantrumTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_TANTRUM);
			TantrumTimer = urand(30000, 40000);
            DoScriptText(SAY_TANTRUM, m_creature);
        }
        else TantrumTimer -= diff;

        if(LightTimer < diff)
        {
            Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if(target && target->isAlive())
                DoCastSpellIfCan(target, SPELL_SEARING_LIGHT);
			LightTimer = urand(8000,10000);
        }
        else LightTimer -= diff;

        if(GravityTimer < diff)
        {
            Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if(target && target->isAlive())
				DoCastSpellIfCan(target, SPELL_GRAVITY);
            GravityTimer = urand(10000,15000);
        }
        else GravityTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_xt002(Creature* pCreature)
{
    return new boss_xt002(pCreature);
}

void AddSC_boss_xt002()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_xt002";
    newscript->GetAI = &GetAI_boss_xt002;
    newscript->RegisterSelf();
}