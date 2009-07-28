
#include "precompiled.h"
#include "def_hyjal.h"
#include "hyjal_trash.h"

// Spells
enum Spells
{
	SPELL_CARRION_SWARM			= 31306,
	SPELL_SLEEP					= 31298,
	SPELL_VAMPIRIC_AURA			= 38196,
	SPELL_INFERNO				= 31299
};

// Speech
enum
{
	SAY_ONDEATH			= -1999926,
	SAY_ONSLAY1			= -1999927,
	SAY_ONSLAY2			= -1999928,
	SAY_ONSLAY3			= -1999929,
	SAY_SWARM1			= -1999930,
	SAY_SWARM2			= -1999931,
	SAY_SLEEP1			= -1999932,
	SAY_SLEEP2			= -1999933,
	SAY_INFERNO1		= -1999934,
	SAY_INFERNO2		= -1999935,
	SAY_ONAGGRO			= -1999936
};

struct MANGOS_DLL_DECL boss_anetheronAI : public hyjal_trashAI
{
    boss_anetheronAI(Creature *c) : hyjal_trashAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        go = false;    
        pos = 0;
        Reset();
        SpellEntry *TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_SLEEP);
        if(TempSpell && TempSpell->EffectImplicitTargetA[0] != 1)
        {
            TempSpell->EffectImplicitTargetA[0] = 1;
            TempSpell->EffectImplicitTargetB[0] = 0;
        }
    }

    uint32 SwarmTimer;
    uint32 SleepTimer;
    uint32 AuraTimer;
    uint32 InfernoTimer;
    bool go;
    uint32 pos;

    void Reset()
    {
        SwarmTimer = 45000;
        SleepTimer = 60000;
        AuraTimer = 5000;
        InfernoTimer = 45000;

        if(pInstance && IsEvent)
            pInstance->SetData(DATA_ANETHERONEVENT, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {    
        if(pInstance && IsEvent)
            pInstance->SetData(DATA_ANETHERONEVENT, IN_PROGRESS);
        DoScriptText(SAY_ONAGGRO, m_creature);
    }

    void KilledUnit(Unit *victim)
    {
        switch(rand()%3)
        {
            case 0:
                DoScriptText(SAY_ONSLAY1, m_creature);
                break;
            case 1:
                DoScriptText(SAY_ONSLAY2, m_creature);
                break;        
            case 2:
                DoScriptText(SAY_ONSLAY3, m_creature);
                break;    
        }        
    }

    void WaypointReached(uint32 i)
    {
        pos = i;
        if (i == 7 && pInstance)
        {
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_JAINAPROUDMOORE));
            if (target && target->isAlive())
                m_creature->AddThreat(target,0.0);
        }        
    }

    void JustDied(Unit *victim)
    {
        if(pInstance && IsEvent)
            pInstance->SetData(DATA_ANETHERONEVENT, DONE);
        DoScriptText(SAY_ONDEATH, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (IsEvent)
        {
            //Must update npc_escortAI
            npc_escortAI::UpdateAI(diff);
            if(!go)
            {
                go = true;
                if(pInstance)
                {
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(0, 4896.08,    -1576.35,    1333.65);
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(1, 4898.68,    -1615.02,    1329.48);
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(2, 4907.12,    -1667.08,    1321.00);
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(3, 4963.18,    -1699.35,    1340.51);
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(4, 4989.16,    -1716.67,    1335.74);
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(5, 5026.27,    -1736.89,    1323.02);
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(6, 5037.77,    -1770.56,    1324.36);
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(7, 5067.23,    -1789.95,    1321.17);
                    ((npc_escortAI*)(m_creature->AI()))->Start(false, true, true);
                    //((npc_escortAI*)(m_creature->AI()))->SetDespawnAtEnd(false);
                }            
            }
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(SwarmTimer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if(target)
                DoCast(target,SPELL_CARRION_SWARM);

            SwarmTimer = 45000+rand()%15000;
            switch(rand()%2)
            {
                case 0:
                    DoScriptText(SAY_SWARM1, m_creature);
                    break;
                case 1:
                    DoScriptText(SAY_SWARM2, m_creature);
                    break;        
            }    
        }else SwarmTimer -= diff;

        if(SleepTimer < diff)
        {
            for(uint8 i=0;i<3;++i)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
                if(target)
                    target->CastSpell(target,SPELL_SLEEP,true);
            }
            SleepTimer = 60000;
            switch(rand()%2)
            {
                case 0:
                    DoScriptText(SAY_SLEEP1, m_creature);
                    break;
                case 1:
                    DoScriptText(SAY_SLEEP2, m_creature);
                    break;        
            }    
        }else SleepTimer -= diff;        
        if(AuraTimer < diff)
        {
            DoCast(m_creature, SPELL_VAMPIRIC_AURA,true);
            AuraTimer = 10000+rand()%10000;            
        }else AuraTimer -= diff;
        if(InfernoTimer < diff)
        {
            DoCast(SelectUnit(SELECT_TARGET_RANDOM,0), SPELL_INFERNO);
            InfernoTimer = 45000;
            switch(rand()%2)
            {
                case 0:
                    DoScriptText(SAY_INFERNO1, m_creature);
                    break;
                case 1:
                    DoScriptText(SAY_INFERNO2, m_creature);
                    break;        
            }    
        }else InfernoTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_anetheron(Creature *_Creature)
{
    return new boss_anetheronAI (_Creature);
}

#define SPELL_IMMOLATION 31303
#define SPELL_INFERNO_EFFECT 31302

struct MANGOS_DLL_DECL mob_towering_infernalAI : public ScriptedAI
{
    mob_towering_infernalAI(Creature *c) : ScriptedAI(c)
    {
        Reset();
    }

    uint32 ImmolationTimer;

    void Reset()
    {
        DoCast(m_creature, SPELL_INFERNO_EFFECT);
        ImmolationTimer = 5000;
    }

    void Aggro(Unit *who)
    {    
        
    }

    void KilledUnit(Unit *victim)
    {
                
    }

    void JustDied(Unit *victim)
    {
        
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (m_creature->GetDistance(who) <= 50 && !m_creature->isInCombat() && m_creature->IsHostileTo(who))
        {
            m_creature->AddThreat(who,0.0);
            m_creature->Attack(who,false);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(ImmolationTimer < diff)
        {
            DoCast(m_creature, SPELL_IMMOLATION);        
            ImmolationTimer = 5000;
        }else ImmolationTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_towering_infernal(Creature *_Creature)
{
    return new mob_towering_infernalAI (_Creature);
}

void AddSC_boss_anetheron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_anetheron";
    newscript->GetAI = &GetAI_boss_anetheron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_towering_infernal";
    newscript->GetAI = &GetAI_mob_towering_infernal;
    newscript->RegisterSelf();    
}
