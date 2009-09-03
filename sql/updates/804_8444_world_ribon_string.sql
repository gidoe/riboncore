ALTER TABLE db_version CHANGE COLUMN required_803_world_db_version required_804_8444_world_ribon_string bit;

DELETE FROM ribon_string WHERE entry IN(348,522);
INSERT INTO ribon_string VALUES
(348,'Game Object (Entry: %u) have invalid data and can\'t be spawned',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(522,'Game Object (Entry: %u) not found',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
