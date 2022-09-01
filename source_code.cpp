#include <windows.h> // using system()
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <direct.h> // using _getced()
#include <fstream>
#include <time.h>
#include <string>
using namespace std;
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define D 9

class Common
{
public:
    virtual void printMainBoard(void){};
    virtual void setupGrid(void) = 0;
};

class Time
{
    double seconds;
    int minutes;

public:
    Time(void) : seconds(0), minutes(0) {}
    Time(double s) : seconds(s), minutes(0) {}
    void convertIntoMinutes(void);
    friend ostream &operator<<(ostream &os, const Time &t);
};

class Init
{
protected:
    int difficulty_level;

public:
    int mainBoard[D][D];
    Init(void);
    Init(int dif);
    int readDificulty(void);
};

class User
{
    int rightEnteries, totalEnteries;
    string name, level;
    Time t1;

public:
    User(void);
    User(int rightEn, int totalEn, string na, double time);
    void gameEnding(void);
    bool showChoices(void);
    bool react_on(int c);
    void saveData(void);
    void setLevel(int l);
};

class Grid : public Init, public Common
{

public:
    Grid(void);
    Grid(int diff);
    bool isSafe(int num, int, int);
    bool isSafe(int grid[D][D], int num, int, int);
    bool isGridRight(int, int);
    void shuffleGrid(void);
    void setupGrid(void);
    void writeGrid(void);
};

class HandleEvents : public Init, public Common
{
    int userBoard[D][D], questionBoard[D][D], nullEnteries, rightEnteries;

public:
    int d;
    HandleEvents(void);
    HandleEvents(int diff);
    Init readGrid(void);
    bool setPositions(int &x, int &y);
    void changeEntry(int &x, int &y);
    void instructions(void) const;
    void setBoard(void);
    void printBoard(int &x, int &y);
    void setupGrid(void);
    void printMainBoard(void);
    int getTotalEnteries(void) { return nullEnteries; }
    int getRightEnteries(void) { return rightEnteries; }
};

/*
0 = Black
8 = Gray
1 = Blue
9 = Light Blue
2 = Green
A = Light Green
3 = Aqua
B = Light Aqua
4 = Red
C = Light Red
5 = Purple
D = Light Purple
6 = Yellow
E = Light Yellow
7 = White
F = BringWhite
*/

int tempBoard[D][D] = {
    {5, 4, 1,    3, 2, 7,    9, 6, 8},
    {8, 6, 2,    4, 1, 9,    7, 3, 5},
    {3, 9, 7,    8, 5, 6,    4, 2, 1},

    {2, 7, 8,    9, 4, 3,    5, 1, 6},
    {4, 1, 6,    5, 8, 2,    3, 9, 7},
    {9, 3, 5,    6, 7, 1,    2, 8, 4},
    
    {1, 2, 4,    7, 3, 8,    6, 5, 9},
    {6, 5, 3,    1, 9, 4,    8, 7, 2},
    {7, 8, 9,    2, 6, 5,    1, 4, 3},
    };

void setConsoleColor(int textColor, int bgColor)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (textColor + (bgColor * 16)));
}

bool solveSudoku(int grid[D][D], int row, int col)
{
    if (row == D - 1 && col == D - 1)
        return true;

    if (col == D)
    {
        row++;
        col = 0;
    }

    if (grid[row][col] > 0)
        return solveSudoku(grid, row, col + 1);

    for (int num = 1; num <= D; num++)
    {
        grid[row][col] = num;
        if (solveSudoku(grid, row, col + 1))
            return true;
        grid[row][col] = 0;
    }
    return false;
}

int Init::readDificulty(void)
{
    system("cls");
    char dif;
    do
    {
        cout << "Choose difficulty:\n"
                "\t1. Easy\n"
                "\t\ti)   Wrong & Right enteries will be highlighted in red & blue respectively.\n"
                "\t\tii)  25 <= empty entries <= 30.\n"
                "\t\tiii) You are allowed to view the solution.\n\n"
                "\t2. Medium\n"
                "\t\ti)   Only zeroes (in red) will be highlighted.\n"
                "\t\tii)  35 <= empty entries <= 40.\n"
                "\t\tiii) You are allowed to view the solution.\n\n"
                "\t3. Hard\n"
                "\t\ti)   No entry will be highlighted.\n"
                "\t\tii)  45 <= empty entries <= 50.\n"
                "\t\tiii) You are not allowed to view the solution.\n\n"
                "\t   Selection: ";
        cin >> dif;
        system("cls");
    } while (int(dif) - 48 < 0 || int(dif) - 48 > 3);
    setConsoleColor(11, 0);
    difficulty_level = (int)dif - 48;
    return difficulty_level;
}

