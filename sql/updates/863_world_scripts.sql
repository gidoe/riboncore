ALTER TABLE db_version CHANGE COLUMN required_855_world_scripts required_863_world_scripts bit;
UPDATE `creature_template` SET `ScriptName`='npc_gymer' WHERE `entry`=29647;
