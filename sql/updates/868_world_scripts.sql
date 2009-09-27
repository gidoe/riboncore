ALTER TABLE db_version CHANGE COLUMN required_863_world_scripts required_868_world_scripts bit;
UPDATE `creature_template` SET `ScriptName`='npc_thorim' WHERE `entry`=29445;