Init::Init(void)
{
    memcpy(mainBoard, tempBoard, sizeof(int) * D * D);
    difficulty_level = 1;
}

Init::Init(int dif)
{
    memcpy(mainBoard, tempBoard, sizeof(int) * D * D);
    difficulty_level = dif;
}

void Time::convertIntoMinutes(void)
{
    if (seconds > 59)
    {
        minutes = seconds / 60;
        seconds = seconds - minutes * 60;
    }
}

ostream &operator<<(ostream &os, const Time &t)
{
    os << t.minutes << ':' << t.seconds << "\t(min:sec)";
    return os;
}

User::User(void)
{
    totalEnteries = 0;
    rightEnteries = 0;
    name = '\0';
    level = '\0';
}

User::User(int rightEn, int totalEn, string na, double time) : t1(time)
{
    rightEnteries = rightEn;
    totalEnteries = totalEn;
    name = na;
}

void User::gameEnding(void)
{
    cout << "-------------------------------------------\n";
    cout << "You got " << rightEnteries << " out of " << totalEnteries << " enteries right.\n";
    cout << "-------------------------------------------\n";
    t1.convertIntoMinutes();
}
bool User::showChoices(void)
{
    int choice;
    cout << "\nSelect one\n"
            "\t1. See leaderboards\n"
            "\t2. See your score\n"
            "\t3. Restart Game\n"
            "\t4. Delete your data\n"
            "\t5. Exit\n"
            "\t   Selection: ";
    cin.ignore();
    do
    {
        cin >> choice;
    } while (choice < 1 || choice > 5);
    system("cls");
    return react_on(choice);
}

void User::saveData(void)
{
    ofstream f("user.dat", ios::app);
    f.write((char *)this, sizeof(*this));
    f.close();
}

void User::setLevel(int l)
{
    if (l == 1)
        level = "Easy";
    else if (l == 2)
        level = "Medium";
    else
        level = "Hard";
}

bool User::react_on(int c)
{
    if (c == 1)
    {
        ifstream r1("user.dat", ios::binary);
        cout << "---------------------------------------------------------------------------------------------------------------------\n";
        cout << "Sr\t\tName\t\t\tLevel\t\tAchieved Score\tTotal Score\tTime Elapsed\n";
        cout << "---------------------------------------------------------------------------------------------------------------------\n";
        int i = 1;
        while (r1.read((char *)this, sizeof(*this)))
        {
            cout << i << "\t\t" << name << "\t\t\t" << level << "\t\t" << rightEnteries * 5 << "\t\t" << totalEnteries * 5 << "\t\t" << t1 << "\n";
            i++;
        }
        cout << "---------------------------------------------------------------------------------------------------------------------\n";
        r1.close();
    }

    else if (c == 2)
    {
        cout << "---------------------------------------------------------------------------------------------------------------------\n"
                "Sr\t\tName\t\t\tLevel\t\tAchieved Score\tTotal Score\tTime Elapsed\n"
                "---------------------------------------------------------------------------------------------------------------------\n";
        cout << "1\t\t" << name << "\t\t\t" << level << "\t\t" << rightEnteries * 5 << "\t\t" << totalEnteries * 5 << "\t\t" << t1 << "\n";
        cout << "---------------------------------------------------------------------------------------------------------------------\n";
    }
    else if (c == 3)
        return 1;
    else if (c == 4)
    {
        string NAME = name;
        ifstream r1("user.dat");
        ofstream f1("userTemp.dat");
        while (r1.read((char *)this, sizeof(*this)))
        {
            if (NAME == name)
                continue;
            f1.write((char *)this, sizeof(*this));
        }
        r1.close();
        f1.close();
        remove("user.dat");
        rename("userTemp.dat", "user.dat");
        cout << "\n>------- Data deleted -------<\n";
    }
    return 0;
}

Grid::Grid()
{
    memcpy(mainBoard, tempBoard, sizeof(int) * D * D);
}

Grid::Grid(int diff) : Init(diff)
{

    memcpy(mainBoard, tempBoard, sizeof(int) * D * D);
}

bool Grid::isSafe(int num, int row, int col)
{
    return isSafe(mainBoard, num, row, col);
}

bool Grid::isSafe(int grid[D][D], int num, int row, int col)
{

    // Check if program find the same num in the similar row
    for (int x = 0; x <= 8; x++)
        if (grid[row][x] == num)
            return false;

    // Check if program find the same num in the similar column
    for (int x = 0; x <= 8; x++)
        if (grid[x][col] == num)
            return false;

    // Check if program find the same num in the particular 3*3 matrix
    int startRow = row - row % 3,
        startCol = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num)
                return false;
    return true;
}

