ALTER TABLE db_version CHANGE COLUMN required_840_world_scripts required_843_world_training_dummy bit;
UPDATE `creature_template` SET `unit_flags` = 393220 WHERE `entry` IN (31144, 24792, 32542, 32546, 32666, 31146, 32545, 32541, 32667, 30527, 32543);
UPDATE `creature_template` SET `mechanic_immune_mask` = 8204 WHERE `entry` IN (31144, 24792, 32542, 32546, 32666, 31146, 32545, 32541, 32667, 30527, 32543);
UPDATE `creature_template` SET `ScriptName` = 'npc_training_dummy' WHERE `entry` IN (31144, 24792, 32542, 32546, 32666, 31146, 32545, 32541, 32667, 30527, 32543);