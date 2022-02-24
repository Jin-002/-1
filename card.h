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
	struct zhanghu* buka;//������Ϣ
	float balance;
}zh;

int serialnum = 12345;
zh* ALLZH = NULL;//��ʼ��Ϊ��ָ��
zh* KaiHu(char studentID[11], char name[9]);
int ZH(FILE* fp);
int CZ(FILE* fp);

/*��������
���룺ѧ�ź�����*/
int ZH(FILE* fp)
{
	char infor[25];
	char studentID[11];
	char name[9];
	int i = 0, k = 0;

	if (!(ALLZH = (zh*)malloc(sizeof(zh)))) {//����ͷ���
		printf("����ռ�ʧ�ܣ�");
		fclose(fp);
		return 2;
	}
	ALLZH->next = NULL;
	while (fgets(infor, 24, fp)) {
		strncpy_s(studentID, 11, infor, 10);//¼��ѧ����Ϣ
		for (i = 0, k = 11; infor[k] != ';'; i++, k++)
			name[i] = infor[k];//¼��������Ϣ
		for (; i < 9; i++)
			name[i] = 0;
		KaiHu(studentID, name);
	}
	return 0;
}

/*��������
���룺�����˻���Ϣ���ļ���*/
zh* KaiHu(char studentID[11], char name[9])
{

	FILE* fp = NULL;
	int i = 0, k = 0;
	int m = 0;
	int n = 0;
	int flag = 0;
	zh* p = NULL;
	zh* q = NULL;

	if (fopen_s(&fp, "D:\\CSE\\csp\\����1\\����\\����\\v1\\record.txt", "a+")) {
		printf("�򿪲�����¼�ļ�ʧ�ܣ�");
		return p;
	}
	p = ALLZH;
	q = ALLZH;

	//�ж�ѧ�ŵ�Ψһ��
	while (q = q->next) {//����ALLZH
		if (!(strncmp(studentID, q->studentID, 10))) {
			if (q->status == 1 || q->status == 0) {
				flag++;
				printf("%s%s�ѿ��������ɽ����ظ�������\n", q->studentID, q->name);
				fprintf(fp, "%s%s ���� ʧ��", q->studentID, q->name);
			}
		}
		p = q;
	}
	if (flag == 0) {//ѧ��Ψһ
		q = (zh*)malloc(sizeof(zh));//��̬����洢�ռ�
		if (q != NULL) {
			p->next = q;
			serialnum++;//��ˮ��+1
			strncpy_s(q->studentID, 11, studentID, 10);//¼��ѧ����Ϣ
			strncpy_s(q->name, 9, name, 9);//¼��������Ϣ
			q->cardnum = 3000000 + serialnum * 10;
			n = q->cardnum;
			for (m = 0, i = 0; i < 7; i++) {
				m += n % 10;
				n = n / 10;
			}
			q->cardnum += 9 - m % 10;//¼�뿨����Ϣ
			q->data = 20240715;//��Ч����
			q->status = 1;//��״̬��Ϊ1
			q->next = NULL;
			q->buka = NULL;//������ϢΪ��
			q->balance = 0;//��������ʼΪ0
		}
		//����������д�뿪����¼�ļ���������־)
		if (q->status == 1) {
			//fprintf(fp, "%s%s ���� �ɹ�\n", q->studentID, q->name);
			fprintf(fp, "%s%s ���� �ɹ�\n", q->studentID, q->name);
			printf("%s %s(%d)�����ɹ���\n", q->studentID, q->name, q->cardnum);
		}
	}
	fclose(fp);
	return q;
}

/*��������
���룺ѧ��*/
int Xiaohu(char studentID[11])
{
	zh* p = ALLZH;//pָ���˻���Ϣ����ͷ���
	char c;
	FILE* fp;
	if (fopen_s(&fp, "D:\\CSE\\csp\\����1\\����\\����\\v1\\record.txt", "a+")) {
		printf("�򿪲�����¼�ļ�ʧ�ܣ�");
		return 1;
	}
	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			if (p->status == 1 || p->status == 0) {//���˻�Ϊ����״̬/��ʧ״̬
				printf("%s%sȷ���Ը��˻�����������  [y]es/[n]o\n", p->studentID, p->name);
				getchar();
				if ((c = getchar()) == 'y') {
					p->status = -1;//�����˻���״̬��λ-1����Ϊ����
					fprintf(fp, "%s%s ���� �ɹ�", p->studentID, p->name);
				}
				else {
					printf("��ȡ������������");
					fprintf(fp, "%s%s ���� ʧ��", p->studentID, p->name);
				}
				break;
			}
			else {//��״̬Ϊ0���Ѿ�������״̬
				printf("��ѯ�������˻���Ϣ��");
				break;
			}
		}
	}
	if (p == NULL)
		printf("��ѯ�������˻���Ϣ��");
	fclose(fp);
	return 0;
}

