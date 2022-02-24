#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"card.h"


extern struct zhanghu;
extern zh* ALLZH;

typedef struct jilu {//���Ѽ�¼�ṹ��
	int number;//���Ѽ�¼λ�ú�
    char data[9];//����yyyymmdd
	char time[9];//ʱ��hhmmssff,��ȷ��0.01��
	int cardnum;//����
	float money;//���ѽ��
	struct jilu* next;//��һ�����Ѽ�¼
}jl;

typedef struct {
	int ck;//���ں�
	int number;//���Ѽ�¼λ�ú�
	int xftime;//�ۼ����Ѵ���
	float xfmoney;//�ۼ����ѽ��
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
			printf("���ļ�ʧ�ܣ�");
			return 1;
		}
		fgets(infor, 24, fp);//�ļ���һ������
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

/*��ȡxf�ļ�����*/
int XF(FILE* fp)
{
	int ck = 0;//���ںų�ʼ��Ϊ0
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
	

	char* filename = "D:\\CSE\\csp\\����1\\����\\����\\v1\\wz003.txt";
	if (fopen_s(&fp1, filename, "r")) {
		printf("�����Ѽ�¼λ���ļ�ʧ�ܣ�\n");
		return 1;
	}

	//���Ѽ�¼�洢�ṹ��ʼ��
	fgets(infor, 10, fp1);
	for (i = 1; i < 100; i++) {
		k = 0;
		ALLJL[i].ck = i;//���ں�
		ALLJL[i].xftime = 0;//�ۼ����Ѵ�����ʼ��
		ALLJL[i].xfmoney = 0;//�ۼ����ѽ���ʼ��
		fgets(infor, 10, fp1);
		while (infor[k] != ',')
			k++;
		for (j = 0, k++; infor[k] != ';'; k++, j++)
			m[j] = infor[k];
		m[j] = '\0';
		ALLJL[i].number = atoi(m);//����λ�ó�ʼ��
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

	//��ȡ���Ѽ�¼�ļ�
	while (fgets(infor, 35, fp)) {
		if (infor[0] == 'W') {
			m[0] = infor[1];
			m[1] = infor[2];
			m[2] = '\0';
			ck = atoi(m);//��ȡ���ں���Ϣ
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

/*�������Ѻ���
���룺���ںš�����ʱ�䡢���ѽ�����*/
int Xiaofei(int ck, char* data, char* time, double a,int cardnum)
{
	int i = 0, k = 0;
	int j = 0;
	jl* p=NULL;
	FILE* fp = NULL;
	char* filename = "D:\\CSE\\csp\\����1\\����\\����\\v1\\xfrecord.txt";
	
	a = (float) a;

	//�����Ѽ�¼������־�ļ�
	if (fopen_s(&fp, filename, "a+")) {
		printf("�����Ѽ�¼������־�ļ�ʧ�ܣ�\n");
		return 1;
	}

	//���ݿ����ҵ���У԰���˻�
	
	//�жϸ��˻����������Ƿ񳬹�20Ԫ
	//�жϿ�������Ƿ��㹻


	//�ҵ����Ѵ���ALLJU[i]
	while (i != ck)
		i++;
	p = ALLJL[i].next;

	//�ҵ����Ѽ�¼��λ��
	k = ALLJL[i].number;//���Ѽ�¼��ʼλ��
	k = k % 60000;//ѭ����¼60000��
	while (p->number != k)
		p = p->next;

	//�޸ĸ����Ѽ�¼λ�õ����Ѽ�¼
	strncpy_s(p->data,9,data,8);
	strncpy_s(p->time,9,time,8);
	p->cardnum = cardnum;
	p->money = a;
	ALLJL[i].xftime++;
	ALLJL[i].xfmoney += a;

	//�����Ѽ�¼д�������־
	fprintf(fp, "%s%s ���ţ�%d ���ѽ�%.2f\n",p->data,p->time,p->cardnum,a);

	printf("%d�Ŵ���  �ô����ۻ����Ѵ�����%d  �ô����ۻ����ѽ�%.2f\n",i ,ALLJL[i].xftime, ALLJL[i].xfmoney);
	fclose(fp);
	return 0;
}