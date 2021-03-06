ALTER TABLE db_version CHANGE COLUMN required_996_world_scripts required_1000_world_ribon_string bit;

-- default = en_US
-- loc3 = de_DE

INSERT INTO `ribon_string` 
(
 `entry`, 
 `content_default`, 
 `content_loc1`, 
 `content_loc2`, 
 `content_loc3`, 
 `content_loc4`, 
 `content_loc5`, 
 `content_loc6`, 
 `content_loc7`, 
 `content_loc8` 
 ) 
VALUES 

(50017, 
 '>> Loaded 0 creature locale strings. DB table `locales_creature` is empty.', 
 NULL, NULL, 
 '>> 0 Kreatur-Lokalisierungen geladen. Die Tabelle `locales_creature` ist leer.', 
 NULL, NULL, NULL, NULL, NULL),

(50018, 
 '>> Loaded %lu creature locale strings', 
 NULL, NULL, 
 '>> %lu Kreatur-Lokalisierungen geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50019, 
 '>> Loaded 0 gameobject locale strings. DB table `locales_gameobject` is empty.', 
 NULL, NULL, 
 '>> 0 Spielobjekt-Lokalisierungen geladen. Die Tabelle `locales_gameobject` ist leer.', 
 NULL, NULL, NULL, NULL, NULL),

(50020, 
 '>> Loaded %lu gameobject locale strings', 
 NULL, NULL, 
 '>> %lu Spielobjekt-Lokalisierungen geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50021, 
 '>> Loaded 0 Item locale strings. DB table `locales_item` is empty.', 
 NULL, NULL, 
 '>> 0 Gegenstand-Lokalisierungen geladen. Die Tabelle `locales_item` ist leer.', 
 NULL, NULL, NULL, NULL, NULL),

(50022, 
 '>> Loaded %lu Item locale strings', 
 NULL, NULL, 
 '>> %lu Gegenstand-Lokalisierungen geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50023, 
 '>> Loaded 0 Quest locale strings. DB table `locales_quest` is empty.', 
 NULL, NULL, 
 '>> 0 Quest-Lokalisierungen geladen. Die Tabelle `locales_quest` ist leer.', 
 NULL, NULL, NULL, NULL, NULL),

(50024, 
 '>> Loaded %lu Quest locale strings', 
 NULL, NULL, 
 '>> %lu Quest-Lokalisierungen geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50025, 
 '>> Loaded 0 NpcText locale strings. DB table `locales_npc_text` is empty.', 
 NULL, NULL, 
 '>> 0 NPC-Text-Lokalisierungen geladen. Die Tabelle `locales_npc_text` ist leer.', 
 NULL, NULL, NULL, NULL, NULL),

(50026, 
 '>> Loaded %lu NpcText locale strings', 
 NULL, NULL, 
 '>> %lu NPC-Text-Lokalisierungen geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50027, 
 '>> Loaded 0 PageText locale strings. DB table `locales_page_text` is empty.', 
 NULL, NULL, 
 '>> 0 Seitentext-Lokalisierungen geladen. Die Tabelle `locales_page_text` ist leer.', 
 NULL, NULL, NULL, NULL, NULL),

(50028, 
 '>> Loaded %lu PageText locale strings', 
 NULL, NULL, 
 '>> %lu Seitentext-Lokalisierungen geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50029, 
 '>> Loaded 0 npc_option locale strings. DB table `locales_npc_option` is empty.', 
 NULL, NULL, 
 '>> 0 NPC-Optionslokalisierungen geladen. Die Tabelle `locales_npc_option` ist leer.', 
 NULL, NULL, NULL, NULL, NULL),

(50030, 
 '>> Loaded %lu npc_option locale strings', 
 NULL, NULL, 
 '>> %lu NPC-Optionslokalisierungen geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50031, 
 '>> Loaded 0 points_of_interest locale strings. DB table `locales_points_of_interest` is empty.', 
 NULL, NULL, 
 '>> 0 POI-Lokalisierungen geladen. Die Tabelle `locales_points_of_interest` ist leer.', 
 NULL, NULL, NULL, NULL, NULL),

