DROP TABLE IF EXISTS `spell_enchant_proc_data`;
CREATE TABLE `spell_enchant_proc_data` (
  `entry` INT(10) UNSIGNED NOT NULL,
  `customChance` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `PPMChance` FLOAT UNSIGNED NOT NULL DEFAULT '0', 
  `procEx` FLOAT UNSIGNED NOT NULL DEFAULT '0'
) ENGINE=MYISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Spell enchant proc data';

INSERT INTO spell_enchant_proc_data (`entry`, `PPMChance`) VALUES
-- Frostbrand Weapon
(2, 8.8),
(12, 8.8),
(524, 8.8),
(1667, 8.8),
(1668, 8.8),
(2635, 8.8),
(3782, 8.8),
(3783, 8.8),
(3784, 8.8),
 -- Wound Poison
(703, 21.43),
(704, 21.43),
(705, 21.43),
(706, 21.43),
(2644, 21.43),
(3772, 21.43),
(3773, 21.43),
 -- Instant Poison
(323, 8.53),
(324, 8.53),
(325, 8.53),
(623, 8.53),
(624, 8.53),
(625, 8.53),
(2641, 8.53),
(3768, 8.53),
(3769, 8.53);