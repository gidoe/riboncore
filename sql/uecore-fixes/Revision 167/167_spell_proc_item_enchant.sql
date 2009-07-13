DROP TABLE IF EXISTS `spell_proc_item_enchant`;
CREATE TABLE `spell_proc_item_enchant` (
  `entry` smallint(6) unsigned NOT NULL,
  `chance` float NOT NULL default '0',
  PRIMARY KEY  (`entry`)
) ENGINE=MYISAM DEFAULT CHARSET=utf8;

--
-- Content not exist
--