(50032, 
 '>> Loaded %lu points_of_interest locale strings', 
 NULL, NULL, 
 '>> %lu POI-Lokalisierungen geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50033, 
 '>>> Localization strings loaded', 
 NULL, NULL, 
 '>>> Lokalisierungen geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50034, 
 'Loading Page Texts...', 
 NULL, NULL, 
 'Lade Seitentexte...', 
 NULL, NULL, NULL, NULL, NULL),

(50035, 
 '>> Loaded %u page texts', 
 NULL, NULL, 
 '>> %u Seitentexte geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50036, 
 'Loading Player info in cache...', 
 NULL, NULL, 
 'Lade Spielerinfos...', 
 NULL, NULL, NULL, NULL, NULL),

(50037, 
 'Loading Player Cache failed.', 
 NULL, NULL, 
 'Laden der Spielerinfos gescheitert!', 
 NULL, NULL, NULL, NULL, NULL),

(50038, 
 '>> Loaded info about %d players', 
 NULL, NULL, 
 '>> Spielerinfos ueber %d Spieler geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50039, 
 'Loading Game Object Templates...', 
 NULL, NULL, 
 'Lade Spielobjektvorlagen...', 
 NULL, NULL, NULL, NULL, NULL),

(50040, 
 '>> Loaded %u game object templates', 
 NULL, NULL, 
 '>> %u Spielobjektvorlagen geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50041, 
 'Loading Spell Chain Data...', 
 NULL, NULL, 
 'Lade Zauberbindungen...', 
 NULL, NULL, NULL, NULL, NULL),

(50042, 
 '>> Loaded %u spell chains', 
 NULL, NULL, 
 '>> %u Zauberbindungen geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50043, 
 'Loading Spell Required Data...', 
 NULL, NULL, 
 'Lade Erforderliche Zauberdaten...', 
 NULL, NULL, NULL, NULL, NULL),

(50044, 
 '>> Loaded 0 spell required records', 
 NULL, NULL, 
 '>> 0 Erforderliche Zauberdaten geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50045, 
 '>> Loaded %u spell required records', 
 NULL, NULL, 
 '>> %u Erforderliche Zauberdaten geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50046, 
 'Loading Spell Elixir types...', 
 NULL, NULL, 
 'Lade Elixierzauberarten...', 
 NULL, NULL, NULL, NULL, NULL),

(50047, 
 '>> Loaded 0 spell elixir definitions', 
 NULL, NULL, 
 '>> 0 Elixierzauberarten gelade.', 
 NULL, NULL, NULL, NULL, NULL),

(50048, 
 '>> Loaded %u spell elixir definitions', 
 NULL, NULL, 
 '>> %u Elixierzauberarten gelade.', 
 NULL, NULL, NULL, NULL, NULL),

(50049, 
 'Loading Spell Learn Skills...', 
 NULL, NULL, 
 'Lade Lernbare Zauberstaerken...', 
 NULL, NULL, NULL, NULL, NULL),

(50050, 
 '>> Loaded %u Spell Learn Skills from DBC', 
 NULL, NULL, 
 '>> %u Lernbare Zauberstaerken geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50051, 
 'Loading Spell Learn Spells...', 
 NULL, NULL, 
 'Lade Lernbare Zaubererweiterungen...', 
 NULL, NULL, NULL, NULL, NULL),

(50052, 
 '>> Loaded 0 spell learn spells', 
 NULL, NULL, 
 '>> 0 Lernbare Zaubererweiterungen geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50053, 
 '>> Loaded %u spell learn spells + %u found in DBC', 
 NULL, NULL, 
 '>> %u Lernbare Zaubererweiterungen + %u aus DBC-Dateien geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50054, 
 'Loading Spell Proc Event conditions...', 
 NULL, NULL, 
 'Lade Zauberereignisse...', 
 NULL, NULL, NULL, NULL, NULL),