bool Grid::isGridRight(int row, int col)
{

    for (int num = 1; num <= D; num++)
    {
        if (isSafe(num, row, col))
        {
            if (solveSudoku(mainBoard, row, col + 1))
                return true;
        }
    }
    return false;
}

void Grid::shuffleGrid(void)
{
    for (int change = 0; change < D; change++)
    {
        int first = (rand() % D) + 1,
            second = (rand() % D) + 1;

        for (int x = 0; x < D; x++)
        {
            for (int y = 0; y < D; y++)
            {
                if (mainBoard[x][y] == first)
                    mainBoard[x][y] = second;
                else if (mainBoard[x][y] == second)
                    mainBoard[x][y] = first;
            }
        }
    }
}

void Grid::setupGrid(void)
{
    if (!isGridRight(0, 0))
    {
        shuffleGrid();
        writeGrid();
    }
    else
    {
        cout << "Internal error. Grid malfunction!...\n";
        exit(0);
    }
}

void Grid::writeGrid(void)
{
    Init c1;
    memcpy(c1.mainBoard, mainBoard, sizeof(int) * D * D);

    ofstream f("grid.dat");
    f.write((char *)&c1, sizeof(c1));
    f.close();
}

HandleEvents::HandleEvents(void)
{
    Init c1 = readGrid();
    memcpy(mainBoard, c1.mainBoard, sizeof(int) * D * D);
    memcpy(questionBoard, c1.mainBoard, sizeof(int) * D * D);
    nullEnteries = 0;
    rightEnteries = 0;
    d = 1;
}

HandleEvents::HandleEvents(int diff) : Init(diff)
{
    Init c1 = readGrid();
    memcpy(mainBoard, c1.mainBoard, sizeof(int) * D * D);
    memcpy(questionBoard, c1.mainBoard, sizeof(int) * D * D);
    nullEnteries = 0;
    rightEnteries = 0;
    d = 1;
}

Init HandleEvents::readGrid(void)
{
    Init c1;
    ifstream r("grid.dat");
    r.read((char *)&c1, sizeof(c1));
    r.close();
    return c1;
}

void HandleEvents::setBoard(void)
{
    int x, y;
    if (difficulty_level == 1)
        nullEnteries = (rand() % 5) + 26;
    else if (difficulty_level == 2)
        nullEnteries = (rand() % 5) + 36;
    else if (difficulty_level == 3)
        nullEnteries = (rand() % 5) + 46;

    for (int change = 0; change < nullEnteries; change++)
    {
        x = rand() % 9;
        y = rand() % 9;
        while (questionBoard[x][y] == 0)
        {
            x = rand() % 9;
            y = rand() % 9;
        }
        questionBoard[x][y] = 0;
    }
}

bool HandleEvents::setPositions(int &x, int &y)
{
    char key = _getch();
    {
        if (key == KEY_UP)
            x--;
        if (key == KEY_DOWN)
            x++;
        if (key == KEY_LEFT)
            y--;
        if (key == KEY_RIGHT)
            y++;
    }
    {
        if (x < 0)
            x = 8;
        else if (x > 8)
            x = 0;

        if (y < 0)
            y = 8;
        else if (y > 8)
            y = 0;
    }
    system("cls");

    cout << "     ----------------------\n";

    if (key == 'x' || rightEnteries == nullEnteries)
        return 0;
    if (key == 'i')
        instructions();
    if (key == '\r')
    {
        if (questionBoard[x][y] == 0 && userBoard[x][y] != mainBoard[x][y])
            changeEntry(x, y);
        else
        {
            setConsoleColor(4, 0);
            cout << "-->  Entry can't be changed\n";
            setConsoleColor(11, 0);
        }
    }
    cout << "     ----------------------\n\n";

    if (key == 'o')
    {
        if (difficulty_level < 3)
        {
            printMainBoard();
        }
        else
        {
            cout << "Not elligilbe to view solution\n.";
        }
        cout << "Resuming the game in\t5...\n";
        for (int i = 4; i > 0; i--)
        {
            cout << "\t\t\t" << i << "...\n";
            Sleep(1000);
        }
        system("cls");
    }
    return 1;
}

void HandleEvents::printMainBoard(void)
{
    setConsoleColor(7, 0);
    cout << "\t\b\b\b1 2 3   4 5 6   7 8 9\n";
    cout << "   -------------------------\n";

    for (int i = 0; i < D; i++)
    {
        cout << i + 1 << "  | ";
        for (int j = 0; j < D; j++)
        {
            cout << mainBoard[i][j] << " ";
            if ((j + 1) % 3 == 0)
                cout << "| ";
        }
        cout << endl;
        if ((i + 1) % 3 == 0)
            cout << "   -------------------------\n";
    }
    setConsoleColor(11, 0);
}

