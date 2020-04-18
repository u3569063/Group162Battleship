//main.cpp
//includes: main operation, prompt input, print board, generate random board

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>  //setw()
#include <cstdlib>  //rand(), srand(), system()
#include <ctime> //time()
#include <string> //stoi()

using namespace std;

const int boardSize = 10;  //maximum size of the board
const int maxSavedRecords = 50; // maximum size allowed to store game records in "Unfinished_Game.txt"

//A structure storing all the boards required
struct Boards {
    char player[boardSize][boardSize]; //Full board of player (Player see this)
    char AI[boardSize][boardSize]; //Full board of AI
    char playerViewAIBoard[boardSize][boardSize]; // AI board but in player's perspectives (Player see this)
    char AIViewPlayerBoard[boardSize][boardSize]; // player board but in AI's perspectives
};

Boards boards;

//structure for storing game data other than boards (ex. game name, moves...)
struct LoadGame {
  int id;
  string gameName;
  string playerName;
  string country;
  string date;
  string time;
  int totalMovesUsed;
};

LoadGame loadGameData;

//structure for storing and sorting all game records (for dynamic array)
struct GameRecords {
  int won;
  int totalMoveUsed;
  string gameName;
  string playerName;
  string country;
  string date;
  GameRecords* next;
};

//A structure stroing all the ships of a country
struct ships
{
  string shipclass;
  string name;
};

struct navy
{
  string name;
  int xposini;
  int xposfnl;
  int yposini;
  int yposfnl;
};

struct Target
{
  int row;
  int col;
};

navy fleet[5];

struct plane
{
  string ijn[4] = {"Type 96 Carrier Attack Bomber", "B6N \"Jill\"", "B5N \"Kate\"", "B7A Ryusei \"Grace\""};
  string kms[2] = {"Fi 167", "He 115"};
  string rm[1] = {"Re.2001"};
  string uss[3] = {"TBY Sea Wolf", "TBF Avenger", "TBD Devastator"};
  string hms[3] = {"Swordfish", "Barracuda", "Vildebeest"};
};

int numofplane[5] = {4,2,1,3,3};
int plane1, plane2, planeflag;


plane bomber;

//array for storing the recon planes used by ijn, kms, rm, uss, hms respectively
string recon[5] = {"C6N Saiun \"Myrt\"","Arado Ar 196","Caproni Ca.316","Supermarine Walrus","OS2U Kingfisher"};

//array for storing the ap shells used by ijn, kms, rm, uss, hms respectively
string shell[5] = {"Type 1 AP shell", "38 cm SK C/34 AP shell", "Proiettile Perforante", "Mk. 8 APC shell", "Mk XXII BNT AP shell"};

//array for storing the radar used by ijn, kms, rm, uss, hms respectively
string radar[5] = {"Type 2 Mark 3 Model 3 Anti-Surface Fire-Control Radar", "FuMO 23 Seetakt radar", "EC 3 ter Gufo radar", "MK 37 Gun Fire Control System", "Type 279 early-warning radar"};

//array for storing the torpedos used by ijn, kms, rm, uss, hms respectively
string torpedo[5] = {"Type 93 torpedo", "21 inch torpedo", "533 mm torpedo", " 21-inch Mark 15 torpedo", "21-inch Mark VII torpedo"};

//number of storage of planes, plane 1 attack quota, plane 2 attack quota, barrage, torpedos of heavy cruiser, torpedos of submarine respectively
int spweapons[6] = {2,1,1,1,2,2};

//function to generate today's date, return the date
string todaysDate(){
  time_t timeNow = time(0);
  tm * currentTime = localtime(&timeNow);
  string temp, fullDate;

  stringstream ys;
  ys << 1900 + currentTime->tm_year;
  ys >> temp;
  fullDate = temp + "-";
  stringstream ms;
  ms << 1 + currentTime->tm_mon;
  ms >> temp;
  temp = (temp.length() == 1) ? "0" + temp : temp;
  fullDate += temp + "-";
  stringstream ds;
  ds << currentTime->tm_mday;
  ds >> temp;
  temp = (temp.length() == 1) ? "0" + temp : temp;
  fullDate += temp;
  return fullDate;
}

//function to generate current time, return the time
string currentTime(){
  time_t timeNow = time(0);
  tm * currentT = localtime(&timeNow);
  string temp, fullTime;

  stringstream hs;
  hs << currentT->tm_hour;
  hs >> temp;
  temp = (temp.length() == 1) ? "0" + temp : temp;
  fullTime = temp + ":";
  stringstream ss;
  ss << currentT->tm_min;
  ss >> temp;
  temp = (temp.length() == 1) ? "0" + temp : temp;
  fullTime += temp;
  
  return fullTime;
}

//function for stroing all the names of all the countries' ships to the given struct
void naming(ships arr[50],string s)
{
  string filename = s +".txt";
  ifstream fin(filename.c_str());

  if (fin.fail())
  {
    cout<<"Error in opening "<<s<<endl;
    exit(1);
  }

  string sclass,x;
  while (fin >> x)
  {
    sclass = x;
    if (sclass == "ac")
      {
        for (int i = 0; i < 10; i++)
        {

          string temp;
          fin >> temp;
          if (temp == "end")
          {
            break;
          }
          arr[i].shipclass = "ac";
          arr[i].name = s +" " + temp;
        }
      }
    if (sclass == "bb")
    {
      for (int i = 10; i < 20; i++)
      {

        string temp;
        fin >> temp;
        if (temp == "end")
        {
          break;
        }
        arr[i].shipclass = "bb";
        arr[i].name = s +" " + temp;
      }
    }
    if (sclass == "ca")
    {
      for (int i = 20; i < 30; i++)
      {

        string temp;
        fin >> temp;
        if (temp == "end")
        {
          break;
        }
        arr[i].shipclass = "ca";
        arr[i].name = s +" " + temp;
      }
    }
    if (sclass == "ss")
    {
      for (int i = 30; i < 40; i++)
      {

        string temp;
        fin >> temp;
        if (temp == "end")
        {
          break;
        }
        arr[i].shipclass = "ss";
        arr[i].name = s +" " + temp;
      }
    }
    if (sclass == "dd")
    {
      for (int i = 40; i < 50; i++)
      {

        string temp;
        fin >> temp;
        if (temp == "end")
        {
          break;
        }
        arr[i].shipclass = "dd";
        arr[i].name = s +" " + temp;
      }
    }
  }
    fin.close();
}

