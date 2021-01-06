#include <cstdio>
#include <cstdlib>

#include "funclib.h"


//儲存各種牌型的分數
int cardTypeScore[] = {0, 60, 120, 200, 280, 360, 500, 800, 1000};

//以牌面數字為優先的填牌方式(梅花2,菱形2,紅心2,黑桃2,梅花3......)
void fillDeckFaceFirst(Card* const wDeck, const char* wFace[],
                       const char* wSuit[])
{
	int i = 0;
	int j;
	for (j = 0; j < 13; j++)
	{
		int k;
		for (k = 0; k < 4; k++)
		{
			wDeck[i].face = wFace[j]; //寫入牌面數字
			wDeck[i].suit = wSuit[k]; //寫入牌面花色
			wDeck[i].id = i; //給每一張牌id,越大的牌id越大(0至52)
			i++;
		}
	}
}


//隨機洗牌(洗52張wDeck的牌)
void shuffle(Card* const wDeck)
{
	int i, j;
	Card temp;

	for (i = 0; i <= 51; i++)
	{
		j = rand() % 52;
		temp = wDeck[i];
		wDeck[i] = wDeck[j];
		wDeck[j] = temp;
	}
}


//發牌(發wDeck的牌，共發quantity張)
void deal(const Card* const wDeck, int quantity)
{
	int i;

	for (i = 0; i < quantity; i++)
		//印出第幾張,甚麼牌,和他的id
		printf("\t第%d張牌 \t %5s of %-8s \t%d%c", i + 1, wDeck[i].face,
		       wDeck[i].suit, wDeck[i].id, '\n');
}

//依次印出所有的牌，並且分成playerCount,每份13張
void dealManyPlayer(int playerCount, const Card* const wDeck)
{
	int i;
	int max = 13 * playerCount - 1;
	int currentCardNum = 1;

	for (i = 0; i <= max; i++)
	{
		printf("\t第%d張牌 \t%d \t %5s of %-8s   %d%c   ", currentCardNum, i + 1, wDeck[i].face,
		       wDeck[i].suit, wDeck[i].id, '\n');
		currentCardNum++;
		if ((i + 1) % 13 == 0)
		{
			printf("\n\n");
			currentCardNum = 1;
		}
	}
}


//判斷同花(回傳1是，0不是)
int isFlush(Card wDeck[], int size)
{
	int isflush = 1; //先假設他是同花
	int i;
	int lastSuit = (wDeck[0].id + 1) % 4; //上一張牌的花色
	for (i = 0; i < size; i++)
	{
		//如果檢查到一張花色不一樣的,就沒有同花了
		if ((wDeck[i].id + 1) % 4 != lastSuit)
		{
			isflush = 0;
		}
		lastSuit = (wDeck[i].id + 1) % 4;
	}

	return isflush;
}

//判斷順(回傳1是，0不是)
int isStraight(Card wDeck[], int size)
{
	int isstraight = 1; //先假設他是順子
	int i;

	int lastFace = (wDeck[0].id) / 4; //上一張牌的牌面數字
	for (i = 1; i < size; i++)
	{
		//如果檢查到一張牌跟上一張不連續就不是順了
		if ((wDeck[i].id) / 4 != lastFace + 1)
		{
			isstraight = 0;
		}
		lastFace = (wDeck[i].id) / 4; //每檢查完一張這張牌就會變成下張的上一張牌
	}
	return isstraight;
}


//判斷特殊的(A2345)的順 (回傳1是，0不是)
int isSpecialStraight(Card wDeck[], int size)
{
	int isspecialstraightraight = 1; //先預設他是特殊順
	int i;

	if (wDeck[0].id / 4 != 0) //如果最id小的那張的數字不是2,就不成立
	{
		isspecialstraightraight = 0;
	}

	//和上面順一樣的檢查方式(這次檢查到倒數第二張)
	int lastFace = (wDeck[0].id) / 4;
	for (i = 1; i < size - 1; i++)
	{
		//如果檢查到一張牌跟上一張不連續就不是順了
		if ((wDeck[i].id) / 4 != lastFace + 1)
		{
			isspecialstraightraight = 0;
		}
		lastFace = (wDeck[i].id) / 4; //每檢查完一張這張牌就會變成下張的上一張牌
	}

	//id最大的那張牌數字應該要是A
	if (wDeck[size - 1].id / 4 != 12)
	{
		isspecialstraightraight = 0;
	}
	return isspecialstraightraight;
}


