#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct zhanghu{
	char studentID[11];
	int cardnum;
	char name[9];
	int data;
	int status;
	struct zhanghu* next;
}zh;

int serialnum=12345;
zh* ALLZH = NULL;//初始化为空指针

int main(void)
{
	char* khfile = "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\kh001.txt";
	KaiHu_1(khfile);
	return 0;
}

/*批量开户函数
输入：存有账户信息的文件名*/
int  KaiHu_1(char* khfile)
{
	char infor[25];
	FILE* fp1 = NULL;
	FILE* fp2 = NULL;
	int i=0, k=0;
	int m = 0;
	int n = 0;
	int flag = 0;
	zh* p = NULL;
	zh* q = NULL;

	if (fopen_s(&fp1, khfile, "r")) {
		printf("打开账户信息文件失败！"); //获取账户信息失败
		return 1;
	}
	if (fopen_s(&fp2, "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\kh_record.txt", "a+")) {
		printf("打开开户记录文件失败！");
		return 1;
	}

	fgets(infor, 24, fp1);//账户信息第一行内容：“ZH”
	if (!(ALLZH = (zh*)malloc(sizeof(zh)))) {//链表头结点
		printf("分配空间失败！");
		fclose(fp1);
		return 2;
	}
	ALLZH->next = NULL;
	p = ALLZH;
	q = ALLZH;
	while (fgets(infor, 24, fp1)) {
		//判断学号的唯一性
		q = ALLZH;
		while (q = q->next)//遍历ALLZH
			if (!(strncmp(infor, q->studentID, 10))) {
				flag++;
				if (q->status == 1) {
					printf("%s%s已开户，不可进行重复开户！",q->studentID,q->name);
					fprintf(fp2, "%s%s 开户 失败", q->studentID, q->name);
				}
			}
		if (flag == 0) {
			q = p;
			p = (zh*)malloc(sizeof(zh));//动态分配存储空间
			if (p != NULL) {
				q->next = p;
				serialnum++;//流水号+1
				strncpy_s(p->studentID, 11, infor, 10);//录入学号信息
				for (i = 0, k = 11; infor[k] != ';'; i++, k++)
					p->name[i] = infor[k];//录入姓名信息
				for (; i < 9; i++)
					p->name[i] = 0;
				p->cardnum = 3000000 + serialnum * 10;
				n = p->cardnum;
				for (m = 0, i = 0; i < 7; i++) {
					m += n % 10;
					n = n / 10;
				}
				p->cardnum += 9 - m % 10;//录入卡号信息
				p->data = 20240715;//有效日期
				p->status = 1;//卡状态设为1
				p->next = NULL;
			}
			//将开户操作写入开户记录文件（操作日志)
			if (p->status == 1) {
				fprintf(fp2, "%s%s 开户 成功\n", p->studentID, p->name);
				printf("%s %s(%d)开户成功！\n", p->studentID, p->name, p->cardnum);
			}
		}
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}

/*个人开户函数
输入：个人信息，包括学号和姓名*/
int KaiHu_2(char studentID[11], char name[9])
{
}