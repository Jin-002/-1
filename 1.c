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
	int w = 0;//���Ѵ��ں�
	int cardnum;
	char infor[4];
	char file[100];
	char c = '\0';
	char studentID[11];
	char name[9];
	float money = 0;
	char time[17];
	for (i = 0; i < 9; i++)
		name[i] = "\0";//name�ַ�����ʼ��
	if (!(ALLZH = (zh*)malloc(sizeof(zh)))) {//����ͷ���
		printf("����ռ�ʧ�ܣ�");
		return 2;
	}
	ALLZH->next = NULL;
	i = WZ();
	if (i) {//���Ѽ�¼�洢�ṹ��ʼ��
		printf("���Ѽ�¼�洢�ṹ��ʼ��ʧ�ܣ�\n");
		return -1;
	}

	printf("1:��������    2����������    3�����˲���\n");

	while (c = getchar()) {
		if (c == '1') {//��������
			printf("�������ļ�����\n");
			scanf_s("%s", file, 100);
			if (fopen_s(&fp, file, "r")) {//���˻���Ϣ�ļ�
				printf("���ļ�ʧ�ܣ�");
				return 1;
			}
			fgets(infor, 4, fp);//�ļ���һ������
			if (!strncmp(infor, "KH\n", 2))
				KH(fp);
			else
				printf("���ļ�����\n");
			fclose(fp);
		}

		else if (c == '2') {//������������Ҫͬʱ��ȡ�����ļ��������ļ�
			printf("���������������ļ�����");
			scanf_s("%s", file, 100);
			if (fopen_s(&fp, file, "r")) {//�����������ļ�
				printf("���ļ�ʧ�ܣ�");
				return 1;
			}
			fgets(infor, 4, fp);//�ļ���һ������
			if (strncmp(infor, "CZ\n", 2)) {//�жϴ򿪵��ļ��Ƿ���ȷ
				printf("���ļ�����\n");
				fclose(fp);
				return 1;
			}
			printf("���������������ļ�����");
			scanf_s("%s", file, 100);
			if (fopen_s(&fp1, file, "r")) {//�����������ļ�
				printf("���ļ�ʧ�ܣ�");
				return 1;
			}
			fgets(infor, 4, fp1);//�ļ���һ������
			if (strncmp(infor, "XF\n", 2)) {//�жϴ򿪵��ļ��Ƿ���ȷ
				printf("���ļ�����\n");
				fclose(fp);
				return 1;
			}
			CZ_XF(fp, fp1);
			sort();
			fclose(fp);
			fclose(fp1);
		}
		else if (c == '3') {//���˲���
			printf("a:����    b:����    c:��ʧ    d:���    e:����    f:��ֵ    g:����\n\
				h:��ѯ    i:    j:\n");
			getchar();
			c = getchar();
			if (c == 'a') {//����
				printf("������ѧ�ź�������");
				scanf_s("%s %s", studentID, 11, name, 9);
				Faka(studentID, name);
			}
			else if (c == 'b') {//����
				printf("�������ڣ�\n");
				scanf_s("%s", time, 17);
				printf("������Ҫ����������ѧ�ţ�");
				scanf_s("%s", studentID, 11);
				Xiaohu(time, studentID, 2);
			}
			else if (c == 'c') {//��ʧ
				printf("�������ڣ�\n");
				scanf_s("%s", time, 17);
				printf("������Ҫ���й�ʧ��ѧ�ţ�");
				scanf_s("%s", studentID, 11);
				Guashi(time, studentID, 2);
			}
			else if (c == 'd') {//���
				printf("�������ڣ�\n");
				scanf_s("%s",time, 17);
				printf("������Ҫ���й�ʧ��ѧ�ţ�");
				scanf_s("%s", studentID, 11);
				Jiegua(time, studentID, 2);
			}
			else if (c == 'e') {//����
				printf("�������ڣ�\n");
				scanf_s("%s",time, 17);
				printf("������Ҫ������ѧ�ţ�");
				scanf_s("%s", studentID, 11);
				Buka(time, studentID, 2);
			}
			else if (c == 'f') {//��ֵ
				printf("�������ڣ�\n");
				scanf_s("%s", time, 17);
				printf("������Ҫ��ֵ��ѧ�źͽ�");
				scanf_s("%s %f", studentID, 11, &money);
				Chongzhi(time, studentID, money, 2);
			}
			else if (c == 'g') {//����
				//�����Ѽ�¼������־�ļ�
				if (fopen_s(&fp, "D:\\CSE\\csp\\����1\\����\\����\\v1\\xfrecord.txt", "a+")) {
					printf("�����Ѽ�¼������־�ļ�ʧ�ܣ�\n");
					return 1;
				}
				printf("���봰�ںţ�");
				scanf_s("%d", &w);
				printf("���ڣ�");
				scanf_s("%s",time, 17);
				printf("���뿨�ź����ѽ�");
				scanf_s("%d %f", &cardnum, &money);
				Xiaofei(w,time, money, cardnum, 2,fp);
				fclose(fp);
			}
			else if (c == 'h') {//��ѯ�˻���Ϣ��֧��ģ����ѯ��
				matchID();
			}
			else if (c == 'i')
				;

		}
		printf("1:��������    2����������    3�����˲���\n");
		getchar();
	}
		return 0;
}