//A function that generates the ships' names for the player's fleet by the country they inputted
void genfleet(ships uss[50], ships kms[50], ships hms[50], ships rm[50], ships ijn[50], string country, navy arr[5])
{
  string ac, bb, ca, ss, dd;
  int num, num1, num2, num3, num4;
  if (country == "us")
  {
    country = "United States";
    num = rand() % 10 + 1;
    ac = uss[num].name;
    num1 = rand() % 10 + 10;
    bb = uss[num1].name;
    num2 = rand() % 10 + 20;
    ca = uss[num2].name;
    num3 = rand() % 10 + 30;
    ss = uss[num3].name;
    num4 = rand() % 10 + 40;
    dd = uss[num4].name;
  }
  else if (country == "gb")
  {
    country = "Great Britain";
    num = rand() % 8 + 0;
    ac = hms[num].name;
    num1 = rand() % 8 + 10;
    bb = hms[num1].name;
    num2 = rand() % 10 + 20;
    ca = hms[num2].name;
    num3 = rand() % 10 + 30;
    ss = hms[num3].name;
    num4 = rand() % 10 + 40;
    dd = hms[num4].name;
  }
  else if (country == "italy")
  {
    country = "The Kingdom of Italy";
    num = rand() % 2 + 0;
    ac = rm[num].name;
    num1 = rand() % 7 + 10;
    bb = rm[num1].name;
    num2 = rand() % 7 + 20;
    ca = rm[num2].name;
    num3 = rand() % 10 + 30;
    ss = rm[num3].name;
    num4 = rand() % 10 + 40;
    dd = rm[num4].name;
  }
  else if (country == "germany")
  {
    country = "German Reich";
    num = rand() % 1 + 0;
    ac = kms[num].name;
    num1 = rand() % 4 + 10;
    bb = kms[num1].name;
    num2 = rand() % 6 + 20;
    ca = kms[num2].name;
    num3 = rand() % 4 + 30;
    ss = kms[num3].name;
    num4 = rand() % 6 + 40;
    dd = kms[num4].name;
  }
  else if (country == "japan")
  {
    country = "the Empire of Great Japan";
    num = rand() % 8 + 0;
    ac = ijn[num].name;
    num1 = rand() % 10 + 10;
    bb = ijn[num1].name;
    num2 = rand() % 10 + 20;
    ca = ijn[num2].name;
    num3 = rand() % 10 + 30;
    ss = ijn[num3].name;
    num4 = rand() % 10 + 40;
    dd = ijn[num4].name;
  }

  arr[0].name = ac;
  arr[1].name = bb;
  arr[2].name = ca;
  arr[3].name = ss;
  arr[4].name = dd;


  cout<<endl<<"You have picked "<<country<<":"<<endl<<"Your aircraft carrier is: "<<ac<<endl;
  cout<<"Your battleship is: "<<bb<<endl;
  cout<<"Your heavy cruiser is: "<<ca<<endl;
  cout<<"Your submarine is: "<<ss<<endl;
  cout<<"Your destroyer is: "<<dd<<endl;

}

//Function to print both your board, and opponent's board in your perspectives
void printBoard() {
    system("CLS"); //clear screen
    cout << setw(30) << "Opponent's Board" << setw(52) << "Your Board" << endl;
    cout << "     0   1   2   3   4   5   6   7   8   9                 0   1   2   3   4   5   6   7   8   9  \n";
    cout << "   +---+---+---+---+---+---+---+---+---+---+             +---+---+---+---+---+---+---+---+---+---+\n";
    for (int i = 0; i < boardSize; i++) {
        //Printing your opponent's board (in your perspective)
        cout << " " << (char)('A' + i);
        for (int j = 0; j < boardSize; j++) {
            cout << " | " << boards.playerViewAIBoard[i][j];
        }
        cout << " |          ";

        //Printing your own board
        cout << " " << (char)('A' + i);
        for (int j = 0; j < boardSize; j++) {
            cout << " | " << boards.player[i][j];
        }
        cout << " |\n";
        cout << "   +---+---+---+---+---+---+---+---+---+---+             +---+---+---+---+---+---+---+---+---+---+\n";
    }
}

//TEMPORARY PRINT BORAD FUNCTION (For developing stage only) (Will print all 4 boards)  (DELETE BEFORE SUBMISSION!!!!!!!!!!!)
void tempPrintBoard() {
    system("CLS"); //clear screen
    cout << "                                          YOUR Perspective                                  |                                       AI Perspective\n";
    cout << "               Opponent's Board                                  Your Board                 |                  AI Board                                     Your Board\n";
    cout << "     0   1   2   3   4   5   6   7   8   9        0   1   2   3   4   5   6   7   8   9     |     0   1   2   3   4   5   6   7   8   9        0   1   2   3   4   5   6   7   8   9  \n";
    cout << "   +---+---+---+---+---+---+---+---+---+---+    +---+---+---+---+---+---+---+---+---+---+   |   +---+---+---+---+---+---+---+---+---+---+    +---+---+---+---+---+---+---+---+---+---+\n";
    for (int i = 0; i < boardSize; i++) {
        //Printing your opponent's board (in your perspective)
        cout << " " << (char)('A' + i);
        for (int j = 0; j < boardSize; j++) {
            cout << " | " << boards.playerViewAIBoard[i][j];
        }
        cout << " | ";

        //Printing your own board
        cout << " " << (char)('A' + i);
        for (int j = 0; j < boardSize; j++) {
            cout << " | " << boards.player[i][j];
        }
        cout << " |   |";

        cout << " " << (char)('A' + i);
        for (int j = 0; j < boardSize; j++) {
            cout << " | " << boards.AI[i][j];
        }
        cout << " | ";

        cout << " " << (char)('A' + i);
        for (int j = 0; j < boardSize; j++) {
            cout << " | " << boards.AIViewPlayerBoard[i][j];
        }
        cout << " |\n";
        cout << "   +---+---+---+---+---+---+---+---+---+---+    +---+---+---+---+---+---+---+---+---+---+   |   +---+---+---+---+---+---+---+---+---+---+    +---+---+---+---+---+---+---+---+---+---+\n";
    }
}

//Function to prompt player input, with input validation
string playerInput() {
    string target;
    int targetRow, targetCol;
    bool validInput;

    //Input validation
    while (true) {
        validInput = false;

        //Prompt input
        cout << "Please choose a target: ";
        getline(cin, target);

        if (target.length() <= 2) {
            //check whether the player input Q for quit or S for save or not (TODO: allow player input other command if have)
            if (target.length() == 1 && (target[0] == 'Q' || target[0] == 'q' || target[0] == 'S' || target[0] == 's')) {
                validInput = true;
                return target;
            }
            //check whether the player input aw for special weapon or not
            if (target.length() == 2 && (target[0] == 'a' || target[0] == 'A') && (target[1] == 'w' || target[1] == 'W')){
              validInput = true;
              return target;
            }

            if (((target[0] >= 'A' && target[0] <= 'J') || (target[0] >= 'a' && target[0] <= 'j')) && target[1] >= '0' && target[1] <= '9') {
                if (target[0] >= 'a' && target[0] <= 'j') {
                    target[0] = target[0] - 'a' + 'A';
                }
                validInput = true;
            }
        }

        if (validInput == true) {
            targetRow = (int)target[0] - 'A';
            targetCol = (int)target[1] - '0';
            if (boards.playerViewAIBoard[targetRow][targetCol] == 'X' || boards.playerViewAIBoard[targetRow][targetCol] == '@') { //TODO: if target is equal to attacked and is ship sign
                cout << "Invalid Input! " << target[0] << target[1] << " already attacked, please choose a non-attacked location!\n";
                validInput == false;
                continue;
            }
            else {
                return target;
            }
        }
        cout << "Invalid Input! Please input a character from 'A' to 'Z' followed by an integer 1 - 9! (Nothing in between)\n";
    }


    return target;
}

