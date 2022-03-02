#include"card.h"


int CX(FILE* fp);
int matchID(void);
int WZ(void);
int sort(void);

int main(void)
{
	FILE* fp = NULL;
	FILE* fp1 = NULL;
	int i = 0;
	int w = 0;//消费窗口号
	int cardnum;
	char infor[4];
	char file[100];
	char c = '\0';
	char studentID[11];
	char name[9];
	float money = 0;
	char time[17];
	for (i = 0; i < 9; i++)
		name[i] = "\0";//name字符串初始化
	if (!(ALLZH = (zh*)malloc(sizeof(zh)))) {//链表头结点
		printf("分配空间失败！");
		return 2;
	}
	ALLZH->next = NULL;
	i = WZ();
	if (i) {//消费记录存储结构初始化
		printf("消费记录存储结构初始化失败！\n");
		return -1;
	}

	printf("1:批量开户    2：批量操作    3：个人操作\n");

	while (c = getchar()) {
		if (c == '1') {//批量开户
			printf("请输入文件名：\n");
			scanf_s("%s", file, 100);
			if (fopen_s(&fp, file, "r")) {//打开账户信息文件
				printf("打开文件失败！");
				return 1;
			}
			fgets(infor, 4, fp);//文件第一行内容
			if (!strncmp(infor, "KH\n", 2))
				KH(fp);
			else
				printf("打开文件错误！\n");
			fclose(fp);
		}

		else if (c == '2') {//批量操作，需要同时读取操作文件和消费文件
			printf("请输入批量操作文件名：");
			scanf_s("%s", file, 100);
			if (fopen_s(&fp, file, "r")) {//打开批量操作文件
				printf("打开文件失败！");
				return 1;
			}
			fgets(infor, 4, fp);//文件第一行内容
			if (strncmp(infor, "CZ\n", 2)) {//判断打开的文件是否正确
				printf("打开文件错误！\n");
				fclose(fp);
				return 1;
			}
			printf("请输入批量消费文件名：");
			scanf_s("%s", file, 100);
			if (fopen_s(&fp1, file, "r")) {//打开批量消费文件
				printf("打开文件失败！");
				return 1;
			}
			fgets(infor, 4, fp1);//文件第一行内容
			if (strncmp(infor, "XF\n", 2)) {//判断打开的文件是否正确
				printf("打开文件错误！\n");
				fclose(fp);
				return 1;
			}
			CZ_XF(fp, fp1);
			sort();
			fclose(fp);
			fclose(fp1);
		}
		else if (c == '3') {//个人操作
			printf("a:开户    b:销户    c:挂失    d:解挂    e:补卡    f:充值    g:消费\n\
				h:查询    i:    j:\n");
			getchar();
			c = getchar();
			if (c == 'a') {//开户
				printf("请输入学号和姓名：");
				scanf_s("%s %s", studentID, 11, name, 9);
				Faka(studentID, name);
			}
			else if (c == 'b') {//销户
				printf("输入日期：\n");
				scanf_s("%s", time, 17);
				printf("请输入要进行销户的学号：");
				scanf_s("%s", studentID, 11);
				Xiaohu(time, studentID, 2);
			}
			else if (c == 'c') {//挂失
				printf("输入日期：\n");
				scanf_s("%s", time, 17);
				printf("请输入要进行挂失的学号：");
				scanf_s("%s", studentID, 11);
				Guashi(time, studentID, 2);
			}
			else if (c == 'd') {//解挂
				printf("输入日期：\n");
				scanf_s("%s",time, 17);
				printf("请输入要进行挂失的学号：");
				scanf_s("%s", studentID, 11);
				Jiegua(time, studentID, 2);
			}
			else if (c == 'e') {//补卡
				printf("输入日期：\n");
				scanf_s("%s",time, 17);
				printf("请输入要补卡的学号：");
				scanf_s("%s", studentID, 11);
				Buka(time, studentID, 2);
			}
			else if (c == 'f') {//充值
				printf("输入日期：\n");
				scanf_s("%s", time, 17);
				printf("请输入要充值的学号和金额：");
				scanf_s("%s %f", studentID, 11, &money);
				Chongzhi(time, studentID, money, 2);
			}
			else if (c == 'g') {//消费
				//打开消费记录操作日志文件
				if (fopen_s(&fp, "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\xfrecord.txt", "a+")) {
					printf("打开消费记录操作日志文件失败！\n");
					return 1;
				}
				printf("输入窗口号：");
				scanf_s("%d", &w);
				printf("日期：");
				scanf_s("%s",time, 17);
				printf("输入卡号和消费金额：");
				scanf_s("%d %f", &cardnum, &money);
				Xiaofei(w,time, money, cardnum, 2,fp);
				fclose(fp);
			}
			else if (c == 'h') {//查询账户信息（支持模糊查询）
				matchID();
			}
			else if (c == 'i')
				;

		}
		printf("1:批量开户    2：批量操作    3：个人操作\n");
		getchar();
	}
		return 0;
}

