ALTER TABLE db_version CHANGE COLUMN required_936_world_utgarde_pinnacle required_937_world_scripts bit;

UPDATE `creature_template` SET `ScriptName`='mob_waste' WHERE entry='16427';
UPDATE `creature_template` SET `ScriptName`='mob_weaver' WHERE entry='16429';