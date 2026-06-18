
#include <stdio.h> //snprintf
#include <stdlib.h>  // Needed for system()
#include <string.h>  // For memset


#define Grass 0
#define Flower 1
#define Tree 2
#define Rock 3
#define Key 4
#define Goldcoin 5
#define Padlock 6
#define Trap 7
#define Monster 8
#define Whirlwind 9

#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define RESET "\033[0m"



// Global message buffer
char message[256] = "";



// Read key and map arrow keys to 8/2/4/6
int read_key() {
    int c = getchar();
    if (c == 27) { // ESC
        if (getchar() == '[') {
            c = getchar();
            switch(c) {
                case 'A': return 8; // Up
                case 'B': return 2; // Down
                case 'C': return 6; // Right
                case 'D': return 4; // Left
            }
        }
    }
    return c; // other keys
}

// Initialisation
void init_card(int arr[20][20]){
    for(int i=0;i<20;i++){
        for(int j=0;j<20;j++){
            arr[i][j]=Grass;
        }
    }

    // Gold coins (10 total)
    int gold_positions[][2] = {
        {0,15},{1,13},{2,19},{9,0},{4,10},
        {8,14},{11,10},{19,19},{0,16},{10,10},{5,11},{17,4}
    };
    for(int i = 0; i < 12; i++) {
        arr[gold_positions[i][0]][gold_positions[i][1]] = Goldcoin;
    }

    // Flowers (12 total)
    int flower_positions[][2] = {
        {0,16},{1,8},{2,12},{4,1},{8,7},{14,2},
        {15,2},{15,8},{17,2},{17,12},{19,13},{19,17}
    };
    for(int i = 0; i < 12; i++) {
        arr[flower_positions[i][0]][flower_positions[i][1]] = Flower;
    }

    // Trees (10 total)
    int tree_positions[][2] = {
        {0,3},{0,19},{6,6},{10,15},{12,3},{14,14},
        {15,3},{16,13},{18,5},{19,10}
    };
    for(int i = 0; i < 10; i++) {
        arr[tree_positions[i][0]][tree_positions[i][1]] = Tree;
    }

    // Rocks (10 total)
    int rock_positions[][2] = {
        {0,6},{3,4},{7,2},{8,2},{10,16},
        {11,9},{13,8},{15,9},{18,15},{17,3}
    };
    for(int i = 0; i < 10; i++) {
        arr[rock_positions[i][0]][rock_positions[i][1]] = Rock;
    }

    // Traps (5 total)
    int trap_positions[][2] = {
        {1,5},{11,10},{15,11},{18,10},{19,12},{3,16},{12,14}
    };
    for(int i = 0; i < 7; i++) {
        arr[trap_positions[i][0]][trap_positions[i][1]] = Trap;
    }

    // Monsters (13 total)
    int monster_positions[][2] = {
        {0,9},{3,13},{11,5},{11,7},
        {12,7},{12,8},{14,18},{14,19},{16,17},
        {16,18},{16,19},{19,18},{9,18} 
    };
    for(int i = 0; i < 13; i++) {
        arr[monster_positions[i][0]][monster_positions[i][1]] = Monster;
    }

    // Key and Padlock
    arr[12][6] = Key;
    arr[14][19] = Padlock;

    // Whirlwinds (e.g., 3 total)
    int whirlwind_positions[][2] = {
    {5, 5}, {10, 3}, {17, 17}
};
for (int i = 0; i < 3; i++) {
    arr[whirlwind_positions[i][0]][whirlwind_positions[i][1]] = Whirlwind;
}
}

// Display stats
void display_stats(int lives, int coins, int key, int injuries) {
    printf("\n");
    printf(RED   " ❤️ Lives: %d\t" RESET, lives);
    printf(YELLOW "💰 Coins: %d\t" RESET, coins);
    printf(GREEN  "🔑 Key: %s\t" RESET, (key ? "Yes" : "No"));
    printf(RED    "🤕 Injuries: %d\n" RESET, injuries);
}