/*读取并执行查询操作函数*/
int CX(FILE* fp)
{
	char infor[11];
	zh* p = ALLZH;
	infor[10] = '\0';
	while (fgets(infor, 10, fp)) {
		while (p = p->next) {
			if (!strncmp(infor, p->studentID, 10)) 
				break;
		}
		if (p == NULL)
			printf("查询不到该账户信息！\n");
		else if (p->status == 0)
			printf("该卡片已挂失！\n");
		else if (p->status == 1)
			printf("姓名：%s\n学号：%s\n卡号：%d\n当前余额：%.2f\n", p->name, p->studentID, p->cardnum, p->balance);
	}
	return 0;
}

/*姓名字符串模糊匹配
输入：模糊匹配字符串pattern
操作：在所有账户信息中寻找符合pattern字符串格式的账户，输出姓名和学号；
返回值：正确执行完毕后返回0；
*/
int match(void)
{
	zh* p = ALLZH;
	int i = 0;//spos
	int k = 0;//ppos
	int flag = 0;
	int len = 0;
	char pattern[9];
	for (i = 0; i < 9; i++)
		pattern[i] = '\0';
	printf("请输入模式字符串：");
	scanf_s("%s", pattern, 9);
	len = strlen(pattern);
	while (p = p->next)
	{
		for (i = 0, k = 0, flag = 0; p->name[i] != '\0'; ) {
			if (p->name[i] == pattern[k] && p->name[i + 1] == pattern[k + 1]) {
				i += 2;
				k += 2;//比较下一个字
				flag += 2;
			}
			else if (pattern[k] == '?') {
				i += 2;
				k++;
				flag++;
			}
			else if (pattern[k] == '*') {
				while (pattern[k] == '?' || pattern[k] == '*') {
					k++;//找到下一个字
					flag++;
				}
				while (p->name[i] != pattern[k])
					i += 2;
			}
			else
				break;
		}
		if (flag==len&&p->name[i]=='\0')
			printf("%s  %s\n", p->name, p->studentID);
	}
}

/*学号字符串模糊查询账户信息
输入：模糊匹配字符串pattern
操作：在所有账户信息中寻找符合pattern字符串格式的账户，输出账户信息；
返回值：正确执行完毕后返回0；
*/
int matchID(void)
{
	zh* p = ALLZH;
	int i = 0;//spos
	int k = 0;//ppos
	int flag = 0;
	int len = 0;
	char pattern[20];

	for (i = 0; i < 20; i++)
		pattern[i] = '\0';//模式字符串初始化

	printf("请输入模式字符串：");
	scanf_s("%s", pattern, 20);
	len = strlen(pattern);
	while (p = p->next)
	{
		for (i = 0, k = 0, flag = 0; p->studentID[i] != '\0'; ) {
			if (p->studentID[i] == pattern[k]||pattern[k]=='?') {
				i++;
				k++;//比较下一个字符
				flag++;
			}
			else if (pattern[k] == '*') {
				while (pattern[k] == '?' || pattern[k] == '*') {
					k++;//找到下一个非模糊字符
					flag++;
				}
				while (p->studentID[i] != pattern[k])
					i++;
			}
			else
				break;
		}
		if (flag == len && p->studentID[i] == '\0')
			printf("%s %s %d 余额：%.2f\n", p->studentID, p->name,p->cardnum,p->balance);
	}
	return 0;
}

