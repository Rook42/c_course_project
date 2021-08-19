include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define OPTIONS 5 // number of menu options defined for knowing array length
#define MAXATTEMPTS 3 // maximum number of attempts per type of problem


enum states {Menu, Addition, Subtraction, Multiplication, Division, Exit};

//keeps track of program state (which operation selected)
int state = Menu;

//total problems attempted (name from assignment requirements)
int ttlProblems = 0;

//problems answered correctly (name from assignment requirements)
int correctAnswers = 0;

// functions:
//     views

void stateController();
int mainMenu();
void operation(int state);
void round(float* const correctptr);
void quit();
char difficulty();

//     random number generation
const float* generateNumbers(char difficultyLevel);

//     evaluation & scoring
const char* evaluate(float answer, float correct);

//     math operations

float addition      (float num1, float num2);
float subtraction   (float num1, float num2);
float multiplication(float num1, float num2);
float division      (float num1, float num2);



// struct to hold data/relationships
typedef struct
{
	char name[50];
	char prompt[50];
	float (*operation) (float, float);// <- pointer to a matching function
	int attempts;// number of attempts for a given type of problem
	int correct; // number of correct answers for a given type of problem
} View;

// main menu strings
static View mainMenuView =
{{"Math Practice Program Main Menu"}, {"Select an option: "}};

// menu options with their respective functions, attempts & correct attempts
static View menuOptions[OPTIONS] =
{
{{"Addition      "},{"+"}, addition, MAXATTEMPTS,0},
{{"Subtraction   "},{"-"}, subtraction, MAXATTEMPTS,0},
{{"Multiplication"},{"*"}, multiplication, MAXATTEMPTS,0},
{{"Division      "},{"/"}, division, MAXATTEMPTS,0},
{{"Exit          "}}
};


int main()
{
	srand((int)time(NULL)); //start rng

	stateController(); //give control to stateController and run

	return 0;
}

//keeps track of state variable & loops between menu & selected event
void stateController() 
{
	// loop until last entry (Exit) in options is selected
	while (state < OPTIONS - 1) 
	{
			state = mainMenu(); //get selected action
			
			//call selected view function
			switch (state + 1){
			case Exit:
				quit();
				break;

			default:
				operation(state);
			}
	}
}

//view function to create main menu and return selected item
int mainMenu()
{
	//store validated user input
	int selected = Menu;

	//purge garbage left in buffer if user enters chars instead of ints
	char trash = '\0';
	
	while (selected == Menu) 
	{
		printf("%s\n\n", mainMenuView.name);
		int i;
		for (i = 0; i < OPTIONS - 1; i++)
		{
			//print a numbered menu with the option names, attempts, correct except exit
			printf("%d. %s\t\tAttempts Left: %d/%d\tCorrect: %d\n",
				i + 1,
				menuOptions[i].name,
				menuOptions[i].attempts,
				MAXATTEMPTS,
				menuOptions[i].correct);
		}
		//print exit
		printf("%d. %s\n", Exit, menuOptions[OPTIONS - 1].name); 
		printf("%s", mainMenuView.prompt);
		scanf_s("%d", &selected);
		scanf_s("%c", &trash, 1);
		// don't allow erroneous entry
		if (selected <= Menu || selected > OPTIONS)
		{
			selected = Menu;
			printf("Please select from the available options.\n\n");
		}
		// don't allow options with no attempts to be selected
		else if (menuOptions[selected - 1].attempts == 0 && selected != Exit)
		{
			selected = Menu;
			printf("Please select an option with attempts left, or exit.\n\n");
		}
	}
	// return one less than selected b/c arrays start at 0
	return --selected;
}

