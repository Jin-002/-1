#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct zhanghu {
	char studentID[11];
	int cardnum;
	char name[9];
	int data;
	int status;
	struct zhanghu* next;
	struct zhanghu* buka;//补卡信息
	float balance;
}zh;

int serialnum = 12345;
zh* ALLZH = NULL;//初始化为空指针
zh* KaiHu(char studentID[11], char name[9]);
int ZH(FILE* fp);
int CZ(FILE* fp);

/*开户函数
输入：学号和姓名*/
int ZH(FILE* fp)
{
	char infor[25];
	char studentID[11];
	char name[9];
	int i = 0, k = 0;

	if (!(ALLZH = (zh*)malloc(sizeof(zh)))) {//链表头结点
		printf("分配空间失败！");
		fclose(fp);
		return 2;
	}
	ALLZH->next = NULL;
	while (fgets(infor, 24, fp)) {
		strncpy_s(studentID, 11, infor, 10);//录入学号信息
		for (i = 0, k = 11; infor[k] != ';'; i++, k++)
			name[i] = infor[k];//录入姓名信息
		for (; i < 9; i++)
			name[i] = 0;
		KaiHu(studentID, name);
	}
	return 0;
}

/*发卡函数
输入：存有账户信息的文件名*/
zh* KaiHu(char studentID[11], char name[9])
{

	FILE* fp = NULL;
	int i = 0, k = 0;
	int m = 0;
	int n = 0;
	int flag = 0;
	zh* p = NULL;
	zh* q = NULL;

	if (fopen_s(&fp, "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\record.txt", "a+")) {
		printf("打开操作记录文件失败！");
		return p;
	}
	p = ALLZH;
	q = ALLZH;

	//判断学号的唯一性
	while (q = q->next) {//遍历ALLZH
		if (!(strncmp(studentID, q->studentID, 10))) {
			if (q->status == 1 || q->status == 0) {
				flag++;
				printf("%s%s已开户，不可进行重复开户！\n", q->studentID, q->name);
				fprintf(fp, "%s%s 开户 失败", q->studentID, q->name);
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
			q->data = 20240715;//有效日期
			q->status = 1;//卡状态设为1
			q->next = NULL;
			q->buka = NULL;//补卡信息为空
			q->balance = 0;//卡中余额初始为0
		}
		//将开户操作写入开户记录文件（操作日志)
		if (q->status == 1) {
			//fprintf(fp, "%s%s 开户 成功\n", q->studentID, q->name);
			fprintf(fp, "%s%s 发卡 成功\n", q->studentID, q->name);
			printf("%s %s(%d)开户成功！\n", q->studentID, q->name, q->cardnum);
		}
	}
	fclose(fp);
	return q;
}

/*销户函数
输入：学号*/
int Xiaohu(char studentID[11])
{
	zh* p = ALLZH;//p指向账户信息链表头结点
	char c;
	FILE* fp;
	if (fopen_s(&fp, "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\record.txt", "a+")) {
		printf("打开操作记录文件失败！");
		return 1;
	}
	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			if (p->status == 1 || p->status == 0) {//该账户为正常状态/挂失状态
				printf("%s%s确定对该账户进行销户吗？  [y]es/[n]o\n", p->studentID, p->name);
				getchar();
				if ((c = getchar()) == 'y') {
					p->status = -1;//将该账户的状态标位-1，即为销户
					fprintf(fp, "%s%s 销户 成功", p->studentID, p->name);
				}
				else {
					printf("已取消销户操作！");
					fprintf(fp, "%s%s 销户 失败", p->studentID, p->name);
				}
				break;
			}
			else {//卡状态为0，已经是销户状态
				printf("查询不到该账户信息！");
				break;
			}
		}
	}
	if (p == NULL)
		printf("查询不到该账户信息！");
	fclose(fp);
	return 0;
}

