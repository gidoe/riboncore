ALTER TABLE `creature_template`
    CHANGE COLUMN `unk1` `KillCredit1` int(11) unsigned NOT NULL default '0',
    CHANGE COLUMN `unk2` `KillCredit2` int(11) unsigned NOT NULL default '0';