//Function to place each ship into the board (Randomly)
void placeShip(char board[][10], int size) {
    bool vertical = rand() % 2; //true = vertical;, false = horizontal
    bool invalidPlacement = true;
    int rowStart, colStart, row, col;

    if (vertical) {  //if place vertically
        while (invalidPlacement) {
            invalidPlacement = false;

            //randomly place ship
            rowStart = rand() % (10 - size);
            col = rand() % 10;
            //Check whether this position overlaps with other ships or not
            for (int i = rowStart; i < rowStart + size; i++) {
                if (board[i][col] == 'O') {
                    invalidPlacement = true;
                    break;
                }
            }
        }
        for (int i = rowStart; i < rowStart + size; i++) {
            board[i][col] = 'O'; //place ship
        }
    }
    else {  //if place horizontally
        while (invalidPlacement) {
            invalidPlacement = false;

            //randomly place ship
            colStart = rand() % (10 - size);
            row = rand() % 10;
            //Check whether this position overlaps with other ships or not
            for (int i = colStart; i < colStart + size; i++) {
                if (board[row][i] == 'O') {
                    invalidPlacement = true;
                    break;
                }
            }
        }
        for (int i = colStart; i < colStart + size; i++) {
            board[row][i] = 'O'; //place ship
        }
    }
}

//Function to generate random board (randomly put the ships into the board)
void generateRandomBoard(char board[][10]) {
    placeShip(board, 5);  //place the size-5 ship
    placeShip(board, 4);  //place the size-4 ship
    placeShip(board, 3);  //place the size-3 ship
    placeShip(board, 3);  //place the size-3ship
    placeShip(board, 2);  //place the size-2 ship
}

//function for validating the final coordinates of a ship when deploying, and store the ships coordinates to the player board if validated successfully
bool poschecker1(int &irow, int &icol, int &frow, int &fcol, char board[][10], int size)
{
  //check if the ship is overlapped by another ship or not
  if (board[frow][fcol] != 'O')
  {
    int flag = 0;

    //check if the ship is placed horizontally
    if (irow == frow)
    {
      //check if the final coordinates is the right of the initial coordinates, and swap them if it is not the case
      if (icol > fcol)
      {
        int temp = icol;
        icol = fcol;
        fcol = temp;
        flag = 1;
      }

      //check whether coordinates in between have been used or not
      for (int i = icol+1; i <= fcol - flag; i++)
      {

        if (board[irow][i] == 'O')
        {
          return false;
        }
      }

      //store the coordinates the ship
      for (int i = icol+1; i <= fcol - flag; i++)
      {
        board[irow][i] = 'O';
      }

      //storing the initial coordinates of the ship if the final and initial coordinates are swapped
      if (flag == 1)
      {
        board[irow][icol] = 'O';
      }
      return true;
    }
    else
    {
      //check if the final coordinates is the below the initial coordinates, and swap them if it is not the case
      if (irow > frow)
      {
        int temp = irow;
        irow = frow;
        frow = temp;
        flag = 1;

      }
      //check whether coordinates in between have been used or not
      for (int i = irow+1; i <= frow - flag; i++)
      {
        if (board[i][icol] == 'O')
        {
          cout<<i<<endl;
          return false;
        }
      }

      //store the coordinates the ship
      for (int i = irow+1; i <= frow - flag; i++)
      {
        board[i][icol] = 'O';
      }

      //storing the initial coordinates of the ship if the final and initial coordinates are swapped
      if (flag == 1)
      {
        board[irow][icol] = 'O';
      }

      return true;
    }
  }
  return false;

}

//function for validating the initial coordinates of a ship when deploying
bool poschecker(int row, int col, char board[][10], int size)
{

  //check if the ship is overlapped by another ship or not
  if (board[row][col] != 'O')
  {

    //check if the left side of the of the initial coordinates has enough spaces for putting the ship
    int left = col -size;
    if (left > 0)
    {
      int flag = 0;

      //check if the any ship used one of the coordinates on the left side
      for (int i = left; i < col; i++)
      {
        if (board[row][i] == 'O')
        {
          flag = 1;
          break;
        }
      }

      //store the initial coordinates and stop the program if there is enough spaces
      if (flag == 0)
      {
        board[row][col] = 'O';
        return true;
      }
    }

    //check if the right side of the of the initial coordinates has enough spaces for putting the ship
    int right = col + size;
    if (right <= 9)
    {
      int flag = 0;

      //check if the any ship used one of the coordinates on the left side
      for (int i = col; i < right; i++)
      {
        if (board[row][i] == 'O')
        {
          flag = 1;
          break;
        }
      }

      //store the initial coordinates and stop the program if there is enough spaces
      if (flag == 0)
      {
        board[row][col] = 'O';
        return true;
      }
    }

    //check if the north side of the of the initial coordinates has enough spaces for putting the ship
    int up = row - size;
    if (up > 0)
    {
      int flag = 0;

      //check if the any ship used one of the coordinates on the north side
      for (int i = up; i < row; i++)
      {
        if (board[i][col] == 'O')
        {
          flag = 1;
          break;
        }
      }

      //store the initial coordinates and stop the program if there is enough spaces
      if (flag == 0)
      {
        board[row][col] = 'O';
        return true;
      }
    }

    //check if the south side of the of the initial coordinates has enough spaces for putting the ship
    int bot = col + size;
    if (bot <= 9)
    {
      int flag = 0;

      //check if the any ship used one of the coordinates on the south side
      for (int i = row; i < bot; i++)
      {
        if (board[i][col] == 'O')
        {
          flag = 1;
          break;
        }
      }

      //store the initial coordinates and stop the program if there is enough spaces
      if (flag == 0)
      {
        board[row][col] = 'O';
        return true;
      }
    }
    return false;
  }
  return false;
}

//function for validating the input
bool checkvalidinput(string &target)
{
  if (target.size() == 2)
  {

    //validate the format of the coordinates
    if (((target[0] >= 'A' && target[0] <= 'J') || (target[0] >= 'a' && target[0] <= 'j')) && target[1] >= '0' && target[1] <= '9')
    {

      //convert the coordinates if it is in small letter
      if (target[0] >= 'a' && target[0] <= 'j')
      {
          target[0] = target[0] - 'a' + 'A';
      }
      return true;
    }
  }

  return false;
}
//function for allowing the player to input the initial and final coordinates of each ships
void deployment(char board[][10])
{
  string pos[2] = {};
  pos[0] = "starting";
  pos[1] = "ending";


  int shipsize[5] = {4,3,2,2,1};
  cout<<endl;

  //prompt user to input coordinates of the ships
  for (int i = 0; i < 5; i++)
  {
    int j = 0;
    while (true)
    {
      cout<<"Please enter the "<<pos[j]<<" coordinates of "<<fleet[i].name<<":"<<endl;
      int invalidinput = 1;
      string target;
      getline(cin,target);

      if (checkvalidinput(target))
      {

        //validate the initial coordinates and store it, else continue to prompt user for valid input
        if (j == 0)
        {
          fleet[i].yposini = (int)target[0] - 'A';
          fleet[i].xposini = (int)target[1] - '0';

          if (poschecker(fleet[i].yposini, fleet[i].xposini, boards.player, shipsize[i]))
          {
            board[fleet[i].yposini][fleet[i].xposini] = 'O';
            j++;
            invalidinput = 0;
            printBoard();
          }
        }

        //validate the final coordinates and store it, else continue to prompt user for valid input
        else if (j == 1)
        {
          fleet[i].yposfnl = (int)target[0] - 'A';
          fleet[i].xposfnl = (int)target[1] - '0';

          if (pow(fleet[i].yposini - fleet[i].yposfnl, 2) + pow(fleet[i].xposini - fleet[i].xposfnl,2) == pow(shipsize[i],2))
          {
            if (poschecker1(fleet[i].yposini, fleet[i].xposini, fleet[i].yposfnl, fleet[i].xposfnl, boards.player, shipsize[i]))
            {
              cout<<endl;
              printBoard();
              break;
            }
          }
        }
      }

      //prompt error message if the input is invalid
      if (invalidinput == 1)
      {
        cout<<"***Invalid input! Please enter the coordinates correctly.***"<<endl;
      }
    }
  }
}

