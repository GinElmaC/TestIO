#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<stdio.h>
#include <stdlib.h>
#include<windows.h>
#include<string.h>
#include<time.h>


//��ӡ�鼮����Ŀ¼(��ҳ)
void printMenu(char book[100][200], int size) {
	printf("===================�������====================\n");
	for (int i = 0; i < size; i++) {
		printf("%s\n", book[i]);
	}
	printf("-----------------------------------------------\n");
}
//��ӡ�鼮����Ŀ¼(��ҳ)
void printMenu2(char book[100][200], int size, int page) {
	printf("===================�������====================\n");
	for (int i = ((page - 1) * 10); i < page * 10; i++) {
		if (i >= size) {
			break;
		}
		printf("%s\n", book[i]);
	}
	printf("..................<��%dҳ>......................\n", page);
	printf("-----------------------------------------------\n");
}
//��ӡ���򶩵�
double printOrder(int buybook[100][2], char book[100][200], double money[100], int count) {
	printf("-------------------���򶩵�--------------------\n");
	//���ѹ�����б��л�ȡ��Ϣ
	double sum = 0.00;
	for (int i = 0; i < count; i++) {
		if (buybook[i][1] > 0) {
			printf("%s\n", book[buybook[i][0] - 1]);
			printf("=================================%d\n", buybook[i][1]);
		}
		sum += money[buybook[i][0] - 1] * buybook[i][1];
	}
	printf("����---%.2fԪ\n", sum);
	printf("==============================================\n");
	return sum;
}
//�˻��鼮
double exchangebook(int buybook[100][2],char book[100][200], double money[100], int count, int sum) {
	int exchangechoose = -1;
	while (exchangechoose != 0) {
		//�õ���ǰ�Ķ���
		sum = printOrder(buybook, book, money, count);
		exchangeinput:
		printf("������Ҫ�˵����鼮���,����0�˳���\n");	
		scanf_s("%d", &exchangechoose);
		//�˳��߼�
		if (exchangechoose == 0) {
			break;
		}
		int FindBook = 1;
		//1  buybook[i][0]   buybook[i][1]
		for (int i = 0; i < count; i++) {
			if (buybook[i][0] == exchangechoose) {
				FindBook = 0;
				int exchangenumber = 0;
				printf("Ŀǰ�ѹ���%d����Ҫ�˼���������0�ص���һ��\n",buybook[i][1]);
				scanf_s("%d", &exchangenumber);
				if (exchangenumber == 0) {
					goto exchangeinput;
				}
				while (exchangenumber<0 || exchangenumber>buybook[i][1]) {
					printf("����������鱾�������㣬������\n");
					scanf_s("%d", &exchangenumber);
				}
				buybook[i][1] -= exchangenumber;
				/*sum -= exchangenumber * money[buybook[i][0] - 1];*/
			}
		}
		if (FindBook) {
			printf("������������⣬����������\n");
			goto exchangeinput;
		}
		system("cls");
		return printOrder(buybook, book, money, count);;
	}
}
//��ȡ��Ǯ������
int getMoney(int cardnum,double sum,int buybook[100][2],char book[100][200],double money[100],int count) {
	//������������double��ʽ��Ǯ
	double nowmoney[4];
	//������ļ�����ȡ���ַ�����ʽ���
	char GetCardMoney[4][1024];
	//���濨���ַ�������ƴ��path��ַ
	char numstr[7];
	//path��ַģ��
	char path[] = { 'd','a','t','a','\\','1','1','1','1','5','6','.','t','x','t' ,'\0'};
	//������Ŀ���ת��Ϊ�ַ�������ƴ��
	snprintf(numstr, 7, "%d", cardnum);
	//������Ŀ����滻pathģ���еĿ���
	for (int i = 0; i < 6; i++) {
		path[i + 5] = numstr[i];
	}
	//�����ļ�
	FILE* filemoney = fopen(path, "r");
	//���ж�ȡ�ļ��е���Ϣ
	for (int i = 0; i < 4; i++) {
		fgets(GetCardMoney[i], 1024, filemoney);
	}
	//���ļ��еĽ�����ת��
	for (int i = 0; i < 4; i++) {
		int start = 0;
		char cardmon[100];
		int index = 0;
		for (int j = 0; j < 1024; j++) {
			if (GetCardMoney[i][j] == '-') {
				start = j+1;
				while (GetCardMoney[i][start] != NULL) {
					cardmon[start - j - 1] = GetCardMoney[i][start];
					start++;
				}
				cardmon[start - j - 1] = '\0';
				double money = atof(cardmon);
				nowmoney[i] = money;
				break;
			}
		}
	}
	//�ر��ļ�
	fclose(filemoney);

getMoneybegin:
	printf("�ϼƣ�%.2fԪ\n", sum);

	printf("����֧��΢�š�֧���������¡����\n");
	//��ʾ���
	printf("�������ֱ�Ϊ��\n");
	for (int i = 0; i < 4; i++) {
		printf("==%.2f==", nowmoney[i]);
	}
	printf("��ѡ��֧����ʽ��0.�����˻����� 1.΢�� 2.֧���� 3.���� 4.����\n");
	int chmoney = 0;
moneyinput:scanf_s("%d", &chmoney);
	//��ֹ�жϽ��
	while (chmoney < 0 || chmoney >= 5) {
		printf("�Ƿ����룬������\n");
		goto moneyinput;
	}
	while (chmoney > 0 && chmoney < 5 && nowmoney[chmoney-1] < sum) {
		printf("���㣬�����֧����ʽ\n");
		goto moneyinput;
	}
	//�˻��߼�
	if (chmoney == 0) {
		system("cls");
		sum = exchangebook(buybook, book, money, count, sum);
		system("cls");
		goto getMoneybegin;
	}
	else {
		//�ɹ�֧���߼�
		nowmoney[chmoney-1] -= sum;
		printf("��лʹ�ã�����˻���Ϣ�ѱ��\n");
		//����˻���Ϣ
		char pre[4][20] = {"΢��-","֧����-","����-","���-"};
		char util[] = { "\n" };
		FILE* filemoney = fopen(path, "w");
		for (int i = 0; i < 4; i++) {
			//������������Ľ���ַ���
			char newmoney[10];
			//ת��
			snprintf(newmoney, 10, "%.2f", nowmoney[i]);
			//д���ļ�
			fputs(strcat(strcat(pre[i], newmoney),util), filemoney);
		}
		//�ر��ļ�
		fclose(filemoney);
		return chmoney;
	}
}
//����
int pay(int cardnum,double sum,int buybook[100][2], char book[100][200], double money[100],int count) {
	return getMoney(cardnum,sum,buybook,book,money,count);
}

