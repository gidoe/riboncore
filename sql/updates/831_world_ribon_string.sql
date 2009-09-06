ALTER TABLE db_version CHANGE COLUMN required_828_01_mangos_creature_ai_texts required_831_world_ribon_string bit;
REPLACE INTO `ribon_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`)
VALUES (326, 'Faction %s (%u) can''nt have reputation.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