//function to score a hit and update the map
void fire(int row, int col)
{

  //validate the coordinates
  if (row >= 0 && row < 10 && col >= 0 && col < 10)
  {
    if (boards.AI[row][col] == 'O' || boards.AI[row][col] == '@')
    {

      //if hit, mark '@'
      boards.AI[row ][col] = '@';
      boards.playerViewAIBoard[row][col] = '@';

    }
    else
    {
      boards.AI[row][col] = 'X';   //if not hit, mark 'X'
      boards.playerViewAIBoard[row ][col] = 'X';
    }

  }

}

//function for basic firing
void generalfire()
{
  string target;
  int targetRow, targetCol;
  target = playerInput();

  //convert the coordinates to digits
  targetRow = (int)target[0] - 'A';
  targetCol = (int)target[1] - '0';

  fire(targetRow, targetCol);
}

//check whether the required ship has been sunk or not
bool sunk(int num)
{
  //getting the position of the ship
  int x1 = fleet[num].xposini;
  int x2 = fleet[num].xposfnl;
  int y1 = fleet[num].yposini;
  int y2 = fleet[num].yposfnl;

  //for the case when the ship is placed vertically
  if (x1 == x2)
  {

    //check if all parts of the ship has been hit
    for (int i = y1; i <= y2; i++)
    {
      if (boards.player[i][x1] != '@')
      {
        return false;
      }
    }
  }

  //for the case when the ship is placed vertically
  else if (y1 == y2)
  {

    //check if all parts of the ship has been hit
    for (int i = x1; i <= x2; i++)
    {
      if (boards.player[y1][i] != '@')
      {
        return false;
      }
    }
  }

  //print sunk message to the player
  cout<<endl<<fleet[num].name<<"has been sunk, type anything to confirm"<<endl;
  return true;
}

//determine which country is selected
int selcountry(string country)
{
  int num = -1;
  if (country == "japan")
  {
    num = 0;
  }
  else if (country == "germany")
  {
    num = 1;
  }
  else if (country == "italy")
  {
    num = 2;
  }
  else if (country == "us")
  {
    num = 3;
  }
  else if (country == "gb")
  {
    num = 4;
  }

  return num;
}

//function for squadron feature
void squadron(string country)
{

  //check if the aircraft carrier is sunk
  if (sunk(0))
  {
    string sth;
    cin>>sth;
  }

  //check if the aircraft carrier is out of planes
  else if (spweapons[0] == 0)
  {
    cout<<endl<<"Out of planes to form attack squadrons or reconnaissance! Type anything to confirm"<<endl;
    string sth;
    cin>>sth;
  }
  else
  {
    int num = selcountry(country);
    cout<<endl<<"***"<<fleet[0].name<<" REPORTING***"<<endl;
    cout<<radar[num]<<" OPERATIVE"<<endl;

    //to randomize the plane for attack squadron once
    if(planeflag == 0)
    {
      plane1 = rand() % numofplane[num];
      planeflag = 1;
    }

    string s = " ready to go";

    //getting the mode for planes
    cout<<endl<<"***AWAITING ORDERS***"<<endl;
    cout<<"To launch assault, type \"1\""<<endl<<"To conduct reconnaissance, type \"2\""<<endl;

    while (true)
    {
      string plane;
      cin>>plane;

      //for attacking
      if (plane == "1")
      {
        if (spweapons[1] == 1)
        {

          //picking a plane for different country
          cout<<endl;
          if (num == 0)
          {
            cout<<bomber.ijn[plane1]<<s;
          }
          else if (num == 1)
          {
            cout<<bomber.kms[plane1]<<s;
          }
          else if (num == 2)
          {
            cout<<bomber.rm[plane1]<<s;
          }
          else if (num == 3)
          {
            cout<<bomber.hms[plane1]<<s;
          }
          else if (num == 4)
          {
            cout<<bomber.uss[plane1]<<s;
          }

          cout<<endl<<endl<<"***AWAITING ASSUALT COORDINATES***"<<endl;
          cout<<"Please enter the center coordinates of the attack zone with a \"+\" pattern"<<endl;

          //getting the center coordinates of the hit zone
          string center;
          cin>>center;

          //validating the coordinates
          while (!(checkvalidinput(center)))
          {
            cout<<"Invalid coordinates! Please input valid coordinates"<<endl;
            getline(cin, center);
          }

          int centerRow, centerCol;
          centerRow = (int)center[0] - 'A';
          centerCol = (int)center[1] - '0';

          //storing all the coordinates of the hit zone to targets[]
          Target targets[5];

          targets[0].row = centerRow -1;
          targets[0].col = centerCol;

          for (int i = 0; i < 3; i++)
          {
            targets[i+1].row = centerRow;
            targets[i+1].col = centerCol-1+i;
          }

          targets[4].row = centerRow +1;
          targets[4].col = centerCol;

          //firing on the hit zone
          for (int i = 0; i < 5; i++)
          {
            fire(targets[i].row, targets[i].col);
          }
          spweapons[1] = 0;
          spweapons[0] -=1;
        }
        else
        {
          cout<<"Out of planes to form attack squadron! Please type anything to confirm"<<endl;
          string sth;
          cin>>sth;
        }
        break;
      }

      //for recon mode
      else if (plane == "2")
      {
        if (spweapons[2] == 1)
        {
          cout<<endl<<recon[num]<<s<<endl;
        }

        cout<<endl<<"***AWAITING RECONNAISSANCE COORDINATES***"<<endl;
        cout<<"Please enter the center coordinates of the recon zone with a \"X\" pattern"<<endl;

        //getting the center coordinates of the recon zone
        string center;
        cin>>center;

        //validating the coordinates
        while (!(checkvalidinput(center)))
        {
          cout<<"Invalid coordinates! Please input valid coordinates"<<endl;
          getline(cin, center);
        }

        int centerRow, centerCol;
        centerRow = (int)center[0] - 'A';
        centerCol = (int)center[1] - '0';

        //storing the coordinates of the recon zone
        Target targets[5];

        int j = 0;
        for (int i = 0; i < 2; i++)
        {
          targets[i].row = centerRow - 1;
          targets[i].col = centerCol - 1 + j + i;
          j++;
        }

        targets[2].row = centerRow;
        targets[2].col = centerCol;

        j = 0;
        for (int i = 0; i < 2; i++)
        {
          targets[i+3].row = centerRow + 1;
          targets[i+3].col = centerCol - 1 + j + i;
          j++;
        }

        char row[10] = {'A','B','C','D','E','F','G','H','I','J'};

        int flag = 0;

        //scanning on the given zone
        for (int i = 0; i < 5; i++)
        {

          //if hostile found
          if (boards.AI[targets[i].row][targets[i].col] == 'O')
          {
            cout<<endl<<"ENEMY SPOTTED IN "<<row[targets[i].row]<<targets[i].col<<endl;
            boards.playerViewAIBoard[targets[i].row][targets[i].col] = '!';
            flag = 1;
          }

          //if no hostile found
          else
          {
            boards.AI[targets[i].row][targets[i].col] = 'X';   //if not hit, mark 'X'
            boards.playerViewAIBoard[targets[i].row][targets[i].col] = 'X';
          }
        }

        cout<<endl<<"***RECONNAISSANCE COMPLETED***"<<endl;
        if (flag == 0)
        {
          cout<<"NO HOSTILE SPOTTED"<<endl;
        }
        cout<<endl<<"Please type anything to confirm"<<endl;
        string sth;
        cin>>sth;
        break;
      }
      else
      {
        cout<<"Invalid command, please type as instructed above"<<endl;
      }
    }
  }
}

