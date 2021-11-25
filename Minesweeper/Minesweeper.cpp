// Minesweeper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector> //for storing the hidden and visible gameboards
#include <random> //for generating random numbers for bomb placement

/* Key:
    *  |Nearby bombs : Numbers
    *  |Bomb : Q
    *  |Flag : P
    *  |Clear Space with no nearby bombs : #
    *  |Unmarked Tile : (char)254u
*  Controls:
    *  |Movement: wasd
    *  |Place Flag: f
    *  |Dig Tile: e
*  Copy-paste loop to cycle through all board positions:
    for (int y = 0; y < boardheight; y++)
    {
        for (int x = 0; x < boardlength; x++)
        {
        }
    }
*/

void makehiddenboard(int& boardlength, int& boardheight, int& bombnumber, std::vector<std::vector<char>>& hdboard)
{
    std::string difficulty;
    
    std::cout << "Welcome to Minesweeper, please enter your difficulty: \"Easy\", \"Medium\", or \"Hard\"." << std::endl;
    std::cout << "Controls for the game are; Movement: wasd, Place Flag: f, Dig: e." << std::endl;
    for (bool validinput = false; validinput == false;)
    {
        std::cin >> difficulty;

        if (difficulty == "Easy" || difficulty == "easy")
        {
            boardlength = 10;
            boardheight = 8;
            bombnumber = 10;
            validinput = true;
        }
        else if (difficulty == "Medium" || difficulty == "medium")
        {
            boardlength = 18;
            boardheight = 14;
            bombnumber = 40;
            validinput = true;
        }
        else if (difficulty == "Hard" || difficulty == "hard")
        {
            boardlength = 24;
            boardheight = 20;
            bombnumber = 99;
            validinput = true;
        }
        else
        {
            std::cout << "You have entered: " << difficulty << ". Please enter: \"Easy\", \"Medium\", or \"Hard\"." << std::endl;
        }
    }
    //sets values of board length, board height, and bomb number for the given difficulty

    hdboard.resize(boardlength, std::vector<char>(boardheight));
    //resizes the hidden game board

    for (int x = 0; x < boardlength; x++)
    {
        for (int y = 0; y < boardheight; y++)
        {
            hdboard[x][y] = '0';
        }
    }
    //fills the hidden game board with all 0's
}
//Asks for difficulty, and initializes the hidden game board with all 0's
//also sets boardheight, boardlength, and bombnumber for later usage.

void printvsboard(const int boardlength, const int boardheight,const int bombnumber, const int characterpos[2], const std::vector<std::vector<char>> vsboard)
{
    system("CLS");
    std::cout << "BOMBS: " << bombnumber << std::endl;
    for (int y = 0; y < boardheight; y++)
    {
        for (int x = 0; x < boardlength; x++)
        {
            if (characterpos[0] == x && characterpos[1] == y)
            {
                std::cout << "[" << vsboard[x][y] << "]";
            }
            else
            {
                std::cout << " " << vsboard[x][y] << " ";
            }
        }
        std::endl(std::cout);
    }
}

void printhdboard(const int boardlength, const int boardheight, const int bombnumber, const int characterpos[2], const std::vector<std::vector<char>> hdboard)
{
    system("CLS");
    std::cout << "BOMBS: " << bombnumber << std::endl;
    for (int y = 0; y < boardheight; y++)
    {
        for (int x = 0; x < boardlength; x++)
        {
            if (characterpos[0] == x && characterpos[1] == y)
            {
                std::cout << "[" << hdboard[x][y] << "]";
            }
            else
            {
                std::cout << " " << hdboard[x][y] << " ";
            }
        }
        std::endl(std::cout);
    }
}

int rng(int min, int max)
{
    int rn;
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(min, max); // define the range
    rn = distr(gen);
    return rn;
}

