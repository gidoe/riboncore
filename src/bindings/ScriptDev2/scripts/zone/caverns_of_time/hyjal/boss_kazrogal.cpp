
#include "precompiled.h"
#include "def_hyjal.h"
#include "hyjal_trash.h"

// Spells
enum Spells
{
    SPELL_CLEAVE    = 31436,
    SPELL_WARSTOMP  = 31480,
    SPELL_MARK      = 31447
};

// Say
enum Say
{
    SAY_ONSLAY1     = -1999926,
    SAY_ONSLAY2     = -1999927,
    SAY_ONSLAY3     = -1999928,

    SAY_MARK1       = -1999929,
    SAY_MARK2       = -1999930,

    SAY_ONAGGRO     = -1999931
};

struct MANGOS_DLL_DECL boss_kazrogalAI : public hyjal_trashAI
{
    boss_kazrogalAI(Creature* pCreature) : hyjal_trashAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        go = false;    
        pos = 0;
        Reset();
        SpellEntry *TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_MARK);
        if(TempSpell && TempSpell->EffectImplicitTargetA[0] != 1)
        {
            TempSpell->EffectImplicitTargetA[0] = 1;
            TempSpell->EffectImplicitTargetB[0] = 0;
        }
    }

    uint32 CleaveTimer;
    uint32 WarStompTimer;
    uint32 MarkTimer;
    uint32 MarkTimerBase;
    bool go;
    uint32 pos;

    void Reset()
    {
        CleaveTimer = 5000;
        WarStompTimer = 15000;
        MarkTimer = 45000;
        MarkTimerBase = 45000;

        if(pInstance && IsEvent)
            pInstance->SetData(TYPE_KAZROGAL, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {    
        if(pInstance && IsEvent)
            pInstance->SetData(TYPE_KAZROGAL, IN_PROGRESS);
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
            Unit* target = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_THRALL));
            if (target && target->isAlive())
                m_creature->AddThreat(target,0.0);
        }        
    }

    void JustDied(Unit *victim)
    {
        if(pInstance && IsEvent)
            pInstance->SetData(TYPE_KAZROGAL, DONE);
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
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(0, 5492.91,    -2404.61,    1462.63);
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(1, 5531.76,    -2460.87,    1469.55);
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(2, 5554.58,    -2514.66,    1476.12);
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(3, 5554.16,    -2567.23,    1479.90);
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(4, 5540.67,    -2625.99,    1480.89);
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(5, 5508.16,    -2659.2,    1480.15);
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(6, 5489.62,    -2704.05,    1482.18);
                    ((npc_escortAI*)(m_creature->AI()))->AddWaypoint(7, 5457.04,    -2726.26,    1485.10);
                    ((npc_escortAI*)(m_creature->AI()))->Start(false, true, true);
                    ((npc_escortAI*)(m_creature->AI()))->SetDespawnAtEnd(false);
                }            
            }
        }

        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if(CleaveTimer < diff)
        {
            DoCast(m_creature, SPELL_CLEAVE);
            CleaveTimer = 6000+rand()%15000;    
        }else CleaveTimer -= diff;

        if(WarStompTimer < diff)
        {
            DoCast(m_creature, SPELL_WARSTOMP);
            WarStompTimer = 60000;
        }else WarStompTimer -= diff;

        if(m_creature->HasAura(SPELL_MARK,0))
            m_creature->RemoveAurasDueToSpell(SPELL_MARK);
        if(MarkTimer < diff)
        {
            //cast dummy, useful for bos addons
            m_creature->CastCustomSpell(m_creature, SPELL_MARK, NULL, NULL, NULL, false, NULL, NULL, m_creature->GetGUID());            
            
            std::list<HostilReference *> t_list = m_creature->getThreatManager().getThreatList();
            for(std::list<HostilReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
            {
                Unit *target = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
                if (target && target->GetTypeId() == TYPEID_PLAYER && target->getPowerType() == POWER_MANA)
                {
                    target->CastSpell(target, SPELL_MARK,true);//only cast on mana users
                }
            }            
            MarkTimerBase -= 5000;
            if(MarkTimerBase < 5500)
                MarkTimerBase = 5500;
            MarkTimer = MarkTimerBase;            
            switch(rand()%3)
            {
                case 0:
                    DoScriptText(SAY_MARK1, m_creature);
                    break;
                case 1:
                    DoScriptText(SAY_MARK2, m_creature);
                    break;    
            }
        }else MarkTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kazrogal(Creature* pCreature)
{
    return new boss_kazrogalAI(pCreature);
}

void AddSC_boss_kazrogal()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_kazrogal";
    newscript->GetAI = &GetAI_boss_kazrogal;
    newscript->RegisterSelf();
}
