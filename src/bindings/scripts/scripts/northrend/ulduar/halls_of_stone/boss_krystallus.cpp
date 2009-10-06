/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss Krystallus
SD%Complete: 80
SDComment:
SDCategory: Ulduar: Halls of Stone
EndScriptData */

#include "precompiled.h"
#include "def_halls_of_stone.h"

enum Spells
{
    SPELL_BOULDER_TOSS                             = 50843,
    H_SPELL_BOULDER_TOSS                           = 59742,
    SPELL_GROUND_SPIKE                             = 59750,
    SPELL_GROUND_SLAM                              = 50827,
    SPELL_SHATTER                                  = 50810,
    H_SPELL_SHATTER                                = 61546,
    SPELL_STOMP                                    = 48131,
    H_SPELL_STOMP                                  = 59744
};

enum Yells
{
    SAY_AGGRO                                   = -1603007,
    SAY_KILL                                    = -1603008,
    SAY_DEATH                                   = -1603009,
    SAY_SHATTER                                 = -1603010
};

struct SCRIPTS_DLL_DECL boss_krystallusAI : public ScriptedAI
{
    boss_krystallusAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = c->GetInstanceData();
    }
    
    uint32 uiBoulderTossTimer;
    uint32 uiGroundSpikeTimer;
    uint32 uiGroundSlamTimer;
    uint32 uiShatterTimer;
    uint32 uiStompTimer;
    
    bool bIsSlam;
    
    ScriptedInstance* pInstance;

    void Reset()
    {
        bIsSlam = false;
        
        uiBoulderTossTimer = 3000 + rand()%6000;
        uiGroundSpikeTimer = 9000 + rand()%5000;
        uiGroundSlamTimer = 15000 + rand()%3000;
        uiStompTimer = 20000 + rand()%9000;
        uiShatterTimer = 0;
        
        if (pInstance)
            pInstance->SetData(DATA_KRYSTALLUS_EVENT, NOT_STARTED);
    }
    void EnterCombat(Unit* who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        
        if (pInstance)
            pInstance->SetData(DATA_KRYSTALLUS_EVENT, IN_PROGRESS);
    }
    
    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;
        
        if (uiBoulderTossTimer < diff)
        {
            Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0);
            while (pTarget && pTarget->GetTypeId() != TYPEID_PLAYER)
                pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (pTarget)
                DoCast(pTarget, HeroicMode ? H_SPELL_BOULDER_TOSS : SPELL_BOULDER_TOSS);
            uiBoulderTossTimer = 9000 + rand()%6000;
        } else uiBoulderTossTimer -= diff;
        
        if (uiGroundSpikeTimer < diff)
        {
            Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0);
            while (pTarget && pTarget->GetTypeId() != TYPEID_PLAYER)
                pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (pTarget)
                DoCast(pTarget, SPELL_GROUND_SPIKE);
            uiBoulderTossTimer = 12000 + rand()%5000;
        } else uiGroundSpikeTimer -= diff;
        
        if (uiStompTimer < diff)
        {
            DoCast(m_creature, HeroicMode ? H_SPELL_STOMP : SPELL_STOMP);
            uiStompTimer = 20000 + rand()%9000;
        } else uiStompTimer -= diff;
        
        if (uiGroundSlamTimer < diff)
        {
            DoCast(m_creature, SPELL_GROUND_SLAM);
            bIsSlam = true;
            uiShatterTimer = 10000;
            uiGroundSlamTimer = 15000 + rand()%3000;
        } else uiGroundSlamTimer -= diff;
        
        if (bIsSlam)
        {
            if(uiShatterTimer < diff)
            {
                DoCast(m_creature, HeroicMode ? H_SPELL_SHATTER : SPELL_SHATTER);
                bIsSlam = false;
            } else uiShatterTimer -= diff;
        }

        DoMeleeAttackIfReady();
    }
    
    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
        
        if (pInstance)
            pInstance->SetData(DATA_KRYSTALLUS_EVENT, DONE);
    }
    
    void KilledUnit(Unit *victim)
    {
        if (victim == m_creature)
            return;
        DoScriptText(SAY_KILL, m_creature);
    }
};

CreatureAI* GetAI_boss_krystallus(Creature* pCreature)
{
    return new boss_krystallusAI (pCreature);
}

void AddSC_boss_krystallus()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_krystallus";
    newscript->GetAI = &GetAI_boss_krystallus;
    newscript->RegisterSelf();
}