void revealtiles(const int boardlength, const int boardheight, const int characterpos[2], const std::vector<std::vector<char>> hdboard, std::vector<std::vector<char>>& vsboard)
{
    int x = characterpos[0]; int y = characterpos[1];
    int revealedpos[2] = { x, y };

    if (hdboard[x][y] != '0') //if the revealed tile is not 0 or Q
    {
        vsboard[x][y] = hdboard[x][y]; //reveals the tile at charpos
    }
    else //if the revealed tiles are 0
    {
        for (int xm = -1; xm <= 1; xm++)
        {
            for (int ym = -1; ym <= 1; ym++) //check all connected tiles
            {
                if (!((ym == 0) && (xm == 0))) //except for the tile you're on
                {
                    if ((x + xm >= 0) && (x + xm < boardlength) && (y + ym >= 0) && (y + ym < boardheight)) //that are on the board
                    {
                        if (vsboard[x + xm][y + ym] == (char)254u) //and if it's still hidden
                        {
                            if (hdboard[x + xm][y + ym] != '0') //and it's not 0
                            {
                                vsboard[x + xm][y + ym] = hdboard[x + xm][y + ym]; //reveal the tile
                            }
                            else //else if it is 0
                            {
                                vsboard[x + xm][y + ym] = hdboard[x + xm][y + ym]; //reveal it
                                revealedpos[0] = x + xm;
                                revealedpos[1] = y + ym;
                                revealtiles(boardlength, boardheight, revealedpos, hdboard, vsboard); //RECURSIFY
                            }
                        }
                    }
                }
            }
        }
    }
    //if a tile is 0, reveal it and check all neighboring tiles for unrevealed 0's
}

void firstmove(const int boardlength, const int boardheight, const int bombnumber, int (&characterpos)[2], std::vector<std::vector<char>>& hdboard, std::vector<std::vector<char>>& vsboard)
{
    char command;
    vsboard = hdboard; //easy resize of vsboard
    for (int y = 0; y < boardheight; y++)
    {
        for (int x = 0; x < boardlength; x++)
        {
            vsboard[x][y] = (char)254u;
        }
    }
    //fills vsboard with tiles
    printvsboard(boardlength, boardheight, bombnumber, characterpos, vsboard);
    for (bool gonext = false; gonext == false;)
    {
        for (bool validinput = false; validinput == false;)
        {
            std::cin >> command;
            if (command == 'w' || command == 'a' || command == 's' || command == 'd' || command == 'e' || command == 'f')
            {
                validinput = true;
            }
            else
            {
                printvsboard(boardlength, boardheight,bombnumber, characterpos, vsboard);
                std::cout << "Invalid input, please try again. Controls are; Movement: wasd, Place Flag: f, Dig: e." << std::endl;
            }
        }
        if (command == 'w')
        {
            if (characterpos[1] != 0)
            {
                characterpos[1]--;
            }
        }
        if (command == 'a')
        {
            if (characterpos[0] != 0)
            {
                characterpos[0]--;
            }
        }
        if (command == 's')
        {
            if (characterpos[1] != (boardheight - 1))
            {
                characterpos[1]++;
            }
        }
        if (command == 'd')
        {
            if (characterpos[0] != (boardlength - 1))
            {
                characterpos[0]++;
            }
        }
        printvsboard(boardlength, boardheight, bombnumber, characterpos, vsboard);
        if (command == 'f')
        {
            std::cout << "Ha ha. Very funny. You think you know where the bombs are already? Jokes on you:" << std::endl;
            std::cout << "I haven't even placed them yet!" << std::endl;
        }
        if (command == 'e')
        {
            gonext = true;
        }
    }
    //player moves to a position and digs at that position
    for (int i = 0; i < bombnumber; i++)
    {
        for (bool validbombplacement = false; validbombplacement == false;)
        {
            int y = rng(0, boardheight - 1);
            int x = rng(0, boardlength - 1);
            if (!((characterpos[0] == x - 1 && characterpos[1] == y - 1) || (characterpos[0] == x - 1 && characterpos[1] == y) || (characterpos[0] == x - 1 && characterpos[1] == y + 1) || (characterpos[0] == x && characterpos[1] == y - 1) || (characterpos[0] == x && characterpos[1] == y) || (characterpos[0] == x && characterpos[1] == y + 1) || (characterpos[0] == x + 1 && characterpos[1] == y - 1) || (characterpos[0] == x + 1 && characterpos[1] == y) || (characterpos[0] == x + 1 && characterpos[1] == y + 1)) && !(hdboard[x][y] == 'Q'))
            {
                hdboard[x][y] = 'Q';
                validbombplacement = true;
            }
        }
    }
    //places the bombs using rng
    //printhdboard(boardlength, boardheight, bombnumber, characterpos, hdboard); //prints the hidden board for debugging purposes
    for (int y = 0; y < boardheight; y++)
    {
        for (int x = 0; x < boardlength; x++)
        {
            if (hdboard[x][y] == 'Q') // If there's a bomb
            {
                for (int xm = -1; xm <= 1; xm++) 
                {
                    for (int ym = -1; ym <= 1; ym++) //for all surrounding tiles
                    {
                        if ((x + xm >= 0) && (x + xm < boardlength) && (y + ym >= 0) && (y + ym < boardheight)) //if it's on the board
                        {
                            if (hdboard[x + xm][y + ym] != 'Q') //If it's not a bomb
                            {
                                hdboard[x + xm][y + ym]++; //Increment by 1. This works because the board starts as all 0's and in ASCII, 1-9 are next to each other
                            }
                        }
                    }
                }
            }
        }
    }
    //update hidden board to show numbers next to bombs
    revealtiles(boardlength, boardheight, characterpos, hdboard, vsboard);
    printvsboard(boardlength, boardheight, bombnumber, characterpos, vsboard);
    //printhdboard(boardlength, boardheight, bombnumber, characterpos, hdboard); //prints the hidden board for debugging purposes
}

