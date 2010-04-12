DELETE FROM script_texts WHERE entry IN (-1658055, -1658056, -1658057, -1658058, -1658059, -1658060, -1658061);
INSERT INTO script_texts (entry, content_default, content_loc1, content_loc2, content_loc3, content_loc4, content_loc5, content_loc6, content_loc7, content_loc8, sound, type, language, emote, comment) VALUES
(-1658055, 'I shall not fail The Lich King! Come and meet your end!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 16760, 1, 0, 0, 'Tyrannus SAY_AGGRO'),
(-1658056, 'Such a shameful display... You are better off dead!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 16761, 1, 0, 0, 'Tyrannus SAY_SLAY_1'),
(-1658057, 'Perhaps you should have stayed in the mountains!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 16762, 1, 0, 0, 'Tyrannus SAY_SLAY_2'),
(-1658058, 'Impossible! Rimefang... Warn...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 16763, 1, 0, 0, 'Tyrannus SAY_DEATH'),
(-1658059, 'Rimefang, destroy this fool!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 16764, 1, 0, 0, 'Tyrannus SAY_MARK_RIMEFANG_1'),
(-1658060, 'The frostwyrm Rimefang gazes at $N and readies an icy attack!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 3, 0, 0, 'Tyrannus SAY_MARK_RIMEFANG_2'),
(-1658061, 'Power... overwhelming!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 16765, 1, 0, 0, 'Tyrannus SAY_DARK_MIGHT_1');

DELETE FROM script_texts WHERE entry IN (-1658006, -1658005, -1658004, -1658003, -1658002, -1658001);
INSERT INTO script_texts (entry, content_default, content_loc1, content_loc2, content_loc3, content_loc4, content_loc5, content_loc6, content_loc7, content_loc8, sound, type, language, emote, comment) VALUES
(-1658006, 'Garfrost tired of puny mortals. Now your bones will freeze!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 16917, 1, 0, 0, 'garfrost SAY_PHASE3'),
(-1658005, 'Axe too weak. Garfrost make better and CRUSH YOU!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 16916, 1, 0, 0, 'garfrost SAY_PHASE2'),
(-1658004, 'Garfrost hope giant underpants clean. Save boss great shame. For later.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 16915, 1, 0, 0, 'garfrost SAY_DEATH'),
(-1658003, 'That one maybe not so good to eat now. Stupid Garfrost! BAD! BAD!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 16914, 1, 0, 0, 'garfrost SAY_SLAY_2'),
(-1658002, 'Will save for snack. For later.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 16913, 1, 0, 0, 'garfrost SAY_SLAY_1'),
(-1658001, 'Tiny creatures under feet, you bring Garfrost something good to eat!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 16912, 1, 0, 0, 'garfrost SAY_AGGRO');