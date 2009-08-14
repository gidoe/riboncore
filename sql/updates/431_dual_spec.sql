ALTER TABLE characters DROP COLUMN latency;
ALTER TABLE `characters` ADD `latency` tinyint(3) NOT NULL DEFAULT 0 AFTER `arena_pending_points`;