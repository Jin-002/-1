#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"consumption.h"

typedef struct xfreco {
	int w;//���ں�
	char time[17];//ʱ��
	int cardnum;//����
	double  money;//���ѽ��
	struct xfreco* next;//��һ�����Ѽ�¼
}reco;

typedef struct zhanghu {
	char studentID[11];
	int cardnum;
	char name[9];
	char data[9];
	int status;
	struct zhanghu* next;
	struct zhanghu* buka;//������Ϣ
	float balance;
	float accumu[3];//�ۻ����ѽ��
}zh;

//���Ѽ�¼�ṹ��
typedef struct jilu {//���Ѽ�¼�ṹ��
	int number;//���Ѽ�¼λ�ú�
	char time[17];//����yyyymmdd,ʱ��hhmmssff,��ȷ��0.01��
	int cardnum;//����
	float money;//���ѽ��
	struct jilu* next;//��һ�����Ѽ�¼
}jl;

//ʳ�ô��ڽṹ��
typedef struct window {
	int w;//���ں�
	int number;//���Ѽ�¼λ�ú�
	int xftime;//�ۼ����Ѵ���
	float xfmoney;//�ۼ����ѽ��
	jl* j;//�ô������Ѽ�¼
	struct window* next;
}win;

typedef struct xfrecord {
	int w;//���ں�
	char time[17];//ʱ��
	int cardnum;//����
	float money;//���ѽ��
};

win ALLJL[100];
int serialnum = 12345;
zh* ALLZH = NULL;//��ʼ��Ϊ��ָ��
int Buka(char time[17], char studentID[11], int mode);
int sort_1(FILE* fp);


/*��������
���룺ѧ�ţ�����
����������ѧ�ţ�ϵͳ��ѯ����ʾ��ͬѧ��ѧ�ź�������ȷ�Ϻ���з�����
      ��������ˮ�����ε����Ŀ��š�������־��¼����������ѧ�š�������
	  ���ź��Ƿ�ɹ�����Ϣ������Ѿ���������ţ������ڲ������ܡ�
����ֵ����ȷִ����Ϸ���0
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

	//�򿪲�����¼�ļ�
	if (fopen_s(&fp, "D:\\CSE\\csp\\����1\\����\\����\\v1\\record.txt", "a+")) {
		printf("�򿪲�����¼�ļ�ʧ�ܣ�");
		return 1;
	}

	p = ALLZH;
	q = ALLZH;

	//�ж�ѧ�ŵ�Ψһ��
	while (q = q->next) {//����ALLZH
		if (!(strncmp(studentID, q->studentID, 10))) {
			if (q->status == 1 || q->status == 0) {
				flag++;
				printf("%s%s�ѷ�������ţ��Ƿ�Ҫ���в�����    [y]es/[n]o\n", q->studentID, q->name);
				getchar();
				c = getchar();
				if (c == 'y') {
					printf("�������ڣ�\n");
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
			strncpy_s(q->data, 9, "20240715\0", 9);//��Ч����
			q->status = 1;//��״̬��Ϊ1
			q->next = NULL;
			q->buka = NULL;//������ϢΪ��
			q->balance = 0;//��������ʼΪ0
			q->accumu[0] = 0;//7-9�㿨Ƭ�ۻ����ѽ���ʼ��Ϊ0
			q->accumu[1] = 0;//11-13�㿨Ƭ�ۻ����ѽ���ʼ��Ϊ0
			q->accumu[2] = 0;//17-19�㿨Ƭ�ۻ����ѽ���ʼ��Ϊ0
		}
		//����������д�뿪����¼�ļ���������־)
		if (q->status == 1) {
			fprintf(fp, "2021090100000000,%s%s,�����ɹ� ���ţ�%d\n", q->studentID, q->name,q->cardnum);
			printf("%s %s(%d)���������ɹ���\n", q->studentID, q->name, q->cardnum);
		}
	}
	fclose(fp);
	return 0;
}

/*���������ļ�������
���룺ָ�����������ļ���ָ��FILE* fp��
���������ж�ȡ�ļ����ݣ������п�������������
���أ���ȷִ����Ϸ���0��
*/
int KH(FILE* fp)
{
	char infor[25];
	char studentID[11];
	char name[9];
	int i = 0, k = 0;

	
	while (fgets(infor, 24, fp)) {
		strncpy_s(studentID, 11, infor, 10);//¼��ѧ����Ϣ
		for (i = 0, k = 11; infor[k] != ';'; i++, k++)
			name[i] = infor[k];//¼��������Ϣ
		for (; i < 9; i++)
			name[i] = 0;
		Faka(studentID, name);
	}
	return 0;
}