//function for the barrage feature
void barrage(string country)
{

  //check if the battleship is sunk
  if (sunk(1))
  {
    string sth;
    getline(cin,sth);
  }

  //check if the battleship is out of shells
  else if (spweapons[3] == 0)
  {
    cout<<endl<<"Out of shells for performing barrage! Type anything to confirm"<<endl;
    string sth;
    getline(cin,sth);
  }
  else
  {
    int num = selcountry(country);
    cout<<endl<<"***"<<fleet[1].name<<" REPORTING***"<<endl;
    cout<<radar[num]<<" OPERATIVE"<<endl;
    cout<<shell[num]<<" ARMED"<<endl;
    cout<<"Ready to launch, input center coordinates of a 3x3 grid"<<endl;

    //getting the center coordinates of the hit zone
    string center;
    getline(cin, center);

    //validating the coordinates
    while (!(checkvalidinput(center)))
    {
      cout<<"Invalid coordinates! Please input valid coordinates"<<endl;
      getline(cin, center);
    }


    int centerRow, centerCol;
    centerRow = (int)center[0] - 'A';
    centerCol = (int)center[1] - '0';

    //storing all the coordinates of the hit zone to targets[]
    Target targets[9];

    //for the first row
    for (int i = 0; i < 3; i++)
    {
      targets[i].row = centerRow -1;
      targets[i].col = centerCol -1 +i;
    }

    //for the second row
    for (int i = 0; i < 3; i++)
    {
      targets[i+3].row = centerRow;
      targets[i+3].col = centerCol-1+i;
    }

    //for the third row
    for(int i = 0; i < 3; i++)
    {
      targets[i+6].row = centerRow+1;
      targets[i+6].col = centerCol-1 +i;
    }

    //fire according to the coordinates
    for (int i = 0; i < 9; i++)
    {
      fire(targets[i].row, targets[i].col);
    }

    spweapons[3] = 0;
  }
}

//function for the cruiser feature
void cruiser(string country)
{

  //check if the heavy cruiser is sunk
  if (sunk(2))
  {
    string sth;
    getline(cin,sth);
  }

  //check if the heavy cruiser is out of torpedos
  else if (spweapons[4] == 0)
  {
    cout<<endl<<"Out of torpedos to perform strike! Type anything to confirm"<<endl;
    string sth;
    getline(cin,sth);
  }
  else
  {
    int num = selcountry(country);
    cout<<endl<<"***"<<fleet[2].name<<" REPORTING***"<<endl;
    cout<<radar[num]<<" OPERATIVE"<<endl;
    cout<<torpedo[num]<<" ARMED"<<endl;
    cout<<"Ready to launch, input center coordinates of a 1x3 or 3x1 grid"<<endl;

    //getting the center coordinates of the torpedo
    string center;
    getline(cin, center);

    //validating the coordinates
    while (!(checkvalidinput(center)))
    {
      cout<<"Invalid coordinates! Please input valid coordinates"<<endl;
      getline(cin, center);
    }

    //getting the firing mode
    cout<<endl<<"***INPUT TORPEDO MODE***"<<endl<<"For horizontal mode, type \"h\""<<endl<<"For vertical mode, type \"v\""<<endl;
    char mode;
    cin>>mode;

    int centerRow, centerCol;

    centerRow = (int)center[0] - 'A';
    centerCol = (int)center[1] - '0';

    //getting the coordinates of the hit zone
    Target targets[3];
    while (true)
    {
      if (mode == 'h')
      {
        for (int i = 0; i < 3; i++)
        {
          targets[i].row = centerRow;
          targets[i].col = centerCol -1 + i;
        }
        break;
      }
      else if (mode == 'v')
      {
        for (int i = 0; i < 3; i++)
        {
          targets[i].row = centerRow -1 + i;
          targets[i].col = centerCol;
        }
        break;
      }
      else
      {
        cout<<"Invalid mode! Please input as instructed above"<<endl;
      }
    }

    //firing on the hit zone
    for (int i = 0; i < 3; i++)
    {
      fire(targets[i].row, targets[i].col);
    }

    spweapons[4] -= 1;
  }
}

