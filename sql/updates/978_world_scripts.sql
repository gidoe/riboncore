ALTER TABLE db_version CHANGE COLUMN required_975_world_command required_978_world_scripts bit;

UPDATE `creature_template` SET `ScriptName`='mob_worshipper' WHERE `entry`='16506';
