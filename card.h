#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"consumption.h"

typedef struct xfreco {
	int w;//窗口号
	char time[17];//时间
	int cardnum;//卡号
	double  money;//消费金额
	struct xfreco* next;//下一条消费记录
}reco;

typedef struct zhanghu {
	char studentID[11];
	int cardnum;
	char name[9];
	char data[9];
	int status;
	struct zhanghu* next;
	struct zhanghu* buka;//补卡信息
	float balance;
	float accumu[3];//累积消费金额
}zh;

//消费记录结构体
typedef struct jilu {//消费记录结构体
	int number;//消费记录位置号
	char time[17];//日期yyyymmdd,时间hhmmssff,精确到0.01秒
	int cardnum;//卡号
	float money;//消费金额
	struct jilu* next;//下一条消费记录
}jl;

//食堂窗口结构体
typedef struct window {
	int w;//窗口号
	int number;//消费记录位置号
	int xftime;//累计消费次数
	float xfmoney;//累计消费金额
	jl* j;//该窗口消费记录
	struct window* next;
}win;

typedef struct xfrecord {
	int w;//窗口号
	char time[17];//时间
	int cardnum;//卡号
	float money;//消费金额
};

win ALLJL[100];
int serialnum = 12345;
zh* ALLZH = NULL;//初始化为空指针
int Buka(char time[17], char studentID[11], int mode);
int sort_1(FILE* fp);


/*发卡函数
输入：学号，姓名
操作：输入学号，系统查询并显示该同学的学号和姓名，确认后进行发卡，
      即分配流水号依次递增的卡号。操作日志记录发卡操作、学号、姓名、
	  卡号和是否成功等信息。如果已经分配过卡号，则属于补卡功能。
返回值：正确执行完毕返回0
*/
int Faka(char studentID[11], char name[9])
{

	FILE* fp = NULL;
	int i = 0, k = 0;
	int m = 0;
	int n = 0;
	int flag = 0;
	char c = '\0';
	char time[17];
	zh* p = NULL;
	zh* q = NULL;

	//打开操作记录文件
	if (fopen_s(&fp, "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\record.txt", "a+")) {
		printf("打开操作记录文件失败！");
		return 1;
	}

	p = ALLZH;
	q = ALLZH;

	//判断学号的唯一性
	while (q = q->next) {//遍历ALLZH
		if (!(strncmp(studentID, q->studentID, 10))) {
			if (q->status == 1 || q->status == 0) {
				flag++;
				printf("%s%s已分配过卡号，是否要进行补卡？    [y]es/[n]o\n", q->studentID, q->name);
				getchar();
				c = getchar();
				if (c == 'y') {
					printf("输入日期：\n");
					scanf_s("%s",time, 17);
					Buka(time, studentID, 2);
				}
				else {
					fclose(fp);
					return -1;
				}
			}
		}
		p = q;
	}
	if (flag == 0) {//学号唯一
		q = (zh*)malloc(sizeof(zh));//动态分配存储空间
		if (q != NULL) {
			p->next = q;
			serialnum++;//流水号+1
			strncpy_s(q->studentID, 11, studentID, 10);//录入学号信息
			strncpy_s(q->name, 9, name, 9);//录入姓名信息
			q->cardnum = 3000000 + serialnum * 10;
			n = q->cardnum;
			for (m = 0, i = 0; i < 7; i++) {
				m += n % 10;
				n = n / 10;
			}
			q->cardnum += 9 - m % 10;//录入卡号信息
			strncpy_s(q->data, 9, "20240715\0", 9);//有效日期
			q->status = 1;//卡状态设为1
			q->next = NULL;
			q->buka = NULL;//补卡信息为空
			q->balance = 0;//卡中余额初始为0
			q->accumu[0] = 0;//7-9点卡片累积消费金额初始化为0
			q->accumu[1] = 0;//11-13点卡片累积消费金额初始化为0
			q->accumu[2] = 0;//17-19点卡片累积消费金额初始化为0
		}
		//将开户操作写入开户记录文件（操作日志)
		if (q->status == 1) {
			fprintf(fp, "2021090100000000,%s%s,发卡成功 卡号：%d\n", q->studentID, q->name,q->cardnum);
			printf("%s %s(%d)开户发卡成功！\n", q->studentID, q->name, q->cardnum);
		}
	}
	fclose(fp);
	return 0;
}

/*批量开户文件处理函数
输入：指向批量开户文件的指针FILE* fp；
操作：按行读取文件内容，并进行开户发卡操作；
返回：正确执行完毕返回0；
*/
int KH(FILE* fp)
{
	char infor[25];
	char studentID[11];
	char name[9];
	int i = 0, k = 0;

	
	while (fgets(infor, 24, fp)) {
		strncpy_s(studentID, 11, infor, 10);//录入学号信息
		for (i = 0, k = 11; infor[k] != ';'; i++, k++)
			name[i] = infor[k];//录入姓名信息
		for (; i < 9; i++)
			name[i] = 0;
		Faka(studentID, name);
	}
	return 0;
}


