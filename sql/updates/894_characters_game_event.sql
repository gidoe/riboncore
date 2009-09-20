ALTER TABLE character_db_version CHANGE COLUMN required_864_characters_character_spell 894_characters_game_event bit;


DELETE `game_event_condition_save` FROM `game_event_condition_save` WHERE `event_id` in (22,75,76,77);
DELETE `game_event_save` FROM `game_event_save` WHERE `event_id` in (22,75,76,77);