/*消费记录存储结构初始化函数
输入：void;
操作：对消费记录存储结构进行初始化，包括消费窗口数据初始化和消费记录窗口初始化；
返回值：正确执行完毕后返回0；
*/
int WZ(void)
{
	FILE* fp1 = NULL;
	char infor[15];
	int j = 0;
	int i = 0;
	int k = 0;
	char m[8];
	jl* q = NULL;
	jl* r = NULL;

	const char* filename = "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\wz003.txt";
	if (fopen_s(&fp1, filename, "r")) {
		printf("打开消费记录位置文件失败！\n");
		return 1;
	}

	//窗口信息初始化
	fgets(infor, 15, fp1);
	for (i = 1; i < 100; i++) {
		k = 0;
		ALLJL[i].w = i;//窗口号
		ALLJL[i].xftime = 0;//累计消费次数初始化
		ALLJL[i].xfmoney = 0;//累计消费金额初始化
		fgets(infor, 15, fp1);
		while (infor[k] != ',')
			k++;
		for (j = 0, k++; infor[k] != ';'; k++, j++)
			m[j] = infor[k];
		m[j] = '\0';
		ALLJL[i].number = atoi(m);//消费位置初始化
		ALLJL[i].number %= 60000;
		ALLJL[i].next = NULL;

		q = (jl*)malloc(sizeof(jl));
		q->number = 0;
		q->cardnum = 0;
		q->money = 0;
		strncpy_s(q->time, 17, "", 17);
		q->next = NULL;
		ALLJL[i].j = q;

		r = ALLJL[i].j;
		for (k = 1; k < 60000; k++) {//60000条消费记录存储空间初始化
			q = (jl*)malloc(sizeof(jl));
			q->number = k;
			q->cardnum = 0;
			q->money = 0;
			strncpy_s(q->time, 17, "", 17);
			q->next = NULL;
			r->next = q;
			r = r->next;
		}
		r->next = ALLJL[i].j;//链表尾指向链表头，形成循环链表
	}


	for (i = 1; i < 100; i++) {
		r = ALLJL[i].j;
		while (r->number != ALLJL[i].number)
			r = r->next;
		ALLJL[i].j = r;
	}//消费记录位置
	return 0;
}



/*汇总分析函数
输入：void;
操作：对当前各收费窗口的数据进行有序汇总，选择一种
多路归并排序和其他的排序算法进行实现；可以组合时间
段、学号、姓名、消费金额范围进行查询和统计，其中学
号、姓名支持模糊匹配；本项功能需要同时计算汇总排序
所耗费的时间、每次组合查询耗费的时间。
返回值：正确执行完毕后返回0；
*/
int sort(void)
{
	jl* p[100];
	jl* q = NULL;
	int i = 0, flag = 99;
	int k = 0;
	int w = 0;
	FILE* fp=NULL;//汇总排序结果存到文件中
	const char* filename = "D:\\CSE\\csp\\课设1\\数据\\数据\\v1\\sort.txt";
	
	if (fopen_s(&fp, filename, "w"))//打开sort文件
	{
		printf("打开文件失败！\n");
		return 1;
	}
	p[0] = ALLJL[1].j;//初始化

	//找到各条消费记录链表中时间最小的结点
	for (i = 1; i < 100; i++) {
		if (!strlen(ALLJL[i].j->time)) //该窗口没有消费记录
			p[i] = ALLJL[i].j;
		else if (strlen(ALLJL[i].j->next->time))//消费记录链表已循环覆盖
			p[i] = ALLJL[i].j->next;
		else {//消费记录未循环覆盖，找到消费记录起始位置
			p[i] = ALLJL[i].j->next;
			while (!(p[i]->money))
				p[i] = p[i]->next;
		}
	}
	//找到99个窗口的消费记录中时间最早的
	while (flag) {
		strncpy_s(p[0]->time, 17, "2022000000000000", 16);
		for (i = 1,w=0; i < 100; i++) {
			if ((strncmp(p[0]->time, p[i]->time, 16) > 0) && (strlen(p[i]->time))) {
				p[0] = p[i];
				w = i;//记下当前时间醉在的记录的窗口号
			}
		}
		p[w]=p[w]->next;
		fprintf(fp, "%s,%d,%d,%.2f;\n", p[0]->time, p[0]->cardnum,w,p[0]->money);
		for (i = 1, flag = 99; i < 100; i++)
			if (p[i]->money=0||p[i]->number==ALLJL[i].number)
				flag--;//flag记录还有几个窗口中有消费记录
	} 
	fclose(fp);
	return 0;
}