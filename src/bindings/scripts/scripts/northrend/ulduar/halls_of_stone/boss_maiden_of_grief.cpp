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
SDName: Boss Maiden of Grief
SD%Complete: 80
SDComment:
SDCategory: Ulduar: Halls of Stone
EndScriptData */

#include "precompiled.h"
#include "def_halls_of_stone.h"

enum Spells
{
    SPELL_PARTING_SORROW                                = 59723,
    SPELL_STORM_OF_GRIEF_N                              = 50752,
    SPELL_STORM_OF_GRIEF_H                              = 59772,
    SPELL_SHOCK_OF_SORROW_N                             = 50760,
    SPELL_SHOCK_OF_SORROW_H                             = 59726,
    SPELL_PILLAR_OF_WOE_N                               = 50761,
    SPELL_PILLAR_OF_WOE_H                               = 59727
};

enum Yells
{
    SAY_AGGRO                                        = -1603000,
    SAY_SLAY_1                                       = -1603001,
    SAY_SLAY_2                                       = -1603002,
    SAY_SLAY_3                                       = -1603003,
    SAY_SLAY_4                                       = -1603004,
    SAY_DEATH                                        = -1603005,
    SAY_STUN                                         = -1603006
};

struct SCRIPTS_DLL_DECL boss_maiden_of_griefAI : public ScriptedAI
{
    boss_maiden_of_griefAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = m_creature->GetInstanceData();
        IsHeroic = m_creature->GetMap()->IsHeroic();
    }

    ScriptedInstance* pInstance;
    bool IsHeroic;

    uint32 PartingSorrowTimer;
    uint32 StormOfGriefTimer;
    uint32 ShockOfSorrowTimer;
    uint32 PillarOfWoeTimer;

    void Reset() 
    {
        PartingSorrowTimer = 25000 + rand()%5000;
        StormOfGriefTimer = 10000;
        ShockOfSorrowTimer = 20000+rand()%5000;
        PillarOfWoeTimer = 5000 + rand()%10000;

        if (pInstance)
            pInstance->SetData(DATA_MAIDEN_OF_GRIEF_EVENT, NOT_STARTED);
    }

    void EnterCombat(Unit* who)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (pInstance)
            pInstance->SetData(DATA_MAIDEN_OF_GRIEF_EVENT, IN_PROGRESS);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        if(IsHeroic)
        {
            if (PartingSorrowTimer < diff)
            {
                Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);

                if(target)
                    DoCast(target,SPELL_PARTING_SORROW);

                PartingSorrowTimer = 30000 + rand()%10000;
            }else PartingSorrowTimer -= diff;
        }

        if (StormOfGriefTimer < diff)
        {
            DoCast(m_creature->getVictim(),IsHeroic ? SPELL_STORM_OF_GRIEF_H : SPELL_STORM_OF_GRIEF_N, true);
            StormOfGriefTimer = 15000 + rand()%5000;
        }else StormOfGriefTimer -= diff;

        if (ShockOfSorrowTimer < diff)
        {
            DoResetThreat();
            DoScriptText(SAY_STUN, m_creature);
            DoCast(m_creature,IsHeroic ? SPELL_SHOCK_OF_SORROW_H : SPELL_SHOCK_OF_SORROW_N);
            ShockOfSorrowTimer = 20000 + rand()%10000;
        }else ShockOfSorrowTimer -= diff;

        if (PillarOfWoeTimer < diff)
        {
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 1);

            if(target)
                DoCast(target,IsHeroic ? SPELL_PILLAR_OF_WOE_H : SPELL_PILLAR_OF_WOE_N);
            else
                DoCast(m_creature->getVictim(),IsHeroic ? SPELL_PILLAR_OF_WOE_H : SPELL_PILLAR_OF_WOE_N);

            PillarOfWoeTimer = 5000 + rand()%20000;
        }else PillarOfWoeTimer -= diff;

        DoMeleeAttackIfReady();
    }
    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (pInstance)
            pInstance->SetData(DATA_MAIDEN_OF_GRIEF_EVENT, DONE);
    }
    void KilledUnit(Unit *victim)
    {
        if (victim == m_creature)
            return;

        DoScriptText(RAND(SAY_SLAY_1,SAY_SLAY_2,SAY_SLAY_3,SAY_SLAY_4), m_creature);
    }
};

CreatureAI* GetAI_boss_maiden_of_grief(Creature* pCreature)
{
    return new boss_maiden_of_griefAI (pCreature);
}

void AddSC_boss_maiden_of_grief()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_maiden_of_grief";
    newscript->GetAI = &GetAI_boss_maiden_of_grief;
    newscript->RegisterSelf();
}
