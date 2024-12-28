#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include<stdio.h>
#include <stdlib.h>
#include<windows.h>
#include<string.h>
#include<time.h>


//打印书籍购买目录(单页)
void printMenu(char book[100][200], int size) {
	printf("===================购买界面====================\n");
	for (int i = 0; i < size; i++) {
		printf("%s\n", book[i]);
	}
	printf("-----------------------------------------------\n");
}
//打印书籍购买目录(分页)
void printMenu2(char book[100][200], int size, int page) {
	printf("===================购买界面====================\n");
	for (int i = ((page - 1) * 10); i < page * 10; i++) {
		if (i >= size) {
			break;
		}
		printf("%s\n", book[i]);
	}
	printf("..................<第%d页>......................\n", page);
	printf("-----------------------------------------------\n");
}
//打印购买订单
double printOrder(int buybook[100][2], char book[100][200], double money[100], int count) {
	printf("-------------------购买订单--------------------\n");
	//从已购买的列表中获取信息
	double sum = 0.00;
	for (int i = 0; i < count; i++) {
		if (buybook[i][1] > 0) {
			printf("%s\n", book[buybook[i][0] - 1]);
			printf("=================================%d\n", buybook[i][1]);
		}
		sum += money[buybook[i][0] - 1] * buybook[i][1];
	}
	printf("共计---%.2f元\n", sum);
	printf("==============================================\n");
	return sum;
}
//退换书籍
double exchangebook(int buybook[100][2],char book[100][200], double money[100], int count, int sum) {
	int exchangechoose = -1;
	while (exchangechoose != 0) {
		//得到当前的订单
		sum = printOrder(buybook, book, money, count);
		exchangeinput:
		printf("请输入要退掉的书籍标号,输入0退出：\n");	
		scanf_s("%d", &exchangechoose);
		//退出逻辑
		if (exchangechoose == 0) {
			break;
		}
		int FindBook = 1;
		//1  buybook[i][0]   buybook[i][1]
		for (int i = 0; i < count; i++) {
			if (buybook[i][0] == exchangechoose) {
				FindBook = 0;
				int exchangenumber = 0;
				printf("目前已购买%d本，要退几本？输入0回到上一步\n",buybook[i][1]);
				scanf_s("%d", &exchangenumber);
				if (exchangenumber == 0) {
					goto exchangeinput;
				}
				while (exchangenumber<0 || exchangenumber>buybook[i][1]) {
					printf("输入有误或书本数量不足，请重试\n");
					scanf_s("%d", &exchangenumber);
				}
				buybook[i][1] -= exchangenumber;
				/*sum -= exchangenumber * money[buybook[i][0] - 1];*/
			}
		}
		if (FindBook) {
			printf("您输入的有问题，请重新输入\n");
			goto exchangeinput;
		}
		system("cls");
		return printOrder(buybook, book, money, count);;
	}
}
//获取金钱并结算
int getMoney(int cardnum,double sum,int buybook[100][2],char book[100][200],double money[100],int count) {
	//储存各个种类的double格式的钱
	double nowmoney[4];
	//储存从文件中提取的字符串格式金额
	char GetCardMoney[4][1024];
	//储存卡号字符，用来拼接path地址
	char numstr[7];
	//path地址模版
	char path[] = { 'd','a','t','a','\\','1','1','1','1','5','6','.','t','x','t' ,'\0'};
	//将输入的卡号转化为字符串用来拼接
	snprintf(numstr, 7, "%d", cardnum);
	//用输入的卡号替换path模版中的卡号
	for (int i = 0; i < 6; i++) {
		path[i + 5] = numstr[i];
	}
	//开启文件
	FILE* filemoney = fopen(path, "r");
	//逐行读取文件中的信息
	for (int i = 0; i < 4; i++) {
		fgets(GetCardMoney[i], 1024, filemoney);
	}
	//对文件中的金额进行转换
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
	//关闭文件
	fclose(filemoney);

getMoneybegin:
	printf("合计：%.2f元\n", sum);

	printf("本店支持微信、支付宝、花呗、借贷\n");
	//显示金额
	printf("您的余额分别为：\n");
	for (int i = 0; i < 4; i++) {
		printf("==%.2f==", nowmoney[i]);
	}
	printf("请选择支付方式：0.进行退换操作 1.微信 2.支付宝 3.花呗 4.贷款\n");
	int chmoney = 0;
moneyinput:scanf_s("%d", &chmoney);
	//防止判断金额
	while (chmoney < 0 || chmoney >= 5) {
		printf("非法输入，请重试\n");
		goto moneyinput;
	}
	while (chmoney > 0 && chmoney < 5 && nowmoney[chmoney-1] < sum) {
		printf("金额不足，请更换支付方式\n");
		goto moneyinput;
	}
	//退换逻辑
	if (chmoney == 0) {
		system("cls");
		sum = exchangebook(buybook, book, money, count, sum);
		system("cls");
		goto getMoneybegin;
	}
	else {
		//成功支付逻辑
		nowmoney[chmoney-1] -= sum;
		printf("感谢使用，你的账户信息已变更\n");
		//变更账户信息
		char pre[4][20] = {"微信-","支付宝-","花呗-","借款-"};
		char util[] = { "\n" };
		FILE* filemoney = fopen(path, "w");
		for (int i = 0; i < 4; i++) {
			//用来储存变更后的金额字符串
			char newmoney[10];
			//转化
			snprintf(newmoney, 10, "%.2f", nowmoney[i]);
			//写入文件
			fputs(strcat(strcat(pre[i], newmoney),util), filemoney);
		}
		//关闭文件
		fclose(filemoney);
		return chmoney;
	}
}
//结算
int pay(int cardnum,double sum,int buybook[100][2], char book[100][200], double money[100],int count) {
	return getMoney(cardnum,sum,buybook,book,money,count);
}

//办卡
void bank(int cardnums[20]) {
	//用来储存用户想创建的卡号
	int cardnumber;
	//初始化新账户的金钱
	double newmoney[4] = { 0.00,0.00,0.00,150000 };
	printf("请输入你的六位卡号\n");
cardinput:scanf_s("%d", &cardnumber);
	//过滤非法输入
	while (cardnumber < 100000 || cardnumber>999999) {
		printf("请重新输入\n");
		scanf_s("%d", &cardnumber);
	}
	//过滤重复卡号
	for (int i = 0; i < 20; i++) {
		if (cardnums[i] == cardnumber) {
			printf("卡号已存在，请重新输入\n");
			goto cardinput;
		}
	}
	//储存卡号字符串
	char numstr[7];
	//path模版
	char path[] = { 'd','a','t','a','\\','1','1','1','1','5','6','.','t','x','t' ,'\0' };
	//转化
	snprintf(numstr, 7, "%d", cardnumber);
	//替换
	for (int i = 0; i < 6; i++) {
		path[i + 5] = numstr[i];
	}
	//开启文件
	FILE* file = fopen(path, "w");
	printf("请问要进行充值吗？1.充值 2.不充值\n");
	//进行选择
	int chongzhi = 0;
	scanf_s("%d", &chongzhi);
	//防止非法输入
	while (chongzhi != 1 && chongzhi != 2) {
		printf("非法输入，请重试\n");
		scanf_s("%d", &chongzhi);
	}
	//如果选了充值的逻辑
	if (chongzhi == 1) {
		int keepchongzhi = 4;
		while (keepchongzhi != 0) {
			printf("请问要充值哪一种金额？0.结束 1.微信 2.支付宝 3.花呗\n");
			scanf_s("%d", &keepchongzhi);
			if (keepchongzhi == 0) {
				break;
			}
			printf("要充值多少？\n");
			int money = 0;
			scanf_s(" %d", &money);
			//更新金额
			newmoney[keepchongzhi - 1] += money;
		}
	}
	//对文件中的信息进行更新
	char pre[4][20] = { "微信-","支付宝-","花呗-","借款-" };
	char util[] = { "\n" };
	for (int i = 0; i < 4; i++) {
		//形成data\后的后缀
		char newmoneymid[10];
		snprintf(newmoneymid, 10, "%.2f", newmoney[i]);
		//写入文件
		fputs(strcat(strcat(pre[i], newmoneymid), util), file);
	}
	printf("请牢记您的卡号\n");
}

//生成小票文件buybook, book, money, count, sum, pays
void loginfo(int buybook[100][2], char book[100][200], double money[100], int count,double sum,int pay) {
	//产生真随机
	srand((unsigned)time(NULL));
	//用随机数来防止小票重复
	int random  = (rand() % 900000) + 100000;
	//储存卡号字符，用来拼接path地址
	char ranstr[7];
	//path地址模版
	char logpath[] = { 'd','a','t','a','\\','1','1','1','1','5','6','.','t','x','t' ,'\0' };
	//将输入的卡号转化为字符串用来拼接
	snprintf(ranstr, 7, "%d", random);
	//用输入的卡号替换path模版中的卡号
	for (int i = 0; i < 6; i++) {
		logpath[i + 5] = ranstr[i];
	}
	//开启文件
	FILE* logfile = fopen(logpath, "w");
	fputs("购买商品：\n", logfile);
	for (int i = 0; i < count; i++) {
		if (buybook[i][1] > 0) {
			fputs(book[buybook[i][0] - 1], logfile);
		}
		/*char logmoney[20];
		snprintf(logmoney, ".2f", 20, money[buybook[i][0]-1]*buybook[i][1]);
		fputs(strcat("-----", logmoney), logfile);*/
	}
	//记录支付方式
	char pre111[4][20] = { "支付方式：微信","支付方式-支付宝","支付方式-花呗","支付方式-借款" };
	fputs(pre111[pay-1], logfile);

	fclose(logfile);
}