/*��������
���룺ѧ��studentID,ʱ��time
      ����ģʽmode��mode=1��������������mode=2���и��˲���
����������ѧ�ţ�ϵͳ��ѯ����ʾ��ͬѧ��ѧ�ź�������ȷ�Ϻ����������
      �Ѿ���������Ϣ�����ܹ���ѯ��������־��¼������ѧ���������Լ�
	  �����Ƿ�ɹ�����Ϣ��
����ֵ����ȷִ����Ϻ󷵻�0��
*/
int Xiaohu(char time[17],char studentID[11],int mode)
{
	zh* p = ALLZH;//pָ���˻���Ϣ����ͷ���
	char c;
	FILE* fp;

	//�򿪲�����¼�ļ�
	if (fopen_s(&fp, "D:\\CSE\\csp\\����1\\����\\����\\v1\\record.txt", "a+")) {
		printf("�򿪲�����¼�ļ�ʧ�ܣ�");
		return 1;
	}

	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			if (p->status == 1 || p->status == 0) {//���˻�Ϊ����״̬/��ʧ״̬
				if (mode == 2) {//���˲���
					printf("%s%sȷ���Ը��˻�����������  [y]es/[n]o\n", p->studentID, p->name);
					getchar();
					if ((c = getchar()) == 'y') {
						p->status = -1;//�����˻���״̬��λ-1����Ϊ����
						fprintf(fp, "%s,%s%s,�����ɹ�\n",time, p->studentID, p->name);
						printf("�����ɹ���\n");
					}
					else {
						printf("��ȡ������������\n");
						fprintf(fp, "%s,%s%s,����ʧ��\n", time,p->studentID, p->name);
					}
					break;
				}
				if (mode == 1) {//��������
					p->status = -1;
					fprintf(fp, "%s,%s%s,�����ɹ�\n", time,p->studentID, p->name);
					printf("�����ɹ���\n");
					break;
				}
			}
			else {//��״̬Ϊ-1���Ѿ�������״̬
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

/*��ʧ����
���룺ѧ��studentID,ʱ��time��
����������ѧ�ţ�ϵͳ��ѯ����ʾ��ͬѧ��ѧ�š������Ϳ��ţ�
      ȷ�Ϻ���й�ʧ�������õ�ǰ�˻����У԰����״̬��
      ������־��¼�������ܡ����ź���Ҫ��Ϣ�ź��Ƿ�ɹ�����Ϣ��
����ֵ����ȷִ����Ϻ󷵻�0��
*/
int Guashi(char time[17],char studentID[11],int mode)
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
				if (mode == 2) {//���˲���
					printf("%s %s %dȷ���Ը��˻����й�ʧ��  [y]es/[n]o\n", p->studentID, p->name, p->cardnum);
					getchar();
					if ((c = getchar()) == 'y') {
						p->status = 0;//�����˻���״̬��λ0����Ϊ��ʧ
						fprintf(fp, "%s,%s%s,��ʧ�ɹ�\n",time, p->studentID, p->name);
						printf("%s%s��ʧ�ɹ���\n", p->studentID, p->name);
					}
					else {
						printf("��ȡ����ʧ������\n");
						fprintf(fp, "%s,%s%s,��ʧʧ��\n",time, p->studentID, p->name);
					}
					break;
				}
				if (mode == 1) {//��������
					p->status = 0;
					fprintf(fp, "%s,%s%s,��ʧ�ɹ�\n", time, p->studentID, p->name);
					printf("%s%s��ʧ�ɹ���\n",p->studentID,p->name);
					break;
				}
			}
			else if (p->status == 0) {//��״̬Ϊ0���Ѿ��ǹ�ʧ״̬
				printf("%s%s�ÿ��Ѵ��ڹ�ʧ״̬!\n", p->studentID, p->name);
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

/*��Һ���
���룺ѧ��studentID,ʱ��time��
����������ѧ�ţ�ϵͳ��ѯ����ʾ��ͬѧ��ѧ�š������Ϳ��ţ�
	  ȷ�Ϻ���н�ң������õ�ǰ�˻����У԰����״̬��
	  ������־��¼�������ܡ����ź���Ҫ��Ϣ�ź��Ƿ�ɹ�����Ϣ��
����ֵ����ȷִ����Ϻ󷵻�0��
*/
int Jiegua(char time[17], char studentID[11], int mode)
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
			if (p->status == 0) {//���˻�Ϊ��ʧ״̬
				if (mode == 2) {//���˲���
					printf("%s %s %dȷ���Ը��˻����н����  [y]es/[n]o\n", p->studentID, p->name, p->cardnum);
					getchar();
					if ((c = getchar()) == 'y') {
						p->status = 1;//�����˻���״̬��λ1����Ϊ���
						fprintf(fp, "%s,%s%s,��� �ɹ�\n",time, p->studentID, p->name);
						printf("%s%s��ҳɹ���\n",p->studentID,p->name);
					}
					else {
						printf("��ȡ����Ҳ�����\n");
						fprintf(fp, "%s,%s%s,���ʧ��\n", time, p->studentID, p->name);
					}
					break;
				}
				if (mode == 1) {//��������
					p->status = 0;
					fprintf(fp, "%s,%s%s,��ҳɹ�\n", time, p->studentID, p->name);
					printf("%s%s��ҳɹ���\n",p->studentID,p->name);
					break;
				}
			}
			else if (p->status == 1) {//��״̬Ϊ1���Ѿ�������״̬
				printf("%s%s�ÿ��Ѵ�������״̬��\n", p->studentID, p->name);
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
���룺ѧ��studentID,ʱ��time,ģʽmode
����������ѧ�ţ�ϵͳ��ѯ����ʾ��ͬѧ��ѧ�š�������
      ȷ�Ϻ����ǰ�Ŀ�Ƭ���й�ʧ�����в�����������
	  ־��¼�������ܡ��¿��Ŀ��ź�ѧ�š���������Ҫ
	  ��Ϣ�ź��Ƿ�ɹ�����Ϣ.
����ֵ:��ȷִ����Ϻ󷵻�0;
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
	if (fopen_s(&fp, "D:\\CSE\\csp\\����1\\����\\����\\v1\\record.txt", "a+")) {
						printf("�򿪲�����¼�ļ�ʧ�ܣ�\n");
						return 1;
					}
	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			i = 0;
			r = p;
			while (r=r->buka)
				i++;//��¼�����Ĵ���

			if (i <= 100) {
				if (p->status >= 0) {//����״̬
					if (mode == 2) {//���˲���
						printf("%s %s ȷ������ǰ�Ŀ�Ƭ���й�ʧ�����в�����  [y]es/[n]o\n", p->studentID, p->name);
						getchar();
						if ((c = getchar()) == 'y') {
							q = (zh*)malloc(sizeof(zh));//��̬����洢�ռ�
							if (q != NULL) {
								serialnum++;//��ˮ��+1
								strncpy_s(q->studentID, 11, p->studentID, 10);//¼��ѧ����Ϣ
								strncpy_s(q->name, 9, p->name, 9);//¼��������Ϣ
								q->cardnum = 3000000 + serialnum * 10;
								n = q->cardnum;
								for (m = 0, i = 0; i < 7; i++) {
									m += n % 10;
									n = n / 10;
								}
								q->cardnum += 9 - m % 10;//¼�뿨����Ϣ
								strncpy_s(q->data, 9, "20240715\0", 9);//��Ч����
								q->status = 1;//��״̬��Ϊ1
								q->next = NULL;
								q->buka = NULL;//������ϢΪ��
								q->balance = p->balance;//��������ʼΪ0
								q->accumu[0] = p->accumu[0];//7-9�㿨Ƭ�ۻ����ѽ���ʼ��Ϊ0
								q->accumu[1] = p->accumu[1];//11-13�㿨Ƭ�ۻ����ѽ���ʼ��Ϊ0
								q->accumu[2] = p->accumu[2];//17-19�㿨Ƭ�ۻ����ѽ���ʼ��Ϊ0
							}
							m = p->cardnum;
							p->cardnum = q->cardnum;
							q->cardnum = m;
							p->status = 1;
							q->status = 0;
							q->buka = p->buka;
							p->buka = q;
							fprintf(fp, "%s,%s%s,�����ɹ� ���ţ�%d\n", time, p->studentID, p->name, p->cardnum);
							printf("�����ɹ���  ���ţ�%d  ��ǰ��%.2f\n", p->cardnum, p->balance);
						}
						else
							printf("��ȡ��������\n");
						break;
					}
					if (mode == 1) {//��������
						q = (zh*)malloc(sizeof(zh));//��̬����洢�ռ�
						if (q != NULL) {
							serialnum++;//��ˮ��+1
							strncpy_s(q->studentID, 11, p->studentID, 10);//¼��ѧ����Ϣ
							strncpy_s(q->name, 9, p->name, 9);//¼��������Ϣ
							q->cardnum = 3000000 + serialnum * 10;
							n = q->cardnum;
							for (m = 0, i = 0; i < 7; i++) {
								m += n % 10;
								n = n / 10;
							}
							q->cardnum += 9 - m % 10;//¼�뿨����Ϣ
							strncpy_s(q->data, 9, "20240715\0", 9);//��Ч����
							q->status = 1;//��״̬��Ϊ1
							q->next = NULL;
							q->buka = NULL;//������ϢΪ��
							q->balance = p->balance;//��������ʼΪ0
							q->accumu[0] = p->accumu[0];//7-9�㿨Ƭ�ۻ����ѽ���ʼ��Ϊ0
							q->accumu[1] = p->accumu[1];//11-13�㿨Ƭ�ۻ����ѽ���ʼ��Ϊ0
							q->accumu[2] = p->accumu[2];//17-19�㿨Ƭ�ۻ����ѽ���ʼ��Ϊ0
						}
						m = p->cardnum;
						p->cardnum = q->cardnum;
						q->cardnum = m;
						p->status = 1;
						q->status = 0;
						q->buka = p->buka;
						p->buka = q;
						fprintf(fp, "%s,%s%s,�����ɹ�\n", time, p->studentID, p->name);
						printf("�����ɹ���  ���ţ�%d  ��ǰ��%.2f\n", p->cardnum, p->balance);
						break;
					}
				}
			}
			else {
				printf("���������Ѵ����ޣ�\n");
				break;
			}
		}
	}
	if (m == 0)
		printf("��ѯ�������˻���Ϣ��\n");
	fclose(fp);
	return 0;
}