//�쿨
void bank(int cardnums[20]) {
	//���������û��봴���Ŀ���
	int cardnumber;
	//��ʼ�����˻��Ľ�Ǯ
	double newmoney[4] = { 0.00,0.00,0.00,150000 };
	printf("�����������λ����\n");
cardinput:scanf_s("%d", &cardnumber);
	//���˷Ƿ�����
	while (cardnumber < 100000 || cardnumber>999999) {
		printf("����������\n");
		scanf_s("%d", &cardnumber);
	}
	//�����ظ�����
	for (int i = 0; i < 20; i++) {
		if (cardnums[i] == cardnumber) {
			printf("�����Ѵ��ڣ�����������\n");
			goto cardinput;
		}
	}
	//���濨���ַ���
	char numstr[7];
	//pathģ��
	char path[] = { 'd','a','t','a','\\','1','1','1','1','5','6','.','t','x','t' ,'\0' };
	//ת��
	snprintf(numstr, 7, "%d", cardnumber);
	//�滻
	for (int i = 0; i < 6; i++) {
		path[i + 5] = numstr[i];
	}
	//�����ļ�
	FILE* file = fopen(path, "w");
	printf("����Ҫ���г�ֵ��1.��ֵ 2.����ֵ\n");
	//����ѡ��
	int chongzhi = 0;
	scanf_s("%d", &chongzhi);
	//��ֹ�Ƿ�����
	while (chongzhi != 1 && chongzhi != 2) {
		printf("�Ƿ����룬������\n");
		scanf_s("%d", &chongzhi);
	}
	//���ѡ�˳�ֵ���߼�
	if (chongzhi == 1) {
		int keepchongzhi = 4;
		while (keepchongzhi != 0) {
			printf("����Ҫ��ֵ��һ�ֽ�0.���� 1.΢�� 2.֧���� 3.����\n");
			scanf_s("%d", &keepchongzhi);
			if (keepchongzhi == 0) {
				break;
			}
			printf("Ҫ��ֵ���٣�\n");
			int money = 0;
			scanf_s(" %d", &money);
			//���½��
			newmoney[keepchongzhi - 1] += money;
		}
	}
	//���ļ��е���Ϣ���и���
	char pre[4][20] = { "΢��-","֧����-","����-","���-" };
	char util[] = { "\n" };
	for (int i = 0; i < 4; i++) {
		//�γ�data\��ĺ�׺
		char newmoneymid[10];
		snprintf(newmoneymid, 10, "%.2f", newmoney[i]);
		//д���ļ�
		fputs(strcat(strcat(pre[i], newmoneymid), util), file);
	}
	printf("���μ����Ŀ���\n");
}

//����СƱ�ļ�buybook, book, money, count, sum, pays
void loginfo(int buybook[100][2], char book[100][200], double money[100], int count,double sum,int pay) {
	//���������
	srand((unsigned)time(NULL));
	//�����������ֹСƱ�ظ�
	int random  = (rand() % 900000) + 100000;
	//���濨���ַ�������ƴ��path��ַ
	char ranstr[7];
	//path��ַģ��
	char logpath[] = { 'd','a','t','a','\\','1','1','1','1','5','6','.','t','x','t' ,'\0' };
	//������Ŀ���ת��Ϊ�ַ�������ƴ��
	snprintf(ranstr, 7, "%d", random);
	//������Ŀ����滻pathģ���еĿ���
	for (int i = 0; i < 6; i++) {
		logpath[i + 5] = ranstr[i];
	}
	//�����ļ�
	FILE* logfile = fopen(logpath, "w");
	fputs("������Ʒ��\n", logfile);
	for (int i = 0; i < count; i++) {
		if (buybook[i][1] > 0) {
			fputs(book[buybook[i][0] - 1], logfile);
		}
		/*char logmoney[20];
		snprintf(logmoney, ".2f", 20, money[buybook[i][0]-1]*buybook[i][1]);
		fputs(strcat("-----", logmoney), logfile);*/
	}
	//��¼֧����ʽ
	char pre111[4][20] = { "֧����ʽ��΢��","֧����ʽ-֧����","֧����ʽ-����","֧����ʽ-���" };
	fputs(pre111[pay-1], logfile);

	fclose(logfile);
}

//���ļ��л�ȡ�鼮��Ϣ,�����鼮�����Լ��۸�
void initAllBook() {
	//�����ļ�
	FILE* file = fopen("data\\book.txt", "r");
	//���濨�ŵ����飬���20����
	int cardnums[20];
	//��ʼ������Ա����
	cardnums[0] = 123456;
	//����һ�����鼮
	char book[100][200];
	//�����Ѿ�������鼮
	int buybook[100][2];
	//��¼һ���鼮������
	int size = 0;
	//��¼ÿ���鼮�Ľ��
	double money[100];
	//��¼�Ѿ����˼�����
	int count = 0;
	//��¼Ŀǰ���ܽ��
	double sum = 0;
	//���ļ��ж�ȡ�鼮��Ϣ����¼
	while (fgets(book[size], 200, file) != NULL) {
		size++;
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 200; j++) {
			//ͨ����-����������ȡ�鼮�Ľ�������ǰԼ�����ļ����ݸ�ʽ��
			if (book[i][j] == '-' && book[i][j + 1] != '-') {
				//�������ַ���
				char mon[10];
				int start = j + 1;
				while (book[i][start] != NULL) {
					mon[start - 1 - j] = book[i][start];
					start++;
				}
				mon[start - 1 - j] = '\0';
				//���ַ���ת��Ϊ��ֵ����
				double m = atof(mon);
				money[i] = m;
				break;
			}
		}
	}
	fclose(file);

	//��¼�û�ѡ��
	int choose = -1;
	//��¼����һ���������
	int number = 0;
	//��¼��ǰ��ҳ��
	int page = 1;
	//��¼��ǰ�ǲ��Ƿ�ҳ
	BOOL flog = FALSE;
	while (choose != 0) {
	begin:
		//����������̫�࣬�Ǿͷ�ҳ��ʾ
		if (size >= 10) {
			flog = TRUE;
			printMenu2(book, size, page);
			printf("����������һҳ����101-��һҳ����102����������\n");
		}
		//��������һҳ��ʾ
		else {
			printMenu(book, size);
		}
		//ʵʱ����ܽ���ӡ��ǰ�Ķ���
		sum = printOrder(buybook, book, money, count);


		//��ȡ������
		printf("������Ҫ������鼮�ı�ţ�����������λ���Ž��н��㡣û�п��Ļ���������999�쿨Ŷ\n");
	input:
		scanf_s("%d", &choose);
		//�쿨�߼�
		if (choose == 999) {
			bank(cardnums);
			break;
		}
		//�϶���������λ���ţ������ж�
		if (choose > 100000) {
			system("cls");
			//�����߼�
			int pays = pay(choose,sum,buybook,book,money,count);
			//��ӡСƱ
			loginfo(buybook, book, money, count, sum, pays);
			break;
		}
		//��һҳ�߼���ͬʱ�ж��ǲ��Ƿ�ҳ�����
		if (flog&&choose == 101) {
			page--;
			if (page <= 0) {
				page = 1;
			}
			system("cls");
			goto begin;
		}
		//��һҳ�߼�
		else if (flog&&choose == 102) {
			page++;
			if ((page - 1) * 10 > size) {
				page--;
			}
			system("cls");
			goto begin;
		}
		//���˷Ƿ�����
		while ((choose > size && choose<100000) || choose < 0 ) {
			printf("����������\n");
			goto input;
		}
		//��ֹ�û��󴥣�ֻ�е����������������һҳ�ſ��Խ��й���
		while ((choose > page * 10&&choose<100000) || choose < (page - 1) * 10 + 1) {
			printf("�뷭����һҳ����Ŷ\n");
			goto input;
		}
		//�˳�ʹ���߼�
		if (choose == 0) {
			break;
		}
		//��ȡ��������
		printf("�򼸱���\n");
		scanf_s("%d", &number);
		while (number <= 0) {
			printf("����������\n");
			scanf_s("%d", &number);
		}
		//�����嵥����
		//����֮ǰ�Ѿ�����飬������ڣ��ͽ���������
		BOOL addbook = TRUE;
		for (int i = 0; i < count; i++) {
			if((buybook[i][0]) == choose){
				buybook[i][1] += number;
				addbook = FALSE;
				break;
			}
		}
		//��������ھ�����
		if (addbook) {
			buybook[count][0] = choose;
			buybook[count][1] = number;
			count++;
		}
		//����ҳ��
		system("cls");
	}
}