/*��ȡ��ִ�в�ѯ��������*/
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
			printf("��ѯ�������˻���Ϣ��\n");
		else if (p->status == 0)
			printf("�ÿ�Ƭ�ѹ�ʧ��\n");
		else if (p->status == 1)
			printf("������%s\nѧ�ţ�%s\n���ţ�%d\n��ǰ��%.2f\n", p->name, p->studentID, p->cardnum, p->balance);
	}
	return 0;
}

/*�����ַ���ģ��ƥ��
���룺ģ��ƥ���ַ���pattern
�������������˻���Ϣ��Ѱ�ҷ���pattern�ַ�����ʽ���˻������������ѧ�ţ�
����ֵ����ȷִ����Ϻ󷵻�0��
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
	printf("������ģʽ�ַ�����");
	scanf_s("%s", pattern, 9);
	len = strlen(pattern);
	while (p = p->next)
	{
		for (i = 0, k = 0, flag = 0; p->name[i] != '\0'; ) {
			if (p->name[i] == pattern[k] && p->name[i + 1] == pattern[k + 1]) {
				i += 2;
				k += 2;//�Ƚ���һ����
				flag += 2;
			}
			else if (pattern[k] == '?') {
				i += 2;
				k++;
				flag++;
			}
			else if (pattern[k] == '*') {
				while (pattern[k] == '?' || pattern[k] == '*') {
					k++;//�ҵ���һ����
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

/*ѧ���ַ���ģ����ѯ�˻���Ϣ
���룺ģ��ƥ���ַ���pattern
�������������˻���Ϣ��Ѱ�ҷ���pattern�ַ�����ʽ���˻�������˻���Ϣ��
����ֵ����ȷִ����Ϻ󷵻�0��
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
		pattern[i] = '\0';//ģʽ�ַ�����ʼ��

	printf("������ģʽ�ַ�����");
	scanf_s("%s", pattern, 20);
	len = strlen(pattern);
	while (p = p->next)
	{
		for (i = 0, k = 0, flag = 0; p->studentID[i] != '\0'; ) {
			if (p->studentID[i] == pattern[k]||pattern[k]=='?') {
				i++;
				k++;//�Ƚ���һ���ַ�
				flag++;
			}
			else if (pattern[k] == '*') {
				while (pattern[k] == '?' || pattern[k] == '*') {
					k++;//�ҵ���һ����ģ���ַ�
					flag++;
				}
				while (p->studentID[i] != pattern[k])
					i++;
			}
			else
				break;
		}
		if (flag == len && p->studentID[i] == '\0')
			printf("%s %s %d ��%.2f\n", p->studentID, p->name,p->cardnum,p->balance);
	}
	return 0;
}

/*���Ѽ�¼�洢�ṹ��ʼ������
���룺void;
�����������Ѽ�¼�洢�ṹ���г�ʼ�����������Ѵ������ݳ�ʼ�������Ѽ�¼���ڳ�ʼ����
����ֵ����ȷִ����Ϻ󷵻�0��
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

	const char* filename = "D:\\CSE\\csp\\����1\\����\\����\\v1\\wz003.txt";
	if (fopen_s(&fp1, filename, "r")) {
		printf("�����Ѽ�¼λ���ļ�ʧ�ܣ�\n");
		return 1;
	}

	//������Ϣ��ʼ��
	fgets(infor, 15, fp1);
	for (i = 1; i < 100; i++) {
		k = 0;
		ALLJL[i].w = i;//���ں�
		ALLJL[i].xftime = 0;//�ۼ����Ѵ�����ʼ��
		ALLJL[i].xfmoney = 0;//�ۼ����ѽ���ʼ��
		fgets(infor, 15, fp1);
		while (infor[k] != ',')
			k++;
		for (j = 0, k++; infor[k] != ';'; k++, j++)
			m[j] = infor[k];
		m[j] = '\0';
		ALLJL[i].number = atoi(m);//����λ�ó�ʼ��
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
		for (k = 1; k < 60000; k++) {//60000�����Ѽ�¼�洢�ռ��ʼ��
			q = (jl*)malloc(sizeof(jl));
			q->number = k;
			q->cardnum = 0;
			q->money = 0;
			strncpy_s(q->time, 17, "", 17);
			q->next = NULL;
			r->next = q;
			r = r->next;
		}
		r->next = ALLJL[i].j;//����βָ������ͷ���γ�ѭ������
	}


	for (i = 1; i < 100; i++) {
		r = ALLJL[i].j;
		while (r->number != ALLJL[i].number)
			r = r->next;
		ALLJL[i].j = r;
	}//���Ѽ�¼λ��
	return 0;
}



/*���ܷ�������
���룺void;
�������Ե�ǰ���շѴ��ڵ����ݽ���������ܣ�ѡ��һ��
��·�鲢����������������㷨����ʵ�֣��������ʱ��
�Ρ�ѧ�š����������ѽ�Χ���в�ѯ��ͳ�ƣ�����ѧ
�š�����֧��ģ��ƥ�䣻�������Ҫͬʱ�����������
���ķѵ�ʱ�䡢ÿ����ϲ�ѯ�ķѵ�ʱ�䡣
����ֵ����ȷִ����Ϻ󷵻�0��
*/
int sort(void)
{
	jl* p[100];
	jl* q = NULL;
	int i = 0, flag = 99;
	int k = 0;
	int w = 0;
	FILE* fp=NULL;//�����������浽�ļ���
	const char* filename = "D:\\CSE\\csp\\����1\\����\\����\\v1\\sort.txt";
	
	if (fopen_s(&fp, filename, "w"))//��sort�ļ�
	{
		printf("���ļ�ʧ�ܣ�\n");
		return 1;
	}
	p[0] = ALLJL[1].j;//��ʼ��

	//�ҵ��������Ѽ�¼������ʱ����С�Ľ��
	for (i = 1; i < 100; i++) {
		if (!strlen(ALLJL[i].j->time)) //�ô���û�����Ѽ�¼
			p[i] = ALLJL[i].j;
		else if (strlen(ALLJL[i].j->next->time))//���Ѽ�¼������ѭ������
			p[i] = ALLJL[i].j->next;
		else {//���Ѽ�¼δѭ�����ǣ��ҵ����Ѽ�¼��ʼλ��
			p[i] = ALLJL[i].j->next;
			while (!(p[i]->money))
				p[i] = p[i]->next;
		}
	}
	//�ҵ�99�����ڵ����Ѽ�¼��ʱ�������
	while (flag) {
		strncpy_s(p[0]->time, 17, "2022000000000000", 16);
		for (i = 1,w=0; i < 100; i++) {
			if ((strncmp(p[0]->time, p[i]->time, 16) > 0) && (strlen(p[i]->time))) {
				p[0] = p[i];
				w = i;//���µ�ǰʱ�����ڵļ�¼�Ĵ��ں�
			}
		}
		p[w]=p[w]->next;
		fprintf(fp, "%s,%d,%d,%.2f;\n", p[0]->time, p[0]->cardnum,w,p[0]->money);
		for (i = 1, flag = 99; i < 100; i++)
			if (p[i]->money=0||p[i]->number==ALLJL[i].number)
				flag--;//flag��¼���м��������������Ѽ�¼
	} 
	fclose(fp);
	return 0;
}