/*销户函数
输入：学号studentID,时间time
      操作模式mode，mode=1进行批量操作，mode=2进行个人操作
操作：输入学号，系统查询并显示该同学的学号和姓名，确认后进行销户；
      已经销户的信息不再能够查询。操作日志记录销户的学生、销户以及
	  销户是否成功等信息。
返回值：正确执行完毕后返回0；
*/
int Xiaohu(char time[17],char studentID[11],int mode)
{
	zh* p = ALLZH;//p指向账户信息链表头结点
	char c;
	FILE* fp;

	//打开操作记录文件
	if (fopen_s(&fp, "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\record.txt", "a+")) {
		printf("打开操作记录文件失败！");
		return 1;
	}

	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			if (p->status == 1 || p->status == 0) {//该账户为正常状态/挂失状态
				if (mode == 2) {//个人操作
					printf("%s%s确定对该账户进行销户吗？  [y]es/[n]o\n", p->studentID, p->name);
					getchar();
					if ((c = getchar()) == 'y') {
						p->status = -1;//将该账户的状态标位-1，即为销户
						fprintf(fp, "%s,%s%s,销户成功\n",time, p->studentID, p->name);
						printf("销户成功！\n");
					}
					else {
						printf("已取消销户操作！\n");
						fprintf(fp, "%s,%s%s,销户失败\n", time,p->studentID, p->name);
					}
					break;
				}
				if (mode == 1) {//批量操作
					p->status = -1;
					fprintf(fp, "%s,%s%s,销户成功\n", time,p->studentID, p->name);
					printf("销户成功！\n");
					break;
				}
			}
			else {//卡状态为-1，已经是销户状态
				printf("查询不到该账户信息！\n");
				break;
			}
		}
	}
	if (p == NULL)
		printf("查询不到该账户信息！\n");
	fclose(fp);
	return 0;
}

/*挂失函数
输入：学号studentID,时间time；
操作：输入学号，系统查询并显示该同学的学号、姓名和卡号，
      确认后进行挂失，即设置当前账户相关校园卡的状态。
      操作日志记录操作功能、卡号和主要信息号和是否成功等信息。
返回值：正确执行完毕后返回0；
*/
int Guashi(char time[17],char studentID[11],int mode)
{
	zh* p = ALLZH;
	char c;
	FILE* fp;
	if (fopen_s(&fp, "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\record.txt", "a+")) {
		printf("打开操作记录文件失败！");
		return 1;
	}
	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			if (p->status == 1) {//该账户为正常状态
				if (mode == 2) {//个人操作
					printf("%s %s %d确定对该账户进行挂失吗？  [y]es/[n]o\n", p->studentID, p->name, p->cardnum);
					getchar();
					if ((c = getchar()) == 'y') {
						p->status = 0;//将该账户的状态标位0，即为挂失
						fprintf(fp, "%s,%s%s,挂失成功\n",time, p->studentID, p->name);
						printf("%s%s挂失成功！\n", p->studentID, p->name);
					}
					else {
						printf("已取消挂失操作！\n");
						fprintf(fp, "%s,%s%s,挂失失败\n",time, p->studentID, p->name);
					}
					break;
				}
				if (mode == 1) {//批量操作
					p->status = 0;
					fprintf(fp, "%s,%s%s,挂失成功\n", time, p->studentID, p->name);
					printf("%s%s挂失成功！\n",p->studentID,p->name);
					break;
				}
			}
			else if (p->status == 0) {//卡状态为0，已经是挂失状态
				printf("%s%s该卡已处于挂失状态!\n", p->studentID, p->name);
				break;
			}
			else {
				printf("查询不到该账户信息！\n");
				break;
			}
		}
	}
	if (p == NULL)
		printf("查询不到该账户信息！\n");
	fclose(fp);
	return 0;
}

/*解挂函数
输入：学号studentID,时间time；
操作：输入学号，系统查询并显示该同学的学号、姓名和卡号，
	  确认后进行解挂，即设置当前账户相关校园卡的状态。
	  操作日志记录操作功能、卡号和主要信息号和是否成功等信息。
返回值：正确执行完毕后返回0；
*/
int Jiegua(char time[17], char studentID[11], int mode)
{
	zh* p = ALLZH;
	char c;
	FILE* fp;
	if (fopen_s(&fp, "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\record.txt", "a+")) {
		printf("打开操作记录文件失败！");
		return 1;
	}
	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			if (p->status == 0) {//该账户为挂失状态
				if (mode == 2) {//个人操作
					printf("%s %s %d确定对该账户进行解挂吗？  [y]es/[n]o\n", p->studentID, p->name, p->cardnum);
					getchar();
					if ((c = getchar()) == 'y') {
						p->status = 1;//将该账户的状态标位1，即为解挂
						fprintf(fp, "%s,%s%s,解挂 成功\n",time, p->studentID, p->name);
						printf("%s%s解挂成功！\n",p->studentID,p->name);
					}
					else {
						printf("已取消解挂操作！\n");
						fprintf(fp, "%s,%s%s,解挂失败\n", time, p->studentID, p->name);
					}
					break;
				}
				if (mode == 1) {//批量操作
					p->status = 0;
					fprintf(fp, "%s,%s%s,解挂成功\n", time, p->studentID, p->name);
					printf("%s%s解挂成功！\n",p->studentID,p->name);
					break;
				}
			}
			else if (p->status == 1) {//卡状态为1，已经是正常状态
				printf("%s%s该卡已处于正常状态！\n", p->studentID, p->name);
				break;
			}
			else {
				printf("查询不到该账户信息！\n");
				break;
			}
		}
	}
	if (p == NULL)
		printf("查询不到该账户信息！\n");
	fclose(fp);
	return 0;
}