(50055, 
 '>> Loaded 0 spell proc event conditions', 
 NULL, NULL, 
 '>> 0 Zauberereignisse geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50056, 
 '>> Loaded %u extra spell proc event conditions + %u custom', 
 NULL, NULL, 
 '>> %u Zauberereignisse + %u Benutzerdefinierte geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50057, 
 '>> Loaded %u extra spell proc event conditions', 
 NULL, NULL, 
 '>> %u Extra-Zauberereignisse geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50058, 
 'Loading Spell Bonus Data...', 
 NULL, NULL, 
 'Lade Zauberboniedaten...', 
 NULL, NULL, NULL, NULL, NULL),

(50059, 
 '>> Loaded 0 spell bonus data', 
 NULL, NULL, 
 '>> 0 Zauberbonie''s geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50060, 
 '>> Loaded %u extra spell bonus data', 
 NULL, NULL, 
 '>> %u Extra-Zauberdaten geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50061, 
 'Loading Aggro Spells Definitions...', 
 NULL, NULL, 
 'Lade Aggro-Definitionen...', 
 NULL, NULL, NULL, NULL, NULL),

(50062, 
 'Loading NPC Texts...', 
 NULL, NULL, 
 'Lade NPC-Texte...', 
 NULL, NULL, NULL, NULL, NULL),

(50063, 
 'Loading Enchant Spells Proc datas...', 
 NULL, NULL, 
 'Lade Entzauberungsdaten...', 
 NULL, NULL, NULL, NULL, NULL),
 
(50064, 
 '>> Loaded %u spell enchant proc event conditions', 
 NULL, NULL, 
 '%u Entzauberungsereignisse geladen.', 
 NULL, NULL, NULL, NULL, NULL),
 
(50065, 
 '>> Loaded %u enchant proc data definitions', 
 NULL, NULL, 
 '%u Entzauberungsdaten geladen.', 
 NULL, NULL, NULL, NULL, NULL),
 
(50066, 
 'Loading Item Random Enchantments Table...', 
 NULL, NULL, 
 'Lade Zufallentzauberungsdaten...', 
 NULL, NULL, NULL, NULL, NULL),

(50067, 
 '>> Loaded %u Item Enchantment definitions', 
 NULL, NULL, 
 '%u Gegenstandsentzauberungen geladen.', 
 NULL, NULL, NULL, NULL, NULL),

(50068, 
 '>> Loaded 0 Item Enchantment definitions. DB table `item_enchantment_template` is empty.', 
 NULL, NULL, 
 '0 Gegenstandsentzauberungen geladen. Die Tabelle `item_enchantment_template` ist leer.', 
 NULL, NULL, NULL, NULL, NULL),

(50069, 
 'Loading Items...', 
 NULL, NULL, 
 'Lade Gegenstände...', 
 NULL, NULL, NULL, NULL, NULL),

(50070, 
 '>> Loading temporary ItemStorage', 
 NULL, NULL, 
 '>> Lade temporären Gegenstandsspeicher', 
 NULL, NULL, NULL, NULL, NULL),

(50071, 
 '>> Temporary ItemStorage loaded, modifying...', 
 NULL, NULL, 
 '>> Temporären Gegenstandsspeicher geladen, passe an...', 
 NULL, NULL, NULL, NULL, NULL),

(50072, 
 '>> Item Templates modified. Temp storage freed.', 
 NULL, NULL, 
 '>> Gegenstandsvorlagen angepasst. Temporären Speicher geleert.', 
 NULL, NULL, NULL, NULL, NULL),

(50073, 
 '   Items will be removed from players at next relogin if required.', 
 NULL, NULL, 
 '   Gegenstände werden beim nächsten Login vom Spieler entfernt, sofern benötigt.', 
 NULL, NULL, NULL, NULL, NULL),

(50074, 
 '>> Now checking data.', 
 NULL, NULL, 
 '>> Überprüfe Daten.', 
 NULL, NULL, NULL, NULL, NULL),

(50075, 
 '>> Loaded %u item prototypes', 
 NULL, NULL, 
 '>> %u Gegenstandsprototypen geladen.', 
 NULL, NULL, NULL, NULL, NULL);

