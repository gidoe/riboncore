ALTER TABLE db_version CHANGE COLUMN required_973_world_scripts required_975_world_command bit;


DELETE FROM `command` WHERE `name` = 'account addon';
INSERT INTO `command` (`name`, `security`, `help`) VALUES ('account addon', 3, 'Syntax: .account addon #addon\nSet expansion addon level allowed. Addon values: 0 - normal, 1 - tbc, 2 - wotlk.');

DELETE FROM `ribon_string` WHERE `entry`=61, 62, 63, 64, 65;
INSERT INTO `ribon_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES
(61, 'Up to %u expansion allowed now.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(62, '|c1f40af20 <Mod>|cffff0000[%s]|c1f40af20:|cffffff00 %s', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(63, '|c1f40af20 <GM>|cffff0000[%s]|c1f40af20:|cffffff00 %s', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(64, '|c1f40af20 <Admin>|cffff0000[%s]|c1f40af20:|cffffff00 %s', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(65, '|cffff0000[System]:|cffffff00 %s|r', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(66, '|cffff0000[Console]:|cffff0f00 %s|r', null, null, null, null, null, null, null, null);
