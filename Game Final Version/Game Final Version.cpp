//Die Trying
//2020-10-26
//Nolan Seymour 100745224
//Lily Wu 100808422
//Nathaniel Moore 100785826
//Nicholas Gauthier 100754586
//Krishanth Varatharajah 100787963

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <sstream>
using namespace std;

int roll();
void cardPickup(int t);
void gBoard();
void move(int t);
void overCard(int iv, int ib, int player);
void zombieWave(int wb);
void endGame();
int playerInfo[4][20];//holds the information for each player in the game
string board[21][21];//holds the gme board that gets displayed
string itemType[4];//holds the names of the different types of items to be displayed when exchnging clothing
int index[13][2];//holds the statistical values for the cards

//this is the main function of the game. it puts items on the board, keeps track of waves, and tracks players turn
//int t: recursive value tracking turns
//int r: recursive value tracking rounds
//int w: recursive value tracking the wave chance
//int wb: recursive value tracking the number of waves
//int ro: recursie value tracking when to roll for cards again
//int turn: tracks which players turn it is
//round: tracks which round it is in the game
//int c: counter setting the array value for playerInfo
//int wave: tracks the chance of a wave happening
//int waveChecks: checks if a wave should happen
//int rolling: determins when cards should be distributed
int main(int t, int r, int w, int wb, int ro)
{
    int turn = t;
    int round = r;
    int end = 1000;
    int c = 0;
    int wave = w;
    int waveBonus = wb;
    int waveCheck;
    int rolling = ro;

    if (waveBonus != 6) {
        if (round > end) {
            round = 1;
            wave = 0;
        }//end if round

        cout << turn << " turn\n" << round << " round\n";

        if (turn == 1 && round == 1)
            rolling = 0;
        //end if turn

        while (round == 1 && c != 4) {

            playerInfo[c][0] = c + 1;//players number

            //player cords
            playerInfo[0][1] = 15;//x cord p1
            playerInfo[0][2] = 9;//y cord p1 
            playerInfo[1][1] = 15;//x cord p2
            playerInfo[1][2] = 10;//y cord p2
            playerInfo[2][1] = 6;//x cord p3
            playerInfo[2][2] = 9;//y cord p3
            playerInfo[3][1] = 6;//x cord p4
            playerInfo[3][2] = 10;//y cord p4

            playerInfo[c][3] = 0;//scavenge boost
            playerInfo[c][4] = 0;//movement boost
            playerInfo[c][5] = 0;//attack boost

            playerInfo[c][6] = 2;//food
            playerInfo[c][7] = 1;//barriers

            c = c + 1;
        }//end while round

        if (turn == 1 && round == 1) {//gets the board from board.txt
            cout << "getting boaard from file\n";
            ifstream file1{ "board.txt" };
            if (!file1.is_open()) return -1;


            for (int i{}; i != 21; ++i) {
                for (int j{}; j != 21; ++j) {
                    file1 >> board[i][j];
                }//end for j
            }//end for i

            ifstream file2{ "index.txt" };
            if (!file2.is_open()) return -1;


            for (int i{}; i != 13; ++i) {//gets the index values for the items from index.txt
                for (int j{}; j != 2; ++j) {
                    file2 >> index[i][j];
                }//end for j
            }//end for i
        }//end if turn

        if (rolling == 0) {

            for (int i{}; i != 4; ++i) {
                int result = roll();

                switch (result) {
                case 1:
                    if (board[3][3] == ".")
                        board[3][3] = "T";
                    break;
                case 2:
                    if (board[3][16] == ".")
                        board[3][16] = "T";
                    break;
                case 3:
                    if (board[10][3] == ".")
                        board[10][3] = "T";
                    break;
                case 4:
                    if (board[10][16] == ".")
                        board[10][16] = "T";
                    break;
                case 5:
                    if (board[17][3] == ".")
                        board[17][3] = "T";
                    break;
                case 6:
                    if (board[17][16] == ".")
                        board[17][16] = "T";
                    break;
                default:
                    break;
                }//end switch result
            }//end for i
        }//end if wave

        rolling = rolling + 1;
        cout << rolling;

        gBoard();

        if (round > 1) {
            while (turn <= 4) {
                move(turn);
                turn = turn + 1;
            }//end while turn
        }//end if round

        if (round > 2 && turn > 4) {

            waveCheck = roll();

            cout << "wave: " << wave << "\n";
            cout << "wave checl: " << waveCheck << "\n";

            if (waveCheck <= wave && waveCheck != 6) {
                zombieWave(waveBonus);
                wave = 0;
                rolling = 0;
                waveBonus = waveBonus + 1;
            }
            else {
                wave = wave + 1;
            }//end if waveCheck
        }//end if round


        turn = turn + 1;

        if (turn > 4) {
            turn = 1;
            round = round + 1;
            cout << "turn: " << turn << "\n";
            cout << "round: " << round << "\n";
        }//end if turn

        main(turn, round, wave, waveBonus, rolling);

    }//end if waveBonus
    else {
        endGame();
    }

    return 0;
}//end int main