/*判斷給定值數量的牌出現幾次
  要判斷對子的話第一個參數就輸入2,三條就3,四條就4
  (回傳值是出現了幾次)
*/
int howManyOfaKind(int multipleKind, Card wDeck[], int size)
{
	int howMany = 0; //出現了幾次
	int i = 0;

	//從第i個開始往後找，看是不是同數字的剛好有multipleKind張
	while (i <= size - multipleKind)
	{
		int currentFace = (wDeck[i].id) / 4; //儲存這輪檢查牌的數字
		int sameCount = 0; //往後找看有幾張牌一樣
		int j = 0;

		while (currentFace == (wDeck[i + j].id) / 4 && j + i < size)
		{
			sameCount++;
			j++;
		}
		//如果一樣數字的牌的數量剛好和multipleKind,才算有找到我們要的
		if (sameCount == multipleKind)
		{
			i = i + multipleKind; //檢查過的牌不用再檢查
			howMany++;
		}
		else
		{
			i = i + sameCount; //檢查過的牌不用再檢查
		}
	}
	return howMany;
}


//排序(在wDeck中排序size張)
void sort(Card wDeck[], int size)
{
	//找到最小的就往前排
	for (int i = 0; i < size - 1; i++)
	{
		Card min = wDeck[i];
		int minIndex = i;
		int j;
		for (j = i; j < size; j++)
		{
			if (wDeck[j].id < min.id)
			{
				min = wDeck[j];
				minIndex = j;
			}
		}
		Card cardTemp;
		cardTemp = wDeck[i];
		wDeck[i] = wDeck[minIndex];
		wDeck[minIndex] = cardTemp;
	}
}

//要先SORT!!
//判斷wDeck中大小為size中的排是甚麼牌型
//回傳牌型的index
int determine(Card wDeck[], int size)
{
	sort(wDeck, size);
	if (isFlush(wDeck, size) == 1 && (isSpecialStraight(wDeck, size) || isStraight(wDeck, size) == 1) && size > 3)
	{
		return 8; //同花順 1000分
	}
	if (howManyOfaKind(4, wDeck, size) >= 1)
	{
		return 7; //四條 800分
	}
	if (howManyOfaKind(3, wDeck, size) >= 1 && howManyOfaKind(2, wDeck, size) >= 1)
	{
		return 6; //葫蘆 500分
	}
	if (isFlush(wDeck, size) == 1 && size > 3)
	{
		return 5; //同花 360分
	}
	if ((isSpecialStraight(wDeck, size) == 1 || isStraight(wDeck, size) == 1) && size > 3)
	{
		return 4; //順子 280分
	}
	if (howManyOfaKind(3, wDeck, size) >= 1)
	{
		return 3; //三條 200分
	}
	if (howManyOfaKind(2, wDeck, size) == 2)
	{
		return 2; //二對 120分
	}
	if (howManyOfaKind(2, wDeck, size) == 1)
	{
		return 1; //對子 60分
	}
	return 0; //散牌 0分
}


//要先SORT!!
/*找到一組牌中,可比較的最大牌是哪一張
  例如葫蘆中有一個三條加上一個對子，但要比較的是其中三條中的最大那張，而不是五張中最大的那張
  回傳是最重要那張的id*/
Card mostImportantCard(int multipleKind, Card wDeck[], int size)
{
	//基本上和howManyOfaKind5找法一樣，但這次是從後面找回來
	int i = size - 1; //從Size-1開始找
	Card veryImoprtantCard = wDeck[i];
	while (i >= multipleKind - 1)
	{
		int currentFace = (wDeck[i].id) / 4;
		int sameCount = 0;
		int j = 0;

		while (currentFace == (wDeck[i - j].id) / 4 && i - j >= 0)
		{
			sameCount++;
			j++;
		}
		if (sameCount == multipleKind)
		{
			veryImoprtantCard = wDeck[i];
			i = i - multipleKind;

			return veryImoprtantCard; //只要一找到就直接return了
		}
		i = i - sameCount;
	}
	return veryImoprtantCard;
}


//要先SORT!!
//給分，回傳是這副牌的分數(牌面分加牌型分)
int giveScore(Card wDeck[], int size)
{
	int type = determine(wDeck, size); //判斷牌型
	int score = 0;

	//牌面分數
	if (type == 7) //四條: 利用mostImportantCard找出四條中最大的那一張
	{
		score = score + mostImportantCard(4, wDeck, size).id;
	}
	else if (type == 6) //葫蘆: 利用mostImportantCard找出葫蘆裡三條中最大的那一張
	{
		score = score + mostImportantCard(3, wDeck, size).id;
	}
	else if (type == 3) //三條: 利用mostImportantCard找出三條中最大的那一張
	{
		score = score + mostImportantCard(3, wDeck, size).id;
	}
	else if (type == 2 || type == 1) //兩對或對子:找到最大對子的最大那張
	{
		score = score + mostImportantCard(2, wDeck, size).id;
	}
	else //其他牌型: 比較最大的那一張
	{
		score = score + (wDeck[size - 1].id);
	}

	score = score + cardTypeScore[type]; //再加上牌型分數
	return score;
}
