//Kevin Yang    ICS3U    Ms. Cullum     3/20/18

/*This program simulates a simplified version of Yahtzee,
where the user can let the computer roll willy-nilly,
select which dice to roll themself, or let the computer
decide the quickest path to yahtzee. */


#pragma warning(disable:4996)
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define DICE_COUNT 5   //number of dice (5 as per requirement)

struct dice_data {   //each dice has a face value and roll count
	int faceValue;
	int rollCount;
};

//Function for menu
char get_choice(void);

//Functions shared between level 3+, 4, and 4+
void rollDice(struct dice_data * dice);
bool checkYahtzee(struct dice_data * dice, int diceCount);
void printDice(struct dice_data  * dice, int turn, int diceCount);


//Functions for level 3+ only
void lvl3Plus(void);


//Functions for level 4 only
void lvl4(void);
void readUserInput(int * diceToReroll, int rerollAmount, int diceCount);


//Functions for level 4+ only
void lvl4Plus(void);
void decide(struct dice_data * dice, bool * flag);


//User selects which game to play
int main(void) {

	char choice;

	while ((choice = get_choice()) != 'q') {  //game loops until get_choice returns 'q'

		switch (choice) {

		case 'a':
			lvl3Plus();

		case 'b':
			lvl4();
			break;

		case 'c':
			lvl4Plus();
			break;

		case 'q':
			exit(0);

		default:
			printf("Program error\n"); //should never occur
			break;

		}
	}
	exit(0);  //stops program if user enters 'q'
}

//Gets user input on which version to play and returns their choice to main
char get_choice(void) {
	char choice;

	printf("Enter the letter of your choice:\n");
	printf("\na. Computer random roll (level 3+) \t b. User rolls dice (level 4)");
	printf("\nc. Computer algorithm (level 4+) \t q. Quit\n");

	fflush(stdin);  //flush buffer in case user inputed more than one character
	choice = getchar();

	while (choice < 'a' || choice > 'c' && choice != 'q') {
		printf("Invalid choice. Please enter a, b, c, or q: ");
		fflush(stdin);
		choice = getchar();
	}
	return choice;
}

//Level 3+ -specific functions begin
//Computer rolls all five dice randomly each turn until Yahtzee is achieved
void lvl3Plus() {

	struct dice_data dice[DICE_COUNT];  //each element of dice[] has a .faceValue and .rollCount
	bool yahtzee = false;
	int i = 0;
	int totalRolls = 0;
	int turn = 1;
	int diceCount = DICE_COUNT;

	srand(time(0));

	printf("\nLevel 3+ game. ");
	system("pause");

	//Initialization
	for (i = 0; i < diceCount; i++)
	{
		dice[i].faceValue = 0;
		dice[i].rollCount = 0;
	}

	//Roll all 5 dice for turn 1
	for (i = 0; i < diceCount; i++) {
		rollDice(&(dice[i]));
	}

	printDice(dice, turn++, diceCount); //function prints each die's face value and increments rollCount of each dice

	while (!checkYahtzee(dice, diceCount)) {  //loop until checkYahtzee returns true

		for (int i = 0; i < diceCount; i++) {  //send all five elements of dice[] to rollDice
			rollDice(&(dice[i]));
		}

		printDice(dice, turn++, diceCount);
	}

	printf("\nYahtzee! ");

	for (i = 0; i < diceCount; i++) {
		totalRolls += dice[i].rollCount;
	}

	printf("It took %d turns and %d rolls.\n\n\n", turn - 1, totalRolls);
	main();  //return to menu

}
//Level 3+ -specific functions end


//Level 4-specific functions begin
void lvl4() {

	struct dice_data dice[DICE_COUNT];
	bool yahtzee = false;
	int i;
	int rerollAmount;  //user-defined quantity of dice to re-roll
	int diceToReroll[DICE_COUNT] = { 0 };  //array holding the dice numbers that the user selects to re-roll (not all elements may be populated)
	int totalRolls = 0;
	int turn = 1;
	int diceCount = DICE_COUNT;

	srand(time(0));

	//Initialization
	for (i = 0; i < diceCount; i++)
	{
		dice[i].faceValue = 0;
		dice[i].rollCount = 0;
	}

	// Roll all 5 dice on turn 1
	for (i = 0; i < diceCount; i++)
	{
		rollDice(&(dice[i]));
	}
	printf("\n");

	printDice(dice, turn++, diceCount);  //print info for turn 1


	while (!checkYahtzee(dice, diceCount)) {  //loop until yahtzee achievced

		fflush(stdin);
		printf("How many dice do you want to re-roll?: ");
		scanf("%d", &rerollAmount);
		printf("\n");

		fflush(stdin);  //clear buffer in case user entered more than 1 number

						//Input validation
		while (rerollAmount < 0 || rerollAmount > diceCount) {
			printf("Invalid quantity of dice to re-roll (must be from 1 to %d): ", diceCount);
			fflush(stdin);
			scanf("%d", &rerollAmount);
		}

		readUserInput(diceToReroll, rerollAmount, diceCount);

		//the core of the lvl 4 program
		for (i = 0; i < rerollAmount; i++) {
			int j = diceToReroll[i] - 1;    //transfer the value in each element of diceToReroll[] to dice[]
			rollDice(&(dice[j]));  //only re-roll dice that user specified
		}

		printDice(dice, turn++, diceCount);
	}

	printf("\nYahtzee! ");

	for (i = 0; i < 5; i++) {
		totalRolls += dice[i].rollCount;
	}

	printf("It took %d turns and %d rolls.\n\n\n", turn - 1, totalRolls);
	main(); //return to menu
}


