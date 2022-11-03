/*
FA21-BSE-134 Muhammad Taimoor
FA21-BSE-105 Tabarak Sohail
FA21-BSE-137 Fahad Ali
*/

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <conio.h>
#include <time.h>

using namespace std;

const int ROW = 5;
const int COL = 10;

const int GAP = 5; // Ranges 1 - 10
const int TIME = 60; // Time 60 seconds
const int INTERVAL = 2; // delay before another fall

const int POST_FRAME_SIZE = (ROW + 2) * (COL + 1) * GAP;

void exiting() {
    cout << "Do you wants to exit? (Y/n)" << endl;
    char key;
    cin >> key;
    if (tolower(key) == 'y')
        exit(EXIT_SUCCESS);
    else
        cout << "Continued..." << endl;
}

void clearScreen() {
    system("cls");
}

char *postProcess(char frame[ROW][COL]) {
    static char postFrame[POST_FRAME_SIZE];
    int position = 0;

    for (int i = 0; i < COL * GAP; ++i) {
        postFrame[position++] = '_';
    }
    postFrame[position++] = '\n';

    for (int row = 0; row < ROW; ++row) {
        for (int col = 0; col < COL; ++col) {
            postFrame[position++] = frame[row][col];
            for (int i = 1; i < GAP; ++i)
                postFrame[position++] = ' ';
        }
        postFrame[position++] = '\n';
    }

    for (int i = 0; i < COL * GAP; ++i) {
        postFrame[position++] = '^';
    }
    postFrame[position++] = '\n';

    return postFrame;
}

void displayFrame(char frame[ROW][COL], int hits, int miss, int wrong, time_t timeRem) {
    clearScreen();

    cout << "\nRemaining Time: " << timeRem << endl;
    const char *postFrame = postProcess(frame);
    puts(postFrame);

    cout << "Hits:\t" << hits << endl;
    cout << "Missed:\t" << miss << endl;
    cout << "Wrong:\t" << wrong << endl;
}

void note() {
    clearScreen();
    string note = "NOTE!!!\n"
                  "This game is based on typing\n"
                  "It aims to fasten typing speed\n"
                  "Easy mode contains only alphabets\n"
                  "Hard mode also contains some special characters\n";

    cout << note << endl;
}

int setDifficulty() {
    cout << "SELECT DIFFICULTY" << endl;
    cout << "1. Easy" << endl;
    cout << "2. Hard" << endl;

    char option;
    while (true) {
        cout << "Enter Option: ";
        cin >> option;

        if (option == '1' || option == '2')
            return option - 48; // 48 is Ascii value of 0
        else
            cout << "Wrong Input..." << endl;
    }
}

void shuffle(char row[COL]) {
    for (int i = 0; i < COL; ++i) {
        int random = rand() % COL;
        char temp = row[i];
        row[i] = row[random];
        row[random] = temp;
    }
}

bool contains(char frame[ROW][COL], char target) {
    for (int row = 0; row < ROW; ++row)
        for (int col = 0; col < COL; ++col)
            if (target == frame[row][col])
                return true;
    return false;
}

bool randomChoice() {
    return (rand() % 2) && (rand() % 2);
}

void generateRow(char frame[ROW][COL], bool isHard) {
    for (char &item: frame[0])
        if (randomChoice()) {
            char randomChar;
            do {
                if (!isHard || !randomChoice())
                    randomChar = 97 + rand() % 26;
                else
                    randomChar = 33 + rand() % 15;
            } while (contains(frame, randomChar));
            item = randomChar;
        }
}

int countItems(char frame[ROW][COL]) {
    int count = 0;
    for (int row = 1; row < ROW; ++row)
        for (int col = 0; col < COL; ++col)
            if (frame[row][col] != ' ')
                ++count;
    return count;
}

int rowFall(char frame[ROW][COL], bool isHard) {
    int miss = 0;
    for (char item: frame[ROW - 1])
        if (item != ' ')
            ++miss;
    for (int row = ROW - 1; row > 0; --row) {
        shuffle(frame[row - 1]);
        for (int col = 0; col < COL; ++col)
            frame[row][col] = frame[row - 1][col];
    }
    for (char &item: frame[0])
        item = ' ';
    if (countItems(frame) <= (isHard ? 41 : 26) - COL)
        generateRow(frame, isHard);
    return miss;
}

bool killChar(char frame[ROW][COL], char target) {
    for (int row = 0; row < ROW; ++row)
        for (int col = 0; col < COL; ++col)
            if (frame[row][col] == target) {
                frame[row][col] = ' ';
                return true;
            }
    return false;
}

void game(int difficulty) {
    static char frame[ROW][COL];
    int hits = 0;
    int miss = 0;
    int wrong = 0;

    for (auto &i: frame)
        for (char &j: i)
            j = ' ';

    time_t start = time(nullptr);
    time_t timeRem = TIME;

    int fall = TIME + INTERVAL;
    displayFrame(frame, hits, miss, wrong, timeRem);

    char key = 0;

    while (timeRem > 0) {
        timeRem = TIME + start - time(nullptr);
        if (timeRem < fall - INTERVAL + 1) {
            fall = (int) timeRem;
            miss += rowFall(frame, difficulty == 2);
            displayFrame(frame, hits, miss, wrong, timeRem);
        }
        if (_kbhit()) {
            key = _getch();
            if (key == 27) {
                exiting();
            }
            if (killChar(frame, key))
                ++hits;
            else
                ++wrong;
            displayFrame(frame, hits, miss, wrong, timeRem);
        }
    }
    cout << "\nYou got " << hits << " Hits, " << miss << " Missed and " << wrong << " Wrong Inputs" << endl;
    cout << "press any key to exit..." << endl;
    getch();
}

int main() {
    srand(time(nullptr));
    note();
    int difficulty = setDifficulty();
    cout << "press any key after 2 seconds to start...";
    sleep(2);
    getch();
    game(difficulty);
    return 0;
}
