-- Fixed spell 67356
DELETE FROM `spell_proc_event` WHERE `entry` = 67356;
INSERT INTO `spell_proc_event` VALUES (67356,0x00,7,0x00000010,0x00000000,0x00000000,0x00000000,0x00000000,0.000000,0.000000,0);

-- Fixed spell 53551 and ranks
DELETE FROM `spell_chain` WHERE `first_spell` = 53551;
INSERT INTO `spell_chain` VALUES (53551,0,53551,1,0), (53552,53551,53551,2,0), (53553,53552,53551,3,0);

DELETE FROM `spell_proc_event` WHERE `entry` IN (53551,53552,53553);
INSERT INTO `spell_proc_event` VALUES (53551,0x00000000,10,0x00001000,0x00000000,0x00000000,0x00000000,0x00000000,0.0,0.0,0);


-- Fixed spell 56342 and ranks
DELETE FROM `spell_chain` WHERE spell_id IN (56342,56343,56344);
INSERT INTO `spell_chain` VALUES (56342,0,56342,1,0), (56343,56342,56342,2,0), (56344,56343,56342,3,0);
DELETE FROM `spell_proc_event` WHERE entry IN (56342,56343,56344);
INSERT INTO `spell_proc_event` VALUES (56342,0,9,0x0C,0x8000000,0x60000,0,0,0,0,22);


-- Fixes passed roll that was displaying as need roll and need roll that had wrong message on need select
-- Fixed spell 56249.