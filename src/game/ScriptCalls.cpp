/*
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * Copyright (C) 2008-2009 Ribon <http://www.dark-resurrection.de/wowsp/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef WIN32
#include <dlfcn.h>
#endif

#include "Common.h"
#include "Platform/Define.h"
#include "ScriptCalls.h"
#include "World.h"
#include "Config/ConfigEnv.h"

ScriptsSet Script=NULL;

void UnloadScriptingModule()
{
    if(Script)
    {
        //todo: some check if some func from script library is called right now
        Script->ScriptsFree();
        RIBON_CLOSE_LIBRARY(Script->hScriptsLib);
        delete Script;
        Script = NULL;
    }
}

bool LoadScriptingModule(char const* libName)
{
    ScriptsSet testScript=new _ScriptSet;

    std::string name = strlen(libName) ? libName : RIBON_SCRIPT_NAME;
    name += RIBON_SCRIPT_EXT;

    testScript->hScriptsLib=RIBON_LOAD_LIBRARY(name.c_str());

    if(!testScript->hScriptsLib )
    {
        printf("Error loading Scripts Library %s !\n",name.c_str());
        delete testScript;
        return false;
    }

    if(   !(testScript->ScriptsInit         =(scriptCallScriptsInit         )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"ScriptsInit"         ))
        ||!(testScript->ScriptsFree         =(scriptCallScriptsFree         )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"ScriptsFree"         ))
        ||!(testScript->OnLogin             =(scriptCallOnLogin             )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"OnLogin"             ))
        ||!(testScript->OnLogout            =(scriptCallOnLogout            )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"OnLogout"            ))
        ||!(testScript->OnPVPKill           =(scriptCallOnPVPKill           )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"OnPVPKill"           ))
        ||!(testScript->ScriptsVersion      =(scriptCallScriptsVersion      )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"ScriptsVersion"      ))
        ||!(testScript->GossipHello         =(scriptCallGossipHello         )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"GossipHello"         ))
        ||!(testScript->GOChooseReward      =(scriptCallGOChooseReward      )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"GOChooseReward"      ))
        ||!(testScript->QuestAccept         =(scriptCallQuestAccept         )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"QuestAccept"         ))
        ||!(testScript->GossipSelect        =(scriptCallGossipSelect        )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"GossipSelect"        ))
        ||!(testScript->GossipSelectWithCode=(scriptCallGossipSelectWithCode)RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"GossipSelectWithCode"))
        ||!(testScript->GOSelect            =(scriptCallGOSelect            )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"GOSelect"            ))
        ||!(testScript->GOSelectWithCode    =(scriptCallGOSelectWithCode    )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"GOSelectWithCode"    ))
        ||!(testScript->QuestSelect         =(scriptCallQuestSelect         )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"QuestSelect"         ))
        ||!(testScript->QuestComplete       =(scriptCallQuestComplete       )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"QuestComplete"       ))
        ||!(testScript->NPCDialogStatus     =(scriptCallNPCDialogStatus     )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"NPCDialogStatus"     ))
        ||!(testScript->GODialogStatus      =(scriptCallGODialogStatus      )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"GODialogStatus"     ))
        ||!(testScript->ChooseReward        =(scriptCallChooseReward        )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"ChooseReward"        ))
        ||!(testScript->ItemHello           =(scriptCallItemHello           )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"ItemHello"           ))
        ||!(testScript->GOHello             =(scriptCallGOHello             )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"GOHello"             ))
        ||!(testScript->scriptAreaTrigger   =(scriptCallAreaTrigger         )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"AreaTrigger"         ))
        ||!(testScript->ItemQuestAccept     =(scriptCallItemQuestAccept     )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"ItemQuestAccept"     ))
        ||!(testScript->GOQuestAccept       =(scriptCallGOQuestAccept       )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"GOQuestAccept"       ))
        ||!(testScript->ItemUse             =(scriptCallItemUse             )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"ItemUse"             ))
        ||!(testScript->ItemExpire          =(scriptCallItemExpire          )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"ItemExpire"          ))
        ||!(testScript->EffectDummyGameObj  =(scriptCallEffectDummyGameObj  )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"EffectDummyGameObj"  ))
        ||!(testScript->EffectDummyCreature =(scriptCallEffectDummyCreature )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"EffectDummyCreature" ))
        ||!(testScript->EffectDummyItem     =(scriptCallEffectDummyItem     )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"EffectDummyItem"     ))
        ||!(testScript->GetAI               =(scriptCallGetAI               )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"GetAI"               ))
        ||!(testScript->CreateInstanceData  =(scriptCallCreateInstanceData  )RIBON_GET_PROC_ADDR(testScript->hScriptsLib,"CreateInstanceData"  ))
        )
    {
        printf("Error loading Scripts Library %s !\n Library missing required functions.",name.c_str());
        RIBON_CLOSE_LIBRARY(testScript->hScriptsLib);
        delete testScript;
        return false;
    }

    sLog.outString();
    sLog.outString( ">>> Scripts Library %s was successfully loaded.\n", name.c_str() );

    //heh we are still there :P we have a valid library
    //we reload script
    UnloadScriptingModule();

    Script=testScript;
    Script->ScriptsInit(sConfig.GetFilename().c_str());

    sWorld.SetScriptsVersion(Script->ScriptsVersion());

    return true;
}