//Random Number Generator used for bomb placement
void play(const int boardlength, const int boardheight, int& bombnumber, int (&characterpos)[2], const std::vector<std::vector<char>> hdboard, std::vector<std::vector<char>>& vsboard)
{
    char command;
    for (bool lose = false; lose == false;)
    {
        for (bool gonext = false; gonext == false;)
        {
            for (bool validinput = false; validinput == false;)
            {
                std::cin >> command;
                if (command == 'w' || command == 'a' || command == 's' || command == 'd' || command == 'e' || command == 'f')
                {
                    validinput = true;
                }
                else
                {
                    printvsboard(boardlength, boardheight, bombnumber, characterpos, vsboard);
                    std::cout << "Invalid input, please try again. Controls are; Movement: wasd, Place Flag: f, Dig: e." << std::endl;
                }
            }
            if (command == 'w')
            {
                if (characterpos[1] != 0)
                {
                    characterpos[1]--;
                }
            }
            if (command == 'a')
            {
                if (characterpos[0] != 0)
                {
                    characterpos[0]--;
                }
            }
            if (command == 's')
            {
                if (characterpos[1] != (boardheight - 1))
                {
                    characterpos[1]++;
                }
            }
            if (command == 'd')
            {
                if (characterpos[0] != (boardlength - 1))
                {
                    characterpos[0]++;
                }
            }
            if (command == 'f')
            {
                if (vsboard[characterpos[0]][characterpos[1]] == 'P')
                {
                    vsboard[characterpos[0]][characterpos[1]] = (char)254u;
                    bombnumber++;
                }
                else
                {
                    vsboard[characterpos[0]][characterpos[1]] = 'P';
                    bombnumber--;
                }
            }
            printvsboard(boardlength, boardheight, bombnumber, characterpos, vsboard);
            if (command == 'e')
            {
                gonext = true;
            }
        }
        revealtiles(boardlength, boardheight, characterpos, hdboard, vsboard);
        if (vsboard[characterpos[0]][characterpos[1]] == 'Q')
        {
            lose = true;
        }
        printvsboard(boardlength, boardheight, bombnumber, characterpos, vsboard);
    }
    std::cout << "BOOM!" << std::endl;
}

int main()
{
    int boardheight = 0; int boardlength = 0;
    int bombnumber = 0;
    std::vector<std::vector<char>> hdboard; //hidden gameboard with all information
    std::vector<std::vector<char>> vsboard; //gameboard visible to player
    int characterpos[2]{ 0,0 };
    
    for (bool keepgoing = true; keepgoing == true;)
    {
        makehiddenboard(boardlength, boardheight, bombnumber, hdboard);
        firstmove(boardlength, boardheight, bombnumber, characterpos, hdboard, vsboard);
        play(boardlength, boardheight, bombnumber, characterpos, hdboard, vsboard);
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu