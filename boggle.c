// Name: Tyler Gauntlett
// NID: ty340586
// Date 11/21/2015
// Course COP 3502
// Assingment: Boggle

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BOARDSIZE 4
#define UNSEDSPACE -1
#define USED 1


struct trie {
	int isWord;
	struct trie* next[26];
};



// Arrays create a clockwise search in 8 directions.
const int DIRECTIONS = 8;
const int DX[] = { 0, -1, -1, -1, 1, 1, 1, 0 };
const int DY[] = { 1, 0, 1, 1, -1, 0, -1, -1 };

struct trie* init();
void insert(struct trie* tree, char word[], int k);
void freeDictionary(struct trie* tree);
void setGameBoard(char gameBoard[][BOARDSIZE + 1]);
void setUsedBoard(int usedBoard[][BOARDSIZE]);
void getCord(char gameBoard[][BOARDSIZE + 1], int usedBoard[][BOARDSIZE], struct trie* tree);
void searhTrie(char gameBoard[][BOARDSIZE + 1], int usedBoard[][BOARDSIZE], struct trie* tree, int currentX, int currentY, char word[], int index);


int main(){

	
	// Read in number of words.
	int n, cases, i;
	
	// Open file and initialize dictionary trie.
	struct trie* myDictionary = init();
	FILE* ifp = fopen("dictionary.txt", "r");
	fscanf(ifp, "%d", &n);
	
	for (i = 0; i<n; i++) {
		char word[100];
		fscanf(ifp, "%s", word);
		insert(myDictionary, word, 0);
	}
	

	// Loop based on cases.
	scanf("%d", &cases);
	while (cases > 0){


		// Initilizes both boards.
		int usedBoard[BOARDSIZE][BOARDSIZE];
		char gameBoard[BOARDSIZE][BOARDSIZE + 1];

		// Function that returns both boards.
		setUsedBoard(usedBoard);
		setGameBoard(gameBoard);

		// Function that solves boggle.
		getCord(gameBoard, usedBoard, myDictionary);
		
		cases--;
	}
	

	fclose(ifp);
	freeDictionary(myDictionary);

	system("pause");
	return 0;
}


void setGameBoard(char gameBoard[][BOARDSIZE + 1]){

	int i;

	for (i = 0; i < BOARDSIZE; i++)
		scanf("%s", gameBoard[i]);

}

void setUsedBoard(int usedBoard[][BOARDSIZE]){

	int i, j;

	for (i = 0; i < BOARDSIZE; i++){
		for (j = 0; j < BOARDSIZE; j++){
			usedBoard[i][j] = -1;
		}
	}

}


struct trie* init() {

	// Create the struct, not a word.
	struct trie* myTree = malloc(sizeof(struct trie));
	myTree->isWord = 0;

	// Set each pointer to NULLL.
	int i;
	for (i = 0; i<26; i++)
		myTree->next[i] = NULL;

	// Return a pointer to the new root.
	return myTree;
}


// Takes in a string and inserts it into a trie.
void insert(struct trie* tree, char word[], int k) {

	// Down to the end, insert the word.
	if (k == strlen(word)) {
		tree->isWord = 1;
		return;
	}

	// See if the next place to go exists, if not, create it.
	int nextIndex = word[k] - 'a';
	if (tree->next[nextIndex] == NULL)
		tree->next[nextIndex] = init();

	insert(tree->next[nextIndex], word, k + 1);
}



void searhTrie(char gameBoard[][BOARDSIZE+1], int usedBoard[][BOARDSIZE], struct trie* tree, int currentX, int currentY, char word[], int index){	
	
	int i;
	
	word[index] = '\0';


	// If the the word is in the dictionary, print and return.
	if (tree->isWord == 1){
		printf("%s", word);
	}

	for (i = 0; i < DIRECTIONS; i++){

		int newX = currentX + DX[i];
		int newY = currentY + DY[i];

		// Check if in bounds and available on used board.
		if ((currentX >= 0) && (currentY >= 0) && (usedBoard[currentX][currentY] == UNSEDSPACE)) {

			// Mark usedBoard with a used space.
			usedBoard[currentX][currentY] = USED;

			// Assign word the char stored in gameBoard.
			word[index] = gameBoard[currentX][currentY];

			int nextIndex = word[index] - 'a';

			if (tree->next[nextIndex] != NULL){
				searhTrie(gameBoard, usedBoard, tree->next[nextIndex], newX, newY, word, index + 1);
			}

		}
	
		

	}

	// Mark usedBoard with a used space.
	usedBoard[currentX][currentY] = UNSEDSPACE;



}

	


void getCord(char gameBoard[][BOARDSIZE + 1], int usedBoard[][BOARDSIZE], struct trie* tree){

	int i, j, index = 0;
	char buffer[20];

	for (i = 0; i < BOARDSIZE; i++){

		for (j = 0; j < BOARDSIZE; j++){

			int k = 0;

			// Resets the used array after every use.
			setUsedBoard(usedBoard);

			// Reseats the buffer after every use.
			memset(buffer, 0, sizeof(buffer));

			// Create a buffer consiting of 1 char to be passed to search.
			// This buffer will roate through all 16 possible starting points.
			buffer[k] = gameBoard[i][j];
			
			// Call search recursive function.
			searhTrie(gameBoard, usedBoard, tree, i, j, buffer, index);

			
		}
	}
}


// Just frees all the memory pointed to by tree (directly and indirectly)
void freeDictionary(struct trie* tree) {

	int i;
	for (i = 0; i<26; i++)
		if (tree->next[i] != NULL)
			freeDictionary(tree->next[i]);

	free(tree);
}
