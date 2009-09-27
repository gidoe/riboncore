
ALTER TABLE db_version CHANGE COLUMN required_847_world_script_text required_854_world_scripts bit;
UPDATE `creature_template` SET `ScriptName`='npc_razael_and_lyana',`npcflag`=`npcflag`|1 WHERE `entry` IN (23778,23998);


UPDATE `gameobject_template` SET `ScriptName`='go_shallow_grave' WHERE `entry` IN (128308,128403);