/*补卡函数
输入：学号studentID,时间time,模式mode
操作：输入学号，系统查询并显示该同学的学号、姓名，
      确认后对以前的卡片进行挂失并进行补卡。操作日
	  志记录操作功能、新卡的卡号和学号、姓名等主要
	  信息号和是否成功等信息.
返回值:正确执行完毕后返回0;
*/
int Buka(char time[17],char studentID[11],int mode)
{
	zh* p = ALLZH;
	zh* q = NULL;
	zh* r = NULL;
	char c;
	FILE* fp=NULL;
	int m = 0;
	int n = 0;
	int i = 0;
	if (fopen_s(&fp, "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\record.txt", "a+")) {
						printf("打开操作记录文件失败！\n");
						return 1;
					}
	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			i = 0;
			r = p;
			while (r=r->buka)
				i++;//记录补卡的次数

			if (i <= 100) {
				if (p->status >= 0) {//开户状态
					if (mode == 2) {//个人操作
						printf("%s %s 确定对以前的卡片进行挂失并进行补卡吗？  [y]es/[n]o\n", p->studentID, p->name);
						getchar();
						if ((c = getchar()) == 'y') {
							q = (zh*)malloc(sizeof(zh));//动态分配存储空间
							if (q != NULL) {
								serialnum++;//流水号+1
								strncpy_s(q->studentID, 11, p->studentID, 10);//录入学号信息
								strncpy_s(q->name, 9, p->name, 9);//录入姓名信息
								q->cardnum = 3000000 + serialnum * 10;
								n = q->cardnum;
								for (m = 0, i = 0; i < 7; i++) {
									m += n % 10;
									n = n / 10;
								}
								q->cardnum += 9 - m % 10;//录入卡号信息
								strncpy_s(q->data, 9, "20240715\0", 9);//有效日期
								q->status = 1;//卡状态设为1
								q->next = NULL;
								q->buka = NULL;//补卡信息为空
								q->balance = p->balance;//卡中余额初始为0
								q->accumu[0] = p->accumu[0];//7-9点卡片累积消费金额初始化为0
								q->accumu[1] = p->accumu[1];//11-13点卡片累积消费金额初始化为0
								q->accumu[2] = p->accumu[2];//17-19点卡片累积消费金额初始化为0
							}
							m = p->cardnum;
							p->cardnum = q->cardnum;
							q->cardnum = m;
							p->status = 1;
							q->status = 0;
							q->buka = p->buka;
							p->buka = q;
							fprintf(fp, "%s,%s%s,补卡成功 卡号：%d\n", time, p->studentID, p->name, p->cardnum);
							printf("补卡成功！  卡号：%d  当前余额：%.2f\n", p->cardnum, p->balance);
						}
						else
							printf("已取消补卡！\n");
						break;
					}
					if (mode == 1) {//批量操作
						q = (zh*)malloc(sizeof(zh));//动态分配存储空间
						if (q != NULL) {
							serialnum++;//流水号+1
							strncpy_s(q->studentID, 11, p->studentID, 10);//录入学号信息
							strncpy_s(q->name, 9, p->name, 9);//录入姓名信息
							q->cardnum = 3000000 + serialnum * 10;
							n = q->cardnum;
							for (m = 0, i = 0; i < 7; i++) {
								m += n % 10;
								n = n / 10;
							}
							q->cardnum += 9 - m % 10;//录入卡号信息
							strncpy_s(q->data, 9, "20240715\0", 9);//有效日期
							q->status = 1;//卡状态设为1
							q->next = NULL;
							q->buka = NULL;//补卡信息为空
							q->balance = p->balance;//卡中余额初始为0
							q->accumu[0] = p->accumu[0];//7-9点卡片累积消费金额初始化为0
							q->accumu[1] = p->accumu[1];//11-13点卡片累积消费金额初始化为0
							q->accumu[2] = p->accumu[2];//17-19点卡片累积消费金额初始化为0
						}
						m = p->cardnum;
						p->cardnum = q->cardnum;
						q->cardnum = m;
						p->status = 1;
						q->status = 0;
						q->buka = p->buka;
						p->buka = q;
						fprintf(fp, "%s,%s%s,补卡成功\n", time, p->studentID, p->name);
						printf("补卡成功！  卡号：%d  当前余额：%.2f\n", p->cardnum, p->balance);
						break;
					}
				}
			}
			else {
				printf("补卡次数已达上限！\n");
				break;
			}
		}
	}
	if (m == 0)
		printf("查询不到该账户信息！\n");
	fclose(fp);
	return 0;
}