//Function gets user's input on which dice to re-roll and stores it in dieToReroll array; no return value since uses pointers
void readUserInput(int * diceToReroll, int rerollAmount, int diceCount)
{
	int i;

	printf("Enter the dice you wish to re-roll (separated by space): "); //i.e. 1, 2, 3, 4, 5
	for (i = 0; i < rerollAmount; i++) {  //each iteration of loop scans for user input in buffer; rerollAmount specifies when to stop scanning
		scanf("%d", &(diceToReroll[i]));
	}

	//Input validation; check if user entered valid dice numbers
	for (i = 0; i < rerollAmount; i++) {
		if (diceToReroll[i] < 1 || diceToReroll[i] > diceCount) {
			printf("Invalid dice (must be 1 to %d). ", diceCount);
			fflush(stdin);
			readUserInput(diceToReroll, rerollAmount, diceCount);  //recursion; restarts function if input invalid
		}
	}

	printf("\n");

}
//level 4-specific functions end


//level 4+ -specific functions begin
void lvl4Plus() {

	struct dice_data dice[DICE_COUNT];
	bool yahtzee = false;
	int i;
	bool flag[DICE_COUNT] = { false };  //dice to-be-rerolled get flagged
	int totalRolls = 0;
	int turn = 1;
	int diceCount = DICE_COUNT;

	srand(time(0));

	//Initialize rollValue and rollCount of each die
	for (i = 0; i < diceCount; i++) {
		dice[i].faceValue = 0;
		dice[i].rollCount = 0;
	}

	//Roll all dice on turn 1
	for (i = 0; i < diceCount; i++) {
		rollDice(&(dice[i]));
	}

	printDice(dice, turn++, diceCount);

	decide(dice, flag); //check if there are 2 or more of the same face value


	while (!checkYahtzee(dice, diceCount)) {

		for (i = 0; i < diceCount; i++)
		{
			if (flag[i] == true) {  //only re-roll dices that have been flagged (flagged = true)
				rollDice(&(dice[i]));
			}
		}

		printDice(dice, turn++, diceCount);

		decide(dice, flag);

	}

	printf("\nYahtzee! ");

	//add up rollCount of each die
	for (i = 0; i < 5; i++) {
		totalRolls += dice[i].rollCount;
	}

	printf("It took %d turns and %d rolls.\n\n\n", turn - 1, totalRolls);
	main(); //return to menu
}

//Function determines which values are most frequently rolled, and flags all dice who do not have that value
//The flagged dice are re-rolled until their value matches the most frequently occuring value, at which point they're untouched
void decide(struct dice_data * dice, bool * flag) {
	int i, j;
	int valueOccurence[7] = { 0 }; //i is the dice's face value and valueOccurence[i] is how many times that value has been rolled
								   //since a die has values 1,2,3,4,5,6, only elements 1 through 6 of the array are populated for clarity (element 0 unused, total of 7 elements)
	int maxSame = 0;  //max number of times any value is rolled
	int mostFreqValue = 0;  //most frequent face value rolled

							//any die previously flagged for re-roll is unflagged (or else those dice would continue to get re-rolled)
	for (i = 0; i < 5; i++) {
		flag[i] = false;
	}

	//determine how many times each value is rolled
	for (i = 0; i < 5; i++) {
		for (j = 1; j <= 6; j++) {  //1 to 6 since dice has 6 sides
			if (dice[i].faceValue == j) {  //if j matches the value rolled, the occurance of that value is increased by 1
				valueOccurence[j]++;
			}
		}
	}

	//determine which value appeared most
	for (i = 1; i <= 6; i++) {
		if (valueOccurence[i]>maxSame) {  //if a value is rolled more than another value, it becomes the most-rolled value (maxSame)
			maxSame = valueOccurence[i];
			mostFreqValue = i;  //i is the face value
		}
	}

	//any dice whose value != the most frequently occuring value is flagged for re-roll
	for (i = 0; i < 5; i++) {
		if (dice[i].faceValue != mostFreqValue) {
			flag[i] = true;  //true = to be re-rolled
		}
	}
}

//lvl 4+ end



//Functions shared between lvl 3, 4, and 4+:

//Function assigns a random number to each member of dice[i].faceValue; no return value since pointers
void rollDice(struct dice_data * dice)
{
	dice->faceValue = rand() % 6 + 1; //-> means *(dice).rollValue; must dereference pointer in order to modify the variable it points to
	dice->rollCount++;
}

//Function prints the value rolled on each die and the number of turns; no return value
void printDice(struct dice_data * dice, int turn, int diceCount) {
	int i;

	printf("Turn %d: ", turn);
	for (i = 0; i < diceCount; i++)
	{
		printf("Dice %d: %d \t", i + 1, dice[i].faceValue); //array notation can be used interchangeably with pointers; dice[i].rollValue same as (dice+i)->rollValue
															//likewise, dice[i] = *(dice+i), since the name of an array is a pointer to its first element
	}
	printf("\n");
}

//Function checks for yahtzee (5 of the same face value); returns whether yahtzee is achieved (true/false)
bool checkYahtzee(struct dice_data * dice, int diceCount)
{
	bool yahtzee = true;
	int  i;

	for (i = 1; i < diceCount; i++) {
		if (dice[i].faceValue != dice[0].faceValue) {   //compares the value of the first dice to the values of other dice
			yahtzee = false;  //any die with a value different from the first die's automatically precludes a yahtzee
			break;
		}
	}
	return (yahtzee);
}
