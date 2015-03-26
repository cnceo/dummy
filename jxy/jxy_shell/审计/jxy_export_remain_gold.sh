#  @File Name: .sh
#  @Company: heihuo
#  @brief: 囧西游：导出剩余元宝脚本
#  @Created Time: 2014年7月14日

#!/bin/bash

source ./util.sh

declare err_cnt=0;
v=""

today=`date '+%Y_%m_%d'`

g_localhostdbopt="-uroot -p123456"

g_mysqldump_no_create_db_no_create_table="--no-create-db --no-create-info --add-locks --extended-insert --lock-tables --single-transaction --compress --complete-insert --quick --default-character-set=utf8 --hex-blob --skip-comments --no-autocommit"

# 不建库，只建表和导数据
g_mysqldump_create_table_no_create_db="--no-create-db --single-transaction --compress --complete-insert --opt --default-character-set=utf8 --hex-blob --skip-comments --no-autocommit"

# 依次链接数据库列表文件中的每一个数据库实例，检测是否成功
function ck_conn_mysql_by_mysql_list()
{
	local zone_list="$1"

	if [ -n "$zone_list" ] && [ ! -f $zone_list ]; then
		echo_err "error: can not found $zone_list file, exiting!"
		exit 1
	fi

	echo_blue "**********************************start connect to $zone_list**********************************************"

	# 依次检测是否能成功连上各个区的数据库
	while read line
	do
		# echo "$line"

		# 分割字符串
		fields=($(echo $line | tr ' ' ' ' | tr -s ' '))
		field_cnt=${#fields[@]}

		# 依次获取区id、游戏库名、游戏库ip、游戏库port、日志库名、日志库ip、日志库port
		zoneid=${fields[0]};
		gamedb=${fields[1]};
		gamedbip=${fields[2]};
		gamedbport=${fields[3]};
		logdb=${fields[4]};
		logdbip=${fields[5]};
		logdbport=${fields[6]};

		gamedbopt="-h$gamedbip -P$gamedbport -uroot -p$dbpwd"
		logdbopt="-h$logdbip -P$logdbport -uroot -p$dbpwd"

		gamedbdesc=$gamedb"     @ [ "$gamedbip" : "$gamedbport" ]"
		logdbdesc=$logdb" @ [ "$logdbip" : "$logdbport" ]"

		# 检测是否能成功连接得上游戏数据库
		ck_conn_ok_by_opt_and_desc "$gamedbdesc" "$gamedbopt"
		conn_mysql_succ=$?
		if [ "$conn_mysql_succ" -ne "0" ]; then
			((err_cnt++))
			exit 1
		fi
	done < $zone_list
	echo_blue "**********************************connect to $zone_list finished*******************************************"
}

# 根据当前的剩余元宝和回滚日期起至今的元宝消耗和获赠表回滚出当天的剩余元宝
function create_roll_back_player_gold()
{
	# 服务器列表文件
	local zone_list=$1
	local roll_back_date=$2

	echo "**********************************start executing at $zone_list*******************************************"

	local roll_back_player_gold=roll_back_player_gold_${today};

mysql $g_paydbopt $g_pay_db << _EOFMYSQL_
		drop table if exists ${roll_back_player_gold};
		create table if not exists ${roll_back_player_gold}(
			PlayerID int(11) unsigned not null comment '角色ID',
			username varchar(64) default '' comment '账号',
			nickname varchar(20) default '' comment '角色昵称',
			Gold int(11) not null default '0' comment '元宝',
			primary key (PlayerID)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8;
_EOFMYSQL_

	# 依次在每个游戏数据库执行传入的sql脚本
	while read line
	do
		# 分割字符串
		fields=($(echo $line | tr ' ' ' ' | tr -s ' '))
		field_cnt=${#fields[@]}

		# 依次获取区id、游戏库名、游戏库ip、游戏库port
		zoneid=${fields[0]};
		gamedb=${fields[1]};
		gamedbip=${fields[2]};
		gamedbport=${fields[3]};

		gamedbopt="-h$gamedbip -P$gamedbport -uroot -p$dbpwd"
		gamedb_ip_port="[ "$gamedbip" : "$gamedbport" ]"

		gamedbdesc=$gamedb" @ "$gamedb_ip_port

		echo_blue "-----------------start roll back gold at $gamedbdesc---------------"
		
mysql $gamedbopt $gamedb << _EOFMYSQL_
		drop table if exists ${roll_back_player_gold};
		create table if not exists ${roll_back_player_gold}(
			PlayerID int(11) not null comment '角色ID',
			Gold int(11) not null comment '元宝',
			primary key (PlayerID)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8;

		CREATE INDEX playerid_idx ON ${roll_back_player_gold}(PlayerID);

		# PlayerID小于1010说明该玩家是机器人
		insert ignore into ${roll_back_player_gold} select PlayerID, Gold from players where PlayerID > 1010 and Pay > 0;

		update ${roll_back_player_gold} r,(select c.PlayerID, sum(c.AddGold) as sumaddgold from player_consume c,players p where c.PlayerID = p.PlayerID and c.AddGold > 0 and c.CreateTime >= '${roll_back_date}' group by PlayerID) g set r.Gold = r.Gold - g.sumaddgold where r.PlayerID = g.PlayerID;

		update ${roll_back_player_gold} r,(select c.PlayerID, sum(c.DecGold) as sumdecgold from player_consume c,players p where c.PlayerID = p.PlayerID and c.DecGold > 0 and c.CreateTime >= '${roll_back_date}' group by PlayerID) g set r.Gold = r.Gold + g.sumdecgold where r.PlayerID = g.PlayerID;

		update ${roll_back_player_gold} set Gold = 0 where Gold < 0;
_EOFMYSQL_

		mysqldump $gamedbopt $g_mysqldump_no_create_db_no_create_table $gamedb ${roll_back_player_gold} | mysql $g_paydbopt $g_pay_db

mysql $gamedbopt $gamedb << _EOFMYSQL_
		drop table ${roll_back_player_gold};
_EOFMYSQL_

		echo_blue "-----------------end roll back gold at $gamedbdesc finished---------------"
	done < $zone_list

	echo_ok "**********************************executing at $zone_list finished*******************************************"
}

function create_rmb_player()
{
	local begin_pay_date=$1
	local end_pay_date=$2

	local rmb_player=rmb_player_${today}

mysql $g_paydbopt $g_pay_db << _EOFMYSQL_
	drop table if exists $g_pay_db.${rmb_player};
	create table if not exists $g_pay_db.${rmb_player}(
		PlayerID int(11) unsigned not null comment '角色ID',
		primary key (PlayerID)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	# 小于一块钱的订单说明是测试订单
	insert ignore into ${rmb_player}(PlayerID) select distinct(playerid) from ${g_usercenter_db}.payed where createtime >= '${begin_pay_date}' and createtime <= '${end_pay_date}' and rmb > 1;
_EOFMYSQL_
}

function query_rmb_player_roll_back_gold()
{
	local zone_list=$1

	local begin_pay_date=$2
	local end_pay_date=$3

	create_rmb_player ${begin_pay_date} ${end_pay_date}
	create_roll_back_player_gold ${zone_list} ${end_pay_date}

	local roll_back_player_gold=roll_back_player_gold_${today};
	local rmb_player=rmb_player_${today}

mysql $g_paydbopt -vvv $g_pay_db << _EOFMYSQL_
	# 从剩余元宝表中删除非充值玩家
	delete from ${roll_back_player_gold} where PlayerID not in (select PlayerID from ${rmb_player});

	update ${roll_back_player_gold} r, ${g_usercenter_db}.users u, ${g_usercenter_db}.globalplayers g set r.username = u.username, r.nickname = g.nickname where r.PlayerID = g.playerid and g.userid = u.userid;
_EOFMYSQL_
}

function export_remain_gold_table()
{
	local exportfile=$1

	local roll_back_player_gold=roll_back_player_gold_${today};
	local rmb_player=rmb_player_${today}

	rm -f ${exportfile}

mysql $g_localhostdbopt mysql << _EOFMYSQL_
	drop table if exists ${roll_back_player_gold};
_EOFMYSQL_

	mysqldump -vvv $g_paydbopt $g_mysqldump_create_table_no_create_db $g_pay_db ${roll_back_player_gold} | mysql $g_localhostdbopt mysql
	#mysqldump -vvv $g_localhostdbopt -T./ mysql ${roll_back_player_gold} --fields-enclosed-by=\" --fields-terminated-by=,

mysql $g_localhostdbopt -vvv mysql << _EOFMYSQL_
	select PlayerID,username,nickname,Gold from ${roll_back_player_gold} into outfile '/mnt/hgfs/proj/dummy/jxy/${exportfile}' fields terminated by '	' enclosed by '"' lines terminated by '\n';
_EOFMYSQL_

	sed '1 i"playerid"	"username"	"nickname"	"gold"' -i ${exportfile}

	for (( i = 0; i < 2; i++ )); do
		echo "" >> ${exportfile}
	done

	echo "" >> ${exportfile}

	echo "《截止至${today}充值玩家元宝剩余明细表》" >> ${exportfile}

mysql $g_localhostdbopt mysql << _EOFMYSQL_ >> ${exportfile}
	select count(*) as 记录数 from ${roll_back_player_gold};
_EOFMYSQL_
}

function query_ios()
{
	ios_zone_list=zone_list/ios_zone_merge_slave.list
	#ios_zone_list=zone_list/ios_zone_1.list

	g_paydbopt="-h118.26.225.102 -P63301 -uroot -p${dbpwd}"
	g_pay_db=ios_app_pay
	g_usercenter_db=ios_app_usercenter

	ck_conn_mysql_by_mysql_list "$ios_zone_list" &> log

	query_rmb_player_roll_back_gold "${ios_zone_list}" "2014-01-01" "2014-07-01" &> log
	export_remain_gold_table "ios_remain_gold_at_${today}.log" 2> log
}

function query_and()
{
	and_zone_list=zone_list/and_zone_merge_slave.list

	g_paydbopt="-h42.62.60.197 -P63301 -uroot -p${dbpwd}"
	g_pay_db=and_app_pay
	g_usercenter_db=and_app_usercenter

	ck_conn_mysql_by_mysql_list "$and_zone_list" &> log

	query_rmb_player_roll_back_gold "${and_zone_list}" "2014-01-01" "2014-07-01" &> log
	export_remain_gold_table "and_remain_gold_at_${today}.log" 2> log
}

# 开始进行配置了

# 2. 其次，获取数据库密码，通过手动输入的方式以防止密码泄露

	echo_blue "preparing to connect to ios db, please enter the mysql password:"
	dbpwd=
	#read -s dbpwd

	if [ "$dbpwd" = "" ]; then
		echo_err "err: you enter nothing, aborted!"
		exit
	fi

# 3. 再次，根据传入的各区数据库列表依次检测是否能成功连上各台服务器的数据库

	# 传入的服务器ip和port列表将如下面所示：
	# --------------------------------------------------------------------------------------------------------
	# ZoneID	    GameDb	        GameDbIP	GameDbPort	    LogDb	        LogDbIP	        LogDbPort
	#  1	    ios_app_s001	118.26.225.102	63316	    ios_app_s001_log	118.26.225.102	63307
	#  2	    ios_app_s002	118.26.225.102	63316	    ios_app_s002_log	118.26.225.102	63307
	#  3	    ios_app_s003	118.26.225.102	63316	    ios_app_s003_log	118.26.225.102	63307
	#  4	    ios_app_s004	118.26.225.102	63316	    ios_app_s004_log	118.26.225.102	63307
	#  5	    ios_app_s005	118.26.225.102	63316	    ios_app_s005_log	118.26.225.102	63307
	#  6	    ios_app_s006	118.26.225.102	63316	    ios_app_s006_log	118.26.225.102	63307
	#  .....
	# --------------------------------------------------------------------------------------------------------

	# 开始检测

# 3. 开始在不同平台的每个区执行sql文件
	query_ios
	query_and