// Display the map and player X
void display_card(int arr[20][20], int card[2], int lives, int coins, int key, int injuries) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    fflush(stdout);


    printf("┌");
    for (int i = 0; i < 20; ++i) printf("──");
    printf("┐\n");

    for (int i = 0; i < 20; i++) {
        printf("│");
        for (int j = 0; j < 20; j++) {
            if (i == card[0] && j == card[1]) {
                printf("🧝🏻‍♀️");
            } 
            else if (arr[i][j] == Whirlwind) {
            printf("🌀");
}
            else {
                switch (arr[i][j]) {
                    case Grass:    printf("🌱"); break;
                    case Flower:   printf("🌺"); break;
                    case Tree:     printf("🌲"); break;
                    case Rock:     printf("🪨"); break;
                    case Key:      printf("🔑"); break;
                    case Goldcoin: printf("🥇"); break;
                    case Padlock:  printf("🔒"); break;
                    case Trap:     printf("🪤"); break;
                    case Monster:  printf("🧌"); break;
                    default:       printf("  "); break;
                }
            }
        }
        printf("│\n");
    }

    printf("└");
    for (int i = 0; i < 20; ++i) printf("──");
    printf("┘\n");

    display_stats(lives, coins, key, injuries);
}



// Move the player
int move_character(int card[20][20], int pos[2], int *goldcounter, int *lifecounter, int *keylock, int *injuriescounter){
    int direction;
    int row = pos[0];
    int column = pos[1];
    


    printf("Enter direction (8=up, 2=down, 6=right, 4=left, 0=quit): ");
    if (scanf("%d", &direction) != 1) {
        // Clear invalid input
        while(getchar() != '\n');
        strcpy(message, "Invalid input, please enter a number.");
        return 0;
    }

    if (direction == 0)
        return -1;

    if (direction != 8 && direction != 2 && direction != 4 && direction != 6) {
        strcpy(message, "Invalid direction!");
        return 0;
    }

    int new_row = row;
    int new_column = column;

    if (direction == 8) new_row--;
    else if (direction == 2) new_row++;
    else if (direction == 6) new_column++;
    else if (direction == 4) new_column--;

    if (new_row < 0 || new_row >= 20 || new_column < 0 || new_column >= 20) {
        strcpy(message, "🚫Hit edge! Enter another direction.");
        return 0;
    }

    // Obstacles
    if (card[new_row][new_column] == Tree || card[new_row][new_column] == Rock) {
        strcpy(message, "Hit obstacle! You cannot move there.");
        system("afplay /Users/anshyamungur/Desktop/game/obstacle.mp3 &");
        return 0;
    }

    // Monster 
    if (card[new_row][new_column] == Monster) {
        (*lifecounter)--;
        snprintf(message, sizeof(message), RED"You encountered a monster! You have only %d lives left."RESET, *lifecounter);
        system("afplay /Users/anshyamungur/Desktop/game/monster.mp3 &");
    }
   

    //trap
    if (card[new_row][new_column] == Trap) {
        (*injuriescounter)++;
        snprintf(message, sizeof(message), RED"You encountered a trap! You have  %d injury/ies"RESET, *injuriescounter);
        system("afplay /Users/anshyamungur/Desktop/game/trap.mp3 &");
        card[new_row][new_column] = Grass;  
    // Every 2 injuries, lose 1 life
        if (*injuriescounter >= 2) {
           (*lifecounter)--;
           *injuriescounter -= 2;
            snprintf(message, sizeof(message), RED"You've accumulated 2 injuries and lost 1 life! Lives left: %d, Injuries: %d"RESET, *lifecounter, *injuriescounter);
    }
}
    

    // Gold coin
    if (card[new_row][new_column] == Goldcoin) {
        (*goldcounter)++;
        snprintf(message, sizeof(message),YELLOW "You collected a goldcoin! You now have %d goldcoin(s)."RESET, *goldcounter);
        card[new_row][new_column] = Grass;
        system("afplay /Users/anshyamungur/Desktop/game/coin.mp3 &");

    }

    // Key
    if (card[new_row][new_column] == Key) {
        (*keylock) = 1;
        snprintf(message, sizeof(message), GREEN"Amazing!!You have now found the key, go to the padlock for a surprise."RESET);
        card[new_row][new_column] = Grass;
        system("afplay /Users/anshyamungur/Desktop/game/key.mp3 &");
    }

    // Padlock
    if (card[new_row][new_column] == Padlock) {
        if (*keylock == 1) {
            (*goldcounter) += 2;
            snprintf(message, sizeof(message), YELLOW"Great! You have unlocked the padlock! You now have %d gold coins\n", *goldcounter);
            card[new_row][new_column] = Grass;
            system("afplay /Users/anshyamungur/Desktop/game/unlocked.mp3 &");
        } else {
            strcpy(message, RED"The padlock is locked! Find the key first."RESET);
            // Player doesn't get extra gold without key
        }
    }
    // Whirlwind
    int whirlwind_hit = 0;
    if (card[new_row][new_column] == Whirlwind) {
    whirlwind_hit = 1;
    snprintf(message, sizeof(message), RED "A whirlwind swept you away! You're back at the start!" RESET);
    system("afplay /Users/anshyamungur/Desktop/game/whirl.mp3 &");
}


 // Update player position
if (whirlwind_hit) {
    pos[0] = 0;
    pos[1] = 0;
} else {
    pos[0] = new_row;
    pos[1] = new_column;
}


    // win/lose
    
    if (*goldcounter >= 10) {
        strcpy(message, GREEN"Congratulations🎉! You have won the game!🎉🎉"RESET);
        system("afplay /Users/anshyamungur/Desktop/game/victory.mp3 &");
        return -1;  // Signal end game
    }
    if (*lifecounter <= 0) {
        strcpy(message, RED"Oh shoot! You have lost..💀\n Better luck next time!\n"RESET);
        system("afplay /Users/anshyamungur/Desktop/game/fail.mp3 &");
        return -1;  // Signal end game
    }

    return 0;
}