/*��ֵ����
���룺ѧ��studentID,��ֵ���m,ʱ��time,ģʽmode
����������ѧ�ţ�ϵͳ��ѯ����ʾ��ͬѧ��ѧ�š�������ȷ�Ϻ����
      ��ֵ�������ֵ���ʹ���������ޣ��򱾴β���ʧ�ܣ���
	  ��ԭ�е���������־��¼�������ܡ�ѧ�š������������
	  ��ֵǰ��Ľ���Լ���ֵ�Ƿ�ɹ�����Ϣ��
����ֵ����ȷִ����Ϻ󷵻�0��
*/
int Chongzhi(char time[17],char studentID[11], float m,int mode)
{
	zh* p = ALLZH;
	char c;
	FILE* fp;
	float n = 0;

	if (fopen_s(&fp, "D:\\CSE\\csp\\����1\\����\\����\\v1\\record.txt", "a+")) {
		printf("�򿪲�����¼�ļ�ʧ�ܣ�");
		return 1;
	}
	while (p = p->next)
	{
		if (!(strncmp(p->studentID, studentID, 10))) {
			if (p->status >=0) {//���˻�Ϊ����״̬
				if (mode == 2) {//���˲���
					printf("%s %s ��ǰ��%f\nȷ���Ը��˻����г�ֵ��  [y]es/[n]o\n", p->studentID, p->name, p->balance);
					getchar();
					if ((c = getchar()) == 'y') {
						if (p->balance + m < 1000) {
							n = p->balance;
							p->balance += m;
							fprintf(fp, "%s,%s%s,��ֵ�ɹ�  ���ţ�%d  ��ֵǰ��%.2f  ��ֵ��%.2f\n",time, p->studentID, p->name, p->cardnum,n, p->balance);
							printf("%s%s��ֵ�ɹ�����ֵǰ��%.2f  ��ֵ��%.2f\n", p->studentID, p->name, n, p->balance);
						}
						else//��ֵ�����������
						{
							printf("��ֵ����������ޣ���ֵʧ�ܣ�   ��ǰ��%.2f\n", p->balance);
							fprintf(fp, "%s,%s%s,��ֵʧ��  ���ţ�%d  ��ֵǰ��%.2f  ��ֵ��%.2f\n",time, p->studentID, p->name,p->cardnum, p->balance, p->balance);
						}
					}
					else {
						printf("��ȡ����ֵ��\n");
						fprintf(fp, "%s,%s%s,��ֵʧ��  ���ţ�%d  ��ֵǰ��%.2f  ��ֵ��%.2f\n",time, p->studentID, p->name,p->cardnum, p->balance, p->balance);
					}
				}
				if (mode == 1) {//������ֵ
					if (p->balance + m < 1000) {
						n = p->balance;
						p->balance += m;
						fprintf(fp, "%s,%s%s,��ֵ�ɹ�  ���ţ�%d  ��ֵǰ��%.2f  ��ֵ��%.2f\n",time, p->studentID, p->name,p->cardnum, n, p->balance);
						printf("%s%s��ֵ�ɹ��� ��ֵǰ��%.2f  ��ֵ��%.2f\n", p->studentID, p->name, n, p->balance);
					}
					else//��ֵ�����������
					{
						printf("��ֵ����������ޣ���ֵʧ�ܣ�   ��ǰ��%.2f\n", p->balance);
						fprintf(fp, "%s,%s%s,��ֵʧ��  ���ţ�%d  ��ֵǰ��%.2f  ��ֵ��%.2f\n",time, p->studentID, p->name, p->cardnum,p->balance, p->balance);
					}
				}
				break;
			}
		}
	}
	if (p == NULL)
		printf("��ѯ�����˻���Ϣ��\n");
	fclose(fp);
	return 0;
}


