DROP TABLE IF EXISTS characters_battleground;
CREATE TABLE characters_battleground (
  guid int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  bgid int(10) unsigned NOT NULL DEFAULT '0',
  bgteam int(10) unsigned NOT NULL DEFAULT '0',
  bgmap int(10) unsigned NOT NULL DEFAULT '0',
  bgx float NOT NULL DEFAULT '0',
  bgy float NOT NULL DEFAULT '0',
  bgz float NOT NULL DEFAULT '0',
  bgo float NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

INSERT INTO characters_battleground
SELECT guid, bgid, bgteam, bgmap, bgx, bgy, bgz, bgo
FROM characters;

ALTER TABLE `characters` DROP COLUMN `bgid`;
ALTER TABLE `characters` DROP COLUMN `bgteam`;
ALTER TABLE `characters` DROP COLUMN `bgmap`;
ALTER TABLE `characters` DROP COLUMN `bgx`;
ALTER TABLE `characters` DROP COLUMN `bgy`;
ALTER TABLE `characters` DROP COLUMN `bgz`;
ALTER TABLE `characters` DROP COLUMN `bgo`;