//This function will simulate a die roll
//return r: returns a value between 1 and 6 inclusively
int roll() {

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 6);

    int r = dis(gen);
    return r;
}//end int roll

//This function outputs the game board onto the screen
void gBoard() {
    cout << "Board\n";
    for (int y = 0; y < 21; y++)
    {
        for (int x = 0; x < 20; x++) {
            cout << board[y][x];
        }
        cout << endl;
    }
}//end void board

//this function handles player movement along the board
//int t: tracks which players turn it is
//int m: hold value for number of moves players get added to the movement boost the player has
//int x: the x coordinate of the player moving
//int y: the y coordinate of the player moving
//char choice: takes in selection from player on which direction to move
//int sCheck: checks if the player can scavenge or not
//string sChoice: holds choice on if player wants to scavenge or not

void move(int t) {
    int m = roll() + roll() + playerInfo[t - 1][4];//determines player movement
    int x;
    int y;
    string choice = " ";
    int sCheck = 0;
    string sChoice = " ";
    int pass;
    int maxS = 2 + playerInfo[t - 1][3];

    //handling when a player can scavenging

    if (playerInfo[t - 1][8] != 0) {
        playerInfo[t - 1][19] = playerInfo[t - 1][19] + 1;
    }//end if playerInfo[i][8]

    if (playerInfo[t - 1][19] >= 2) {
        playerInfo[t - 1][19] = 0;
        playerInfo[t - 1][8] = 0;
    }//end if plyerinfo[i][19]

    if (playerInfo[t - 1][8] == 7) {
        playerInfo[t - 1][8] = 0;
        playerInfo[t - 1][19] = 0;
    }//end if playerInfo[i][8]

    x = playerInfo[t - 1][1];
    y = playerInfo[t - 1][2];

    //displaying players stats
    cout << "Player " << t << "'s stats:\nAttack: " << playerInfo[t - 1][5] << "\nBonus moves: " << playerInfo[t - 1][4];
    cout << "\nScavenging boost: " << playerInfo[t - 1][3] << "\n";
    cout << "Food: " << playerInfo[t - 1][6] << "\nBarriers: " << playerInfo[t - 1][7] << "\n";

    while (m != 0) {//player movement

        cout << "Player " << t << " has " << m << " moves remaining\n";

        m = m - 1;

        cout << "\n\nPress w to move up\nPress s to move down\nPress a to move left\nPress d to move right\npress o to skip your remaining moves\n";
        cout << "Select a movement: ";
        cin >> choice;

        if (choice == "w") {//moves player up
            x = x - 1;
            if (board[x][y] == ".") {
                board[x][y] = to_string(t);
                board[x + 1][y] = ".";
                playerInfo[t - 1][1] = x;
                playerInfo[t - 1][2] = y;
                gBoard();
            }
            else if (board[x][y] == "T") {
                board[x][y] = to_string(t);
                board[x + 1][y] = ".";
                playerInfo[t - 1][1] = x;
                playerInfo[t - 1][2] = y;
                playerInfo[t - 1][8] = 7;
                gBoard();
                cardPickup(t);
            }
            else {
                cout << "You cannot move there\n";
                m = m + 1;
                x = x + 1;
                gBoard();
            }//end if board
        }
        else if (choice == "s") {//moves player down
            x = x + 1;
            if (board[x][y] == ".") {
                board[x][y] = to_string(t);
                board[x - 1][y] = ".";
                playerInfo[t - 1][1] = x;
                playerInfo[t - 1][2] = y;
                gBoard();
            }
            else if (board[x][y] == "T") {
                board[x][y] = to_string(t);
                board[x - 1][y] = ".";
                playerInfo[t - 1][1] = x;
                playerInfo[t - 1][2] = y;
                playerInfo[t - 1][8] = 7;
                gBoard();
                cardPickup(t);
            }
            else {
                cout << "You cannot move there\n";
                m = m + 1;
                x = x - 1;
                gBoard();
            }//end if board
        }
        else if (choice == "a") {//moves player left
            y = y - 1;
            if (board[x][y] == ".") {
                board[x][y] = to_string(t);
                board[x][y + 1] = ".";
                playerInfo[t - 1][1] = x;
                playerInfo[t - 1][2] = y;
                gBoard();
            }
            else if (board[x][y] == "T") {
                board[x][y] = to_string(t);
                board[x][y + 1] = ".";
                playerInfo[t - 1][1] = x;
                playerInfo[t - 1][2] = y;
                playerInfo[t - 1][8] = 7;
                gBoard();
                cardPickup(t);
            }
            else {
                cout << "You cannot move there\n";
                m = m + 1;
                y = y + 1;
                gBoard();
            }//end if board
        }
        else if (choice == "d") {//moves player right
            y = y + 1;
            if (board[x][y] == ".") {
                board[x][y] = to_string(t);
                board[x][y - 1] = ".";
                playerInfo[t - 1][1] = x;
                playerInfo[t - 1][2] = y;
                gBoard();
            }
            else if (board[x][y] == "T") {
                board[x][y] = to_string(t);
                board[x][y - 1] = ".";
                playerInfo[t - 1][1] = x;
                playerInfo[t - 1][2] = y;
                playerInfo[t - 1][8] = 7;
                gBoard();
                cardPickup(t);
            }
            else {//error handling for out of bounds moving
                cout << "You cannot move there\n";
                m = m + 1;
                y = y - 1;
                gBoard();
            }//end if board
        }
        else if (choice == "o") {
            m = 0;
            gBoard();
        }
        else {//error handling for incorrect entry
            cout << "Invalid entry\n";
            m = m + 1;
        }//end if c
    }//end while m

    if (playerInfo[t - 1][8] != 7) {

        if (playerInfo[t - 1][1] == 3 && playerInfo[t - 1][2] == 3 && playerInfo[t - 1][8] != 1) {
            sCheck = 1;
            playerInfo[t - 1][8] = 1;
        }
        else if (playerInfo[t - 1][1] == 3 && playerInfo[t - 1][2] == 16 && playerInfo[t - 1][8] != 2) {
            sCheck = 1;
            playerInfo[t - 1][8] = 2;
        }
        else if (playerInfo[t - 1][1] == 10 && playerInfo[t - 1][2] == 3 && playerInfo[t - 1][8] != 3) {
            sCheck = 1;
            playerInfo[t - 1][8] = 3;
        }
        else if (playerInfo[t - 1][1] == 10 && playerInfo[t - 1][2] == 16 && playerInfo[t - 1][8] != 4) {
            sCheck = 1;
            playerInfo[t - 1][8] = 4;
        }
        else if (playerInfo[t - 1][1] == 17 && playerInfo[t - 1][2] == 3 && playerInfo[t - 1][8] != 5) {
            sCheck = 1;
            playerInfo[t - 1][8] = 5;
        }
        else if (playerInfo[t - 1][1] == 17 && playerInfo[t - 1][2] == 16 && playerInfo[t - 1][8] != 6) {
            sCheck = 1;
            playerInfo[t - 1][8] = 6;
        }

        if (sCheck == 1) {
            while (sChoice != "1" || sChoice != "2") {//error check for sChoice
                cout << "Would you like to scavenge for an item?\n";
                cout << "If yes enter 1, if no enter 2: ";
                cin >> sChoice;


                if (sChoice == "1") {
                    pass = roll();
                    cout << maxS << "\n";
                    cout << pass << "\n";
                    if (pass >= 1 && maxS >= pass) {
                        cardPickup(t);
                        break;
                    }
                    else {
                        cout << "You did not find anything.\n";
                        break;
                    }//end if
                }
                else if (sChoice == "2") {
                    break;
                }
                else {
                    cout << "Invalid Input\n";
                    sChoice = 1;
                }//end if sChoice
            }//end while sChoice
        }//end if sCheck
    }//end if playerInfo
}//end void move