// Entry point
int main() {
 

    printf(GREEN"𝑾𝑬𝑳𝑪𝑶𝑴𝑬 𝑻𝑶 𝑨𝑫𝑽𝑬𝑵𝑻𝑼𝑹𝑬 𝑸𝑼𝑬𝑺𝑻!\n"RESET);
    printf(RED"Rules of the game:\n"RESET);
    printf(YELLOW"• You have a total of 10 lives.\n"RESET);
    printf(YELLOW"• Every time you encounter a monster, you lose 1 life.\n"RESET);
    printf(YELLOW"• 2 injuries will cost you 1 life.\n"RESET);
    printf(YELLOW"• Rocks and trees act as obstacles meaning you cannot move past them.\n"RESET);
    printf(YELLOW"• Landing in a trap will injure you.\n"RESET);
    printf(YELLOW"• Be careful of whirlwinds. \n"RESET);
    printf(YELLOW"• If you collect 10 gold coins, you win.\n"RESET);
    printf(YELLOW"• If your life count reaches 0, you lose.\n\n"RESET);

    int card[20][20];
    int pos[2] = {0, 0}; // Starting position at top-left corner
    int goldcounter = 0;
    int lifecounter = 10;
    int injuriescounter=0;
    int keylock = 0;

    init_card(card);

    int game_status = 0;
    while (game_status == 0) {
        display_card(card, pos, lifecounter, goldcounter, keylock,injuriescounter);
;
        if (message[0] != '\0') {
            printf("\n%s\n\n", message);
            message[0] = '\0';  // Clear message after displaying
        }
        game_status = move_character(card, pos, &goldcounter, &lifecounter, &keylock, &injuriescounter);
    }

    // Final display and message on game end
    display_card(card, pos, lifecounter, goldcounter, keylock,injuriescounter);
    if (message[0] != '\0') {
        printf("\n%s\n\n", message);
    }
    printf(GREEN"Thank you for playing!\n"RESET);
   
    return 0;
}