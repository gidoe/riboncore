UPDATE `mangos_string` SET `content_default` = 'Active connections: %u (max: %u) Queued connections: %u (max: %u)' WHERE `entry` = 12;

DELETE FROM `mangos_string` WHERE `entry` = 64;
INSERT INTO `mangos_string` (`entry`, `content_default`) VALUES (64, 'Online players: %u (max: %u)');