/*挂失/解挂函数
输入：输入学号，系统查询并显示该同学的学号、姓名和卡号，
确认后进行挂失/解挂，即设置当前账户相关校园卡的状态。
操作日志记录操作功能、卡号和主要信息号和是否成功等信息。*/
int Gua_Jie(char studentID[11])
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
				printf("%s %s %d确定对该账户进行挂失吗？  [y]es/[n]o\n", p->studentID, p->name, p->cardnum);
				getchar();
				if ((c = getchar()) == 'y') {
					p->status = 0;//将该账户的状态标位0，即为挂失
					fprintf(fp, "%s%s 挂失 成功", p->studentID, p->name);
					printf("挂失成功！\n");
				}
				else {
					printf("已取消挂失操作！");
					fprintf(fp, "%s%s 挂失 失败", p->studentID, p->name);
				}
				break;
			}
			if (p->status == 0) {//卡状态为0，已经是挂失状态
				printf("%s %s %d确定对该账户进行解挂吗？  [y]es/[n]o\n", p->studentID, p->name, p->cardnum);
				getchar();
				if ((c = getchar()) == 'y') {
					p->status = 1;//将该账户的状态标位1，即为解挂
					fprintf(fp, "%s%s 解挂 成功", p->studentID, p->name);
					printf("解挂成功！\n");
				}
				else {
					printf("已取消解挂操作！");
					fprintf(fp, "%s%s 解挂 失败", p->studentID, p->name);
				}
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
输入学号，系统查询并显示该同学的学号、姓名，
确认后对以前的卡片进行挂失并进行补卡。
操作日志记录操作功能、新卡的卡号和学号、姓名
等主要信息号和是否成功等信息*/
int Buka(char studentID[11])
{
	zh* p = ALLZH;
	zh* q = NULL;
	char c;
	FILE* fp;
	int m = 0;
	if (fopen_s(&fp, "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\record.txt", "a+")) {
		printf("打开操作记录文件失败！\n");
		return 1;
	}
	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			if (p->status >= 0) {//开户状态
				printf("%s %s 确定对以前的卡片进行挂失并进行补卡吗？  [y]es/[n]o\n", p->studentID, p->name);
				getchar();
				if ((c = getchar()) == 'y') {
					q = KaiHu(p->studentID, p->name);
					m = p->cardnum;
					p->cardnum = q->cardnum;
					q->cardnum = m;
					p->status = 1;
					q->status = 0;
					p->buka = q;
					fprintf(fp, "%s%s 补卡 成功", p->studentID, p->name);
					printf("补卡成功！\n");
				}
			}
		}
	}
	if (m == 0)
		printf("查询不到该账户信息！\n");
	fclose(fp);
	return 0;
}

/*充值函数
输入学号，系统查询并显示该同学的学号、姓名，
确认后进行充值，如果充值结果使得余额超过上限，
则本次操作失败，恢复原有的余额。
操作日志记录操作功能、学号、姓名、余额
在充值前后的金额以及充值是否成功等信息。*/
int Chongzhi(char studentID[11], int m)
{
	zh* p = ALLZH;
	char c;
	FILE* fp;
	int n = 0;

	if (fopen_s(&fp, "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\record.txt", "a+")) {
		printf("打开操作记录文件失败！");
		return 1;
	}
	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			if (p->status == 1) {//该账户为正常状态
				printf("%s %s 当前余额：%f\n确定对该账户进行充值吗？  [y]es/[n]o\n", p->studentID, p->name, p->balance);
				getchar();
				if ((c = getchar()) == 'y') {
					if (p->balance + m < 1000) {
						n = p->balance;
						p->balance += m;
						fprintf(fp, "%s%s 充值 成功；充值前：%d  充值后：%d\n", p->studentID, p->name, n, p->balance);
						printf("充值成功！\n");
					}
					else//充值结果余额超过上限
					{
						printf("充值结果余额超过上限，充值失败！\n");
						fprintf(fp, "%s%s 充值 失败；充值前：%d  充值后：%d\n", p->studentID, p->name, p->balance, p->balance);
					}
				}
				else {
					printf("已取消充值！\n");
					fprintf(fp, "%s%s 充值 失败；充值前：%d  充值后：%d\n", p->studentID, p->name, p->balance, p->balance);
				}
			}
			else if (p->status == 0)//校园卡已挂失
				printf("该卡已挂失，不能进行充值!\n");
			else if (p->status == -1 || p == NULL)
				printf("查询不到该账户信息！\n");
		}
	}
	fclose(fp);
	return 0;
}

int CZ(FILE* fp)
{
	char infor[40];
	char studentID[11];
	int i = 0, k = 0;
	float m = 0;
	char n[6];
	while (fgets(infor, 40, fp)) {
		for (i = 22, k = 0; k < 10; i++, k++)
			studentID[k] = infor[i];
		studentID[k] = '\0';
		if (strstr(infor, "挂失") || strstr(infor, "解挂"))
			Gua_Jie(studentID);
		else if (strstr(infor, "充值")) {
			for (k = 0, i++; infor[i] != ';'; i++, k++)
				n[k] = infor[i];//读取充值金额
			n[k] = '\0';
			m = atof(n);
			Chongzhi(studentID, m);
		}
		else if (strstr(infor, "补卡"))
			Buka(studentID);
	}
	return 0;
}