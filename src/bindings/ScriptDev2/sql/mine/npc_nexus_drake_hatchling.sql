UPDATE creature_template SET ScriptName = 'npc_nexus_drake_hatchling' WHERE entry =26127;
-- Drake Hunt
UPDATE quest_template SET ReqItemId1 = '35506', ReqItemCount1 = '1', ReqSourceId1 = '0', ReqSourceCount1 = '0' WHERE entry IN (11919,11940);