#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct {
	char studentID[10];
    char cardnum[7];
	char name[12];
	int data;
}zhanghu;

char serialnum[5] = { '1','2','3','4','5' };
zhanghu* ALLZH;

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
	FILE* fp = NULL;
	int i=0, k=0;
	int m = 0;
	int n = 0;

	if (fopen_s(&fp, khfile, "r")) {
		printf("打开账户信息文件失败！"); //获取账户信息失败
		return 1;
	}
	fgets(infor, 24, fp);
	//检查学号的唯一性

	while (fgets(infor, 24, fp)) {
		i++;//要进行批量开户的学生个数
		//printf("%s", infor);
	}
	ALLZH = (zhanghu*)malloc( i * sizeof(zhanghu));
	for (k=0; k<i; k++) {
		strcpy_s(ALLZH[k].studentID,10,infor);//录入账户信息中的学号
		for (m = 11; infor[m] != ';'; m++)
			ALLZH[k].name[m - 11] = infor[m];//录入账户信息中的姓名
		ALLZH[k].cardnum[0] = '3';
		strcat_s(ALLZH[k].cardnum, 5, serialnum);
		for (m = 0; m < 7; m++) 
			n += ALLZH[k].cardnum[m];
		ALLZH[k].cardnum[6] = 9 - (n-6*48) % 10+48;//计算卡号的校验码
		ALLZH[k].data = 20240715;//有效日期
		printf("%s %s开户成功", ALLZH[k].cardnum, ALLZH[k].name);
	}
	fclose(fp);
	return 0;
}