/*充值函数
输入：学号studentID,充值金额m,时间time,模式mode
操作：输入学号，系统查询并显示该同学的学号、姓名，确认后进行
      充值，如果充值结果使得余额超过上限，则本次操作失败，恢
	  复原有的余额。操作日志记录操作功能、学号、姓名、余额在
	  充值前后的金额以及充值是否成功等信息。
返回值：正确执行完毕后返回0；
*/
int Chongzhi(char time[17],char studentID[11], float m,int mode)
{
	zh* p = ALLZH;
	char c;
	FILE* fp;
	float n = 0;

	if (fopen_s(&fp, "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\record.txt", "a+")) {
		printf("打开操作记录文件失败！");
		return 1;
	}
	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			if (p->status >=0) {//该账户为正常状态
				if (mode == 2) {//个人操作
					printf("%s %s 当前余额：%f\n确定对该账户进行充值吗？  [y]es/[n]o\n", p->studentID, p->name, p->balance);
					getchar();
					if ((c = getchar()) == 'y') {
						if (p->balance + m < 1000) {
							n = p->balance;
							p->balance += m;
							fprintf(fp, "%s,%s%s,充值成功  卡号：%d  充值前：%.2f  充值后：%.2f\n",time, p->studentID, p->name, p->cardnum,n, p->balance);
							printf("%s%s充值成功！充值前：%.2f  充值后：%.2f\n", p->studentID, p->name, n, p->balance);
						}
						else//充值结果余额超过上限
						{
							printf("充值结果余额超过上限，充值失败！   当前余额：%.2f\n", p->balance);
							fprintf(fp, "%s,%s%s,充值失败  卡号：%d  充值前：%.2f  充值后：%.2f\n",time, p->studentID, p->name,p->cardnum, p->balance, p->balance);
						}
					}
					else {
						printf("已取消充值！\n");
						fprintf(fp, "%s,%s%s,充值失败  卡号：%d  充值前：%.2f  充值后：%.2f\n",time, p->studentID, p->name,p->cardnum, p->balance, p->balance);
					}
				}
				if (mode == 1) {//批量充值
					if (p->balance + m < 1000) {
						n = p->balance;
						p->balance += m;
						fprintf(fp, "%s,%s%s,充值成功  卡号：%d  充值前：%.2f  充值后：%.2f\n",time, p->studentID, p->name,p->cardnum, n, p->balance);
						printf("%s%s充值成功！ 充值前：%.2f  充值后：%.2f\n", p->studentID, p->name, n, p->balance);
					}
					else//充值结果余额超过上限
					{
						printf("充值结果余额超过上限，充值失败！   当前余额：%.2f\n", p->balance);
						fprintf(fp, "%s,%s%s,充值失败  卡号：%d  充值前：%.2f  充值后：%.2f\n",time, p->studentID, p->name, p->cardnum,p->balance, p->balance);
					}
				}
				break;
			}
		}
	}
	if (p == NULL)
		printf("查询不到账户信息！\n");
	fclose(fp);
	return 0;
}


/*窗口消费函数
输入：窗口号w、消费时间time、消费金额a、卡号cardnum、模式mode、指向消费记录文件的指针fp
操作：在确定的程序时间和食堂窗口场景下，输入消费的卡号和消费金额，
	  系统检查该校园卡是否正常的卡，检查余额是否足够，然后进行实际
	  的余额扣减；如果本次消费成功，则生成并保存具有完整信息项的消
	  费记录，并显示当前该窗口在当天的累计消费次数和消费金额。操作
	  日志记录该项消费操作、卡号、余额的变化和消费是否成功等信息。
返回值：正确执行完毕后返回0；
*/
int Xiaofei(int w, char* time, double a, int cardnum, int mode,FILE* fp)
{
	int i = 0, k = 0;
	int j = 0;
	float accumu = 0;//个人累积消费金额
	char password[5];
	char t[8]="";
	jl* p = NULL;
	zh* q = ALLZH;
	

	a = (float)a;
	for (i = 0; i < 8; i++) 
		t[i] = time[i + 8];


	//根据卡号找到该校园卡账户
	while (q = q->next)
		if (q->cardnum == cardnum && q->status >= 0)
			break;
	if (q == NULL) {
		printf("查询不到该账户信息！\n");
		return -1;
	}
	//对个人累积消费金额进行更新
	

	//判断卡是否能进行消费
	if (q->balance < a)//卡中余额不足
		printf("卡中余额不足！  当前余额：%.2f\n", q->balance);
	else if (q->status == 0)//卡片已挂失
		printf("该卡已挂失，无法进行消费！\n");
	else {//卡中余额充足

		//判断该时间段消费是否超过20元
		j = atoi(t);
		if (j >= 7000000 && j < 9000000) {
			if (q->accumu[0] + a >= 20) {//累积消费金额超过20元，需要输入密码
				if (mode == 2) {//个人操作
					printf("请输入密码：");
					scanf_s("%s", password, 5);
					if (strncmp(password, "8888", 4)) {
						printf("密码错误！\n");
						fclose(fp);
						return 0;
					}
					else q->accumu[0] = 0;//输入密码正确
				}
				if (mode == 1)//批量操作，默认输入密码正确
					q->accumu[0] = 0;
			}
			q->balance -= a;//从账户余额中扣除消费金额
			q->accumu[0] += a;//累积消费金额增加
		}
		else if (j >= 11000000 && j < 13000000) {
			if (q->accumu[1] >= 20) {//累积消费金额超过20元，需要输入密码
				if (mode == 2) {//个人操作
					printf("请输入密码：");
					scanf_s("%s", password, 5);
					if (strncmp(password, "8888", 4)) {
						printf("密码错误！\n");
						fclose(fp);
						return 0;
					}
					else q->accumu[1] = 0;//密码输入正确
				}
				if (mode == 1)//批量操作，默认输入密码正确
					q->accumu[1] = 0;
			}
			q->balance -= a;//从账户余额中扣除消费金额
			q->accumu[1] += a;//累积消费金额增加
		}
		else if (j >= 17000000 && j < 19000000) {
			if (q->accumu[2] >= 20) {//累积消费金额超过20元，需要输入密码
				if (mode == 2) {//个人操作
					printf("请输入密码：");
					scanf_s("%s", password);
					if (strncmp(password, "8888", 4)) {
						printf("密码错误！\n");
						fclose(fp);
						return 0;
					}
					else q->accumu[2] = 0;//密码输入正确
				}
				if (mode == 1)//批量操作，默认输入密码正确
					q->accumu[2] = 0;
			}
			q->balance -= a;//从账户余额中扣除消费金额
			q->accumu[2] += a;//累积消费金额增加
		}

		p = ALLJL[w].j;//找到当前消费记录位置

		//对该窗口的累积消费次数和金额进行更新
		if (strncmp(p->time, time, 8)) {
			ALLJL[w].xftime = 0;
			ALLJL[w].xfmoney = 0;
		}
		p = p->next;
		//修改该消费记录位置的消费记录
		strncpy_s(p->time, 17, time, 17);
		p->cardnum = cardnum;
		p->money = a;
		ALLJL[w].xftime++;
		ALLJL[w].xfmoney += a;
		ALLJL[w].number++;
		ALLJL[w].j = p;

		//将消费记录写入操作日志
		fprintf(fp, "%s 学号：%s 卡号：%d 消费金额：%.2f  卡中余额：%.2f\n", p->time, q->studentID, p->cardnum, a, q->balance);
		//printf("%d号窗口  该窗口累积消费次数：%d  该窗口累积消费金额：%.2f  本条消费记录位置：%d %d\n", w, ALLJL[w].xftime, ALLJL[w].xfmoney,p->number,ALLJL[w].number);
	}
	return 0;
}

