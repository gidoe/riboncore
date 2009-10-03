SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for account
-- ----------------------------
DROP TABLE IF EXISTS `account`;
CREATE TABLE `account` (
  `id` int(11) unsigned NOT NULL auto_increment COMMENT 'Identifier',
  `username` varchar(32) NOT NULL default '',
  `sha_pass_hash` varchar(40) NOT NULL default '',
  `gmlevel` tinyint(3) unsigned NOT NULL default '0',
  `sessionkey` longtext,
  `v` longtext,
  `s` longtext,
  `email` text,
  `joindate` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `last_ip` varchar(30) NOT NULL default '127.0.0.1',
  `failed_logins` int(11) unsigned NOT NULL default '0',
  `locked` tinyint(3) unsigned NOT NULL default '0',
  `last_login` timestamp NOT NULL default '0000-00-00 00:00:00',
  `online` tinyint(4) NOT NULL default '0',
  `expansion` tinyint(3) unsigned NOT NULL default '0',
  `mutetime` bigint(40) unsigned NOT NULL default '0',
  `locale` tinyint(3) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `idx_username` (`username`),
  KEY `idx_gmlevel` (`gmlevel`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Account System';

-- ----------------------------
-- Table structure for account_banned
-- ----------------------------
DROP TABLE IF EXISTS `account_banned`;
CREATE TABLE `account_banned` (
  `id` int(11) NOT NULL default '0' COMMENT 'Account id',
  `bandate` bigint(40) NOT NULL default '0',
  `unbandate` bigint(40) NOT NULL default '0',
  `bannedby` varchar(50) NOT NULL,
  `banreason` varchar(255) NOT NULL,
  `active` tinyint(4) NOT NULL default '1',
  PRIMARY KEY  (`id`,`bandate`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Ban List';

-- ----------------------------
-- Table structure for ip_banned
-- ----------------------------
DROP TABLE IF EXISTS `ip_banned`;
CREATE TABLE `ip_banned` (
  `ip` varchar(32) NOT NULL default '127.0.0.1',
  `bandate` bigint(40) NOT NULL,
  `unbandate` bigint(40) NOT NULL,
  `bannedby` varchar(50) NOT NULL default '[Console]',
  `banreason` varchar(255) NOT NULL default 'no reason',
  PRIMARY KEY  (`ip`,`bandate`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Banned IPs';

-- ----------------------------
-- Table structure for logon_db_version
-- ----------------------------
DROP TABLE IF EXISTS `logon_db_version`;
CREATE TABLE `logon_db_version` (
  `required_875_logon_uptime` bit(1) default NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Last applied sql update to DB';

-- ----------------------------
-- Table structure for logs
-- ----------------------------
DROP TABLE IF EXISTS `logs`;
CREATE TABLE `logs` (
  `time` int(14) NOT NULL,
  `realm` int(4) NOT NULL,
  `type` int(4) NOT NULL,
  `string` text
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for realmcharacters
-- ----------------------------
DROP TABLE IF EXISTS `realmcharacters`;
CREATE TABLE `realmcharacters` (
  `realmid` int(11) unsigned NOT NULL default '0',
  `acctid` bigint(20) unsigned NOT NULL,
  `numchars` tinyint(3) unsigned NOT NULL default '0',
  PRIMARY KEY  (`realmid`,`acctid`),
  KEY `acctid` (`acctid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Realm Character Tracker';

-- ----------------------------
-- Table structure for realmlist
-- ----------------------------
DROP TABLE IF EXISTS `realmlist`;
CREATE TABLE `realmlist` (
  `id` int(11) unsigned NOT NULL auto_increment,
  `name` varchar(32) NOT NULL default '',
  `address` varchar(32) NOT NULL default '127.0.0.1',
  `port` int(11) NOT NULL default '8085',
  `icon` tinyint(3) unsigned NOT NULL default '0',
  `color` tinyint(3) unsigned NOT NULL default '2',
  `timezone` tinyint(3) unsigned NOT NULL default '0',
  `allowedSecurityLevel` tinyint(3) unsigned NOT NULL default '0',
  `population` float unsigned NOT NULL default '0',
  `gamebuild` int(11) unsigned NOT NULL default '10505',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `idx_name` (`name`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Realm System';

-- ----------------------------
-- Table structure for uptime
-- ----------------------------
DROP TABLE IF EXISTS `uptime`;
CREATE TABLE `uptime` (
  `realmid` int(11) unsigned NOT NULL,
  `starttime` bigint(20) unsigned NOT NULL default '0',
  `startstring` varchar(64) NOT NULL default '',
  `uptime` bigint(20) unsigned NOT NULL default '0',
  `maxplayers` smallint(5) unsigned NOT NULL default '0',
  `revision` varchar(255) NOT NULL default 'RibonCore',
  PRIMARY KEY  (`realmid`,`starttime`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Uptime system';

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `account` VALUES ('1', 'ADMINISTRATOR', 'a34b29541b87b7e4823683ce6c7bf6ae68beaaac', '3', '', '0', '0', '', '2006-04-25 12:18:56', '127.0.0.1', '0', '0', '0000-00-00 00:00:00', '0', '0', '0', '0');
INSERT INTO `account` VALUES ('2', 'GAMEMASTER', '7841e21831d7c6bc0b57fbe7151eb82bd65ea1f9', '2', '', '0', '0', '', '2006-04-25 12:18:56', '127.0.0.1', '0', '0', '0000-00-00 00:00:00', '0', '0', '0', '0');
INSERT INTO `account` VALUES ('3', 'MODERATOR', 'a7f5fbff0b4eec2d6b6e78e38e8312e64d700008', '1', '', '0', '0', '', '2006-04-25 12:19:35', '127.0.0.1', '0', '0', '0000-00-00 00:00:00', '0', '0', '0', '0');
INSERT INTO `account` VALUES ('4', 'PLAYER', '3ce8a96d17c5ae88a30681024e86279f1a38c041', '0', '', '0', '0', '', '2006-04-25 12:19:35', '127.0.0.1', '0', '0', '0000-00-00 00:00:00', '0', '0', '0', '0');
INSERT INTO `logon_db_version` VALUES (null);
INSERT INTO `realmlist` VALUES ('1', 'Ribon', '127.0.0.1', '8085', '1', '0', '3', '0', '0', '10505');
