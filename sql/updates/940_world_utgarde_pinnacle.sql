/* if you used 936_world_utgarde_pinnacle.sql to early */
ALTER TABLE db_version CHANGE COLUMN required_937_world_scripts required_940_world_utgarde_pinnacle bit;
UPDATE `instance_template` SET `script`='instance_utgarde_pinnacle' WHERE `map`=575;