/*批量操作文件处理函数
输入：指向批量操作文件的指针fp1,指向批量消费文件的指针fp2;
操作：按时间先后顺序执行卡操作或消费；
返回值：正确执行完毕后返回0；
*/
int CZ_XF(FILE* fp1,FILE* fp2)
{
	int i = 0, k = 0;
	int w = 0;//暂存窗口号初始化为0
	float money = 0;//暂存消费金额
	char infor1[40]="";//操作信息
	char infor2[40]="";//操作信息
	char studentID[11];//暂存学号
	char time1[17];//暂存时间
	char time2[17];//暂存时间
	char cardnum[8];//暂存卡号
	char m[6];
	char* tag1;
	char* tag2;
	jl* q = NULL;
	jl* r = NULL;
	FILE* fp = NULL;
	const char* filename = "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\xfrecord.txt";

	sort_1(fp2);
	fclose(fp2);
	if (fopen_s(&fp2, "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\sort_1.txt", "r")) {
		printf("打开文件失败！\n");
		return 1;
	}//打开排序后的批量消费文件

	//打开消费记录操作日志文件
	if (fopen_s(&fp, filename, "a+")) {
		printf("打开消费记录操作日志文件失败！\n");
		return 1;
	}

	//9月23日前卡片管理
	while (tag1 = fgets(infor1, 40, fp1)) {
		for (i = 0, k = 0; i < 16; i++, k++)
			time1[k] = infor1[i];//时间time1
		time1[16] = '\0';
		if (strncmp(time1, "2021090700000000", 16)<0) {
			for (i = 22, k = 0; k < 10; i++, k++)
				studentID[k] = infor1[i];//学号
			studentID[k] = '\0';
			if (strstr(infor1, "挂失"))
				Guashi(time1, studentID, 1);
			else if (strstr(infor1, "解挂"))
				Jiegua(time1, studentID, 1);
			else if (strstr(infor1, "充值")) {
				for (k = 0, i++; infor1[i] != ';'; i++, k++)
					m[k] = infor1[i];//读取充值金额
				m[k] = '\0';
				money = atof(m);
				Chongzhi(time1, studentID, money, 1);
			}
			else if (strstr(infor1, "补卡"))
				Buka(time1, studentID, 1);
			else if (strstr(infor1, "销户"))
				Xiaohu(time1, studentID, 1);
		}
		else break;
	}
	//9月23日-10月13日批量消费
	while (fgets(infor2, 40, fp2)) {
		for (i = 0, k = 0; infor2[i] != ',';k++, i++)
			time2[k] = infor2[i];
		time2[16] = '\0';//时间
		if ((strncmp(time2, "2021092300000000", 16) > 0)\
			&& (strncmp(time2, "2021101400000000", 16) < 0)) {
			for (k = 0, i++; infor2[i] != ','; k++, i++)
				cardnum[k] = infor2[i];//卡号
			cardnum[7] = '\0';
			i++;
			for (k = 0; infor2[i] != ','; k++, i++)
				m[k] = infor2[i];
			m[k] = '\0';
			w = atoi(m);//窗口号
			i++;
			for (k = 0; infor2[i] != ';'; k++, i++) {
				m[k] = infor2[i];
			}
			m[k] = '\0';
			i = atoi(cardnum);
			money = atof(m);
			Xiaofei(w, time2, money, i, 1,fp);
		}
		else break;
	}
	;
	//第2次充值
	for (i = 0, k = 0; i < 16; i++, k++)
		time1[k] = infor1[i];//时间time1
	time1[16] = '\0';
	for (i = 22, k = 0; k < 10; i++, k++)
		studentID[k] = infor1[i];//学号
	studentID[k] = '\0';
	for (k = 0, i++; infor1[i] != ';'; i++, k++)
		m[k] = infor1[i];//读取充值金额
	m[k] = '\0';
	money = atof(m);
	Chongzhi(time1, studentID, money, 1);
	while (tag1 = fgets(infor1, 40, fp1)) {
		for (i = 0, k = 0; i < 16; i++, k++)
			time1[k] = infor1[i];//时间time1
		time1[16] = '\0';
		if (strncmp(time1, "20211014", 8) == 0) {
			for (i = 22, k = 0; k < 10; i++, k++)
				studentID[k] = infor1[i];//学号
			studentID[k] = '\0';
			for (k = 0, i++; infor1[i] != ';'; i++, k++)
				m[k] = infor1[i];//读取充值金额
			m[k] = '\0';
			money = atof(m);
			Chongzhi(time1, studentID, money, 1);
		}
		else break;
	}

	//10月14日到11月3日消费
	for (i = 0, k = 0; infor2[i] != ','; k++, i++)
		time2[k] = infor2[i];
	time2[16] = '\0';//时间
	if ((strncmp(time2, "2021101400000000", 16) > 0)\
		&& (strncmp(time2, "2021110400000000", 16) < 0)) {
		for (k = 0, i++; infor2[i] != ','; k++, i++)
			cardnum[k] = infor2[i];//卡号
		cardnum[7] = '\0';
		i++;
		for (k = 0; infor2[i] != ','; k++,i++)
			m[k] = infor2[i];
		m[k] = '\0';
		w = atoi(m);//窗口号
		i++;
		for (k = 0; infor2[i] != ';'; k++, i++) {
			m[k] = infor2[i];
		}
		m[k] = '\0';
		i = atoi(cardnum);
		money = atof(m);
		Xiaofei(w, time2, money, i, 1,fp);
	}
	while (fgets(infor2, 40, fp2)) {
		for (i = 0, k = 0; infor2[i] != ','; k++, i++)
			time2[k] = infor2[i];
		time2[16] = '\0';//时间
		if ((strncmp(time2, "2021101400000000", 16) > 0)\
			&& (strncmp(time2, "2021110400000000", 16) < 0)) {
			for (k = 0, i++; infor2[i] != ','; k++, i++)
				cardnum[k] = infor2[i];//卡号
			cardnum[7] = '\0';
			i++;
			for (k = 0; infor2[i] != ','; k++, i++)
				m[k] = infor2[i];
			m[k] = '\0';
			w = atoi(m);//窗口号
			i++;
			for (k = 0; infor2[i] != ';'; k++, i++) {
				m[k] = infor2[i];
			}
			m[k] = '\0';
			i = atoi(cardnum);
			money = atof(m);
			Xiaofei(w, time2, money, i, 1,fp);
		}
		else break;
	}
	//11月4日充值（第3次充值）
	for (i = 0, k = 0; i < 16; i++, k++)
		time1[k] = infor1[i];//时间time1
	time1[16] = '\0';
	for (i = 22, k = 0; k < 10; i++, k++)
		studentID[k] = infor1[i];//学号
	studentID[k] = '\0';
	for (k = 0, i++; infor1[i] != ';'; i++, k++)
		m[k] = infor1[i];//读取充值金额
	m[k] = '\0';
	money = atof(m);
	Chongzhi(time1, studentID, money, 1);
	while (tag1 = fgets(infor1, 40, fp1)) {
		for (i = 0, k = 0; i < 16; i++, k++)
			time1[k] = infor1[i];//时间time1
		time1[16] = '\0';
		if (strncmp(time1, "20211104", 8) == 0) {
			for (i = 22, k = 0; k < 10; i++, k++)
				studentID[k] = infor1[i];//学号
			studentID[k] = '\0';
			for (k = 0, i++; infor1[i] != ';'; i++, k++)
				m[k] = infor1[i];//读取充值金额
			m[k] = '\0';
			money = atof(m);
			Chongzhi(time1, studentID, money, 1);
		}
		else break;
	}
	;
	//11月4日到11月24日消费
	for (i = 0, k = 0; infor2[i] != ','; k++, i++)
		time2[k] = infor2[i];
	time2[16] = '\0';//时间
	if ((strncmp(time2, "2021110400000000", 16) > 0)\
		&& (strncmp(time2, "2021112500000000", 16) < 0)) {
		for (k = 0, i++; infor2[i] != ','; k++, i++)
			cardnum[k] = infor2[i];//卡号
		cardnum[7] = '\0';
		i++;
		for (k = 0; infor2[i] != ','; k++,i++)
			m[k] = infor2[i];
		m[k] = '\0';
		w = atoi(m);//窗口号
		i++;
		for (k = 0; infor2[i] != ';'; k++, i++) {
			m[k] = infor2[i];
		}
		m[k] = '\0';
		i = atoi(cardnum);
		money = atof(m);
		Xiaofei(w, time2, money, i, 1,fp);
	}
	while (fgets(infor2, 40, fp2)) {
		for (i = 0, k = 0; infor2[i] != ','; k++, i++)
			time2[k] = infor2[i];
		time2[16] = '\0';//时间
		if ((strncmp(time2, "2021110400000000", 16) > 0)\
			&& (strncmp(time2, "2021112500000000", 16) < 0)) {
			for (k = 0, i++; infor2[i] != ','; k++, i++)
				cardnum[k] = infor2[i];//卡号
			cardnum[7] = '\0';
			i++;
			for (k = 0; infor2[i] != ','; k++, i++)
				m[k] = infor2[i];
			m[k] = '\0';
			w = atoi(m);//窗口号
			i++;
			for (k = 0; infor2[i] != ';'; k++, i++) {
				m[k] = infor2[i];
			}
			m[k] = '\0';
			i = atoi(cardnum);
			money = atof(m);
			Xiaofei(w, time2, money, i, 1,fp);
		}
		else break;
	}
	;
	//11月25日充值（第4次充值）
	for (i = 0, k = 0; i < 16; i++, k++)
		time1[k] = infor1[i];//时间time1
	time1[16] = '\0';
	for (i = 22, k = 0; k < 10; i++, k++)
		studentID[k] = infor1[i];//学号
	studentID[k] = '\0';
	for (k = 0, i++; infor1[i] != ';'; i++, k++)
		m[k] = infor1[i];//读取充值金额
	m[k] = '\0';
	money = atof(m);
	Chongzhi(time1, studentID, money, 1);
	while (tag1 = fgets(infor1, 40, fp1)) {
		for (i = 0, k = 0; i < 16; i++, k++)
			time1[k] = infor1[i];//时间time1
		time1[16] = '\0';
		if (strncmp(time1, "20211125", 8) == 0) {
			for (i = 22, k = 0; k < 10; i++, k++)
				studentID[k] = infor1[i];//学号
			studentID[k] = '\0';
			for (k = 0, i++; infor1[i] != ';'; i++, k++)
				m[k] = infor1[i];//读取充值金额
			m[k] = '\0';
			money = atof(m);
			Chongzhi(time1, studentID, money, 1);
		}
		else break;
	}
	;
	//11月25日到12月15日消费
	for (i = 0, k = 0; infor2[i] != ','; k++, i++)
		time2[k] = infor2[i];
	time2[16] = '\0';//时间
	if ((strncmp(time2, "2021112500000000", 16) > 0)\
		&& (strncmp(time2, "2021121600000000", 16) < 0)) {
		for (k = 0, i++; infor2[i] != ','; k++, i++)
			cardnum[k] = infor2[i];//卡号
		cardnum[7] = '\0';
		i++;
		for (k = 0; infor2[i] != ',';k++, i++)
			m[k] = infor2[i];
		m[k] = '\0';
		w = atoi(m);//窗口号
		i++;
		for (k = 0; infor2[i] != ';'; k++, i++) {
			m[k] = infor2[i];
		}
		m[k] = '\0';
		i = atoi(cardnum);
		money = atof(m);
		Xiaofei(w, time2, money, i, 1,fp);
	}
	while (fgets(infor2, 40, fp2)) {
		for (i = 0, k = 0; infor2[i] != ','; k++, i++)
			time2[k] = infor2[i];
		time2[16] = '\0';//时间
		if ((strncmp(time2, "2021112500000000", 16) > 0)\
			&& (strncmp(time2, "2021121600000000", 16) < 0)) {
			for (k = 0, i++; infor2[i] != ','; k++, i++)
				cardnum[k] = infor2[i];//卡号
			cardnum[7] = '\0';
			i++;
			for (k = 0; infor2[i] != ','; k++, i++)
				m[k] = infor2[i];
			m[k] = '\0';
			w = atoi(m);//窗口号
			i++;
			for (k = 0; infor2[i] != ';'; k++, i++) {
				m[k] = infor2[i];
			}
			m[k] = '\0';
			i = atoi(cardnum);
			money = atof(m);
			Xiaofei(w, time2, money, i, 1,fp);
		}
		else break;
	}
	;
	//12月16日充值（第5次充值）
	for (i = 0, k = 0; i < 16; i++, k++)
		time1[k] = infor1[i];//时间time1
	time1[16] = '\0';
	for (i = 22, k = 0; k < 10; i++, k++)
		studentID[k] = infor1[i];//学号
	studentID[k] = '\0';
	for (k = 0, i++; infor1[i] != ';'; i++, k++)
		m[k] = infor1[i];//读取充值金额
	m[k] = '\0';
	money = atof(m);
	Chongzhi(time1, studentID, money, 1);
	while (tag1 = fgets(infor1, 40, fp1)) {
		for (i = 0, k = 0; i < 16; i++, k++)
			time1[k] = infor1[i];//时间time1
		time1[16] = '\0';
		if (strncmp(time1, "20211216", 8) == 0) {
			for (i = 22, k = 0; k < 10; i++, k++)
				studentID[k] = infor1[i];//学号
			studentID[k] = '\0';
			for (k = 0, i++; infor1[i] != ';'; i++, k++)
				m[k] = infor1[i];//读取充值金额
			m[k] = '\0';
			money = atof(m);
			Chongzhi(time1, studentID, money, 1);
		}
		else break;
	}
	;
	//12月16日到12月31日消费
	for (i = 0, k = 0; infor2[i] != ','; k++, i++)
		time2[k] = infor2[i];
	time2[16] = '\0';//时间
	if ((strncmp(time2, "2021121600000000", 16) > 0)\
		&& (strncmp(time2, "2021123200000000", 16) < 0)) {
		for (k = 0, i++; infor2[i] != ','; k++, i++)
			cardnum[k] = infor2[i];//卡号
		cardnum[7] = '\0';
		i++;
		for (k = 0; infor2[i] != ',';k++, i++)
			m[k] = infor2[i];
		m[k] = '\0';
		w = atoi(m);//窗口号
		i++;
		for (k = 0; infor2[i] != ';'; k++, i++) {
			m[k] = infor2[i];
		}
		m[k] = '\0';
		i = atoi(cardnum);
		money = atof(m);
		Xiaofei(w, time2, money, i, 1,fp);
	}
	while (fgets(infor2, 40, fp2)) {
		for (i = 0, k = 0; infor2[i] != ','; k++, i++)
			time2[k] = infor2[i];
		time2[16] = '\0';//时间
		if ((strncmp(time2, "2021121600000000", 16) > 0)\
			&& (strncmp(time2, "2021123200000000", 16) < 0)) {
			for (k = 0, i++; infor2[i] != ','; k++, i++)
				cardnum[k] = infor2[i];//卡号
			cardnum[7] = '\0';
			i++;
			for (k = 0; infor2[i] != ','; k++, i++)
				m[k] = infor2[i];
			m[k] = '\0';
			w = atoi(m);//窗口号
			i++;
			for (k = 0; infor2[i] != ';'; k++, i++) {
				m[k] = infor2[i];
			}
			m[k] = '\0';
			i = atoi(cardnum);
			money = atof(m);
			Xiaofei(w, time2, money, i, 1,fp);
		}
		else break;
	}
	fclose(fp);
	return 0;
}



