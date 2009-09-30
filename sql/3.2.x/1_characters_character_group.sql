-- /320/4_groups.sql
alter table `groups`
    add column `raiddifficulty` int(11) UNSIGNED DEFAULT '0' NOT NULL after `difficulty`;

alter table `bugreport`
    change `type` `type` longtext NOT NULL,
    change `content` `content` longtext NOT NULL;
    