//this function deals with a player picking up a card off the board
//int t: tracks player turn
//int itemVlue: holds the item value from the index array
//int itemBuff: holds the item stat from the index array
//int player: holds the player turn value
//int decision: hold the decision the player makes on discarding a weapon card
//int r: the value that determines the card the player gets
void cardPickup(int t) {

    int itemValue;
    int itemBuff;
    int player = t - 1;
    string decision;

    //rolls for what card player gets
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 12);

    int r = dis(gen);

    itemValue = index[r][0];
    itemBuff = index[r][1];

    if (itemValue == 1) {//if player is recieving a weapon card
        cout << "You picked up a weapon with a strength of " << itemBuff << "\n";

        if (playerInfo[player][9] == 0) {//sets the card values to player's weapon slot 1
            playerInfo[player][9] = itemBuff;
            playerInfo[player][5] = playerInfo[player][5] + itemBuff;
        }
        else if (playerInfo[player][10] == 0) {//sets the card values to player's weapon slot 2
            playerInfo[player][10] = itemBuff;
            playerInfo[player][5] = playerInfo[player][5] + itemBuff;
        }
        else {//if player already has 2 weapons

            while (decision != "1" || decision != "2" || decision != "3") {//error handling for dropping weapon
                cout << "You cannot hold any more weapons you need to drop one\n";
                cout << "Enter 1 to drop your first weapon with a strength of: " << playerInfo[player][9] << "\n";
                cout << "Enter 2 to drop your first weapon with a strength of: " << playerInfo[player][10] << "\n";
                cout << "Enter 3 to drop the weapon you picked up with a strength of: " << itemBuff << "\n";
                cout << "Make your selection: ";
                cin >> decision;

                if (decision == "1") {//subtracts strength of weapon being thrown away and adds strength of weapon being picked up
                    playerInfo[player][5] = playerInfo[player][5] - playerInfo[player][9] + itemBuff;
                    playerInfo[player][9] = itemBuff;

                    cout << "You dropped your first weapon. Your new strength is: " << playerInfo[player][5] << "\n";
                }
                else if (decision == "2") {
                    playerInfo[player][5] = playerInfo[player][5] - playerInfo[player][10] + itemBuff;
                    playerInfo[player][10] = itemBuff;

                    cout << "You dropped your second weapon. Your new strength is: " << playerInfo[player][5] << "\n";
                }
                else if (decision == "3") {
                    cout << "dropped the weapon you picked up\n";
                }
                else {
                    cout << "Invalid Input\n";
                }//endn if decision
            }//end while decision
        }//end if playerInfo
    }
    else if (itemValue == 2) {//if player is recieving a weapon piece of clothes
        cout << "You picked up an article of clothing that gives " << itemBuff << " additional strength.\n";
        if (playerInfo[player][14] == 0) {//assigning clothing to clothing slot 1
            playerInfo[player][14] = itemValue;
            playerInfo[player][11] = itemBuff;
            playerInfo[player][5] = playerInfo[player][5] + itemBuff;
        }
        else if (playerInfo[player][12] == 0) {//assigning clothing to clothing slot 2
            playerInfo[player][15] = itemValue;
            playerInfo[player][12] = itemBuff;
            playerInfo[player][5] = playerInfo[player][5] + itemBuff;
        }
        else if (playerInfo[player][13] == 0) {//assigning clothing to clothing slot 3
            playerInfo[player][16] = itemValue;
            playerInfo[player][13] = itemBuff;
            playerInfo[player][5] = playerInfo[player][5] + itemBuff;
        }
        else {//calls function for if player has 3 pieces of clothes already
            overCard(itemValue, itemBuff, player);
        }//end if playerInfo

    }
    else if (itemValue == 3) {//if player is recieving a movement boost clothing
        cout << "You picked up an article of clothing tht gives you " << itemBuff << " additional movement(s).\n";
        if (playerInfo[player][14] == 0) {//assigning clothing to clothing slot 1
            playerInfo[player][14] = itemValue;
            playerInfo[player][11] = itemBuff;
            playerInfo[player][4] = playerInfo[player][4] + itemBuff;
        }
        else if (playerInfo[player][12] == 0) {//assigning clothing to clothing slot 2
            playerInfo[player][15] = itemValue;
            playerInfo[player][12] = itemBuff;
            playerInfo[player][4] = playerInfo[player][4] + itemBuff;
        }
        else if (playerInfo[player][13] == 0) {//assigning clothing to clothing slot 3
            playerInfo[player][16] = itemValue;
            playerInfo[player][13] = itemBuff;
            playerInfo[player][4] = playerInfo[player][4] + itemBuff;
        }
        else {//calls function for if player has too many clothes
            overCard(itemValue, itemBuff, player);
        }//end if playerInfo
    }
    else if (itemValue == 4) {//if player is recieving a scavange boost clothess
        cout << "You picked up an article of clothing tht gives " << itemBuff << " scavenging bonus.\n";
        if (playerInfo[player][14] == 0) {//assigning clothing to clothing slot 1
            playerInfo[player][14] = itemValue;
            playerInfo[player][11] = itemBuff;
            playerInfo[player][3] = playerInfo[player][3] + itemBuff;
        }
        else if (playerInfo[player][12] == 0) {//assigning clothing to clothing slot 2
            playerInfo[player][15] = itemValue;
            playerInfo[player][12] = itemBuff;
            playerInfo[player][3] = playerInfo[player][3] + itemBuff;
        }
        else if (playerInfo[player][13] == 0) {//assigning clothing to clothing slot 3
            playerInfo[player][16] = itemValue;
            playerInfo[player][13] = itemBuff;
            playerInfo[player][3] = playerInfo[player][3] + itemBuff;
        }
        else {//calls function if player has 3 pieces of clothes
            overCard(itemValue, itemBuff, player);
        }//end if playerInfo
    }
    else if (itemValue == 5) {//player recieves food card
        cout << "You picked up " << itemBuff << " Food.\n";
        playerInfo[player][6] = playerInfo[player][6] + itemBuff;
    }
    else {//yea lets hope this one never happens
        cout << "Something went wrong\n";
    }//end if itemValue
}//end viod cardPickup