//function for the sub feature
void sub(string country)
{

  //check if the submarine is sunk
  if (sunk(3))
  {
    string sth;
    getline(cin,sth);
  }
  else
  {
    int num = selcountry(country);
    cout<<endl<<"***"<<fleet[3].name<<" REPORTING***"<<endl;
    cout<<"PERISCOPE DEPTH AND STABLE"<<endl;

    //check the if there are any torpedo left
    if (spweapons[5] == 0)
    {
      cout<<torpedo[num]<<" DEPLEPTED"<<endl;

    }
    else
    {
      cout<<torpedo[num]<<" ARMED"<<endl;
    }
    cout<<endl<<"***AWAITING ORDERS***"<<endl;

    //getting the mode for operation
    cout<<"To launch torpedo, type \"torpedo\""<<endl<<"To perform scanning, type \"scanning\""<<endl;


    while (true)
    {
      string plan;
      getline(cin,plan);
      //if torpedo is chosen
      if (plan == "torpedo")
      {
        //check if the submarine is out of torpedos
        if (spweapons[5] == 0)
        {
          cout<<endl<<"Out of torpedos to perform strike! Type anything to confirm"<<endl;
          string sth;
          getline(cin,sth);
        }
        else
        {
          cout<<endl<<"***ENTER FIRING PATTERN***"<<endl<<"To fire the torpedo horizontally, type \"h\""<<endl<<"To fire the torpedo vertically, type \"v\""<<endl;

          char mode;
          cin>>mode;

          //validating the mode
          while (true)
          {
            if (mode != 'h' && mode != 'v')
            {
              cout<<"Invalid mode! Please input as instructed above"<<endl;
              cin>>mode;
            }
            else
            {
              break;
            }
          }

          //getting the starting point of the torpedo
          cout<<endl<<"Input the starting point the torpedo's course which is a point along the edge"<<endl;
          string center;
          cin>>center;

          //validating the coordinates
          while (!(checkvalidinput(center)))
          {
            cout<<"Invalid coordinates! Please input valid coordinates"<<endl;
            getline(cin, center);
          }

          int centerRow, centerCol;

          centerRow = (int)center[0] - 'A';
          centerCol = (int)center[1] - '0';

          //firing the torpedo horizontally
          if (mode == 'h')
          {

            //firing the torpedo from the right side of the map
            if (centerCol == 9)
            {
              for (int i = 0; i < 10; i++)
              {
                fire(centerRow, 9-i);
                if (boards.AI[centerRow][9-i] == '@')
                {
                  break;
                }
              }
            }

            //firing the torpedo from the left side of the map
            else
            {
              for (int i = 0; i < 10; i++)
              {
                fire(centerRow, i);
                if (boards.AI[centerRow][i] == '@')
                {
                  break;
                }
              }
            }
          }

          //firing the torpedo vertically
          else
          {

            //firing the torpedo from the south side of the map
            if (centerRow == 9)
            {
              for (int i = 0; i < 10; i++)
              {
                fire(9-i, centerCol);
                if (boards.AI[9-i][centerCol] == '@')
                {
                  break;
                }
              }
            }

            //firing the torpedo from the north side of the map
            else
            {
              for (int i = 0; i < 10; i++)
              {
                fire(i, centerCol);
                if (boards.AI[i][centerCol] == '@')
                {
                  break;
                }
              }
            }
          }
        spweapons[5] -= 1;
        }
        break;
      }

      //if scanning mode is chosen
      else if (plan == "scanning")
      {

        //getting the center coordinates of the scanning zone
        cout<<"Please enter the center coordinates of a 3x3 grid"<<endl;
        string center;
        cin>>center;

        //validating the coordinates
        while (!(checkvalidinput(center)))
        {
          cout<<"Invalid coordinates! Please input valid coordinates"<<endl;
          getline(cin, center);
        }

        int centerRow, centerCol;
        centerRow = (int)center[0] - 'A';
        centerCol = (int)center[1] - '0';

        //storing all the coordinates of the scanning zone to targets[]
        Target targets[9];

        //for the first row
        for (int i = 0; i < 3; i++)
        {
          targets[i].row = centerRow -1;
          targets[i].col = centerCol -1 +i;
        }

        //for the second row
        for (int i = 0; i < 3; i++)
        {
          targets[i+3].row = centerRow;
          targets[i+3].col = centerCol-1+i;
        }

        //for the third row
        for(int i = 0; i < 3; i++)
        {
          targets[i+6].row = centerRow+1;
          targets[i+6].col = centerCol-1 +i;
        }

        //scanning the zone
        int flag = 0;
        for (int i = 0; i < 9; i++)
        {

          //if AI vessel found in the zone
          if (boards.AI[targets[i].row][targets[i].col] == 'O')
          {
            cout<<"***SONAR SWEEP DETECTS ENEMY VESSEL --- PRECISE LOCATION UNCONFIRMED***"<<endl;
            flag = 1;
            break;
          }
        }

        //if no AI vessel are found in the zone
        if (flag == 0)
        {
          cout<<"***SONAR SWEEP CONFIRMS NO HOSTILE IN SELECTED WATERS***"<<endl;
        }

        cout<<"Type anything to confirm"<<endl;
        string sth;
        cin>>sth;
        break;
      }
      else
      {
        cout<<"Invalid command, please type as instructed above"<<endl;
      }
    }
  }
}

//function for allowing player to pick which advanced weapon
void advancedweapons(string country)
{
  printBoard();

  cout<<endl<<"To change back to regular rounds, type \"ap\""<<endl;
  cout<<"To launch "<<fleet[0].name<<"'s torpedo bomber squadron, type \"plane\""<<endl;
  cout<<"To launch "<<fleet[1].name<<"'s barrage, type \"barrage\""<<endl;
  cout<<"To launch "<<fleet[2].name<<"'s torpedo, type \"cruiser\""<<endl;
  cout<<"To launch "<<fleet[3].name<<"'s torpedo, type \"sub\""<<endl;
  while(true)
  {
    string mode;
    getline(cin,mode);

    if (mode == "ap")
    {
      generalfire();
      break;
    }
    else if (mode == "plane")
    {
      squadron(country);
      break;
    }
    else if (mode == "barrage")
    {
      barrage(country);
      break;
    }
    else if (mode == "cruiser")
    {
      cruiser(country);
      break;
    }
    else if (mode == "sub")
    {
      sub(country);
      break;
    }
    else if (mode == "q")
    {
      break;
    }
    else
    {
      cout<<"***INVALID ORDER, PLEASE RETRY INPUTTING ORDERS AS INSTRUCTED ABOVE***"<<endl;
    }
  }
}

//function to randomly generate an id that has not been exist yet and directly store it into the file
void generateID(){
  int existedID[maxSavedRecords], count = 0;
  bool validID = true;
  string line, firstWord, id;

  ifstream fin;
  fin.open("Unfinished_Game.txt");
  if(fin.fail()){
    cout << "Failed opening file!" << endl;
    exit(1);
  }

  while(getline(fin, line)){
    istringstream iss(line);
    iss >> firstWord  >> id;
    if(firstWord == "loadGameData:"){
      existedID[count] = stoi(id);
      count++;
    }
  }

  for(int i = 0; i < maxSavedRecords; i++){//TODO when have time: notify when there are 50 records in the Unfinished_Game.txt (have to delete a record first)
    validID = true;
    for(int j = 0; j < count; j++){
      if (i == existedID[j]){
        validID = false;
      }
    }
    if (validID){
      loadGameData.id = i;
      break;
    }
  }
  fin.close();
}

//save game into "Unfinished_Game.txt" for future
void saveGame(){
  //input game name and player name
  cout << "Please enter your name for saving: ";
  getline(cin, loadGameData.playerName);
  cout << "Please name your game: ";
  getline(cin, loadGameData.gameName);
  generateID();
  
  ofstream fout;
  fout.open("Unfinished_Game.txt", ios::app);
  if(fout.fail()){
    cout << "Failed opening file!\n";
    exit(1);
  }

  //store first line (recording data other than boards)
  fout << "loadGameData: " << loadGameData.id << " " << loadGameData.playerName << " " << loadGameData.gameName << " ";
  fout << loadGameData.country << " " << loadGameData.totalMovesUsed << " ";
  fout << todaysDate() << " ";
  fout << currentTime() << endl;

  //store all four boards into the file
  for(int i = 0; i < boardSize; i++){
    for (int j = 0; j < boardSize; j++){
      fout << boards.player[i][j];
    }
  }
  fout << endl;
  for(int i = 0; i < boardSize; i++){
    for (int j = 0; j < boardSize; j++){
      fout << boards.AI[i][j];
    }
  }
  fout << endl;
  for(int i = 0; i < boardSize; i++){
    for (int j = 0; j < boardSize; j++){
      fout << boards.playerViewAIBoard[i][j];
    }
  }
  fout << endl;
  for(int i = 0; i < boardSize; i++){
    for (int j = 0; j < boardSize; j++){
      fout << boards.AIViewPlayerBoard[i][j];
    }
  }
  fout << endl;
  fout << endl;

  fout.close();
}

