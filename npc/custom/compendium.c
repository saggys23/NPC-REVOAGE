//===== rAthena Script ===========================================|
//= Compendium                                                   =|
//===== Original By: =============================================|
//= crazyarashi                                                  =|
//===== Improved By: =============================================|
//= Einer 				                                         =|
//===== Current Version: =========================================|
//= 1.0 Initial Version                                          =|
//= 1.1 Fix Slight Errors                                        =|
//================================================================|


-	script	Compendium Manager#0	4_ENERGY_WHITE,{
	doevent("compendium_core::OnCompendium");
	end;
}

prontera,143,163,5	duplicate(Compendium Manager#0)	Compendium Manager#1	4_F_DRKAFRA01


//prontera,145,162,6	script	competest#pay1	837,{
-	script	compendiumquest	837,{
OnQuest:
	.@account_id = getcharid(3);
	if(!query_sql("SELECT `id` FROM `compendium` WHERE `account_id` = '"+.@account_id+"'",.@sql_id)){
		dispbottom "System : You don't have a compendium on this account.",0xCC0000;
		end;
	}

	mes "[Reward]";
	mes "Daily Hunting: ^0000FF25 Exp Points^000000";
	mes "Daily Gathering: ^0000FF25 Exp Points^000000";
	mes "Daily Check-In: ^0000FF25 Exp Points^000000";
	mes " ";
	mes "Pilih jenis quest yang ingin kamu lakukan";
//	switch(select("Daily Compendium Hunting","Daily Compendium Gathering","Daily Check-In","Exit")){
	switch(select((#hunting < 1)?"Hunting Quests":"",
				(#collection2 < 1)?"Collection Quests":"",
				(#checkin < 1)?"Check-in":"",
			  "Exit")) {
	case 1:
		next;
		switch(select("Level 1 - 50","Level 51 - 75","Level 75 - 99")){
			case 1:
			callfunc "H_Compend_Board", 1, 50, 1, 1,		//LEVEL 1 - 50	
				1057,	63011,	0,		0,	"prt_fild03", // YOYO
				1014,	80003,	0,		0,	"pay_dun00",	// spore
				1067,	63014,	0,		0,	"iz_dun02", // CORNOTUS
				1060,	63003,	0,		0,	"pay_fild09"; // BIGFOOT
			end;
			case 2:
			callfunc "H_Compend_Board", 51, 75, 1, 1,		//LEVEL 61 - 75	
				1177,	63030,	0,		0,	"orcsdun02", // ZENORC
				1273,	63029,	0,		0,	"gef_fild10", // ORCLADY
				1069,	63016,	0,		0,	"iz_dun03",	// SWORDFISH
				1041,	63021,	0,		0,	"moc_pryd03", // MUMMY
				1016,	63020,	0,		0,	"moc_pryd02"; // ARCHERSKEL
			end;
			case 3:
			callfunc "H_Compend_Board", 76, 99, 1, 1,		//LEVEL 75 - 99	
				1322,	63054,	0,		0,	"tur_dun01", // SPRINGRABBIT
				1163,	63037,	0,		0,	"gl_knt01",	// RAYDRIC
				1197,	63043,	0,		0,	"gl_prison",	// ZOMBIEPRISON
				1196,	63044,	0,		0,	"gl_prison1";	// SKIELPRISON
			end;
		}
		end;
	case 2:
		doevent("Questboard2::OnCollectionStart");
		close;
		
	case 3:
		next;
		mes "[Compendium Manager]";
		mes "Please enter your nickname to Check-in.";
		input .@charname$;
		if (.@charname$!=strcharinfo(0)) {
			mes "[Compendium Manager]";
			mes "Sorry, but I don't think thats ";
			mes "your name.";
			close;
		}
		next;
		mes "[Compendium Manager]";
		mes "Thankyou for checking-in.";
		skilleffect 459,0;
		skilleffect 34,0;
		skilleffect 380,0;
		doevent("compendium_core::OnGetExp");
		#checkin++;
		#Mission_Delay++;
		close;
	case 4:
		next;
		mes "[Compendium Manager]";
		mes "There are currently no Escort Missions registered on the board.";
		close;
	}
	end;

L_Quest:
	//QuestID,ReqLvl,Quest,Client,Loc,Desc
	.@string$ = getarg(2);
	if (BaseLevel < getarg(1,26)) {
		mes "Your level doesn't meet the requirements. You can't take the '"+ .@string$ +"' mission.";
		close;
	}
	mes "["+ .@string$ +" !!]";
	mes "Client : "+getarg(3);
	mes "Client Location : "+getarg(4);
	mes "Requested Mission : "+getarg(5);
	mes " ";
	mes "Will you accept this mission?";
	next;
	if(select("No, I won't accept it.:Yes, I will accept it.") == 1) {
		mes "You have declined the "+ .@string$ +" mission.";
		close;
	}
	if (checkquest(getarg(0)) > -1) mes "You are already doing this mission.";
	else {
		setquest getarg(0);
		#Mission_Delay++;
		mes "You have accepted the "+ .@string$ +" mission.";
	}
	mes "You can get a reward from the client after you finish the mission.";
	next;
	mes "For more details, please press 'Alt + U', to get more information from the Quest Window.";
	close;	
}

prontera,145,162,6	duplicate(compendiumquest)	Compendium Board Quest#1	837

prontera,145,158,6	script	compereset#pay1	101,{
	mes "You have taken "+(#Mission_Delay)+"/3 mission(s)";
	switch(select("Reset","Cancel")){
		case 1:
		set #Mission_delay, 0;
		set #hunting, 0;
		set #collection2, 0;
		set #checkin, 0;
		end;
		
		case 2:
		end;
	}
}

-	script	compendium_core	-1,{
	doevent strnpcinfo(0) + "::OnCompendium";
	end;
	
OnInit:
	//== General Data ==|
	.compendium_id = 30580;
	.compendium_level_id = 30581;
	.max_level = 50; //= Max Compendium Level
	.point_exp = 100; //= EXP Given by Purchaseable Level
	.quest_count = 3; //= Daily Quest Count
	
	//== Quest Data : Add your own data ==|
	//= min~max amount of hunt/gather
	.min_hunt = 50;
	.max_hunt = 100;
	.min_gather = 50;
	.max_gather = 200;
	
	//= exp given of quest type
	.gathering_exp = 25;
	.hunt_exp = 25;
	.mvp_hunt_exp = 100;
	
	//= item, mob, mvp list to randomize from.
	setarray .mvp_list,1038,1039,1046,1059,1086,1087,1112,1115,1147,1150,1157,1159,1190,1251,1252,1272,1312,1373,1389,1418,1492,1511,1583,1623,1630,1658,1685,1688,1719,1734,1751,1768,1779,1785,1832,1871,1874;
	setarray .hunt_list,1001,1002,1004,1005,1007;
	setarray .gather_list,501,502,503,504;
	
	.name$ = "[ ^0000CDCompendium Info^000000 ]";
	bindatcmd "compendium",strnpcinfo(0) + "::OnCompendium";
	
	//== Rewards : "Level | Reward ID, Amount, Reward ID, Amount,... ==|
	setarray .compendium_reward$, //= Level order doesn't matter but 'duplicates' will bug out the script.
	"1|501:10,502:20", //1
	"2|12411:2",	//2
	"3|7060:20",	//3
	"4|12412:2",	//4
	"5|14179:1",	//5
	"6|3001:1",	//6
	"7|17263:1",	//7
	"8|14165:1",	//8
	"9|14003:5",	//9
	"10|31682:1",	//10
	"11|12221:5",	//11
	"12|3012:3",	//12
	"13|607:10",	//13
	"14|7060:50",	//14
	"15|30582:1",	//15
	"16|7622:1",    //16
	"17|14179:1",    //17
	"18|14003:5",    //18
	"19|12214:2",    //19
	"20|12103:1",	//20
	"21|603:3",    //21
	"22|12411:3",    //22
	"23|12412:3",    //23
	"24|12214:3",    //24
	"25|12817:1",    //25
	"26|6291:2",    //26
	"27|6292:2",    //27
	"28|6241:1",    //28
	"29|6240:1",    //29
	"30|35003:1";	//30
	
	.size = getarraysize(.compendium_reward$);
	for(.@i = 0; .@i < .size; .@i++){
		explode(.@temp$,.compendium_reward$[.@i],"|");
		.level[getarraysize(.level)] = atoi(.@temp$[0]);
		.index[getarraysize(.index)] = .@i;
	}
	query_sql("CREATE TABLE IF NOT EXISTS `compendium` (`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,`account_id` INT(11) UNSIGNED NOT NULL DEFAULT '0',`level` INT(11) UNSIGNED NOT NULL DEFAULT '1',`exp` INT(11) UNSIGNED NOT NULL DEFAULT '0',`quest` INT(11) UNSIGNED NOT NULL DEFAULT '"+.quest_count+"',PRIMARY KEY (`id`)) ENGINE=MyISAM DEFAULT CHARSET=utf8");
end;

OnPCLoginEvent:
	if(gettime(DT_DAYOFMONTH) != #reward_date){
		#reward_date = gettime(DT_DAYOFMONTH);
		c_quest_data$ = "";
	}
end;

OnCompendium:
	disable_items;
	.@account_id = getcharid(3);
	if(!query_sql("SELECT `id` FROM `compendium` WHERE `account_id` = '"+.@account_id+"'",.@sql_id)){
		dispbottom "System : You don't have a compendium on this account.",0xCC0000;
		end;
	}
	query_sql("SELECT `level`,`exp`,`quest` FROM `compendium` WHERE `id` = '"+.@sql_id+"'",.@level,.@exp,.@quest);
	.@temp = .@level;
	while(!.@next_reward){
		.@temp++;
		if(inarray(.level,.@temp) != -1){
			.@next_reward = .@temp;
			break;
		}
		if(.@temp == .max_level){
			.@temp = 0;
			break;
		}
	}
	.@quest = 3 - #Mission_Delay;
	.@prefix$ = .@quest > 0 ? "^32CD32":"^DC143C";
	mes .name$;
	mes "Compendium Level - [^0000CD" + .@level + "^000000]";
	if(.@level < .max_level){
		mes "Compendium EXP - [^32CD32" + .@exp + "^000000/100]";	
//		mes "Quest Remaining - [" + .@prefix$ + "" + .@quest + "^000000/" + .quest_count + "]";
		if(.@temp)
			mes "Next Reward - ^0000CDLevel " + .@next_reward + "^000000";
	}
	if(.@level == .max_level){
		next;
		mes .name$;
		mes "^DC143CYour compendium level is at max already. Wait for the next month's compendium.^000000";
		close;
	}
	next;
//	switch(select(((.@next_reward > 0)?"Check Next Reward":""),"Information","Exit")){
//	switch(select(((.@quest > 0)?"Manage Quest":""),((.@next_reward > 0)?"Check Next Reward":""),"Information","Exit")){
	switch(select("Manage Quest",((.@next_reward > 0)?"Check Next Reward":""),"Information","Exit")){
		case 1:
			next;
			doevent "compendiumquest::OnQuest";
			end;
		case 2:
			mes .name$;
			mes "^0000CDLevel " + .@temp + " Rewards^000000 :";
			.@index = inarray(.level,.@temp);
			explode(.@temp$,.compendium_reward$[.@index],"|");
			explode(.@item$,.@temp$[1],",");
			for(.@x = 0; .@x < getarraysize(.@item$); .@x++){
				explode(.@id$,.@item$[.@x],":");
				mes .@id$[1] + " <ITEM>" + getitemname(atoi(.@id$[0])) + "<INFO>" + atoi(.@id$[0]) + "</INFO></ITEM>";
			}
			next;
			goto OnCompendium;
			
		case 3:
			mes .name$;
			mes "Compendium has a maximum of " + .max_level + " levels.";
			mes "You'll get a reward once you reach a certain level.";
			mes "To level up your compendium, you can do your compendium daily quests.";
			mes "Or buy a levels from the shop.";
			end;
			
		case 4:
			end;
	}
end;

OnActivate:
	function compendium_reward;
	.@account_id = getcharid(3);
	.@item_id = .compendium_id;
	if(query_sql("SELECT `id` FROM `compendium` WHERE `account_id` = '"+.@account_id+"'")){
		dispbottom "System : You already have a compendium activated on this account.",0xCC0000;
		end;
	}
	if(.@item_id < 501){
		mes "Unknown error has occured.";
		end;
	}
	if(!countitem(.@item_id)){
		mes "Did you remove your " + getitemname(.@item_id) + " from your inventory?";
		end;
	}
	delitem .@item_id,1;
	query_sql("INSERT INTO `compendium` (`account_id`,`level`,`exp`,`quest`) VALUES ('"+.@account_id+"',1,0,'"+.quest_count+"')");
	set #Mission_Delay, 0;
	compendium_reward(1);
	dispbottom "You have activated the compendium to this account. Your basic compendium rewards has been sent to your inventory.";
end;

OnUseLevel:
	.@account_id = getcharid(3);
	query_sql("SELECT `level` FROM `compendium` WHERE `account_id` = '"+.@account_id+"'",.@level);
	if(.@level == .max_level){
		dispbottom "Your compendium level is at max already. Wait for the next month's compendium.",0xDC143C;
		end;
	}
	function compendium_getexp;
	delitem .compendium_level_id,1;
	compendium_getexp(.point_exp);
end;

OnGetExp:
	.@account_id = getcharid(3);
	query_sql("SELECT `level` FROM `compendium` WHERE `account_id` = '"+.@account_id+"'",.@level);
	if(.@level == .max_level){
		dispbottom "Your compendium level is at max already. Wait for the next month's compendium.",0xDC143C;
		end;
	}
	function compendium_getexp;
	compendium_getexp(.hunt_exp);
end;

OnGetExpGather:
	.@account_id = getcharid(3);
	query_sql("SELECT `level` FROM `compendium` WHERE `account_id` = '"+.@account_id+"'",.@level);
	if(.@level == .max_level){
		dispbottom "Your compendium level is at max already. Wait for the next month's compendium.",0xDC143C;
		end;
	}
	function compendium_getexp;
	compendium_getexp(.gathering_exp);
end;

OnHour00:
	if(gettime(DT_DAYOFMONTH) == 1){
		query_sql("TRUNCATE `compendium`");
		query_sql("DELETE FROM `acc_reg_num` WHERE `key` = '#Mission_delay'");
		query_sql("DELETE FROM `acc_reg_num` WHERE `key` = '#hunting'");
		query_sql("DELETE FROM `acc_reg_num` WHERE `key` = '#collection2'");
		query_sql("DELETE FROM `acc_reg_num` WHERE `key` = '#check-in'");
		query_sql("DELETE FROM `acc_reg_num` WHERE `key` = '#h_run'");
		end;
	}
	set #Mission_Delay, 0;
	set #hunting, 0;
	set #collection2, 0;
	set #check-in, 0;
//	query_sql("UPDATE `compendium` SET `quest` = '"+.quest_count+" WHERE `quest` < '"+.quest_count+"'");
//	query_sql("DELETE FROM `acc_reg_num` WHERE `key` = '#Mission_Delay'");
end;

function	compendium_getexp	{
	.@exp_value = getarg(0);
	.@account_id = getcharid(3);
	query_sql("SELECT `id`,`exp` FROM `compendium` WHERE `account_id` = '"+.@account_id+"'",.@id,.@exp);
	.@total = .@exp + .@exp_value;
	if(.@total >= 100){
		if(.@total > 100)
			.@excess = .@total - 100;
		query_sql("UPDATE `compendium` SET `exp` = '"+.@excess+"' WHERE `id` = '"+.@id+"'");
		function compendium_uplevel;
		compendium_uplevel();
	} else
		query_sql("UPDATE `compendium` SET `exp` = '"+.@total+"' WHERE `id` = '"+.@id+"'");
	dispbottom "You gain "+(.@exp_value)+" Experience Points on your Compendium.";
	return;
}

function	compendium_uplevel	{
	function compendium_reward;
	.@account_id = getcharid(3);
	query_sql("SELECT `id`,`level` FROM `compendium` WHERE `account_id` = '"+.@account_id+"'",.@id,.@level);
	.@level++;
	query_sql("UPDATE `compendium` SET `level` = '"+.@level+"' WHERE `id` = '"+.@id+"'");
	if(inarray(.level,.@level) != -1){
		compendium_reward(.@level);
		dispbottom "Congratulations for reaching level " + .@level + ". Your rewards has been sent to your inventory.";
	}
	return;
}

function	compendium_reward	{
	.@level = getarg(0);
	.@index = inarray(.level,.@level);
	explode(.@temp$,.compendium_reward$[.@index],"|");
	explode(.@item$,.@temp$[1],",");
	for(.@x = 0; .@x < getarraysize(.@item$); .@x++){
		explode(.@id$,.@item$[.@x],":");
		.@item_id[.@x] = atoi(.@id$[0]);
		.@item_amount[.@x] = atoi(.@id$[1]);
	}
//	mail getcharid(0),"Compendium Rewards","Level " + .@level + " Reward","This is your reward for reaching level " + .@level + " on your compendium.",0,.@item_id,.@item_amount;
	getitem .@item_id,.@item_amount,getcharid(3);
	return;
}

}