//this function will have a player discard a piece of clothing when they have a full inventory
//int iv: the item value pulled from the index array
//int ib: the item stat pulled from the index array
//int player: holds which players turn it is
//int decision: holds the decision the player makes on the clothing they are discarding
//int hold: holds the item stat as values are being changed
void overCard(int iv, int ib, int player) {

    string decision;
    int hold;

    if (iv == 2) {//determins the type of item picked up by player for text purposes
        itemType[3] = "attack";
    }
    else if (iv == 3) {
        itemType[3] = "movement";
    }
    else if (iv == 4) {
        itemType[3] = "scavenginng";
    }//end if iv

    for (int i{}; i != 3; ++i) {//determines the types of items the player has in their inventory for text purposes
        if (playerInfo[player][i + 14] == 2)
            itemType[i] = "attack";
        else if (playerInfo[player][i + 14] == 3)
            itemType[i] = "movement";
        else if (playerInfo[player][i + 14] == 4)
            itemType[i] = "scsavenging";
        //end if playerInfo
    }//end for i
    while (decision != "1" || decision != "2" || decision != "3" || decision != "4") {//error handling for dropping clothing
        cout << "You cannot wear any more cloting\n";
        cout << "You need to drop a piece of clothing\n";
        cout << "Enter 1 to drop your first piece of clothing that gives " << playerInfo[player][11] << " " << itemType[0] << "\n";
        cout << "Enter 2 to drop your second piece of clothing that gives " << playerInfo[player][12] << " " << itemType[1] << "\n";
        cout << "Enter 3 to drop your third piece of clothing that gives " << playerInfo[player][13] << " " << itemType[2] << "\n";
        cout << "Enter 4 to drop the piece of clothing you picked up that gives " << ib << " " << itemType[3] << "\n";
        cout << "Make a selection: ";
        cin >> decision;

        if (decision == "1") { //exchanges the players first piece of clothing with the piece of clothing they picked up
            cout << "You dropped your first piece of clothing that gave " << playerInfo[player][11] << " " << itemType[0] << "\n";
            hold = playerInfo[player][11];

            if (playerInfo[player][14] == 2) {//clothes beingd dropped is attack boosting
                playerInfo[player][5] = playerInfo[player][5] - hold;
                playerInfo[player][11] = ib;
                playerInfo[player][14] = iv;
                cout << "A" << playerInfo[player][5] << "\n";
            }
            else if (playerInfo[player][14] == 3) {//clothes being dropped is movement boosting
                playerInfo[player][4] = playerInfo[player][4] - hold;
                playerInfo[player][11] = ib;
                playerInfo[player][14] = iv;
                cout << hold;
                cout << "B" << playerInfo[player][4] << "\n";
            }
            else if (playerInfo[player][14] == 4) {//clothes being dropped is scavenge boosting
                playerInfo[player][3] = playerInfo[player][3] - hold;
                playerInfo[player][11] = ib;
                playerInfo[player][14] = iv;
                cout << "C" << playerInfo[player][3] << "\n";
            }//end if playerInfo

            if (iv == 2)//adds the picked up card to the corsponding stat
                playerInfo[player][5] = playerInfo[player][5] + ib;
            else if (iv == 3)
                playerInfo[player][4] = playerInfo[player][4] + ib;
            else if (iv == 4)
                playerInfo[player][3] = playerInfo[player][3] + ib;
            //end if iv
        }
        else if (decision == "2") { //exchanges the players second piece of clothing with the piece of clothing they picked up
            cout << "You dropped your second piece of clothing that gave " << playerInfo[player][12] << " " << itemType[1] << "\n";
            hold = playerInfo[player][12];

            if (playerInfo[player][15] == 2) {//clothes being dropped is attack boosting
                playerInfo[player][5] = playerInfo[player][5] - hold;
                playerInfo[player][12] = ib;
                playerInfo[player][15] = iv;
                cout << "D" << playerInfo[player][5] << "\n";
            }
            else if (playerInfo[player][15] == 3) {//clothes being dropped is movement boosting
                playerInfo[player][4] = playerInfo[player][4] - hold;
                playerInfo[player][12] = ib;
                playerInfo[player][15] = iv;
                cout << "E" << playerInfo[player][4] << "\n";
            }
            else if (playerInfo[player][15] == 4) {//clothes being dropped is scavenge boosting
                playerInfo[player][3] = playerInfo[player][3] - hold;
                playerInfo[player][12] = ib;
                playerInfo[player][15] = iv;
                cout << "F" << playerInfo[player][3] << "\n";
            }//end if playerInfo

            if (iv == 2)//adds the picked up card to the corsponding stat
                playerInfo[player][5] = playerInfo[player][5] + ib;
            else if (iv == 3)
                playerInfo[player][4] = playerInfo[player][4] + ib;
            else if (iv == 4)
                playerInfo[player][3] = playerInfo[player][3] + ib;
            //end if iv
        }
        else if (decision == "3") {//exchanages the players third piece of clothing for the one they picke up
            cout << "You dropped your first piece of clothing that gave " << playerInfo[player][11] << " " << itemType[0] << "\n";
            hold = playerInfo[player][13];

            if (playerInfo[player][16] == 2) {//clothes being dropped is attack boosting
                playerInfo[player][5] = playerInfo[player][5] - hold;
                playerInfo[player][13] = ib;
                playerInfo[player][16] = iv;
                cout << "G" << playerInfo[player][5] << "\n";
            }
            else if (playerInfo[player][16] == 3) {//clothes being dropped is movement boosting
                playerInfo[player][4] = playerInfo[player][4] - hold;
                playerInfo[player][13] = ib;
                playerInfo[player][16] = iv;
                cout << "H" << playerInfo[player][4] << "\n";
            }
            else if (playerInfo[player][16] == 4) {//clothes being dropped is scavenge boosting
                playerInfo[player][3] = playerInfo[player][3] - hold;
                playerInfo[player][13] = ib;
                playerInfo[player][16] = iv;
                cout << "J" << playerInfo[player][3] << "\n";
            }//end if playerInfo

            if (iv == 2)//add the picked up card to the corsponding stat
                playerInfo[player][5] = playerInfo[player][5] + ib;
            else if (iv == 3)
                playerInfo[player][4] = playerInfo[player][4] + ib;
            else if (iv == 4)
                playerInfo[player][3] = playerInfo[player][3] + ib;
            //end if iv
        }
        else if (decision == "4") {//tells player they dropped the item they picked up
            cout << "You dropped the piece of clothes you picked up.\n";
        }
        else {//tells player they made an invalid entry
            cout << "Invalid entry.\n";
        }//end if decision
    }//end while dedcision
}//end viod overCard


