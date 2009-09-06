ALTER TABLE db_version CHANGE COLUMN required_804_8444_world_ribon_string required_822_world_scripts_dk bit;

UPDATE creature_template SET spell1=52435,spell2=52576,spell5=52588,VehicleId=79,speed=0 WHERE entry = 28833;

UPDATE `creature_template` SET vehicleid=87 WHERE (`entry`='28817'); -- mine car