/*输入：批量消费文件指针*/
int sort_1(FILE* fp)
{
	reco r[100];
	reco* p = NULL;
	int w = 0;
	int flag = 99;
	char info[40] = "";
	char* tag = NULL;
	char m[8] = "";
	int i = 0, k = 0;
	const char* filename = "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\sort_1.txt";
	FILE* fp1 = NULL;

	//读取文件中的所有消费记录
	while (tag = fgets(info, 40, fp)) {
		while (info[0] == 'W' && tag) {
			m[0] = info[1];
			m[1] = info[2];
			m[2] = '\0';
			w = atoi(m);//读取窗口号信息
			p = NULL;
			p = &r[w];
			tag = fgets(info, 40, fp);
		}
		if (tag) {
			p->w = w;
			for (k = 0, i = 8; info[i] != ','; i++, k++)
				p->time[k] = info[i];//日期
			i++;
			for (; info[i] != ','; i++, k++)
				p->time[k] = info[i];//时间
			p->time[16] = '\0';

			for (i = 0, k = 0; info[i] != ','; i++, k++)
				m[k] = info[i];//卡号
			m[7] = '\0';
			p->cardnum = atoi(m);

			for (k = 0, i = 26; info[i] != ';'; i++, k++)
				m[k] = info[i];//消费金额
			m[k] = '\0';
			p->money = atof(m);

			p->next = (reco*)malloc(sizeof(reco));
			p = p->next;
			p->w = 0;
			p->cardnum = 0;
			p->money = 0;
			p->next = NULL;
			strncpy_s(p->time, 17, "", 16);
		}
	}

	if (fopen_s(&fp1, filename, "w"))//打开sort_1文件
	{
		printf("打开文件失败！\n");
		return 1;
	}
	//找到99个窗口的消费记录中时间最早的
	while (flag) {
		strncpy_s(r[0].time, 17, "2022000000000000", 16);
		for (i = 1; i < 100; i++) {
			if ((strncmp(r[0].time, r[i].time, 16) > 0) && (strlen(r[i].time)))
				r[0] = r[i];
		}
		i = r[0].w;
		r[i].w = r[i].next->w;
		r[i].cardnum = r[i].next->cardnum;
		strncpy_s(r[i].time, 17, r[i].next->time, 16);
		r[i].money = r[i].next->money;
		r[i].next = r[i].next->next;
		fprintf(fp1, "%s,%d,%d,%.2f;\n", r[0].time, r[0].cardnum, r[0].w, r[0].money);
		for (i = 1, flag = 99; i < 100; i++)
			if (r[i].w <= 0 || r[i].cardnum == 0)
				flag--;//flag记录还有几个窗口中有消费记录
	}
	fclose(fp1);
	return 0;
}