void HandleEvents::changeEntry(int &x, int &y)
{
    char tempNumber;
    setConsoleColor(9, 0);
    do
    {
        cout << "-->  Enter number (1-9): ";
        cin >> tempNumber;
    } while (int(tempNumber) - 48 < 1 || int(tempNumber) - 48 > 9);

    setConsoleColor(11, 0);

    userBoard[x][y] = int(tempNumber) - 48;

    if (userBoard[x][y] == mainBoard[x][y])
        rightEnteries++;
}

void HandleEvents::instructions(void) const
{
    bool temp = 1;
    cout << "-------------------------------------------------------------------\n";
    cout << "\t\t\t   INSTRUCTIONS\n";
    cout << "-------------------------------------------------------------------\n";
    cout << "\t1. Use cursor keys to move along the board\n"
            "\t2. Press 'Enter' key to change the value at a certain position\n"
            "\t3. Press 'x' key to exit\n"
            "\t4. Press 'o' key to get solution\n"
            "\t5. Press 'i' key to get instructions at any time\n";
    cout << "-------------------------------------------------------------------\n";
    cout << "\t\t\t   GOOD LUCK\n";
    cout << "-------------------------------------------------------------------\n"
            "\nPress any key to start the game...\n";
    while (temp)
    {
        if (_kbhit())
            temp = 0;
    }
}

void HandleEvents::setupGrid(void)
{
    setConsoleColor(14, 0);
    d = readDificulty();
    setBoard();
    memcpy(userBoard, questionBoard, sizeof(int) * D * D);
}

void HandleEvents::printBoard(int &x, int &y)
{
    setConsoleColor(11, 0);
    cout << "\t\b\b\b1 2 3   4 5 6   7 8 9\n";
    cout << "   -------------------------\n";

    for (int i = 0; i < D; i++)
    {
        cout << i + 1 << "  | ";
        for (int j = 0; j < D; j++)
        {
            if (questionBoard[i][j] == 0 && userBoard[i][j] == mainBoard[i][j])
            {
                if (difficulty_level == 1)
                {
                    setConsoleColor(9, 0);
                    if (x == i && y == j)
                        setConsoleColor(0, 9);
                }
                else if (difficulty_level == 2 || difficulty_level == 3)
                {
                    setConsoleColor(11, 0);
                    if (x == i && y == j)
                        setConsoleColor(0, 11);
                }
            }
            else if (userBoard[i][j] != mainBoard[i][j])
            {
                if (difficulty_level == 1)
                {
                    setConsoleColor(4, 0);
                    if (x == i && y == j)
                        setConsoleColor(0, 4);
                }
                else if (difficulty_level == 2 && userBoard[i][j] == 0)
                {
                    setConsoleColor(4, 0);
                    if (x == i && y == j)
                        setConsoleColor(0, 4);
                }
                else
                {
                    setConsoleColor(11, 0);
                    if (x == i && y == j)
                        setConsoleColor(0, 11);
                }
            }
            else if (x == i && y == j)
                setConsoleColor(0, 11);

            cout << userBoard[i][j];
            setConsoleColor(11, 0);
            cout << " ";

            if ((j + 1) % 3 == 0)
                cout << "| ";
        }
        cout << endl;
        if ((i + 1) % 3 == 0)
            cout << "   -------------------------\n";
    }
}

int main()
{
    bool lastChoice = 1;
    int timeStart, temp = 0;
    time_t start, end;
    double dif;
    srand(time(NULL));
    time(&start);

    while (lastChoice)
    {
        string name;
        int x = 0, y = 0;
        timeStart = 0;

        Common *c1 = nullptr;
        Grid grid;
        c1 = &grid;
        c1->setupGrid();

        HandleEvents hEvents;
        hEvents.instructions();
        system("cls");

        cout << "Enter your name: ";
        if (temp > 0)
            cin.ignore();
        getline(cin, name);
        temp++;

        c1 = &hEvents;
        c1->setupGrid();
        cout << "Press any key except 'Enter' to start...";
        while (hEvents.setPositions(x, y))
        {
            timeStart++;
            hEvents.printBoard(x, y);
            if (timeStart == 1)
            {
                time(&start);
                cout << "Clock Started";
            }
            if (timeStart > 10)
                timeStart = 10;
        }
        time(&end);
        dif = difftime(end, start);

        User u1(hEvents.getRightEnteries(), hEvents.getTotalEnteries(), name, dif);
        u1.setLevel(hEvents.d);
        u1.gameEnding();
        u1.saveData();
        lastChoice = u1.showChoices();
    }
    system("pause");
    return 0;
}
