#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"card.h"


extern struct zhanghu;
extern zh* ALLZH;

typedef struct jilu {//消费记录结构体
	int number;//消费记录位置号
    char data[9];//日期yyyymmdd
	char time[9];//时间hhmmssff,精确到0.01秒
	int cardnum;//卡号
	float money;//消费金额
	struct jilu* next;//下一条消费记录
}jl;

typedef struct {
	int ck;//窗口号
	int number;//消费记录位置号
	int xftime;//累计消费次数
	float xfmoney;//累计消费金额
	jl* next;
}jlhead;

jlhead ALLJL[100];

int main(void)
{
	FILE* fp = NULL;
	char infor[25];
	char file[100];
	//scanf_s("%s", file);
	while (file != EOF) {
		scanf_s("%s", file, 100);
		if (fopen_s(&fp, file, "r")) {
			printf("打开文件失败！");
			return 1;
		}
		fgets(infor, 24, fp);//文件第一行内容
		if (!strncmp(infor, "KH\n", 2))
			ZH(fp);
		else if (!strncmp(infor, "CZ\n", 2))
			CZ(fp);
		else if (!strncmp(infor, "XF\n", 2))
			XF(fp);
		fclose(fp);
	}
		return 0;
}

/*读取xf文件函数*/
int XF(FILE* fp)
{
	int ck = 0;//窗口号初始化为0
	int i = 0, k = 0;
	int j = 0;
	float a = 0;
	char infor[35];
	char m[8];
	char cardnum[7];
	char data[9];
	char time[9];
	char money[6];
	jlhead* p = NULL;
	jl* q = NULL;
	jl* r = NULL;
	FILE* fp1=NULL;
	

	char* filename = "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\wz003.txt";
	if (fopen_s(&fp1, filename, "r")) {
		printf("打开消费记录位置文件失败！\n");
		return 1;
	}

	//消费记录存储结构初始化
	fgets(infor, 10, fp1);
	for (i = 1; i < 100; i++) {
		k = 0;
		ALLJL[i].ck = i;//窗口号
		ALLJL[i].xftime = 0;//累计消费次数初始化
		ALLJL[i].xfmoney = 0;//累计消费金额初始化
		fgets(infor, 10, fp1);
		while (infor[k] != ',')
			k++;
		for (j = 0, k++; infor[k] != ';'; k++, j++)
			m[j] = infor[k];
		m[j] = '\0';
		ALLJL[i].number = atoi(m);//消费位置初始化
		ALLJL[i].next = NULL;
	}

	for (i = 1; i < 100; i++) {
		q = (jl*)malloc(sizeof(jl));
		q->number = 0;
		q->cardnum = 0;
		strncpy_s(q->data,8, "",8);
		q->money = 0;
		strncpy_s(q->time,8, "", 8);
		q->next = NULL;
		ALLJL[i].next = q;
	}

	for (i = 1; i < 100; i++) {
		r = ALLJL[i].next;
		for (k = 1; k < 60000; k++) {
			q = (jl*)malloc(sizeof(jl));
			q->number = k;
			q->cardnum = 0;
			strncpy_s(q->data,8, "", 8);
			q->money = 0;
			strncpy_s(q->data,8, "", 8);
			q->next = NULL;
			r->next = q;
			r = r->next;
		}
	}

	//读取消费记录文件
	while (fgets(infor, 35, fp)) {
		if (infor[0] == 'W') {
			m[0] = infor[1];
			m[1] = infor[2];
			m[2] = '\0';
			ck = atoi(m);//读取窗口号信息
		}
		else {
			for (i = 0, k = 0; infor[i] != ','; i++, k++)
				cardnum[k] = infor[i];
			i++;
			for (k = 0; infor[i] != ','; i++,k++)
				data[k] = infor[i];
			i++;
			for (k = 0; infor[i] != ','; i++, k++)
				time[k] = infor[i];
			i++;
			for (k = 0; infor[i] != ';'; i++, k++)
				money[k] = infor[i];
			money[k] = '\0';
			a = atof(money);
			Xiaofei(ck, data, time, a, atoi(cardnum));
		}
	}
	return 0;
}

/*窗口消费函数
输入：窗口号、消费时间、消费金额、卡号*/
int Xiaofei(int ck, char* data, char* time, double a,int cardnum)
{
	int i = 0, k = 0;
	int j = 0;
	jl* p=NULL;
	FILE* fp = NULL;
	char* filename = "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\xfrecord.txt";
	
	a = (float) a;

	//打开消费记录操作日志文件
	if (fopen_s(&fp, filename, "a+")) {
		printf("打开消费记录操作日志文件失败！\n");
		return 1;
	}

	//根据卡号找到该校园卡账户
	
	//判断该账户今日消费是否超过20元
	//判断卡中余额是否足够


	//找到消费窗口ALLJU[i]
	while (i != ck)
		i++;
	p = ALLJL[i].next;

	//找到消费记录的位置
	k = ALLJL[i].number;//消费记录初始位置
	k = k % 60000;//循环记录60000条
	while (p->number != k)
		p = p->next;

	//修改该消费记录位置的消费记录
	strncpy_s(p->data,9,data,8);
	strncpy_s(p->time,9,time,8);
	p->cardnum = cardnum;
	p->money = a;
	ALLJL[i].xftime++;
	ALLJL[i].xfmoney += a;

	//将消费记录写入操作日志
	fprintf(fp, "%s%s 卡号：%d 消费金额：%.2f\n",p->data,p->time,p->cardnum,a);

	printf("%d号窗口  该窗口累积消费次数：%d  该窗口累积消费金额：%.2f\n",i ,ALLJL[i].xftime, ALLJL[i].xfmoney);
	fclose(fp);
	return 0;
}