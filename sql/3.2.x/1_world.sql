-- /320/1_creature_template.sql
alter table `creature_template`
    add column `questItem5` int(11) UNSIGNED DEFAULT '0' NOT NULL after `questItem4`,
    add column `questItem6` int(11) UNSIGNED DEFAULT '0' NOT NULL after `questItem5`;
    
-- /320/2_gameobject_template.sql
alter table `gameobject_template`
    add column `questItem5` int(11) UNSIGNED DEFAULT '0' NOT NULL after `questItem4`,
    add column `questItem6` int(11) UNSIGNED DEFAULT '0' NOT NULL after `questItem5`;
    
-- /320/3_item_template.sql
alter table `item_template`
    add column `Faction` int(11) UNSIGNED DEFAULT '0' NOT NULL after `Flags`;
    
-- /320/5_battleground_template.sql
insert into `battleground_template`(`id`,`MinPlayersPerTeam`,`MaxPlayersPerTeam`,`MinLvl`,`MaxLvl`,`AllianceStartLoc`,`AllianceStartO`,`HordeStartLoc`,`HordeStartO`) values (30,20,40,71,80,1485,0,1486,0);
insert into `battleground_template`(`id`,`MinPlayersPerTeam`,`MaxPlayersPerTeam`,`MinLvl`,`MaxLvl`,`AllianceStartLoc`,`AllianceStartO`,`HordeStartLoc`,`HordeStartO`) values (32,0,40,0,80,0,0,0,0);

-- EOF
