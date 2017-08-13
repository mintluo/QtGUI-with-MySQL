insert into stations(sname) values('大连');
insert into stations(sname) values('上海');
insert into stations(sname) values('北京');
insert into stations(sname) values('锦州');
insert into stations(sname) values('天津');
insert into stations(sname) values('沧州');
insert into stations(sname) values('唐山');
insert into stations(sname) values('山海关');
insert into stations(sname) values('盘锦');
insert into stations(sname) values('德州');
insert into stations(sname) values('济南');
insert into stations(sname) values('徐州');
insert into stations(sname) values('南京');

insert into trains(tid,ttype,startstation,endstation) values(131,'T',1,2);
insert into trains(tid,ttype,startstation,endstation) values(278,'T',1,3);

insert into stations_train_pass(tid,sid,time,seq,starttime) values(131,1,'0',1,'12:02');
insert into stations_train_pass(tid,sid,time,seq,starttime) values(131,3,'14:10',25,'0');
insert into stations_train_pass(tid,sid,time,seq,starttime) values(131,4,'13:35',6,'13:50');
insert into stations_train_pass(tid,sid,time,seq,starttime) values(131,5,'22:30',15,'22:35');
insert into stations_train_pass(tid,sid,time,seq,starttime) values(131,6,'00:10',18,'00:15');
insert into stations_train_pass(tid,sid,time,seq,starttime) values(131,8,'19:18',13,'19:28');
insert into stations_train_pass(tid,sid,time,seq,starttime) values(131,9,'17:18',8,'17:28');
insert into stations_train_pass(tid,sid,time,seq,starttime) values(131,10,'02:08',19,'02:18');
insert into stations_train_pass(tid,sid,time,seq,starttime) values(131,11,'03:38',20,'03:43');
insert into stations_train_pass(tid,sid,time,seq,starttime) values(131,12,'05:58',21,'06:03');
insert into stations_train_pass(tid,sid,time,seq,starttime) values(131,13,'11:08',23,'11:13');

insert into stations_train_pass(tid,sid,time,seq,starttime) values(278,1,'0',1,'18:00');
insert into stations_train_pass(tid,sid,time,seq,starttime) values(278,3,'5:46',9,'0');
insert into stations_train_pass(tid,sid,time,seq,starttime) values(278,7,'04:06',7,'04:15');
insert into stations_train_pass(tid,sid,time,seq,starttime) values(278,8,'00:06',4,'00:15');
insert into stations_train_pass(tid,sid,time,seq,starttime) values(278,4,'02:06',6,'02:15');

