ALTER TABLE db_version CHANGE COLUMN required_962_world_scripts required_965_world_scripts bit;



UPDATE `creature_template` SET `ScriptName`='mob_slad_ran_viper' WHERE `entry`=29680;

/* Eck the Ferocious */
UPDATE `creature_template` SET `ScriptName`='boss_eck' WHERE `entry`=29932;