//view function to prompt for & return difficulty selection (E, M, H)
char difficulty()
{
	//temp holds input before validated
	char temp = '0';
	char out = '0';

	printf("Select difficulty level e-easy m-medium h-hard ");

	//validate input		
	while (temp != 'E' && temp != 'M' && temp != 'H')
	{
		scanf_s(" %c", &temp, 1);
		temp = toupper(temp);
		
		if (temp != 'E' && temp != 'M' && temp != 'H')
		{
			printf("Please enter e, m, or h ");
		}
	}
	out = temp;
	//print selection as in assignment example screenshot
	printf("%c\n\n", out);
	return out;
}

//view function for operations besides division and exit
void operation(int state)
{
	// generate & display problem prompt, evaluate if response correct
	
	//variable to hold user input
	float answer = 0; 

	//purge garbage left in buffer if user enters chars
	char trash = '\0';

	//prompt user for difficulty
	char difficultyLevel = difficulty();

	//get random numbers
	const float* nums = generateNumbers(difficultyLevel);
	float num1 = nums[0];
	float num2 = nums[1];

	//actual answer
	float correct = menuOptions[state].operation(num1, num2);

	//round answer & tell user if decimal beyond hundredths
	if (((correct * 100) - (int)(correct * 100)) != 0)
	{ 
		float* const correctptr = &correct;
		round(correctptr);
	}

	// print prompt, get user's answer.
	printf("%.0f %s %.0f = ? ", num1, menuOptions[state].prompt, num2);
	scanf_s("%f", &answer);
	scanf_s("%c", &trash, 1);
	printf("%s", evaluate(correct, answer));
}

void round(float* const correctptr)
{
	float correct = *correctptr;
	//Round the answer
	//Cut off all decimals after thousandths by abusing typecasting
	correct = (float)((int)(1000 * correct));
	//Round up if necessary
	if ((int)correct % 10 >= 5) { correct += 10; }
	//Cut off thousandths and return decimal place to proper position 
	correct = (float)((int)(correct / 10)) / 100.0f;
	//Inform user that answer has been rounded to hundredths
	printf("Round to the nearest hundredth.\n");
	*correctptr = correct;
}

//function for exit case. Displays score, etc
void quit() 
{
	//If 0 questions attempted, divide 0 by 1 instead. Double used here for accuracy.
	double percent = 100.0 * correctAnswers / ((ttlProblems == 0.0) ? 1.0 : ttlProblems);
	printf("You attempted %d problems and got %d correct\n", ttlProblems, correctAnswers);
	printf("For a percentage correct of %4.2f percent\n", percent);
}

//generate a size 2 random float array of given difficulty
const float* generateNumbers(char difficultyLevel)
{
	static float out[2];
	float temp;
	//easy
	if (difficultyLevel == 'E')
	{
		//variables w/ random digits 1-10 (so no division by zero)
		out[0] = (float)(rand() % 10) + 1;
		out[1] = (float)(rand() % 10) + 1;
	}
	//medium
	else if (difficultyLevel == 'M')
	{
		//variables w/ random digits 1-100 
		out[0] = (float)(rand() % 100) + 1;
		out[1] = (float)(rand() % 100) + 1;
	}
	//hard
	else
	{
		out[0] = (float)(rand() % 1000) + 1;
		out[1] = (float)(rand() % 1000) + 1;
	}
	
	//make out[0] always greater than out[1] (no division answer < 1)
	if (out[0] < out[1])
	{
		temp = out[0];
		out[0] = out[1];
		out[1] = temp;
	}

	return out;
}

const char* evaluate(float answer, float correct)
{
	const char * out;
	menuOptions[state].attempts--;
	ttlProblems++;
	if (answer == correct)
	{
		out = "Correct!\n\n";
		menuOptions[state].correct++;
		correctAnswers++;
	}
	else
	{
		out = "Sorry, that's incorrect.\n\n";
	}
	return out;
}

// math functions

float addition(float num1, float num2) 
{
	return (num1 + num2);
}

float subtraction(float num1, float num2)
{
	return (num1 - num2);
}

float multiplication(float num1, float num2)
{
	return (num1 * num2);
}

float division(float num1, float num2)
{
	return (num1 / num2);
}