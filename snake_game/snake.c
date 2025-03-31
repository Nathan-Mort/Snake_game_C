#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define HEIGHT 20
#define WIDTH 45
#define STOP 0
#define UP 1
#define LEFT 2
#define RIGHT 3
#define DOWN 4
#define FOOD '*'
#define MAX_LENGTH 773 // (HEIGHT - 2) * (WIDTH - 2) - 1
#define MAX_SCORE 23190 // 773 * 30

void gotoxy(int x, int y);
void hide_cursor();
void draw(char sheild[HEIGHT][WIDTH], int score);
char init_sheild(char sheild[HEIGHT][WIDTH],  int snake_head_x, int snake_head_y, char snake_head, int food_x, int food_y, int snake_body_x[MAX_LENGTH], int snake_body_y[MAX_LENGTH], int snake_length);
void change_direction(int *direction, char* snake_head, char choice);
void logic(int* direction, int* snake_head_x, int* snake_head_y, int* food_x, int* food_y, int* score, int snake_body_x[MAX_LENGTH], int snake_body_y[MAX_LENGTH], int* snake_length);

int main(){
    hide_cursor(); // Скрыть курсор
    char sheild[HEIGHT][WIDTH], next_sheild[HEIGHT][WIDTH]; // Два буфера
    char snake_head = '^', choice;
    int snake_head_x = 10, snake_head_y = 12, dir = UP, food_x = rand() % (WIDTH - 2) + 1, food_y = rand() % (HEIGHT - 2) + 1, score = 0; 
    int snake_body_x[MAX_LENGTH], snake_body_y[MAX_LENGTH], snake_length = 0;

    while(dir != STOP && score != MAX_SCORE){
        if (_kbhit()) {
            choice = _getch();
            change_direction(&dir, &snake_head, choice);
        }
        logic(&dir, &snake_head_x, &snake_head_y, &food_x, &food_y, &score, snake_body_x, snake_body_y, &snake_length);
        init_sheild(next_sheild, snake_head_x, snake_head_y, snake_head, food_x, food_y, snake_body_x, snake_body_y, snake_length);
        gotoxy(0, 0);
        draw(next_sheild, score);
        Sleep(40);
    }
    return 0;
}

void draw(char sheild[HEIGHT][WIDTH], int score){
    printf("\n\nSCORE: %d\n", score);
    for(int i = 0 ; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            if(j == WIDTH - 1 && i != HEIGHT - 1) printf("%c\n", sheild[i][j]);
            else printf("%c", sheild[i][j]);
        }
    }
}

char init_sheild(char sheild[HEIGHT][WIDTH], int snake_head_x, int snake_head_y, char snake_head, int food_x, int food_y, int snake_body_x[MAX_LENGTH], int snake_body_y[MAX_LENGTH], int snake_length){
    for(int i = 0 ; i < HEIGHT; i++){
        sheild[i][0] = '|';
        sheild[i][WIDTH - 1] = '|';
    }
    for(int j = 0 ; j < WIDTH; j++){
        sheild[0][j] = '#';
        sheild[HEIGHT - 1][j] = '#';
    }
    for(int i = 1 ; i < HEIGHT - 1; i++){
        for(int j = 1; j < WIDTH - 1; j++){
            if(i == snake_head_y && j == snake_head_x) sheild[i][j] = snake_head;
            else if(i == food_y && j == food_x) sheild[i][j] = FOOD;
            else {       
                int is_body_part = 0;
                for (int k = 0; k < snake_length; k++) {
                    if (snake_body_x[k] == j && snake_body_y[k] == i) {
                        sheild[i][j] = 'o'; // Отрисовка сегмента тела
                        is_body_part = 1;
                        break;
                    }
                }
                if (!is_body_part) {
                    sheild[i][j] = ' ';
                }}
        }
    }
}

void change_direction(int *direction, char* snake_head, char choice){
    switch(choice){
        case 'w':
            *direction = UP;
            *snake_head = '^';
            break;
        case 's':
            *direction = DOWN;
            *snake_head = 'v';
            break;
        case 'a':
            *direction = LEFT;
            *snake_head = '<';
            break;
        case 'd':
            *direction = RIGHT;
            *snake_head = '>';
            break;  
        case 'q':
            *direction = STOP;
            break;
        default:
            break;
    }
}

void logic(int* direction, int* snake_head_x, int* snake_head_y, int* food_x, int* food_y, int* score, int snake_body_x[MAX_LENGTH], int snake_body_y[MAX_LENGTH], int* snake_length){
    int prev_x = *snake_head_x;
    int prev_y = *snake_head_y;
    switch(*direction){
        case UP:
            (*snake_head_y)--;
            break;
        case DOWN:
            (*snake_head_y)++; 
            break;
        case LEFT:
            (*snake_head_x)--; 
            break;
        case RIGHT:
            (*snake_head_x)++;
            break;
        default:
            break;
    }

    for(int i = *snake_length; i > 0; i--){
        snake_body_x[i] = snake_body_x[i - 1];
        snake_body_y[i] = snake_body_y[i - 1];
    }

    for(int i = 0; i < *snake_length; i++){
        if(*snake_head_x == snake_body_x[i] && *snake_head_y == snake_body_y[i]) *direction = STOP;
        if(*food_x == snake_body_x[i] && *food_y == snake_body_y[i]){
            *food_x = rand() % (WIDTH - 2) + 1;
            *food_y = rand() % (HEIGHT - 2) + 1;  
        }
    }

    snake_body_x[0] = prev_x;
    snake_body_y[0] = prev_y;

    if(*snake_head_x >= WIDTH - 1) *snake_head_x = 1;
    if(*snake_head_x <= 0) *snake_head_x = WIDTH - 2;
    if(*snake_head_y >= HEIGHT - 1) *snake_head_y = 1;
    if(*snake_head_y <= 0) *snake_head_y = HEIGHT - 2;

    if(*snake_head_x == *food_x && *snake_head_y == *food_y)
    {
        *food_x = rand() % (WIDTH - 2) + 1;
        *food_y = rand() % (HEIGHT - 2) + 1;
        *score += 30;
        (*snake_length)++;
    }

}
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void hide_cursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Скрыть курсор
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}