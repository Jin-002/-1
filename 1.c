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
zh* ALLZH = NULL;//��ʼ��Ϊ��ָ��

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
	FILE* fp1 = NULL;
	FILE* fp2 = NULL;
	int i=0, k=0;
	int m = 0;
	int n = 0;
	int flag = 0;
	zh* p = NULL;
	zh* q = NULL;

	if (fopen_s(&fp1, khfile, "r")) {
		printf("���˻���Ϣ�ļ�ʧ�ܣ�"); //��ȡ�˻���Ϣʧ��
		return 1;
	}
	if (fopen_s(&fp2, "D:\\CSE\\csp\\����1\\����\\����\\v1\\kh_record.txt", "a+")) {
		printf("�򿪿�����¼�ļ�ʧ�ܣ�");
		return 1;
	}

	fgets(infor, 24, fp1);//�˻���Ϣ��һ�����ݣ���ZH��
	if (!(ALLZH = (zh*)malloc(sizeof(zh)))) {//����ͷ���
		printf("����ռ�ʧ�ܣ�");
		fclose(fp1);
		return 2;
	}
	ALLZH->next = NULL;
	p = ALLZH;
	q = ALLZH;
	while (fgets(infor, 24, fp1)) {
		//�ж�ѧ�ŵ�Ψһ��
		q = ALLZH;
		while (q = q->next)//����ALLZH
			if (!(strncmp(infor, q->studentID, 10))) {
				flag++;
				if (q->status == 1) {
					printf("%s%s�ѿ��������ɽ����ظ�������",q->studentID,q->name);
					fprintf(fp2, "%s%s ���� ʧ��", q->studentID, q->name);
				}
			}
		if (flag == 0) {
			q = p;
			p = (zh*)malloc(sizeof(zh));//��̬����洢�ռ�
			if (p != NULL) {
				q->next = p;
				serialnum++;//��ˮ��+1
				strncpy_s(p->studentID, 11, infor, 10);//¼��ѧ����Ϣ
				for (i = 0, k = 11; infor[k] != ';'; i++, k++)
					p->name[i] = infor[k];//¼��������Ϣ
				for (; i < 9; i++)
					p->name[i] = 0;
				p->cardnum = 3000000 + serialnum * 10;
				n = p->cardnum;
				for (m = 0, i = 0; i < 7; i++) {
					m += n % 10;
					n = n / 10;
				}
				p->cardnum += 9 - m % 10;//¼�뿨����Ϣ
				p->data = 20240715;//��Ч����
				p->status = 1;//��״̬��Ϊ1
				p->next = NULL;
			}
			//����������д�뿪����¼�ļ���������־)
			if (p->status == 1) {
				fprintf(fp2, "%s%s ���� �ɹ�\n", p->studentID, p->name);
				printf("%s %s(%d)�����ɹ���\n", p->studentID, p->name, p->cardnum);
			}
		}
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}

/*���˿�������
���룺������Ϣ������ѧ�ź�����*/
int KaiHu_2(char studentID[11], char name[9])
{
}