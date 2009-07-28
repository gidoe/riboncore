UPDATE spell_proc_event SET SpellFamilyMask0 = 385, SpellFamilyMask1 = 8519872 WHERE entry IN(30293,30295,30296);  

DELETE FROM `spell_proc_event` WHERE entry IN('18073','18096','63245');
INSERT INTO `spell_proc_event` VALUES('18073','0','5','256','8388608','0','0','2','0','0','0');
INSERT INTO `spell_proc_event` VALUES('18096','0','5','256','8388608','0','0','2','0','0','0');
INSERT INTO `spell_proc_event` VALUES('63245','0','5','256','8388608','0','0','2','0','0','0');  

UPDATE spell_proc_event SET SpellFamilyMask0 = 0, SpellFamilyMask1 = 8388608 WHERE entry IN(18119,18120);  

DELETE FROM `spell_proc_event` WHERE entry IN('63730','63733','63737'); 
INSERT INTO `spell_proc_event` VALUES('63730','0','6','2048','4','0','0','0','0','0','0'); 
INSERT INTO `spell_proc_event` VALUES('63733','0','6','2048','4','0','0','0','0','0','0'); 
INSERT INTO `spell_proc_event` VALUES('63737','0','6','2048','4','0','0','0','0','0','0');  

DELETE FROM spell_proc_event WHERE entry = 53646; 
INSERT INTO spell_proc_event VALUES(53646,0,0,0,0,0,0,2,0,0,0);