ALTER TABLE db_version CHANGE COLUMN required_978_world_scripts required_990_world_template bit;

alter table `creature_template`
    add column `questItem5` int(11) UNSIGNED DEFAULT '0' NOT NULL after `questItem4`,
    add column `questItem6` int(11) UNSIGNED DEFAULT '0' NOT NULL after `questItem5`;
    
alter table `gameobject_template`
    add column `questItem5` int(11) UNSIGNED DEFAULT '0' NOT NULL after `questItem4`,
    add column `questItem6` int(11) UNSIGNED DEFAULT '0' NOT NULL after `questItem5`;
    
alter table `item_template`
    add column `Faction` int(11) UNSIGNED DEFAULT '0' NOT NULL after `Flags`;
    
insert into `battleground_template`(`id`,`MinPlayersPerTeam`,`MaxPlayersPerTeam`,`MinLvl`,`MaxLvl`,`AllianceStartLoc`,`AllianceStartO`,`HordeStartLoc`,`HordeStartO`) values (30,20,40,71,80,1485,0,1486,0);
insert into `battleground_template`(`id`,`MinPlayersPerTeam`,`MaxPlayersPerTeam`,`MinLvl`,`MaxLvl`,`AllianceStartLoc`,`AllianceStartO`,`HordeStartLoc`,`HordeStartO`) values (32,0,40,0,80,0,0,0,0);

DELETE FROM `spell_elixir` WHERE `entry`=67019;

INSERT INTO `spell_elixir` (`entry`, `mask`) VALUES
(67019,0x3);

ALTER TABLE item_template
  CHANGE COLUMN ItemLevel ItemLevel smallint(5) unsigned NOT NULL DEFAULT 0;

