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
	char* khfile = "D:\\CSE\\csp\\����1\\����\\����\\v1\\kh001.txt";
	KaiHu_1(khfile);
	return 0;
}

/*������������
���룺�����˻���Ϣ���ļ���*/
int  KaiHu_1(char* khfile)
{
	char infor[25];
	FILE* fp = NULL;
	int i=0, k=0;
	int m = 0;
	int n = 0;

	if (fopen_s(&fp, khfile, "r")) {
		printf("���˻���Ϣ�ļ�ʧ�ܣ�"); //��ȡ�˻���Ϣʧ��
		return 1;
	}
	fgets(infor, 24, fp);
	//���ѧ�ŵ�Ψһ��

	while (fgets(infor, 24, fp)) {
		i++;//Ҫ��������������ѧ������
		//printf("%s", infor);
	}
	ALLZH = (zhanghu*)malloc( i * sizeof(zhanghu));
	for (k=0; k<i; k++) {
		strcpy_s(ALLZH[k].studentID,10,infor);//¼���˻���Ϣ�е�ѧ��
		for (m = 11; infor[m] != ';'; m++)
			ALLZH[k].name[m - 11] = infor[m];//¼���˻���Ϣ�е�����
		ALLZH[k].cardnum[0] = '3';
		strcat_s(ALLZH[k].cardnum, 5, serialnum);
		for (m = 0; m < 7; m++) 
			n += ALLZH[k].cardnum[m];
		ALLZH[k].cardnum[6] = 9 - (n-6*48) % 10+48;//���㿨�ŵ�У����
		ALLZH[k].data = 20240715;//��Ч����
		printf("%s %s�����ɹ�", ALLZH[k].cardnum, ALLZH[k].name);
	}
	fclose(fp);
	return 0;
}