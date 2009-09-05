ALTER TABLE db_version CHANGE COLUMN required_827_world_mammuth required_828_01_mangos_creature_ai_texts bit;

ALTER TABLE creature_ai_texts CHANGE emote emote smallint(5) unsigned NOT NULL default '0';
