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

/* ScriptDataStart
SDName: Boss Chrono-Lord Epoch
SD%Complete: 
SDComment:
SDCategory:
ScriptDataEnd */

#include "precompiled.h"

//Spells
enum Spells
{
	SPELL_CURSE_OF_EXERTION			= 52772,
	SPELL_TIME_WARP					= 52776, //Time slows down, reducing attack, casting and movement speed by 70% for 6 sec.
	SPELL_TIME_STOP					= 58848, //Stops time in a 50 yard sphere for 2 sec.
	SPELL_WOUNDING_STRIKE_N			= 52771, //Used only on the tank
	SPELL_WOUNDING_STRIKE_H			= 58830,
};

//Say
enum
{
	SAY_INTRO				= -1595000, //"Prince Arthas Menethil, on this day, a powerful darkness has taken hold of your soul. The death you are destined to visit upon others will this day be your own."
	SAY_AGGRO				= -1595001, //"We'll see about that, young prince."
	SAY_TIME_WARP_1			= -1595002, //"Tick tock, tick tock..."
	SAY_TIME_WARP_2			= -1595003, //"Not quick enough!"
	SAY_TIME_WARP_3			= -1595004, //"Let's get this over with. "
	SAY_SLAY_1				= -1595005, //"There is no future for you."
	SAY_SLAY_2				= -1595006, //"This is the hour of our greatest triumph!"
	SAY_SLAY_3				= -1595007, //"You were destined to fail. "
	SAY_DEATH				= -1595008, //"*gurgles*"
};

struct MANGOS_DLL_DECL boss_chrono_lord_epochAI : public ScriptedAI
{
    boss_chrono_lord_epochAI(Creature *c) : ScriptedAI(c) {}

    void Reset() {}

    void EnterCombat(Unit* who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}
    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit *victim)
    {
        if (victim == m_creature)
            return;

        switch(rand()%3)
        {
            case 0:DoScriptText(SAY_SLAY_1, m_creature);break;
            case 1:DoScriptText(SAY_SLAY_2, m_creature);break;
            case 2:DoScriptText(SAY_SLAY_3, m_creature);break;
        }
    }
};

CreatureAI* GetAI_boss_chrono_lord_epoch(Creature *_Creature)
{
    return new boss_chrono_lord_epochAI (_Creature);
}

void AddSC_boss_chrono_lord_epoch()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_chrono_lord_epoch";
    newscript->GetAI = &GetAI_boss_chrono_lord_epoch;
    newscript->RegisterSelf();
}