//when game finished, same record into "Game_History.txt"
void saveRecord(bool playerWon){
  ofstream fout;
  fout.open("Game_History.txt", ios::app);
  if(fout.fail()){
    cout << "Failed opening file! this???" << endl;
    exit(1);
  }
  //if player won, then game_history file save "1"
  fout << ((playerWon) ? "1" : "0") << " " << loadGameData.totalMovesUsed << " " << loadGameData.playerName << " " << loadGameData.gameName << " " << loadGameData.country << " " << todaysDate() << endl;

  fout.close();
}

//check if game finished, return true if finished, will also check whether player won or lose
bool gameFinished(bool &playerWon){
  bool gameFin = true;

  //check if player's board is empty or not
  for(int i = 0; i < boardSize; i++){
    for (int j = 0; j < boardSize; j++){
      if(boards.player[i][j] == 'O'){
        gameFin = false;
        break;
      }
    }
    if(! gameFin){
      break;
    }
  }
  if(gameFin){
    playerWon = false;
    return true;
  }

  //check if AI's board is empty or not
  gameFin = true;
  for(int i = 0; i < boardSize; i++){
    for (int j = 0; j < boardSize; j++){
      if(boards.AI[i][j] == 'O'){
        gameFin = false;
        break;
      }
    }
    if(! gameFin){
      break;
    }
  }
  if(gameFin){
    playerWon = true;
    return true;
  }

  return false;
}

//function for actual game process
void playGame(string country){
  string input;
  bool playerWon, gameOver = false;
  while (! gameOver) {
    tempPrintBoard();
    
    cout<< endl <<"AWAITING ORDER!!!"<<endl;
    cout<<"To fire regualr rounds, type the coordinate (row, column)"<<endl;
    cout<<"To use special weapons, type \"aw\""<<endl;
    cout<<"To save the game, type \"s\""<<endl;
    cout<<"To quit the game, type \"q\""<<endl;
    cout << endl;


    input = playerInput();

    //different actions based on which type of player input
    if (input == "aw")   //advanced weapons
    {
      advancedweapons(country);
    }
    else if (input == "Q" || input == "q")   //quit the game
    {
        //ask user if he/she wants to save the file
        cout << "Are you sure you want to quit the game without saving? (\'y\' to quit, \'n\' to save): ";
        getline(cin, input);
        while (input.length() != 1 || (input[0] != 'Y' && input[0] != 'y' && input[0] != 'N' && input[0] != 'n')){
          cout << "Invalid input! (\'y\' to quit, \'n\' to save)";
          cout << "Your choice: ";
          getline(cin, input);
        }
        if(input == "N" || input == "n"){
          saveGame();
        }
        break;
    }
    else if (input == "S" || input == "s"){   //save the game
      saveGame();
      break;
    }
    else{    //normal fire
      fire((int)input[0] - 'A', (int)input[1] - '0');
    }
    loadGameData.totalMovesUsed++;
    gameOver = gameFinished(playerWon);
  }

  //when game is over, save game records
  if(gameOver){
    saveRecord(playerWon);
  }
}

//function to read a line of "Game_History.txt" file
void readFromFileToProgram(string inputLine, char board[][boardSize]){
  for(int i = 0; i < boardSize; i++){
    for(int j = 0; j < boardSize; j++){
      board[i][j] = inputLine[i*boardSize+j];
    }
  }
}

//function to display all existing unfinished games (return whether unfinished games exist or not)
bool displayExistingGames(string existingID[]){
  string line, firstWord, temp;
  int count = 0;
  bool gameExist = false;
  
  system("CLS");
  cout << "All existing game:\n";
  cout << "ID " << setw(13) << "Player Name" << setw(13) << "Game Name" << setw(10) << "Country";
  cout << setw(5) << "Move" << setw(13) << "Date" << setw(8) << "Time" << endl;
  cout << "--- ------------ ------------ --------- ---- ------------ -------\n";

  ifstream fin;
  fin.open("Unfinished_Game.txt");
  if(fin.fail()){
    cout << "Failed opening file!";
    exit(1);
  }
  //for every heading line in the file, output its information
  while(getline(fin, line)){
    istringstream iss(line);
    iss >> firstWord;
    if(firstWord == "loadGameData:"){
      gameExist = true;
      iss >> existingID[count] >> loadGameData.playerName >> loadGameData.gameName >> loadGameData.country >> temp >> loadGameData.date >> loadGameData.time;
      cout << setw(2) << existingID[count] << ": " << setw(12) << loadGameData.playerName << " ";
      cout << setw(12) << loadGameData.gameName << " " << setw(9) << loadGameData.country << " ";
      cout << setw(4) << temp << " " << setw(12) << loadGameData.date << " " << setw(7) << loadGameData.time << endl;
      count++;
    }
  }
  fin.close();

  return gameExist;
}

//function to allow player to select an existing game to load (with input validation), return ID of existing game or -1 if quit
int selectExistingGame(string existingID[]){
  bool validFormat = true;
  int selectedGame;
  string input;

  while (true){
    validFormat = true;
    cout << endl << "Select a game to load ('Q' to quit): ";
    getline(cin, input);

    //if player wants to quit
    if (input == "Q" || input == "q"){
      return -1;
    }

    //check if format is correct or not
    for(int i = 0; input[i] != '\0'; i++){
      if (input[i]< '0' || input[i] > '9'){
        validFormat = false;
        break;
      }
    }

    if (validFormat){
      selectedGame = stoi(input);

      //check if input is valid or not, if yes, then return the selected existing game
      for(int i = 0; i < maxSavedRecords && ! existingID[i].empty(); i++){
        if (selectedGame == stoi(existingID[i])){
          return selectedGame;
        }
      }
      cout << "Invalid input! Please select an existing game!" << endl;
    }
    else {
      cout << "Invalid input! Please type an integer or \'Q\' to quit!" << endl;
    }
  }
} 

