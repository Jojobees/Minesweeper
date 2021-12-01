// Minesweeper 2.0.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector> //for storing the hidden and visible gameboards
#include <random> //for generating random numbers for bomb placement
#include <cstring>
#include <string>

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

void clearconsole()
{
#ifdef _WIN32
    system("CLS");
    return;
#else
#if defined __APPLE__ || defined __linux__
    system("CLEAR");
    std::cout << "ERROR: USER IS NOT A WINDOWS USER.";
    return;
#else
    std::cout << "Failed to clear console: Unknown OS detected.\n";
#endif
#endif
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
//rng for bomb placement
void difficultyselection(int& boardlength, int& boardheight, int& bombnumber, int& bombcounter)
{
    clearconsole();
    std::string input;

    std::cout << "Welcome to Minesweeper, please enter your difficulty: \"Easy\", \"Medium\", or \"Hard\"." << std::endl;
    std::cout << "Controls for the game are; Movement: wasd, Place Flag: f, Dig: e." << std::endl;
    for (bool validinput = false; validinput == false;)
    {
        std::getline(std::cin, input);

        if (input == "Easy" || input == "easy")
        {
            boardlength = 10;
            boardheight = 8;
            bombnumber = 10;
            bombcounter = bombnumber;
            validinput = true;
        }
        else if (input == "Medium" || input == "medium")
        {
            boardlength = 18;
            boardheight = 14;
            bombnumber = 40;
            bombcounter = bombnumber;
            validinput = true;
        }
        else if (input == "Hard" || input == "hard")
        {
            boardlength = 24;
            boardheight = 20;
            bombnumber = 99;
            bombcounter = bombnumber;
            validinput = true;
        }
        else
        {
            std::cout << "You have entered: " << input << ". Please enter: \"Easy\", \"Medium\", or \"Hard\"." << std::endl;
        }
    }
}
//sets values of board length, board height, and bomb number for the given difficulty
void printvsboard(const int boardlength, const int boardheight, const int bombcounter, const int characterpos[2], const std::vector<std::vector<char>>& vsboard)
{
    clearconsole();
    std::cout << "BOMBS: " << bombcounter << std::endl;
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
//prints the visual game board with a bomb counter for playing the game
void printhdboard(const int boardlength, const int boardheight, const int bombcounter, const int characterpos[2], const std::vector<std::vector<char>>& hdboard)
{
    system("CLS");
    std::cout << "BOMBS: " << bombcounter << std::endl;
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
//prints the hidden game board with a bomb counter for debugging purposes
void dropflag(const int boardlength, const int boardheight, int& bombcounter, const int characterpos[2], std::vector<std::vector<char>>& vsboard)
{
    if (vsboard[characterpos[0]][characterpos[1]] == (char)254u)
    {
        vsboard[characterpos[0]][characterpos[1]] = 'P';
        bombcounter--;
        printvsboard(boardlength, boardheight, bombcounter, characterpos, vsboard);
    }
    else if (vsboard[characterpos[0]][characterpos[1]] == 'P')
    {
        vsboard[characterpos[0]][characterpos[1]] = (char)254u;
        bombcounter++;
        printvsboard(boardlength, boardheight, bombcounter, characterpos, vsboard);
    }
    else
    {
        printvsboard(boardlength, boardheight, bombcounter, characterpos, vsboard);
        std::cout << "Invalid flag placement. Flags must be placed on an uncleared tile or removed from a flagged tile.\n";
    }
}
//places a flag at player position if it is an uncleared tile, removes flags if the tile is flagged, and modifies bomb number
void playermovement(const int boardlength, const int boardheight, int& bombcounter, int (&characterpos)[2], std::vector<std::vector<char>>& vsboard)
{
    char input;
    std::string line;
    while (true)
    {
        while (true)
        {
            std::getline(std::cin, line);
            std::cout << "[" << line << "]\n";
            if (line.size() == 0)
            {
                std::cout << "Invalid input. Try again.\n";
                continue;
            }
            else if (line.size() == 1)
            {
                input = line[0];
            }
            else
            {
                std::cout << "Your line is too long.\n";
                continue;
            }
            if (input == 'w' || input == 'a' || input == 's' || input == 'd' || input == 'e' || input == 'f')
            {
                break;
            }
            else
            {
                std::cout << "Invalid input, please try again. You entered ascii code: " << (int)input << ". Controls are; Movement: wasd, Place Flag: f, Dig: e." << std::endl;
                continue;
            }
        }
        printvsboard(boardlength, boardheight, bombcounter, characterpos, vsboard);
        if (input == 'w')
        {
            if (characterpos[1] != 0)
            {
                characterpos[1]--;
            }
            printvsboard(boardlength, boardheight, bombcounter, characterpos, vsboard);
        }
        if (input == 'a')
        {
            if (characterpos[0] != 0)
            {
                characterpos[0]--;
            }
            printvsboard(boardlength, boardheight, bombcounter, characterpos, vsboard);
        }
        if (input == 's')
        {
            if (characterpos[1] != (boardheight - 1))
            {
                characterpos[1]++;
            }
            printvsboard(boardlength, boardheight, bombcounter, characterpos, vsboard);
        }
        if (input == 'd')
        {
            if (characterpos[0] != (boardlength - 1))
            {
                characterpos[0]++;
            }
            printvsboard(boardlength, boardheight, bombcounter, characterpos, vsboard);
        }
        if (input == 'f')
        {
            dropflag(boardlength, boardheight, bombcounter, characterpos, vsboard);
        }
        if (input == 'e' && vsboard[characterpos[0]][characterpos[1]] == 'P')
        {
            printvsboard(boardlength, boardheight, bombcounter, characterpos, vsboard);
            std::cout << "Cannot dig on a flagged tile.\n";
        }
        if (input == 'e' && vsboard[characterpos[0]][characterpos[1]] != 'P')
        {
            break;
        }
    }
}
//allows the player to move and place flags, and modifies character pos so when it exits, you have the position that the character will dig at.
void dig(const int boardlength, const int boardheight, const int characterpos[2], const std::vector<std::vector<char>>& hdboard, std::vector<std::vector<char>>& vsboard)
{
    int x = characterpos[0]; int y = characterpos[1];
    int revealedpos[2] = { x, y };
    int tilevalue;
    int flags = 0;
    if (vsboard[x][y] != (char)254u) //if the tile you tried to dig on is a number
    {
        tilevalue = vsboard[x][y] - '0';
        for (int xm = -1; xm <= 1; xm++) //add all surrounding flags
        {
            for (int ym = -1; ym <= 1; ym++)
            {
                if ((x + xm >= 0) && (x + xm < boardlength) && (y + ym >= 0) && (y + ym < boardheight))
                {
                    if (vsboard[x + xm][y + ym] == 'P')
                    {
                        flags++;
                    }
                }
            }
        }
        if (flags == tilevalue) //if the surrounding flags are equal to the value of the tile
        {
            for (int xm = -1; xm <= 1; xm++)
            {
                for (int ym = -1; ym <= 1; ym++)
                {
                    if ((x + xm >= 0) && (x + xm < boardlength) && (y + ym >= 0) && (y + ym < boardheight))
                    {
                        if (vsboard[x + xm][y + ym] == (char)254u)
                        {
                            revealedpos[0] = x + xm;
                            revealedpos[1] = y + ym;
                            dig(boardlength, boardheight, revealedpos, hdboard, vsboard);
                        }
                    }
                }
            }
        }
        //if the number of flags are equal to the value of the tile, dig all the non-flagged tiles
    }
    //if the tile you dug is already dug check to see if there's the same number of flags, and if there are, dig all tiles around it that aren't flagged
    vsboard[x][y] = hdboard[x][y]; //reveals the tile at characterpos
    if (hdboard[x][y] == '0') //if the revealed tile is 0
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
                                dig(boardlength, boardheight, revealedpos, hdboard, vsboard); //RECURSIFY
                            }
                        }
                    }
                }
            }
        }
    }
    //if a tile is 0, reveal it and check all neighboring tiles for unrevealed 0's
}
//modifies the vsgameboard based on the characterpos
void makehdboard(const int boardlength, const int boardheight, const int bombnumber, const int characterpos[2], std::vector<std::vector<char>>& hdboard)
{
    hdboard.resize(boardlength);
    for (int i = 0; i < boardlength; i++)
    {
        hdboard[i].resize(boardheight);
    }
    //resize hdboard
    for (int y = 0; y < boardheight; y++)
    {
        for (int x = 0; x < boardlength; x++)
        {
            hdboard[x][y] = '0';
        }
    }
    //initializes hdboard with 0's
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
    //place bombs
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
    //place numbers
}
//makes the hdboard
void makevsboard(const int boardlength, const int boardheight, std::vector<std::vector<char>>& vsboard)
{
    vsboard.resize(boardlength);
    for (int i = 0; i < boardlength; i++)
    {
        vsboard[i].resize(boardheight);
    }
    for (int y = 0; y < boardheight; y++)
    {
        for (int x = 0; x < boardlength; x++)
        {
            vsboard[x][y] = (char)254u;
        }
    }
}
//initializes the vsboard
bool checkloss(const int boardlength, const int boardheight, const std::vector<std::vector<char>>& vsboard)
{
    for (int x = 0; x < boardlength; x++)
    {
        for (int y = 0; y < boardheight; y++)
        {
            if (vsboard[x][y] == 'Q')
            {
                return true;
            }
        }
    }
    return false;
}
bool checkwin(const int boardlength, const int boardheight, const int bombnumber, const std::vector<std::vector<char>>& vsboard)
{
    int hiddentiles = 0;
    for (int x = 0; x < boardlength; x++)
    {
        for (int y = 0; y < boardheight; y++)
        {
            if (vsboard[x][y] == 'P' || vsboard[x][y] == (char)254u)
            {
                hiddentiles++;
            }
        }
    }
    //cycles through all tiles to count hidden tiles
    if (hiddentiles == bombnumber)
    {
        return true;
    }
    return false;
}
void play()
{
    int boardlength;
    int boardheight;
    int bombnumber;
    int bombcounter;
    std::vector<std::vector<char>> vsboard; //visible gameboard
    std::vector<std::vector<char>> hdboard; //hidden gameboard
    int characterpos[2]{ 0,0 };
    bool lose = false;
    bool win = false;

    difficultyselection(boardlength, boardheight, bombnumber, bombcounter);
    makevsboard(boardlength, boardheight, vsboard);
    printvsboard(boardlength, boardheight, bombcounter, characterpos, vsboard);
    playermovement(boardlength, boardheight, bombcounter, characterpos, vsboard);
    makehdboard(boardlength, boardheight, bombnumber, characterpos, hdboard);
    dig(boardlength, boardheight, characterpos, hdboard, vsboard);
    printvsboard(boardlength, boardheight, bombcounter, characterpos, vsboard);
    //first move
    while (true)
    {
        playermovement(boardlength, boardheight, bombcounter, characterpos, vsboard);
        dig(boardlength, boardheight, characterpos, hdboard, vsboard);
        printvsboard(boardlength, boardheight, bombcounter, characterpos, vsboard);
        lose = checkloss(boardlength, boardheight, vsboard);
        win = checkwin(boardlength, boardheight, bombnumber, vsboard);
        if (lose == true)
        {
            std::cout << "BOOM!\n";
            break;
        }
        else if (win == true)
        {
            std::cout << "Win!\n";
            break;
        }
    }
}
int main()
{
    bool stop = false;
    std::string input;
    while (true)
    {
        play();
        while (true)
        {
            std::cout << "Would you like to play again? Type yes or no.\n";
            std::getline(std::cin, input);
            if (input == "yes" || input == "Yes")
            {
                break;
            }
            if (input == "no" || input == "No")
            {
                stop = true;
                break;
            }
            std::cout << "You entered " << input << ". Please type yes or no like a normal human being. \n";
            continue;
        }
        if (stop == true)
        {
            break;
        }
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