//从文件中获取书籍信息,包括书籍名字以及价格
void initAllBook() {
	//开启文件
	FILE* file = fopen("data\\book.txt", "r");
	//储存卡号的数组，最大20个卡
	int cardnums[20];
	//初始化管理员卡号
	cardnums[0] = 123456;
	//储存一共的书籍
	char book[100][200];
	//储存已经购买的书籍
	int buybook[100][2];
	//记录一共书籍的种类
	int size = 0;
	//记录每种书籍的金额
	double money[100];
	//记录已经买了几种书
	int count = 0;
	//记录目前的总金额
	double sum = 0;
	//从文件中读取书籍信息并记录
	while (fgets(book[size], 200, file) != NULL) {
		size++;
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 200; j++) {
			//通过“-”符号来截取书籍的金额，这是提前约定好文件内容格式的
			if (book[i][j] == '-' && book[i][j + 1] != '-') {
				//储存金额字符串
				char mon[10];
				int start = j + 1;
				while (book[i][start] != NULL) {
					mon[start - 1 - j] = book[i][start];
					start++;
				}
				mon[start - 1 - j] = '\0';
				//将字符串转化为等值数字
				double m = atof(mon);
				money[i] = m;
				break;
			}
		}
	}
	fclose(file);

	//记录用户选择
	int choose = -1;
	//记录买这一种书的数量
	int number = 0;
	//记录当前的页码
	int page = 1;
	//记录当前是不是分页
	BOOL flog = FALSE;
	while (choose != 0) {
	begin:
		//如果书的种类太多，那就分页显示
		if (size >= 10) {
			flog = TRUE;
			printMenu2(book, size, page);
			printf("《《《《上一页输入101-下一页输入102》》》》》\n");
		}
		//否则正常一页显示
		else {
			printMenu(book, size);
		}
		//实时获得总金额并打印当前的订单
		sum = printOrder(buybook, book, money, count);


		//获取买书编号
		printf("请输入要购买的书籍的编号，输入您的六位卡号进行结算。没有卡的话可以输入999办卡哦\n");
	input:
		scanf_s("%d", &choose);
		//办卡逻辑
		if (choose == 999) {
			bank(cardnums);
			break;
		}
		//断定输入了六位卡号，进行判断
		if (choose > 100000) {
			system("cls");
			//结账逻辑
			int pays = pay(choose,sum,buybook,book,money,count);
			//打印小票
			loginfo(buybook, book, money, count, sum, pays);
			break;
		}
		//上一页逻辑，同时判断是不是分页的情况
		if (flog&&choose == 101) {
			page--;
			if (page <= 0) {
				page = 1;
			}
			system("cls");
			goto begin;
		}
		//下一页逻辑
		else if (flog&&choose == 102) {
			page++;
			if ((page - 1) * 10 > size) {
				page--;
			}
			system("cls");
			goto begin;
		}
		//过滤非法输入
		while ((choose > size && choose<100000) || choose < 0 ) {
			printf("请重新输入\n");
			goto input;
		}
		//防止用户误触，只有当翻到有这种书的那一页才可以进行购买
		while ((choose > page * 10&&choose<100000) || choose < (page - 1) * 10 + 1) {
			printf("请翻到那一页在买哦\n");
			goto input;
		}
		//退出使用逻辑
		if (choose == 0) {
			break;
		}
		//获取买书数量
		printf("买几本？\n");
		scanf_s("%d", &number);
		while (number <= 0) {
			printf("请重新输入\n");
			scanf_s("%d", &number);
		}
		//更新清单数据
		//遍历之前已经买的书，如果存在，就仅更新数量
		BOOL addbook = TRUE;
		for (int i = 0; i < count; i++) {
			if((buybook[i][0]) == choose){
				buybook[i][1] += number;
				addbook = FALSE;
				break;
			}
		}
		//如果不存在就新增
		if (addbook) {
			buybook[count][0] = choose;
			buybook[count][1] = number;
			count++;
		}
		//更新页面
		system("cls");
	}
}