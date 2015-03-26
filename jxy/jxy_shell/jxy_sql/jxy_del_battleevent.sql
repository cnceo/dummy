drop event if exists `DelBattleEvent`;
CREATE DEFINER=`root`@`%` EVENT `DelBattleEvent` ON SCHEDULE EVERY 1 DAY STARTS '2013-05-01 02:30:00' ON COMPLETION NOT PRESERVE ENABLE DO delete from battlelog where DATE_SUB(CURDATE(), INTERVAL 7 DAY) >= date(CreateTime)