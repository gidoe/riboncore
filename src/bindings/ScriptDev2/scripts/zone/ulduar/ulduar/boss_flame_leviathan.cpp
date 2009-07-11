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

#include "precompiled.h"
#include "def_ulduar.h"
#include "Vehicle.h"

#define SPELL_PURSUED           62374
#define SPELL_GATHERING_SPEED   62375
#define SPELL_BATTERING_RAM     62376
#define SPELL_FLAME_VENTS       62396
#define SPELL_MISSILE_BARRAGE   62400
#define SPELL_SYSTEMS_SHUTDOWN  62475

#define SPELL_CANNON            62397

#define SPELL_OVERLOAD_CIRCUIT  62399

#define SPELL_SEARING_FLAME     62402

struct MANGOS_DLL_DECL boss_flame_leviathanAI : public ScriptedAI
{
    boss_flame_leviathanAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
        assert(c->isVehicle());
        Reset();
	}

    void Reset(){}

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if(m_creature->hasUnitState(UNIT_STAT_STUNNED))
            return;

        DoMeleeAttackIfReady();
    }
};


struct MANGOS_DLL_DECL boss_flame_leviathan_seatAI : public ScriptedAI
{
    boss_flame_leviathan_seatAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        assert(c->isVehicle());
    }

    void Reset()
    {
        if(const CreatureInfo *cInfo = m_creature->GetCreatureInfo())
            m_creature->SetDisplayId(cInfo->DisplayID_H[1]); // A for gm, H1 invisible
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        if(m_creature->hasUnitState(UNIT_STAT_STUNNED))
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_flame_leviathan(Creature* pCreature)
{
    return new boss_flame_leviathanAI(pCreature);
}

CreatureAI* GetAI_boss_flame_leviathan_seat(Creature* pCreature)
{
    return new boss_flame_leviathan_seatAI(pCreature);
}

void AddSC_boss_flame_leviathan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_flame_leviathan";
    newscript->GetAI = &GetAI_boss_flame_leviathan;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="boss_flame_leviathan_seat";
    newscript->GetAI = &GetAI_boss_flame_leviathan_seat;
    newscript->RegisterSelf();
}