//function to load the existing game from the file to the program and start playing it
void loadGame(){
  string line, firstWord, secondWord, existingID[maxSavedRecords], temp;
  ships uss[50], kms[50], hms[50], rm[50], ijn[50];
  int selectedGame, lineRead;
  bool gameExist, startReadingLine = false;
  
  gameExist = displayExistingGames(existingID);

  if (gameExist){
    selectedGame = selectExistingGame(existingID);
    
    if(selectedGame != -1){
      ifstream fin;
      fin.open("Unfinished_Game.txt");
      if(fin.fail()){
        cout << "Failed opening file!";
        exit(1);
      }

      //store the game data in the film into our program
      while(getline(fin, line)){
        
        if(!startReadingLine){
          istringstream iss(line);
          iss >> firstWord;
          if(firstWord == "loadGameData:"){
            lineRead = 0;
            iss >> secondWord >> loadGameData.playerName >> loadGameData.gameName >> loadGameData.country >> temp;
            loadGameData.id = stoi(secondWord);
            loadGameData.totalMovesUsed = stoi(temp);
            cout << loadGameData.id << selectedGame << endl;
            if(loadGameData.id == selectedGame){
              startReadingLine = true;
              continue;
            }
          }
        }
        if(startReadingLine){
          switch(lineRead){
            case 0:
              readFromFileToProgram(line, boards.player);
              break;
            case 1:
              readFromFileToProgram(line, boards.AI);
              break;
            case 2:
              readFromFileToProgram(line, boards.playerViewAIBoard);
              break;
            case 3:
              readFromFileToProgram(line, boards.AIViewPlayerBoard);
              break;
          }
          lineRead++;
        }
        if (lineRead == 4){
          break;
        }
      }
      fin.close();


      //start the game
      naming(uss,"USS");
      naming(kms,"KMS");
      naming(hms,"HMS");
      naming(rm,"RM");
      naming(ijn,"IJN");
      genfleet(uss, kms, hms, rm, ijn, loadGameData.country, fleet);
      playGame(loadGameData.country);
    }
  }
  else{
    cout << endl << "There is no existing game!" << endl;
  }
}

//function to generate a new Game
void newGame() {
  ships uss[50], kms[50], hms[50], rm[50], ijn[50];

  naming(uss,"USS");
  naming(kms,"KMS");
  naming(hms,"HMS");
  naming(rm,"RM");
  naming(ijn,"IJN");

  srand(time(NULL));

  system("CLS");
  int flag = 0;
  cout<<"To play as the United States, type \"us\""<<endl;
  cout<<"To play as Great Britain, type \"gb\""<<endl;
  cout<<"To play as German Reich, type \"germany\""<<endl;
  cout<<"To play as the Kingdom of Italy, type \"italy\""<<endl;
  cout<<"To play as the Empire of Great Japan, type \"japan\""<<endl << endl;
  string country;

  while (true)
  {
    cout << "Choose a country: ";
    getline(cin,country);
    if (!(country == "us" || country == "gb" || country == "germany" || country == "italy" || country == "japan"))
    {
      cout<<"***Invalid country name inputted! Please enter the country name according to the above instructions***"<<endl;
      continue;
    }
    else
    {
      break;
    }
  }
  loadGameData.country = country;
  loadGameData.totalMovesUsed = 0;

  genfleet(uss, kms, hms, rm, ijn, country, fleet);
  deployment(boards.player);
  //generateRandomBoard(boards.player);
  generateRandomBoard(boards.AI);

  playGame(country);
}

//function to print all game records history
void printAllRecords(GameRecords* &head){
  GameRecords* current = head;

  system("CLS");

  cout << "Rank  Player Name    Game Name   Country Result Move         Date" << endl;
  cout << "---- ------------ ------------ --------- ------ ---- ------------" << endl;

  for(int i = 1; current!=NULL; i++){
    cout << setw(4) << i << " " << setw(12) << current->playerName << " " << setw(12) << current->gameName << " ";
    cout << setw(9) << current->country << " " << setw(6) << ((current->won) ? "Won" : "Lose") << " ";
    cout << setw(4) << current->totalMoveUsed << " " << setw(12) << current->date << endl;
    current = current->next;
  }
  current = new GameRecords;
  delete current;
}

//function to sort and store the game history records in order of (win, then lose), then (ascending order of amount of moves used)
void newGameRecord(GameRecords* &head, string line, int size){
  int wonInt, movesInt;
  string won, move, gameName, playerName, date, country;
  bool newRecordInserted = false;

  //store all information of this record into a temporary dynamic array of data
  cout << line << endl;
  istringstream iss(line);
  iss >> won >> move >> playerName >> gameName >> country >> date;
  wonInt = stoi(won);
  movesInt = stoi(move);
  
  GameRecords* temp = new GameRecords;
  GameRecords* current = head;
  GameRecords* data = new GameRecords;
  data->won = wonInt;
  data->totalMoveUsed = movesInt;
  data->playerName = playerName;
  data->gameName = gameName;
  data->country = country;
  data->date = date;
  data->next = NULL;

  //store records into the dynamic array in a specific order
  if(head == NULL || (wonInt > head->won || (wonInt == head->won && movesInt <= head->totalMoveUsed))){
    temp = head;
    data->next = temp;
    head = data;
    newRecordInserted = true;
  }
  else{
    while (current->next != NULL){    
      if (wonInt > current->next->won || (wonInt == current->next->won && movesInt <= current->next->totalMoveUsed)){
        data->next = current->next;
        current->next = data;
        
        newRecordInserted = true;
        break;
      }
      current = current->next;
    }
  }
  if(!newRecordInserted){
    current->next = data;
  }

  temp = new GameRecords;
  current = new GameRecords;
  data = new GameRecords;
  delete temp;
  delete current;
  delete data;
}

//function to access file "Game_History.txt", extracting all game history records
void showGameRecord(){
  int size = 0;
  string input, line;
  GameRecords* head = NULL;

  ifstream fin;
  fin.open("Game_History.txt");
  if(fin.fail()){
    cout << "Failed opening file!" << endl;
    exit(1);
  }
  
  //loop until all records in the file have been optained, and stored in a dynamic array structure
  while(getline(fin, line)){
    newGameRecord(head, line, size);
  }
  fin.close();

  printAllRecords(head);

  cout << endl << "Q to quit: ";
  getline(cin, input);

}

//function that prints the Main Menu Screen (allow player to select choices)
int mainMenu(){
  system("CLS");
  string choice;

  //main menu screen
  cout << "********************************************" << endl;
  cout << "***** Welcome to Battleship Remastered *****" << endl;
  cout << "********************************************" << endl;
  cout << "1). New Game" << endl;
  cout << "2). Load Existing Game" << endl;
  cout << "3). Show Game History" << endl << endl;
  cout << "Your choice (\'q\' to quit): ";
  getline(cin, choice);

  //input validation
  while (!(choice.length() == 1 && ((choice[0] >= '1' && choice[0] <= '3') || choice[0] == 'q' || choice[0] == 'Q'))){
    cout << "Invalid input! Please input (1-3) or \'Q\' for quit!" << endl;
    getline(cin, choice);
  }
  return (choice[0] >= '1' && choice[0] <= '3') ? (int) (choice[0] - '0') : 0;

}

//function to wipe board, clearing all the data in the board
void wipeBoard(char board[][boardSize]){
  for(int i = 0; i < boardSize; i++){
    for(int j = 0; j < boardSize; j++){
      board[i][j] = (char) 0;
    }
  }
}

//main function
int main(){
  int choice;
  choice = mainMenu();
  //loop whil player didn't enter 'q' or 'Q' for quit
  while(choice != 0){
    wipeBoard(boards.player);
    wipeBoard(boards.AI);
    wipeBoard(boards.playerViewAIBoard);
    wipeBoard(boards.AIViewPlayerBoard);
    switch (choice){
      //create new game (generating a new board)
      case 1:
        newGame();
        break;
      //load game (extracting an existing game from the file)
      case 2:
        loadGame();
        break;
      //show all game history
      case 3:
        showGameRecord();
        break;
    }
    
    choice = mainMenu();
  }

  return 0;
}