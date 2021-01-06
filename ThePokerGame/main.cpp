#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include <iostream>

#include "funclib.h"
using namespace std;

void gameInit(); //遊戲初始化(選擇人數，制定換牌規則)
Card* handCard[4]; //四副手牌
int changeWithWhom[4]; //各個玩家要跟誰換牌
int playerCount; //儲存有幾個玩家(支援2或4人)
Card deck[52]; //牌堆

//牌面(數字)
const char* face[] = {
	"Deuce", "Three",
	"Four", "Five",
	"Six", "Seven", "Eight",
	"Nine", "Ten",
	"Jack", "Queen", "King", "Ace"
};

//牌面(花色)
const char* suit[] = {"Clubs", "Diamonds", "Hearts", "Spades"};

//這個陣列是牌型(順序配合Determine)
const char* cardType[] = {"散牌", "對子", "二對", "三條", "順子", "同花", "葫蘆", "四條", "同花順"};


int main()
{
	////////////////////

	srand(time(nullptr));
	int i, j;


	printf("歡迎來到神奇的撲克遊戲!!\n");


	gameInit(); //初始化


	int operation = 0; //存玩家的操作
	fillDeckFaceFirst(deck, face, suit); //填入牌

	int score[4] = {0}; //每個人的分數先歸零

	system("cls");
	while (true)
	{
		printf("目前分數:");
		for (i = 0; i < playerCount; i++)
		{
			printf("P%d:%d  ", i + 1, score[i]);
		}
		printf("\n動作選項:\n\t(1)重新開始(重設開始積分)\n\t(2) 繼續下一回\n\t(3) 不玩了\n");
		printf("Input your operation(1~3):");
		scanf("%d", &operation);

		if (operation == 1) //遊戲重置
		{
			system("cls");
			for (i = 0; i < 4; i++)
			{
				score[i] = 0; //歸零每個人的分數
			}
			printf("遊戲重制\n");
			gameInit(); //重新初始化
			system("cls");
			printf("遊戲已重制\n");
			system("pause");
			system("cls");
		}
		else if (operation == 2) //繼續遊戲
		{
			shuffle(deck); //洗牌


			int changeCardIndexBuffer[4][3]; //暫存要換的牌是哪些

			int scoreThisRoundTemp[4][3]; //暫存分數 4個玩家 前中後3個墩

			int currentPlayer; //現在的動作是針對哪位玩家


			//選要換的牌
			for (currentPlayer = 0; currentPlayer < playerCount; currentPlayer++)
			{
				system("cls");
				printf("\t現在輪到第%d位玩家\n\t你要跟第%d位玩家換牌\n", currentPlayer + 1, changeWithWhom[currentPlayer] + 1);
				printf("\n以下是您的牌\n");
				sort(handCard[currentPlayer], 13); //先整理
				deal(handCard[currentPlayer], 13); //再印出牌
				printf("\n請選擇三張你要換的牌:");
				for (i = 0; i < 3; i++)
				{
					scanf("%d", &changeCardIndexBuffer[currentPlayer][i]); //輸入要換的牌是哪幾張
					changeCardIndexBuffer[currentPlayer][i]--; //牌的編號是從1開始,但振烈index是從0開始
				}
			}
			system("cls");


			//開始換牌
			for (currentPlayer = 0; currentPlayer < playerCount; currentPlayer++)
			{
				for (i = 0; i < 3; i++)
				{
					//SWAP換牌

					//先暫存第currentPlayer位玩家手牌的第"currentPlayer要換的牌"張牌;
					//簡單來說就是要換出的牌
					Card temp = handCard[currentPlayer][changeCardIndexBuffer[currentPlayer][i]];

					//換牌目標:"要跟currentPlayer換牌的玩家" 的手牌的第"(要跟currentPlayer換牌的玩家)要換的牌" 張牌
					//簡單來說就是要換入的牌
					Card toBeChanged = handCard[changeWithWhom[currentPlayer]][changeCardIndexBuffer[changeWithWhom[
						currentPlayer]][i]];

					//兩張牌交換
					handCard[currentPlayer][changeCardIndexBuffer[currentPlayer][i]] = toBeChanged;
					handCard[changeWithWhom[currentPlayer]][changeCardIndexBuffer[changeWithWhom[currentPlayer]][i]] =
						temp;
				}

				//互換的對象不用重複換(假如規則是13互換,那互換執行在第一個玩家時第三位也換好了)
				if (changeWithWhom[0] == 1) //如果是1 2 互換，那互換的動作只需要作用在第1位和第3位就好
				{
					if (currentPlayer == 0)
					{
						currentPlayer++;
					}
					if (currentPlayer == 2)
					{
						break;
					}
				}
				else //如果是 13換 或是 14換，互換只要執行到第二位就全部換完了
				{
					if (currentPlayer == 1)
					{
						break;
					}
				}
			}


			//排列牌組，順便計算手牌分數
			for (currentPlayer = 0; currentPlayer < playerCount; currentPlayer++)
			{
				Card handCardTemp[13]; //暫存手牌
				int indexTemp; //暫存玩家輸入的牌的編號
				printf("\t現在輪到第%d位玩家\n", currentPlayer + 1);
				printf("\n以下是您的牌\n");
				sort(handCard[currentPlayer], 13);
				deal(handCard[currentPlayer], 13);

				//先把玩家輸入的排序存到handCardTemp中,再複製回handCard				
				//暫存前墩三張
				printf("請輸入前墩三張:");
				for (i = 0; i < 3; i++)
				{
					scanf("%d", &indexTemp);
					getchar();
					handCardTemp[i] = handCard[currentPlayer][indexTemp - 1];
				}

				//暫存中墩五張
				printf("請輸入中墩五張:");
				for (; i < 8; i++)
				{
					scanf("%d", &indexTemp);
					getchar();
					handCardTemp[i] = handCard[currentPlayer][indexTemp - 1];
				}
				//暫存後墩五張
				printf("請輸入後墩五張:");
				for (; i < 13; i++)
				{
					// scanf_s("%d", &indexTemp, 10);
					scanf("%d", &indexTemp);
					getchar();
					handCardTemp[i] = handCard[currentPlayer][indexTemp - 1];
				}

				//把handCardTemp再複製回handCard
				for (i = 0; i < 13; i++)
				{
					handCard[currentPlayer][i] = handCardTemp[i];
				}

				sort(handCard[currentPlayer], 3); //排序前墩
				sort(handCard[currentPlayer] + 3, 5); //排序中墩
				sort(handCard[currentPlayer] + 8, 5); //排序後墩

				printf("\n換牌後:\n");
				deal(handCard[currentPlayer], 13);

				//計算手牌的分數
				if (isStraight(handCard[currentPlayer], 13) == 1) //特例:如果是一條龍就讓她贏了
				{
					printf("你的牌是一條龍!!");
					for (i = 0; i < 3; i++)
					{
						scoreThisRoundTemp[currentPlayer][i] = 100000;
					}
				}
				else //正常狀況下存入分數
				{
					//利用determine印出牌型,利用giveScore手牌的分數
					printf("\n前墩: %s  分數:%d\n", cardType[determine(handCard[currentPlayer], 3)],
					       giveScore(handCard[currentPlayer], 3));
					printf("中墩: %s 分數:%d\n", cardType[determine(handCard[currentPlayer] + 3, 5)],
					       giveScore(handCard[currentPlayer] + 3, 5));
					printf("後墩: %s 分數:%d\n", cardType[determine(handCard[currentPlayer] + 8, 5)],
					       giveScore(handCard[currentPlayer] + 8, 5));


					//把各玩家的三墩分數暫存起來
					scoreThisRoundTemp[currentPlayer][0] = giveScore(handCard[currentPlayer], 3);
					scoreThisRoundTemp[currentPlayer][1] = giveScore(handCard[currentPlayer] + 3, 5);
					scoreThisRoundTemp[currentPlayer][2] = giveScore(handCard[currentPlayer] + 8, 5);


					//規則判斷: 後墩必須大於等於中墩，中墩必須大於等於前墩，否則視為三墩全輸
					if (scoreThisRoundTemp[currentPlayer][0] >= scoreThisRoundTemp[currentPlayer][1] ||
						scoreThisRoundTemp[currentPlayer][1] >= scoreThisRoundTemp[currentPlayer][2])
					{
						printf("\n但你違反了規則要你的排序方式,所以全部零分");
						printf("(後墩必須大於等於中墩，中墩必須大於等於前墩，否則視為三墩全輸)\n\n");
						for (i = 0; i < 3; i++)
						{
							scoreThisRoundTemp[currentPlayer][i] = 0; //分數值歸零
						}
					}
				}


				system("pause");
				system("cls");
			}


			system("cls");

			//計算比較各墩後的最終分數
			for (currentPlayer = 0; currentPlayer < playerCount; currentPlayer++)
			{
				printf("現在結算 第%d位玩家這輪的分數:\n", currentPlayer + 1);
				printf("\n\t前墩\t中墩\t後墩\n");
				printf("我自己\t%d\t%d\t%d\n", scoreThisRoundTemp[currentPlayer][0], scoreThisRoundTemp[currentPlayer][1],
				       scoreThisRoundTemp[currentPlayer][2]);
				printf("\n");
				int homerunCount = 0; //計算全贏的場數
				for (i = 0; i < playerCount; i++)
				{
					if (i != currentPlayer)
					{
						printf("第%d位\t%d\t%d\t%d\n", i + 1, scoreThisRoundTemp[i][0], scoreThisRoundTemp[i][1],
						       scoreThisRoundTemp[i][2]);
					}
				}
				printf("\n\n\n"); //印出和其他玩家的對照

				for (i = 0; i < playerCount; i++)
				{
					int winTimesWithOtherPlayer = 0; //儲存和另一個玩家比較 贏的場次
					int tieTimesWithOtherPlayer = 0; //儲存和另一個玩家比較 平手的場次
					if (i != currentPlayer) //不用和自己比
					{
						for (j = 0; j < 3; j++)
						{
							if (scoreThisRoundTemp[currentPlayer][j] > scoreThisRoundTemp[i][j])
							{
								winTimesWithOtherPlayer++;
							}
							if (scoreThisRoundTemp[currentPlayer][j] == scoreThisRoundTemp[i][j])
							{
								tieTimesWithOtherPlayer++;
							}
						}
						int totalPlus = 0; //暫存總分要加的分

						if (tieTimesWithOtherPlayer == 3) //全部平手 加0分
						{
							totalPlus = 0;
						}
						else if (winTimesWithOtherPlayer == 3) //全贏 加6分
						{
							score[currentPlayer] = score[currentPlayer] + 6;
							totalPlus = 6;
							homerunCount++;
						}
						else if (winTimesWithOtherPlayer == 2) //輸一場 加2分
						{
							score[currentPlayer] = score[currentPlayer] + 2;
							totalPlus = 2;
						}
						else if (winTimesWithOtherPlayer == 1) //贏一場 減1分
						{
							score[currentPlayer] = score[currentPlayer] - 1;
							totalPlus = -1;
						}
						else if (winTimesWithOtherPlayer == 0) //全輸 減6分
						{
							score[currentPlayer] = score[currentPlayer] - 6;
							totalPlus = -6;
						}


						printf("第%d位玩家 對第%d位玩家 總分加%d\n", currentPlayer + 1, i + 1, totalPlus);
					}
				}
				if (homerunCount >= 3 && playerCount == 4) //如果對其他玩家都是全贏,就HOMERUN(僅限4人局)
				{
					printf("恭喜你紅不讓了!!! 總分再加18分\n");
					score[currentPlayer] = score[currentPlayer] + 18;
				}

				printf("結果: 第%d位玩家總分:%d\n\n\n", currentPlayer + 1, score[currentPlayer]);
			}

			system("pause");
			system("cls");
		}
		else if (operation == 3) //結束遊戲
		{
			break;
		}
	}

	system("cls"); // 洗牌
	printf("感謝遊玩");

	system("pause");
	return 0;
}


void gameInit()
{
	int i;
	printf("請選擇遊戲人數:");
	scanf("%d", &playerCount);


	for (i = 0; i < 4; i++)
	{
		handCard[i] = deck + 13 * i;
	}
	if (playerCount == 4)
	{
		int buffer[4]; //暫存輸入的數字
		printf("制定換牌規則(輸入四個玩家代號 第一二互換 三四戶換):");
		for (i = 0; i < 4; i++)
		{
			//scanf_s("%d", &buffer[i], 10);
			scanf("%d", &buffer[i]);
			getchar();
		}

		//存入要跟誰換(index是從0開始，玩家編號是從 1開始)
		changeWithWhom[buffer[0] - 1] = buffer[1] - 1;
		changeWithWhom[buffer[1] - 1] = buffer[0] - 1; //輸入的1,2位互換
		changeWithWhom[buffer[2] - 1] = buffer[3] - 1;
		changeWithWhom[buffer[3] - 1] = buffer[2] - 1; //輸入的3,4位互換
	}
	else
	{
		changeWithWhom[0] = 1;
		changeWithWhom[1] = 0;
	}
}
