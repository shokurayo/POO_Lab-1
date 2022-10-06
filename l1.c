/ 15 puzzle game
// Mirovski Artiom

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "stdbool.h"

const int SIZE_BOARD = 4;

void show_board(int **the_board);
void change_place(int **the_board, const int number);
void random_board(int **the_board);
void read_board(int **the_board);
void save_board(int **the_board);
bool win_condition(int **the_board);
void play_the_game(int **the_board);

void almost_win_board(int **the_board);

void swap_two(int *first, int *second);
void shuffle(int *array, const int size);

int main(void)
{
    // Create our board
    int **board = (int**) malloc(sizeof(int*) * SIZE_BOARD);
    for (int i = 0; i < SIZE_BOARD; i++)
        board[i] = (int*) malloc(sizeof(int) * SIZE_BOARD);

    // Menu
    printf("\033[0;36m");
    int interaction = 0;
    do
    {
        printf("Welcome to the 15 puzzle\n");
        printf("1: Start the game with a random board\n");
        printf("2: Start the game with saved board\n");
        printf("3: Start the game with one step til win (for losers)\n");
        printf("4: Exit\n");

        printf("Your choice: ");
        scanf("%i", &interaction);

        switch(interaction)
        {
            case 1:
                random_board(board);
                play_the_game(board);
                break;

            case 2:
                read_board(board);
                play_the_game(board);
                break;

            case 3:
                almost_win_board(board);
                play_the_game(board);
                break;
        }
    }
    while (interaction != 4);

    // Clear memory
    for (int i = 0; i < SIZE_BOARD; i++)
        free(board[i]);
    free(board);
    board = NULL;

    return 0;
}

// Show board
void show_board(int **the_board)
{
    // You know how it goes, right?
    for (int i = 0; i < SIZE_BOARD; i++)
    {
        for (int j = 0; j < SIZE_BOARD; j++)
        {
            printf("%6i ", the_board[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}

// Changing place of two squares
void change_place(int **the_board, const int number)
{
    // Find zero (place with which we're gonna switch)
    for (int i = 0; i < SIZE_BOARD; i++)
    {
        for (int j = 0; j < SIZE_BOARD; j++)
        {
            if (the_board[i][j] == 0)
            {
                // Find our number and switch
                // 4 cases - above, below, on the right, on the left
                if (i - 1 >= 0)
                    if (the_board[i - 1][j] == number)
                    {
                        swap_two(&the_board[i][j], &the_board[i - 1][j]);
                        return;
                    }
                if (j - 1 >= 0)
                    if (the_board[i][j - 1] == number)
                    {
                        swap_two(&the_board[i][j], &the_board[i][j - 1]);
                        return;
                    }
                if (i + 1 < SIZE_BOARD)
                    if (the_board[i + 1][j] == number)
                    {
                        swap_two(&the_board[i][j], &the_board[i + 1][j]);
                        return;
                    }
                if (j + 1 < SIZE_BOARD)
                    if (the_board[i][j + 1] == number)
                    {
                        swap_two(&the_board[i][j], &the_board[i][j + 1]);
                        return;
                    }    
            }
        }
    }
}

// Create a board with random numbers (RNGesus time)
void random_board(int **the_board)
{
    // Shuffle array
    const int size_array = 15;
    int array[size_array];
    for (int i = 0; i < size_array; i++)
        array[i] = size_array - i;
    shuffle(array, size_array);

    // Transfer array values to matrix
    int number = 0;
    for (int i = 0; i < SIZE_BOARD; i++)
        for (int j = 0; j < SIZE_BOARD; j++)
            the_board[i][j] = (number < size_array ? array[number++] : 0); // pretty cool
    /*
    For me
    if (number < size_array)
    {
        the_board[i][j] = number;
        number++;
    }
    else
        the_board[i][j] = 0;
    */
}

// Create a board from saved file
void read_board(int **the_board)
{
    // Create array
    const int size_array = 16;
    int array[size_array];

    // Read from file
    FILE *board_file;
    board_file = fopen("our_board.txt", "r");
    for (int i = 0; i < size_array; i++)
        fscanf(board_file, "%i", &array[i]);
    fclose(board_file);
    
    // Transfer array values to matrix
    int number = 0;
    for (int i = 0; i < SIZE_BOARD; i++)
        for (int j = 0; j < SIZE_BOARD; j++)
            the_board[i][j] = array[number++];
}

// Save board
void save_board(int **the_board)
{
    FILE *board_save;
    board_save = fopen("our_board.txt", "w");
    for (int i = 0; i < SIZE_BOARD; i++)
        for (int j = 0; j < SIZE_BOARD; j++)
            fprintf(board_save, "%i ", the_board[i][j]);
    fclose(board_save);
}

// Check for win
bool win_condition(int **the_board)
{
    int number = 1;
    for (int i = 0; i < SIZE_BOARD; i++)
        for (int j = 0; j < SIZE_BOARD; j++)
            {
                if (number == the_board[i][j])
                    number++;
                else
                    return false;

                if (number == 16 && the_board[3][3] == 0)
                    return true;
            }
    return false;
}

// In game menu
void play_the_game(int **the_board)
{
    printf("The game has started\n");
    printf("Menu in-game\n");
    printf("1-15: Change place with 0\n");
    printf("20: Save game\n");
    printf("21: Exit game\n");

    printf("TO-DO: Press 1-15!\n");
    printf("TO-UNDO: Press the same number as you pressed before!\n");

    show_board(the_board);

    int interaction = 0;

    do
    {
        printf("Choice: ");
        scanf("%i", &interaction);

        switch(interaction)
        {
            case 20:
                save_board(the_board);
                printf("SAVED\n");
                break;
            case 21:
                printf("GOODBYE\n");
                break;
            default:
                change_place(the_board, interaction);
                show_board(the_board);
                break;
        }

        if (win_condition(the_board))
            printf("YAY, Victory!!!!!!\n");
    }
    while (!win_condition(the_board) && interaction != 21);
}

// The board that is almost won
void almost_win_board(int **the_board)
{
    int number = 1;
    for (int i = 0; i < SIZE_BOARD; i++)
        for (int j = 0; j < SIZE_BOARD; j++)
            the_board[i][j] = number++;
    the_board[3][2] = 0;
    the_board[3][3] = 15;
}

// Swap two elements
void swap_two(int *first, int *second)
{
    // Pretty simple
    int temp = *first;
    *first = *second;
    *second = temp;
}

// Shuffling algorithm
void shuffle(int *array, const int size)
{
    srand(time(NULL));

    for (int i = size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap_two(&array[i], &array[j]);
    }
}
