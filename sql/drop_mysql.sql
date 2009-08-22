REVOKE ALL PRIVILEGES ON * . * FROM 'ribon'@'localhost';

REVOKE ALL PRIVILEGES ON `world` . * FROM 'ribon'@'localhost';

REVOKE GRANT OPTION ON `world` . * FROM 'ribon'@'localhost';

REVOKE ALL PRIVILEGES ON `characters` . * FROM 'ribon'@'localhost';

REVOKE GRANT OPTION ON `characters` . * FROM 'ribon'@'localhost';

REVOKE ALL PRIVILEGES ON `realmd` . * FROM 'ribon'@'localhost';

REVOKE GRANT OPTION ON `realmd` . * FROM 'ribon'@'localhost';

DELETE FROM `user` WHERE CONVERT( User USING utf8 ) = CONVERT( 'ribon' USING utf8 ) AND CONVERT( Host USING utf8 ) = CONVERT( 'localhost' USING utf8 ) ;

DROP DATABASE IF EXISTS `world` ;

DROP DATABASE IF EXISTS `characters` ;

DROP DATABASE IF EXISTS `logon` ;