/*�������Ѻ���
���룺���ں�w������ʱ��time�����ѽ��a������cardnum��ģʽmode��ָ�����Ѽ�¼�ļ���ָ��fp
��������ȷ���ĳ���ʱ���ʳ�ô��ڳ����£��������ѵĿ��ź����ѽ�
	  ϵͳ����У԰���Ƿ������Ŀ����������Ƿ��㹻��Ȼ�����ʵ��
	  �����ۼ�������������ѳɹ��������ɲ��������������Ϣ�����
	  �Ѽ�¼������ʾ��ǰ�ô����ڵ�����ۼ����Ѵ��������ѽ�����
	  ��־��¼�������Ѳ��������š����ı仯�������Ƿ�ɹ�����Ϣ��
����ֵ����ȷִ����Ϻ󷵻�0��
*/
int Xiaofei(int w, char* time, double a, int cardnum, int mode,FILE* fp)
{
	int i = 0, k = 0;
	int j = 0;
	float accumu = 0;//�����ۻ����ѽ��
	char password[5];
	char t[8]="";
	jl* p = NULL;
	zh* q = ALLZH;
	

	a = (float)a;
	for (i = 0; i < 8; i++) 
		t[i] = time[i + 8];


	//���ݿ����ҵ���У԰���˻�
	while (q = q->next)
		if (q->cardnum == cardnum && q->status >= 0)
			break;
	if (q == NULL) {
		printf("��ѯ�������˻���Ϣ��\n");
		return -1;
	}
	//�Ը����ۻ����ѽ����и���
	

	//�жϿ��Ƿ��ܽ�������
	if (q->balance < a)//��������
		printf("�������㣡  ��ǰ��%.2f\n", q->balance);
	else if (q->status == 0)//��Ƭ�ѹ�ʧ
		printf("�ÿ��ѹ�ʧ���޷��������ѣ�\n");
	else {//����������

		//�жϸ�ʱ��������Ƿ񳬹�20Ԫ
		j = atoi(t);
		if (j >= 7000000 && j < 9000000) {
			if (q->accumu[0] + a >= 20) {//�ۻ����ѽ���20Ԫ����Ҫ��������
				if (mode == 2) {//���˲���
					printf("���������룺");
					scanf_s("%s", password, 5);
					if (strncmp(password, "8888", 4)) {
						printf("�������\n");
						fclose(fp);
						return 0;
					}
					else q->accumu[0] = 0;//����������ȷ
				}
				if (mode == 1)//����������Ĭ������������ȷ
					q->accumu[0] = 0;
			}
			q->balance -= a;//���˻�����п۳����ѽ��
			q->accumu[0] += a;//�ۻ����ѽ������
		}
		else if (j >= 11000000 && j < 13000000) {
			if (q->accumu[1] >= 20) {//�ۻ����ѽ���20Ԫ����Ҫ��������
				if (mode == 2) {//���˲���
					printf("���������룺");
					scanf_s("%s", password, 5);
					if (strncmp(password, "8888", 4)) {
						printf("�������\n");
						fclose(fp);
						return 0;
					}
					else q->accumu[1] = 0;//����������ȷ
				}
				if (mode == 1)//����������Ĭ������������ȷ
					q->accumu[1] = 0;
			}
			q->balance -= a;//���˻�����п۳����ѽ��
			q->accumu[1] += a;//�ۻ����ѽ������
		}
		else if (j >= 17000000 && j < 19000000) {
			if (q->accumu[2] >= 20) {//�ۻ����ѽ���20Ԫ����Ҫ��������
				if (mode == 2) {//���˲���
					printf("���������룺");
					scanf_s("%s", password);
					if (strncmp(password, "8888", 4)) {
						printf("�������\n");
						fclose(fp);
						return 0;
					}
					else q->accumu[2] = 0;//����������ȷ
				}
				if (mode == 1)//����������Ĭ������������ȷ
					q->accumu[2] = 0;
			}
			q->balance -= a;//���˻�����п۳����ѽ��
			q->accumu[2] += a;//�ۻ����ѽ������
		}

		p = ALLJL[w].j;//�ҵ���ǰ���Ѽ�¼λ��

		//�Ըô��ڵ��ۻ����Ѵ����ͽ����и���
		if (strncmp(p->time, time, 8)) {
			ALLJL[w].xftime = 0;
			ALLJL[w].xfmoney = 0;
		}
		p = p->next;
		//�޸ĸ����Ѽ�¼λ�õ����Ѽ�¼
		strncpy_s(p->time, 17, time, 17);
		p->cardnum = cardnum;
		p->money = a;
		ALLJL[w].xftime++;
		ALLJL[w].xfmoney += a;
		ALLJL[w].number++;
		ALLJL[w].j = p;

		//�����Ѽ�¼д�������־
		fprintf(fp, "%s ѧ�ţ�%s ���ţ�%d ���ѽ�%.2f  ������%.2f\n", p->time, q->studentID, p->cardnum, a, q->balance);
		//printf("%d�Ŵ���  �ô����ۻ����Ѵ�����%d  �ô����ۻ����ѽ�%.2f  �������Ѽ�¼λ�ã�%d %d\n", w, ALLJL[w].xftime, ALLJL[w].xfmoney,p->number,ALLJL[w].number);
	}
	return 0;
}

