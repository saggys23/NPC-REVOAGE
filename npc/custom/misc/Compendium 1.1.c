//===== rAthena Script ===========================================|
//= Compendium                                                   =|
//===== By: ======================================================|
//= crazyarashi                                                  =|
//===== Current Version: =========================================|
//= 1.0 Initial Version                                          =|
//= 1.1 Fix Slight Errors                                        =|
//================================================================|


//= 30580,Compendium_Pass,Battle Pass,11,120,,70,,,,,0xFFFFFFFF,63,2,,,,,,{ doevent "compendium_core::OnActivate"; },{},{}
//= 30581,Compendium Level,Compendium Level,11,120,,70,,,,,0xFFFFFFFF,63,2,,,,,,{ doevent "compendium_core::OnUseLevel"; },{},{}

-	script	Compendium Manager#0	4_ENERGY_WHITE,{
	doevent("compendium_core::OnCompendium");
	end;
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
	.gathering_exp = 50;
	.hunt_exp = 50;
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
		#c_quest_data$ = "";
	}
//	#c_quest_data$ = "";
end;

OnNPCKillEvent:
	if(!c_hunt) end;
	explode(.@T$,#c_quest_data$,":");
	for(.@i = 0; .@i < .quest_count; .@i++){
		explode(.@TT$,.@T$[.@i],",");
		.@count = atoi(.@TT$[3]);
		if(atoi(.@TT$[0]) == 2 || atoi(.@TT$[0]) == 4 || atoi(.@TT$[1]) != killedrid || atoi(.@TT$[3]) >= atoi(.@TT$[2]))
			continue;
		.@count += 1;
		.@TT$[3] = .@count;
		.@T$[.@i] = implode(.@TT$,",");
		#c_quest_data$ = implode(.@T$,":");	
		dispbottom "Compendium Hunt Status : "+getmonsterinfo(atoi(.@TT$[1]),MOB_NAME)+" ["+.@count+" / "+.@TT$[2]+"]",0xFFA500;
		if(.@count == atoi(.@TT$[2]))
			dispbottom "Compendium Hunt Status : "+getmonsterinfo(atoi(.@TT$[1]),MOB_NAME)+" [Completed]",0xFFA500;
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
	.@prefix$ = .@quest > 0 ? "^32CD32":"^DC143C";
	mes .name$;
	mes "Compendium Level - [^0000CD" + .@level + "^000000]";
	if(.@level < .max_level){
		mes "Compendium EXP - [^32CD32" + .@exp + "^000000/100]";	
		mes "Quest Remaining - [" + .@prefix$ + "" + .@quest + "^000000/" + .quest_count + "]";
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
	switch(select(((.@quest > 0)?"Manage Quest":""),((.@next_reward > 0)?"Check Next Reward":""),"Information","Exit")){
		case 1:
		OnQuest:
			.@menu$ = "";
			.@q = 0;
			explode(.@T$,#c_quest_data$,":");
			for(.@i = 0; .@i < .quest_count; .@i++){
				if(.@T$[.@i] == "")
					.@menu$ += "^777777Quest Unselected^000000" + ":";
				else {
					explode(.@TT$,.@T$[.@i],",");
					switch(atoi(.@TT$[0])){
						case 1: 
							if(atoi(.@TT$[3]) >= atoi(.@TT$[2]))
								.@status$ = "^00FF00Completed^000000";
							else
								.@status$ = "^0000CDOngoing^000000";
							.@quest$ = "Monster Hunt[" + .@status$ + "]"; 
							break;
							
						case 2:
							if(countitem(atoi(.@TT$[1])) >= atoi(.@TT$[2]))
								.@status$ = "^00FF00Completed^000000";
							else
								.@status$ = "^0000CDOngoing^000000";
							.@quest$ = "Item Gathering[" + .@status$ + "]"; 
							break;
							
						case 3: 
							if(atoi(.@TT$[3]) >= atoi(.@TT$[2]))
								.@status$ = "^00FF00Completed^000000";
							else
								.@status$ = "^0000CDOngoing^000000";
							.@quest$ = "MVP Hunt[" + .@status$ + "]";						
							break;
							
						case 4: .@quest$ = "^0000CDCompleted^000000"; break;
					}
					.@menu$ += .@quest$ + ":";
				}
			}
			mes .name$;
			mes "Select a quest to manage.";
			next;
			.@menu$ += "Exit";
			.@s = select(.@menu$) - 1;
			if(.@s == 3)
				end;
			explode(.@TT$,.@T$[.@s],",");
			if(atoi(.@TT$[0]) == 4){
				mes .name$;
				mes "You have already completed the quest for this slot.";
				next;
				goto OnQuest;
			}
			if(.@T$[.@s] == ""){
				mes .name$;
				mes "Select a quest to add in this quest slot.";
				next;
				switch(select("Monster Hunt:Item Gathering:MVP Hunt:Random")){
					case 1: .@type = 1; break;
					case 2: .@type = 2; break;
					case 3: .@type = 3; break;
					case 4: .@type = rand(1,3); break;
				}
				switch(.@type){
					case 1:
						.@id = .hunt_list[rand(getarraysize(.hunt_list))];
						.@amount = rand(.min_hunt,.max_hunt);
						.@type$ = "Monster Hunt";
						.@prefix$ = "Hunt";
						c_hunt++;
						break;
						
					case 2:
						.@id = .gather_list[rand(getarraysize(.gather_list))];
						.@amount = rand(.min_gather,.max_gather);
						.@type$ = "Item Gathering";
						.@prefix$ = "Gather";
						break;
						
					case 3:
						.@id = .mvp_list[rand(getarraysize(.mvp_list))];
						.@amount = 1;
						.@type$ = "MVP Hunt";
						.@prefix$ = "MVP";
						c_hunt++;
						break;
				}
				.@TT$[0] = .@type;
				.@TT$[1] = .@id;
				.@TT$[2] = .@amount;
				.@TT$[3] = 0;
				.@T$[.@s] = implode(.@TT$,",");
				#c_quest_data$ = implode(.@T$,":");
				mes .name$;
				mes "Quest " + (.@s + 1) + ": " + .@type$;
				mes .@prefix$ + " ID : " + .@id;
				mes .@prefix$ + " Name : " + ((.@type == 2)?getitemname(.@id):getmonsterinfo(.@id,MOB_NAME));
				if(.@type < 3)
					mes .@prefix$ + " Amount : " + .@amount;
				next;
				goto OnQuest;
			}
			.@type = atoi(.@TT$[0]);
			switch(.@type){
				case 1:
					.@id = atoi(.@TT$[1]);
					.@amount = atoi(.@TT$[2]);
					.@type$ = "Monster Hunt";
					.@prefix$ = "Hunt";
					break;
					
				case 2:
					.@id = atoi(.@TT$[1]);
					.@amount = atoi(.@TT$[2]);
					.@type$ = "Item Gathering";
					.@prefix$ = "Gather";
					break;
					
				case 3:
					.@id = atoi(.@TT$[1]);
					.@amount = atoi(.@TT$[2]);
					.@type$ = "MVP Hunt";
					.@prefix$ = "MVP";
					break;
			}
			mes .name$;
			mes "Quest " + (.@s + 1) + ": " + .@type$;
			mes .@prefix$ + " ID : " + .@id;
			mes .@prefix$ + " Name : " + ((.@type == 2)?getitemname(.@id):getmonsterinfo(.@id,MOB_NAME));
			if(.@type < 3)
				mes .@prefix$ + " Amount : " + .@amount;
			next;
			if(.@type == 2){
				if(countitem(.@id) >= .@amount)
					.@pass = true;
			} else {
				if(atoi(.@TT$[3]) >= .@amount)
					.@pass = true;
			}
			switch(select(((.@pass > 0)?"Submit Quest":""),"Return")){	
				case 1:
					if(.@type == 2)
						delitem .@id,.@amount;
					else
						c_hunt--;
					.@T$[.@s] = "4,0,0,0";
					#c_quest_data$ = implode(.@T$,":");
					.@q = .@quest - 1;
					query_sql("UPDATE `compendium` SET `quest` = '"+.@q+"' WHERE `id` = '"+.@sql_id+"'");
					switch(.@type){
						case 1: .@exp = .hunt_exp; break;
						case 2: .@exp = .gathering_exp; break;
 						case 3: .@exp = .mvp_hunt_exp; break;
					}
					function compendium_getexp;
					next;
					mes .name$;
					mes "Quest has been submitted.";
					mes "Compendium EXP Reward : ^0000CD" + .@exp;
					compendium_getexp(.@exp);
					next;
					goto OnQuest;
					
				case 2:
					goto OnQuest;						
			}
			break;
			

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

OnHour00:
	if(gettime(DT_DAYOFMONTH) == 1){
		query_sql("TRUNCATE `compendium`");
		end;
	}
	query_sql("UPDATE `compendium` SET `quest` = '"+.quest_count+" WHERE `quest` < '"+.quest_count+"'");
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

prontera,156,168,5	duplicate(Compendium Manager#0)	Compendium Manager#1	4_F_DRKAFRA01