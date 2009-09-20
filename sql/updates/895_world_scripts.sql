ALTER TABLE db_version CHANGE COLUMN required_894_world_scripts required_895_world_scripts bit;

UPDATE `gameobject_template` SET `ScriptName`='go_fel_crystalforge' WHERE `entry`=185919;
UPDATE `gameobject_template` SET `ScriptName`='go_bashir_crystalforge' WHERE `entry`=185921;


UPDATE `gameobject_template` SET `ScriptName`='go_southfury_moonstone' WHERE `entry`=185566;
