ALTER TABLE db_version CHANGE COLUMN required_885_world_scripts required_890_world_scripts bit;


UPDATE `creature_template` SET `ScriptName`='npc_training_dummy' WHERE `entry`=16111;
