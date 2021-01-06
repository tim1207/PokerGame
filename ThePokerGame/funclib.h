#ifndef FIG1003_H  // Á×§K­«ÂÐ¥]§t¼ÐÀYÀÉ
#define FIG1003_H


struct card {
    const char* face;
    const char* suit;

    int id;
};



typedef struct card Card;

void fillDeck(Card* const, const char* [], const char* []);	// ³]µPªì©l­È
void shuffle(Card* const);																		// ¬~µP
void deal(const Card* const,int quantity);

void dealManyPlayer(int playerCount, const Card* const wDeck);

void fillDeckFaceFirst(Card* const wDeck, const char* wFace[],const char* wSuit[]);


int isFlush(Card wDeck[], int size);
int isStraight(Card wDeck[], int size);
Card mostImportantCard(Card wDeck[], int size);
int giveScore(Card wDeck[], int size);
int howManyOfaKind(int multipleKind, Card wDeck[], int size);
int determine(Card wDeck[], int size);
void sort(Card wDeck[], int size);
#endif

