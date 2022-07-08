#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
// ROWS and COLS for the board
#define ROWS 20
#define COLS 30
// directions for the snake to move in
enum dir {
	UP, DOWN, LEFT, RIGHT
};
enum gameState { // state of the game
	PLAY, GAMEOVER, WINNER
};
typedef struct vector // position on the board
{
	int x, y;
}vec;
// game vairables
enum dir snakeDir = RIGHT;
enum gameState game = PLAY;
char board[ROWS][COLS+1];
int snakeLenght = 3;
int score = 0;
vec* snake;
vec fruit = { COLS / 2, ROWS / 2 };
char key;

// functions to run the game
void render();
void init();
void updateTail();
void updateSnake();
void input();
void updateFruit();
void logic();

void init() {
	// initial snake position
	vec x = { 1,3 };
	vec y = { 1,2 };
	vec z = { 1,1 };
	snake[0] = x;
	snake[1] = y;
	snake[2] = z;
	// filling up the board with spaces and the edges being #
	bool isEdge;
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			isEdge = (i == 0 || j == 0 || j == COLS - 1 || i == ROWS-1);
			board[i][j] = isEdge? '#' : ' ';
		}
		board[i][COLS] = '\0'; // adding a null terminator so printing cols are easier
	}
}

void render() { // goes through the board and prints each col
	for (int i = 0; i < ROWS; i++) 
		printf("%s\n",board[i]);
	printf("Score: %d\n",score);
}

void updateTail() { // starting at the end of the tail, each part of the snake takes the position of the one ahead of it, except for the head of the snake
	int endOfTail = snakeLenght - 1; // calculating the end of the tail
	board[snake[endOfTail].y][snake[endOfTail].x] = ' '; // clearing the board space where the end of the tail was
	// the loop through each tail part and give it the value of the one in front
	for (int i = endOfTail; i > 0; i--) {
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}
}

void updateSnake() {
	updateTail(); // updates the tail before moving the head
	switch (snakeDir) { // calculates where the head should be next
		//checks for boundaries, if it is at boundary then it moves it to the opposite end
		case UP:
			snake[0].y = (snake[0].y == 1) ? ROWS - 2 : snake[0].y - 1;
			break;
		case DOWN:
			snake[0].y = (snake[0].y == ROWS-2) ? 1 : snake[0].y + 1;
			break;
		case LEFT:
			snake[0].x = (snake[0].x == 1) ? COLS - 2 : snake[0].x - 1;
			break;
		case RIGHT:
			snake[0].x = (snake[0].x == COLS - 2) ? 1 : snake[0].x + 1;
			break;
	}
	// updates the board with the snake position
	for (int i = 0; i < snakeLenght; i++) {
		board[snake[i].y][snake[i].x] = 'x';
	}
}

void input() {
	// checks if keyboard is pressed
	if (_kbhit()) {
		key = _getch(); // captures the charecter
		// based on the charecter we will change the direction
		// it does not allow you to move 180 degrees, only 90 degree turns ex. if you are going up you cannot change directions to down, only left or right
		switch (key) 
		{
		case 'w':
			snakeDir = (snakeDir == DOWN) ? DOWN : UP;
			break;
		case 'a':
			snakeDir = (snakeDir == RIGHT) ? RIGHT : LEFT;
			break;
		case 's':
			snakeDir = (snakeDir == UP) ? UP : DOWN;
			break;
		case 'd':
			snakeDir = (snakeDir == LEFT) ? LEFT : RIGHT;
			break;
		case 'q':
			game = GAMEOVER;
			break;
		}
		fflush(stdin);
	}
}

void updateFruit() {  // draws the fruit on the board
	board[fruit.y][fruit.x] = '*';
}

void logic() {
	if ((snake[0].x == fruit.x) && (snake[0].y == fruit.y)) { // checks if you captured the fruits
		score++;
		snakeLenght++; // snake gets longer
		if (score == 10) { // if you reached a certain snake lenght / score then you win
			game = WINNER;
			return;
		}
		// updates the new snake end with the previous snake end value
		vec temp = { snake[snakeLenght - 2].x, snake[snakeLenght - 2].y }; 
		snake[snakeLenght - 1].x = temp.x;
		snake[snakeLenght - 1].y = temp.y;
		
		// update the fruit position in a spot the snake is not occupying
		bool isTaken = false;
		do {
			// random position within the boundary
			fruit.x = rand() % (COLS - 1);
			fruit.y = rand() % (ROWS - 1);
			// checks if the snake exists within this new position
			for (int i = 0; i < snakeLenght; i++) {
				if (snake[i].x == fruit.x || snake[i].y == fruit.y || fruit.x == 0 || fruit.x == COLS-1 || fruit.y == 0 || fruit.x == ROWS-1) {
					isTaken = true;
					break;
				}
				isTaken = false;
			}

		} while (isTaken); // continues to loop until the fruit is in a spot the snake does not occupy
	}
	else { // checks if the head of the snake hit any part of the tail
		for (int i = 1; i < snakeLenght; i++) {
			if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
				game = GAMEOVER;
				break;
			}

		}
	}
	updateFruit();
}


int main() {
	snake = malloc(sizeof(vec) * ((ROWS + COLS)/2)); // the max size of the snake is half the size of the board
	init();
	updateSnake();

	while (game == PLAY) {
		input();
		updateSnake();
		render();
		logic();
		Sleep(0.4);
		system("cls");
	}
	if (game == GAMEOVER) {
		printf("YOU LOSE!\n");
		printf("Final Score: %d\n", score);
	}
	else if (game == WINNER) {
		printf("YOU WIN!\n");
		printf("Final Score: %d\n", score);
	}
	free(snake);
}