/*���������ļ�������
���룺ָ�����������ļ���ָ��fp1,ָ�����������ļ���ָ��fp2;
��������ʱ���Ⱥ�˳��ִ�п����������ѣ�
����ֵ����ȷִ����Ϻ󷵻�0��
*/
int CZ_XF(FILE* fp1,FILE* fp2)
{
	int i = 0, k = 0;
	int w = 0;//�ݴ洰�ںų�ʼ��Ϊ0
	float money = 0;//�ݴ����ѽ��
	char infor1[40]="";//������Ϣ
	char infor2[40]="";//������Ϣ
	char studentID[11];//�ݴ�ѧ��
	char time1[17];//�ݴ�ʱ��
	char time2[17];//�ݴ�ʱ��
	char cardnum[8];//�ݴ濨��
	char m[6];
	char* tag1;
	char* tag2;
	jl* q = NULL;
	jl* r = NULL;
	FILE* fp = NULL;
	const char* filename = "D:\\CSE\\csp\\����1\\����\\����\\v1\\xfrecord.txt";

	sort_1(fp2);
	fclose(fp2);
	if (fopen_s(&fp2, "D:\\CSE\\csp\\����1\\����\\����\\v1\\sort_1.txt", "r")) {
		printf("���ļ�ʧ�ܣ�\n");
		return 1;
	}//�����������������ļ�

	//�����Ѽ�¼������־�ļ�
	if (fopen_s(&fp, filename, "a+")) {
		printf("�����Ѽ�¼������־�ļ�ʧ�ܣ�\n");
		return 1;
	}

	//9��23��ǰ��Ƭ����
	while (tag1 = fgets(infor1, 40, fp1)) {
		for (i = 0, k = 0; i < 16; i++, k++)
			time1[k] = infor1[i];//ʱ��time1
		time1[16] = '\0';
		if (strncmp(time1, "2021090700000000", 16)<0) {
			for (i = 22, k = 0; k < 10; i++, k++)
				studentID[k] = infor1[i];//ѧ��
			studentID[k] = '\0';
			if (strstr(infor1, "��ʧ"))
				Guashi(time1, studentID, 1);
			else if (strstr(infor1, "���"))
				Jiegua(time1, studentID, 1);
			else if (strstr(infor1, "��ֵ")) {
				for (k = 0, i++; infor1[i] != ';'; i++, k++)
					m[k] = infor1[i];//��ȡ��ֵ���
				m[k] = '\0';
				money = atof(m);
				Chongzhi(time1, studentID, money, 1);
			}
			else if (strstr(infor1, "����"))
				Buka(time1, studentID, 1);
			else if (strstr(infor1, "����"))
				Xiaohu(time1, studentID, 1);
		}
		else break;
	}
	//9��23��-10��13����������
	while (fgets(infor2, 40, fp2)) {
		for (i = 0, k = 0; infor2[i] != ',';k++, i++)
			time2[k] = infor2[i];
		time2[16] = '\0';//ʱ��
		if ((strncmp(time2, "2021092300000000", 16) > 0)\
			&& (strncmp(time2, "2021101400000000", 16) < 0)) {
			for (k = 0, i++; infor2[i] != ','; k++, i++)
				cardnum[k] = infor2[i];//����
			cardnum[7] = '\0';
			i++;
			for (k = 0; infor2[i] != ','; k++, i++)
				m[k] = infor2[i];
			m[k] = '\0';
			w = atoi(m);//���ں�
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
	//��2�γ�ֵ
	for (i = 0, k = 0; i < 16; i++, k++)
		time1[k] = infor1[i];//ʱ��time1
	time1[16] = '\0';
	for (i = 22, k = 0; k < 10; i++, k++)
		studentID[k] = infor1[i];//ѧ��
	studentID[k] = '\0';
	for (k = 0, i++; infor1[i] != ';'; i++, k++)
		m[k] = infor1[i];//��ȡ��ֵ���
	m[k] = '\0';
	money = atof(m);
	Chongzhi(time1, studentID, money, 1);
	while (tag1 = fgets(infor1, 40, fp1)) {
		for (i = 0, k = 0; i < 16; i++, k++)
			time1[k] = infor1[i];//ʱ��time1
		time1[16] = '\0';
		if (strncmp(time1, "20211014", 8) == 0) {
			for (i = 22, k = 0; k < 10; i++, k++)
				studentID[k] = infor1[i];//ѧ��
			studentID[k] = '\0';
			for (k = 0, i++; infor1[i] != ';'; i++, k++)
				m[k] = infor1[i];//��ȡ��ֵ���
			m[k] = '\0';
			money = atof(m);
			Chongzhi(time1, studentID, money, 1);
		}
		else break;
	}

	//10��14�յ�11��3������
	for (i = 0, k = 0; infor2[i] != ','; k++, i++)
		time2[k] = infor2[i];
	time2[16] = '\0';//ʱ��
	if ((strncmp(time2, "2021101400000000", 16) > 0)\
		&& (strncmp(time2, "2021110400000000", 16) < 0)) {
		for (k = 0, i++; infor2[i] != ','; k++, i++)
			cardnum[k] = infor2[i];//����
		cardnum[7] = '\0';
		i++;
		for (k = 0; infor2[i] != ','; k++,i++)
			m[k] = infor2[i];
		m[k] = '\0';
		w = atoi(m);//���ں�
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
		time2[16] = '\0';//ʱ��
		if ((strncmp(time2, "2021101400000000", 16) > 0)\
			&& (strncmp(time2, "2021110400000000", 16) < 0)) {
			for (k = 0, i++; infor2[i] != ','; k++, i++)
				cardnum[k] = infor2[i];//����
			cardnum[7] = '\0';
			i++;
			for (k = 0; infor2[i] != ','; k++, i++)
				m[k] = infor2[i];
			m[k] = '\0';
			w = atoi(m);//���ں�
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
	//11��4�ճ�ֵ����3�γ�ֵ��
	for (i = 0, k = 0; i < 16; i++, k++)
		time1[k] = infor1[i];//ʱ��time1
	time1[16] = '\0';
	for (i = 22, k = 0; k < 10; i++, k++)
		studentID[k] = infor1[i];//ѧ��
	studentID[k] = '\0';
	for (k = 0, i++; infor1[i] != ';'; i++, k++)
		m[k] = infor1[i];//��ȡ��ֵ���
	m[k] = '\0';
	money = atof(m);
	Chongzhi(time1, studentID, money, 1);
	while (tag1 = fgets(infor1, 40, fp1)) {
		for (i = 0, k = 0; i < 16; i++, k++)
			time1[k] = infor1[i];//ʱ��time1
		time1[16] = '\0';
		if (strncmp(time1, "20211104", 8) == 0) {
			for (i = 22, k = 0; k < 10; i++, k++)
				studentID[k] = infor1[i];//ѧ��
			studentID[k] = '\0';
			for (k = 0, i++; infor1[i] != ';'; i++, k++)
				m[k] = infor1[i];//��ȡ��ֵ���
			m[k] = '\0';
			money = atof(m);
			Chongzhi(time1, studentID, money, 1);
		}
		else break;
	}
	;
	//11��4�յ�11��24������
	for (i = 0, k = 0; infor2[i] != ','; k++, i++)
		time2[k] = infor2[i];
	time2[16] = '\0';//ʱ��
	if ((strncmp(time2, "2021110400000000", 16) > 0)\
		&& (strncmp(time2, "2021112500000000", 16) < 0)) {
		for (k = 0, i++; infor2[i] != ','; k++, i++)
			cardnum[k] = infor2[i];//����
		cardnum[7] = '\0';
		i++;
		for (k = 0; infor2[i] != ','; k++,i++)
			m[k] = infor2[i];
		m[k] = '\0';
		w = atoi(m);//���ں�
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
		time2[16] = '\0';//ʱ��
		if ((strncmp(time2, "2021110400000000", 16) > 0)\
			&& (strncmp(time2, "2021112500000000", 16) < 0)) {
			for (k = 0, i++; infor2[i] != ','; k++, i++)
				cardnum[k] = infor2[i];//����
			cardnum[7] = '\0';
			i++;
			for (k = 0; infor2[i] != ','; k++, i++)
				m[k] = infor2[i];
			m[k] = '\0';
			w = atoi(m);//���ں�
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
	//11��25�ճ�ֵ����4�γ�ֵ��
	for (i = 0, k = 0; i < 16; i++, k++)
		time1[k] = infor1[i];//ʱ��time1
	time1[16] = '\0';
	for (i = 22, k = 0; k < 10; i++, k++)
		studentID[k] = infor1[i];//ѧ��
	studentID[k] = '\0';
	for (k = 0, i++; infor1[i] != ';'; i++, k++)
		m[k] = infor1[i];//��ȡ��ֵ���
	m[k] = '\0';
	money = atof(m);
	Chongzhi(time1, studentID, money, 1);
	while (tag1 = fgets(infor1, 40, fp1)) {
		for (i = 0, k = 0; i < 16; i++, k++)
			time1[k] = infor1[i];//ʱ��time1
		time1[16] = '\0';
		if (strncmp(time1, "20211125", 8) == 0) {
			for (i = 22, k = 0; k < 10; i++, k++)
				studentID[k] = infor1[i];//ѧ��
			studentID[k] = '\0';
			for (k = 0, i++; infor1[i] != ';'; i++, k++)
				m[k] = infor1[i];//��ȡ��ֵ���
			m[k] = '\0';
			money = atof(m);
			Chongzhi(time1, studentID, money, 1);
		}
		else break;
	}
	;
	//11��25�յ�12��15������
	for (i = 0, k = 0; infor2[i] != ','; k++, i++)
		time2[k] = infor2[i];
	time2[16] = '\0';//ʱ��
	if ((strncmp(time2, "2021112500000000", 16) > 0)\
		&& (strncmp(time2, "2021121600000000", 16) < 0)) {
		for (k = 0, i++; infor2[i] != ','; k++, i++)
			cardnum[k] = infor2[i];//����
		cardnum[7] = '\0';
		i++;
		for (k = 0; infor2[i] != ',';k++, i++)
			m[k] = infor2[i];
		m[k] = '\0';
		w = atoi(m);//���ں�
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
		time2[16] = '\0';//ʱ��
		if ((strncmp(time2, "2021112500000000", 16) > 0)\
			&& (strncmp(time2, "2021121600000000", 16) < 0)) {
			for (k = 0, i++; infor2[i] != ','; k++, i++)
				cardnum[k] = infor2[i];//����
			cardnum[7] = '\0';
			i++;
			for (k = 0; infor2[i] != ','; k++, i++)
				m[k] = infor2[i];
			m[k] = '\0';
			w = atoi(m);//���ں�
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
	//12��16�ճ�ֵ����5�γ�ֵ��
	for (i = 0, k = 0; i < 16; i++, k++)
		time1[k] = infor1[i];//ʱ��time1
	time1[16] = '\0';
	for (i = 22, k = 0; k < 10; i++, k++)
		studentID[k] = infor1[i];//ѧ��
	studentID[k] = '\0';
	for (k = 0, i++; infor1[i] != ';'; i++, k++)
		m[k] = infor1[i];//��ȡ��ֵ���
	m[k] = '\0';
	money = atof(m);
	Chongzhi(time1, studentID, money, 1);
	while (tag1 = fgets(infor1, 40, fp1)) {
		for (i = 0, k = 0; i < 16; i++, k++)
			time1[k] = infor1[i];//ʱ��time1
		time1[16] = '\0';
		if (strncmp(time1, "20211216", 8) == 0) {
			for (i = 22, k = 0; k < 10; i++, k++)
				studentID[k] = infor1[i];//ѧ��
			studentID[k] = '\0';
			for (k = 0, i++; infor1[i] != ';'; i++, k++)
				m[k] = infor1[i];//��ȡ��ֵ���
			m[k] = '\0';
			money = atof(m);
			Chongzhi(time1, studentID, money, 1);
		}
		else break;
	}
	;
	//12��16�յ�12��31������
	for (i = 0, k = 0; infor2[i] != ','; k++, i++)
		time2[k] = infor2[i];
	time2[16] = '\0';//ʱ��
	if ((strncmp(time2, "2021121600000000", 16) > 0)\
		&& (strncmp(time2, "2021123200000000", 16) < 0)) {
		for (k = 0, i++; infor2[i] != ','; k++, i++)
			cardnum[k] = infor2[i];//����
		cardnum[7] = '\0';
		i++;
		for (k = 0; infor2[i] != ',';k++, i++)
			m[k] = infor2[i];
		m[k] = '\0';
		w = atoi(m);//���ں�
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
		time2[16] = '\0';//ʱ��
		if ((strncmp(time2, "2021121600000000", 16) > 0)\
			&& (strncmp(time2, "2021123200000000", 16) < 0)) {
			for (k = 0, i++; infor2[i] != ','; k++, i++)
				cardnum[k] = infor2[i];//����
			cardnum[7] = '\0';
			i++;
			for (k = 0; infor2[i] != ','; k++, i++)
				m[k] = infor2[i];
			m[k] = '\0';
			w = atoi(m);//���ں�
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



/*���룺���������ļ�ָ��*/
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
	const char* filename = "D:\\CSE\\csp\\����1\\����\\����\\v1\\sort_1.txt";
	FILE* fp1 = NULL;

	//��ȡ�ļ��е��������Ѽ�¼
	while (tag = fgets(info, 40, fp)) {
		while (info[0] == 'W' && tag) {
			m[0] = info[1];
			m[1] = info[2];
			m[2] = '\0';
			w = atoi(m);//��ȡ���ں���Ϣ
			p = NULL;
			p = &r[w];
			tag = fgets(info, 40, fp);
		}
		if (tag) {
			p->w = w;
			for (k = 0, i = 8; info[i] != ','; i++, k++)
				p->time[k] = info[i];//����
			i++;
			for (; info[i] != ','; i++, k++)
				p->time[k] = info[i];//ʱ��
			p->time[16] = '\0';

			for (i = 0, k = 0; info[i] != ','; i++, k++)
				m[k] = info[i];//����
			m[7] = '\0';
			p->cardnum = atoi(m);

			for (k = 0, i = 26; info[i] != ';'; i++, k++)
				m[k] = info[i];//���ѽ��
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

	if (fopen_s(&fp1, filename, "w"))//��sort_1�ļ�
	{
		printf("���ļ�ʧ�ܣ�\n");
		return 1;
	}
	//�ҵ�99�����ڵ����Ѽ�¼��ʱ�������
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
				flag--;//flag��¼���м��������������Ѽ�¼
	}
	fclose(fp1);
	return 0;
}