//This function carries out the events that occure when a zombie wave happens
//int waveStrength: determins the strength of the zombies
//int bonus: keeps track of when players use food or barriers
//string decision: tracks the players decision on if they use a barrier or not
void zombieWave(int wb) {
    int waveStrength = roll() + wb + playerInfo[0][17] + playerInfo[1][17] + playerInfo[2][17] + playerInfo[3][17]; //determins strength of wave
    int bonus = 0;
    string decision;

    cout << "\nThe Strength of the wave is " << waveStrength << "\n";

    for (int i{}; i != 4; ++i) {

        int foodUse = -1;

        while (foodUse < 0 || playerInfo[i][6] < foodUse) {

            bonus = 0;

            //prompts player if they want to use food
            cout << "\nPlayer " << i + 1 << " has a strength of " << playerInfo[i][5] << "\n";
            cout << "Player " << i + 1 << " has " << playerInfo[i][6] << " Food. How much food do you want to use.\n1 food gives +1 strength\n";
            cout << "Enter an amount: ";
            cin >> foodUse;

            if (foodUse >= 0 && playerInfo[i][6] >= foodUse) {
                bonus = bonus + foodUse;
                playerInfo[i][6] = playerInfo[i][6] - foodUse;
                foodUse = -1;
                break;
            }
            else {
                cout << "Invalid entry. Plese enter a value within the amount of food you have\n";
            }//end if foodUse
        }//end while foodUse

        //prompts player if they want to use their barrier
        if (playerInfo[i][7] == 1) {
            while (decision != "1" || decision != "2") {
                cout << "Player " << i + 1 << "'s strength after eating is: " << playerInfo[i][5] + bonus << "\n";
                cout << "Does player " << i + 1 << " want to use their barrier for an aditional 3 strength?\n If yes enter 1, if no enter 2\n";
                cout << "Make a selection: ";
                cin >> decision;

                if (decision == "1") {
                    bonus = bonus + 3;
                    playerInfo[i][7] = 0;
                    break;
                }
                else if (decision == "2") {
                    break;
                }
                else {
                    cout << "Invalid entry\n";
                }//end if decisison
            }//end while decision
        }//end if playerInfo

        if (playerInfo[i][5] + bonus < waveStrength) {
            cout << "Player " << i + 1 << " had a strength of " << playerInfo[i][5] + bonus << " did not survive the wave.\n";
            //resets the players stats and position to how they started at begining of game
            playerInfo[i][3] = 0;
            playerInfo[i][4] = 0;
            playerInfo[i][5] = 0;
            playerInfo[i][6] = 2;
            playerInfo[i][9] = 0;
            playerInfo[i][10] = 0;
            playerInfo[i][11] = 0;
            playerInfo[i][12] = 0;
            playerInfo[i][13] = 0;
            playerInfo[i][14] = 0;
            playerInfo[i][15] = 0;
            playerInfo[i][16] = 0;
            playerInfo[i][17] = playerInfo[i][17] + 1;//death counter

            switch (i) {//visually reseting where player is on board after death
            case 0:
                board[playerInfo[0][1]][playerInfo[0][2]] = ".";
                board[15][9] = "1";
                playerInfo[0][1] = 15;//x cord p1
                playerInfo[0][2] = 9;//y cord p1 

                //determins if player survived final wave or not
                if (wb == 5)
                    playerInfo[0][18] = 1;
                //end if wb

                break;
            case 1:
                board[playerInfo[1][1]][playerInfo[1][2]] = ".";
                board[15][10] = "2";
                playerInfo[1][1] = 15;//x cord p2
                playerInfo[1][2] = 10;//y cord p2

                if (wb == 5)
                    playerInfo[1][18] = 1;
                //end if wb

                break;
            case 2:
                board[playerInfo[2][1]][playerInfo[2][2]] = ".";
                board[6][9] = "3";
                playerInfo[2][1] = 6;//x cord p3
                playerInfo[2][2] = 9;//y cord p3

                if (wb == 5)
                    playerInfo[2][18] = 1;
                //end if wb

                break;
            case 3:
                board[playerInfo[3][1]][playerInfo[3][2]] = ".";
                board[6][10] = "4";
                playerInfo[3][1] = 6;//x cord p4
                playerInfo[3][2] = 10;//y cord p4

                if (wb == 5)
                    playerInfo[3][18] = 1;
                //end if wb

            }//end switch i
        }
        else {
            cout << "Player " << i + 1 << " survived the wave.\n";
        }//end if playerInfo[i][5]
    }//end for i
}//end void zombieWave

