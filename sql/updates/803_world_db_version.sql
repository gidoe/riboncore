--
-- Table structure for table `db_version`
--

DROP TABLE IF EXISTS `db_version`;
CREATE TABLE `db_version` (
  `required_803_world_db_version` bit(1) default NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Last applied sql update to DB';

--
-- Dumping data for table `db_version`
--

LOCK TABLES `db_version` WRITE;
INSERT INTO `db_version` VALUES
(NULL);
UNLOCK TABLES;