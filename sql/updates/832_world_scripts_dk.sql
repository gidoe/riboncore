ALTER TABLE db_version CHANGE COLUMN required_831_world_ribon_string required_832_world_scripts_dk bit;
UPDATE `creature_template` SET `ScriptName`='npc_scarlet_ghoul' WHERE `entry`=28845;
