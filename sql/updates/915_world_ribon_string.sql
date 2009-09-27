ALTER TABLE db_version CHANGE COLUMN required_905_world required_915_world_ribon_string bit;

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

(12000,
 'BattleGround: Player (GUID: %u) not found!', 
 NULL, NULL, 
 'BattleGround: Die Player (GUID: %u) wurde nicht gefunden!', 
 NULL, NULL, NULL, NULL, NULL),

(50010, 
 'Initialize data stores...', 
 NULL, NULL, 
 'Initialisiere Datenspeicher...', 
 NULL, NULL, NULL, NULL, NULL),

(50011, 
 'Loading Script Names...', 
 NULL, NULL, 
 'Lade Script-Namen...', 
 NULL, NULL, NULL, NULL, NULL),
 
(50012, 
 'Loading InstanceTemplate...', 
 NULL, NULL, 
 'Lade Instanzvorlagen...', 
 NULL, NULL, NULL, NULL, NULL),

(50013, 
 'Loading SkillLineAbilityMultiMap Data...', 
 NULL, NULL, 
 'Lade SkillLineAbilityMultiMap Daten...', 
 NULL, NULL, NULL, NULL, NULL),

(50014, 
 'Cleaning up instances...', 
 NULL, NULL, 
 'Bereinige Instanzen...', 
 NULL, NULL, NULL, NULL, NULL),

(50015, 
 'Packing instances...', 
 NULL, NULL, 
 'Verpacke Instanzen...', 
 NULL, NULL, NULL, NULL, NULL),

(50016, 
 'Loading Localization strings...', 
 NULL, NULL, 
 'Lade Lokalisierungen...', 
 NULL, NULL, NULL, NULL, NULL);