/*��ʧ/��Һ���
���룺����ѧ�ţ�ϵͳ��ѯ����ʾ��ͬѧ��ѧ�š������Ϳ��ţ�
ȷ�Ϻ���й�ʧ/��ң������õ�ǰ�˻����У԰����״̬��
������־��¼�������ܡ����ź���Ҫ��Ϣ�ź��Ƿ�ɹ�����Ϣ��*/
int Gua_Jie(char studentID[11])
{
	zh* p = ALLZH;
	char c;
	FILE* fp;
	if (fopen_s(&fp, "D:\\CSE\\csp\\����1\\����\\����\\v1\\record.txt", "a+")) {
		printf("�򿪲�����¼�ļ�ʧ�ܣ�");
		return 1;
	}
	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			if (p->status == 1) {//���˻�Ϊ����״̬
				printf("%s %s %dȷ���Ը��˻����й�ʧ��  [y]es/[n]o\n", p->studentID, p->name, p->cardnum);
				getchar();
				if ((c = getchar()) == 'y') {
					p->status = 0;//�����˻���״̬��λ0����Ϊ��ʧ
					fprintf(fp, "%s%s ��ʧ �ɹ�", p->studentID, p->name);
					printf("��ʧ�ɹ���\n");
				}
				else {
					printf("��ȡ����ʧ������");
					fprintf(fp, "%s%s ��ʧ ʧ��", p->studentID, p->name);
				}
				break;
			}
			if (p->status == 0) {//��״̬Ϊ0���Ѿ��ǹ�ʧ״̬
				printf("%s %s %dȷ���Ը��˻����н����  [y]es/[n]o\n", p->studentID, p->name, p->cardnum);
				getchar();
				if ((c = getchar()) == 'y') {
					p->status = 1;//�����˻���״̬��λ1����Ϊ���
					fprintf(fp, "%s%s ��� �ɹ�", p->studentID, p->name);
					printf("��ҳɹ���\n");
				}
				else {
					printf("��ȡ����Ҳ�����");
					fprintf(fp, "%s%s ��� ʧ��", p->studentID, p->name);
				}
				break;
			}
			else {
				printf("��ѯ�������˻���Ϣ��\n");
				break;
			}
		}
	}
	if (p == NULL)
		printf("��ѯ�������˻���Ϣ��\n");
	fclose(fp);
	return 0;
}


/*��������
����ѧ�ţ�ϵͳ��ѯ����ʾ��ͬѧ��ѧ�š�������
ȷ�Ϻ����ǰ�Ŀ�Ƭ���й�ʧ�����в�����
������־��¼�������ܡ��¿��Ŀ��ź�ѧ�š�����
����Ҫ��Ϣ�ź��Ƿ�ɹ�����Ϣ*/
int Buka(char studentID[11])
{
	zh* p = ALLZH;
	zh* q = NULL;
	char c;
	FILE* fp;
	int m = 0;
	if (fopen_s(&fp, "D:\\CSE\\csp\\����1\\����\\����\\v1\\record.txt", "a+")) {
		printf("�򿪲�����¼�ļ�ʧ�ܣ�\n");
		return 1;
	}
	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			if (p->status >= 0) {//����״̬
				printf("%s %s ȷ������ǰ�Ŀ�Ƭ���й�ʧ�����в�����  [y]es/[n]o\n", p->studentID, p->name);
				getchar();
				if ((c = getchar()) == 'y') {
					q = KaiHu(p->studentID, p->name);
					m = p->cardnum;
					p->cardnum = q->cardnum;
					q->cardnum = m;
					p->status = 1;
					q->status = 0;
					p->buka = q;
					fprintf(fp, "%s%s ���� �ɹ�", p->studentID, p->name);
					printf("�����ɹ���\n");
				}
			}
		}
	}
	if (m == 0)
		printf("��ѯ�������˻���Ϣ��\n");
	fclose(fp);
	return 0;
}

/*��ֵ����
����ѧ�ţ�ϵͳ��ѯ����ʾ��ͬѧ��ѧ�š�������
ȷ�Ϻ���г�ֵ�������ֵ���ʹ���������ޣ�
�򱾴β���ʧ�ܣ��ָ�ԭ�е���
������־��¼�������ܡ�ѧ�š����������
�ڳ�ֵǰ��Ľ���Լ���ֵ�Ƿ�ɹ�����Ϣ��*/
int Chongzhi(char studentID[11], int m)
{
	zh* p = ALLZH;
	char c;
	FILE* fp;
	int n = 0;

	if (fopen_s(&fp, "D:\\CSE\\csp\\����1\\����\\����\\v1\\record.txt", "a+")) {
		printf("�򿪲�����¼�ļ�ʧ�ܣ�");
		return 1;
	}
	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			if (p->status == 1) {//���˻�Ϊ����״̬
				printf("%s %s ��ǰ��%f\nȷ���Ը��˻����г�ֵ��  [y]es/[n]o\n", p->studentID, p->name, p->balance);
				getchar();
				if ((c = getchar()) == 'y') {
					if (p->balance + m < 1000) {
						n = p->balance;
						p->balance += m;
						fprintf(fp, "%s%s ��ֵ �ɹ�����ֵǰ��%d  ��ֵ��%d\n", p->studentID, p->name, n, p->balance);
						printf("��ֵ�ɹ���\n");
					}
					else//��ֵ�����������
					{
						printf("��ֵ����������ޣ���ֵʧ�ܣ�\n");
						fprintf(fp, "%s%s ��ֵ ʧ�ܣ���ֵǰ��%d  ��ֵ��%d\n", p->studentID, p->name, p->balance, p->balance);
					}
				}
				else {
					printf("��ȡ����ֵ��\n");
					fprintf(fp, "%s%s ��ֵ ʧ�ܣ���ֵǰ��%d  ��ֵ��%d\n", p->studentID, p->name, p->balance, p->balance);
				}
			}
			else if (p->status == 0)//У԰���ѹ�ʧ
				printf("�ÿ��ѹ�ʧ�����ܽ��г�ֵ!\n");
			else if (p->status == -1 || p == NULL)
				printf("��ѯ�������˻���Ϣ��\n");
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
		if (strstr(infor, "��ʧ") || strstr(infor, "���"))
			Gua_Jie(studentID);
		else if (strstr(infor, "��ֵ")) {
			for (k = 0, i++; infor[i] != ';'; i++, k++)
				n[k] = infor[i];//��ȡ��ֵ���
			n[k] = '\0';
			m = atof(n);
			Chongzhi(studentID, m);
		}
		else if (strstr(infor, "����"))
			Buka(studentID);
	}
	return 0;
}