//this function tallies up final scores
//int p1: player 1 points
//int p2: player 2 points
//int p3: player 3 points
//int p4: player 4 points
//int point: tracks who gets the points
void endGame() {

    int p1 = 0;
    int p2 = 0;
    int p3 = 0;
    int p4 = 0;
    int point;

    // gives players points based on highest scavenging boost
    if (playerInfo[0][3] > playerInfo[1][3])
        point = 2;
    else if (playerInfo[2][3] > playerInfo[1][3])
        point = 3;
    else if (playerInfo[2][3] > playerInfo[3][3])
        point = 4;
    else
        point = 1;
    //end if playerInfo[1][3]

    switch (point) {
    case 1:
        p1 = p1 + 2;
        cout << "Player 1 had the highest scavenging skill.\n\n";
        break;
    case 2:
        p2 = p2 + 2;
        cout << "Player 2 had the highest scavenging skill.\n\n";
        break;
    case 3:
        p3 = p3 + 2;
        cout << "Player 3 had the highest scavenging skill.\n\n";
        break;
    case 4:
        p4 = p4 + 2;
        cout << "Player 4 had the highest scavenging skill.\n\n";
    }//end switch point


    // gives players points based on most movement
    if (playerInfo[0][4] > playerInfo[1][4])
        point = 2;
    else if (playerInfo[2][4] > playerInfo[1][4])
        point = 3;
    else if (playerInfo[2][4] > playerInfo[3][4])
        point = 4;
    else
        point = 1;
    //end if playerInfo[1][3]

    switch (point) {
    case 1:
        p1 = p1 + 1;
        cout << "Player 1 could move the furthest.\n\n";
        break;
    case 2:
        p2 = p2 + 1;
        cout << "Player 2 could move the furthest.\n\n";
        break;
    case 3:
        p3 = p3 + 1;
        cout << "Player 3 could move the furthest.\n\n";
        break;
    case 4:
        p4 = p4 + 1;
        cout << "Player 4 could move the furthest.\n\n";
    }//end switch point


    // gives players points based on most attack
    if (playerInfo[0][5] > playerInfo[1][5])
        point = 2;
    else if (playerInfo[2][5] > playerInfo[1][5])
        point = 3;
    else if (playerInfo[2][5] > playerInfo[3][5])
        point = 4;
    else
        point = 1;
    //end if playerInfo[1][3]

    switch (point) {
    case 1:
        p1 = p1 + 1;
        cout << "Player 1 had the highest strength.\n\n";
        break;
    case 2:
        p2 = p2 + 1;
        cout << "Player 2 had the highest strength.\n\n";
        break;
    case 3:
        p3 = p3 + 1;
        cout << "Player 3 had the highest strength.\n\n";
        break;
    case 4:
        p4 = p4 + 1;
        cout << "Player 4 had the highest strength.\n\n";
    }//end switch point


    //gives players points if they did not die
    if (playerInfo[0][17] == 0) {
        p1 = p1 + 2;
        cout << "Player 1 did survived all waves\n\n";
    }//end if p1

    if (playerInfo[1][17] == 0) {
        p2 = p2 + 2;
        cout << "Player 2 did survived all waves\n\n";
    }//end if p2

    if (playerInfo[2][17] == 0) {
        p3 = p3 + 2;
        cout << "Player 3 did survived all waves\n\n";
    }//end if p3

    if (playerInfo[3][17] == 0) {
        p4 = p4 + 2;
        cout << "Player 4 did survived all waves\n\n";
    }//end if p4


    //gives player points if they lived last wave
    if (playerInfo[0][18] == 0) {
        p1 = p1 + 1;
        cout << "Player 1 survived the final wave\n\n";
    }//end if p1

    if (playerInfo[1][18] == 0) {
        p2 = p2 + 1;
        cout << "Player 2 survived the final wave\n\n";
    }//end if p2

    if (playerInfo[2][18] == 0) {
        p3 = p3 + 1;
        cout << "Player 3 survived the final wave\n\n";
    }//end if p3

    if (playerInfo[3][18] == 0) {
        p4 = p4 + 1;
        cout << "Player 4 survived the final wave\n\n";
    }//end if p4

    //final point display
    cout << "\n\nPlayer 1 finished with: " << p1 << " points\n";
    cout << "Player 2 finished with: " << p2 << " points\n";
    cout << "Player 3 finished with: " << p3 << " points\n";
    cout << "Player 4 finished with: " << p4 << " points\n";

}//end void endGame