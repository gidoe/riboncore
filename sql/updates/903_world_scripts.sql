ALTER TABLE db_version CHANGE COLUMN required_902_world_scripts required_903_world_scripts bit;

UPDATE `creature_template` SET `ScriptName`='boss_volazj' WHERE `entry`=29311;
