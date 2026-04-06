#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

// Base Class (Abstraction)
class GameObject {
public:
    int x, y;
    GameObject(int x, int y) : x(x), y(y) {}
    virtual char getSymbol() = 0;
};

// Player Class
class Car : public GameObject {
public:
    Car(int x, int y) : GameObject(x, y) {}

    void moveLeft() {
        if (x > 0) x--;
    }

    void moveRight(int width) {
        if (x < width - 1) x++;
    }

    char getSymbol() override {
        return 'A';
    }
};

// Enemy Class
class Enemy : public GameObject {
public:
    Enemy(int x, int y) : GameObject(x, y) {}

    void moveDown(int height, int width) {
        y++;
        if (y >= height) {
            y = 0;
            x = rand() % width;
        }
    }

    char getSymbol() override {
        return 'V';
    }
};

// Game Engine
class Game {
private:
    int width, height;
    Car player;
    Enemy enemy;
    int score;
    bool gameOver;

public:
    Game(int w, int h)
        : width(w), height(h), player(w / 2, h - 1), enemy(rand() % w, 0) {
        score = 0;
        gameOver = false;
    }

    void draw() {
        vector<vector<char> > screen(height, vector<char>(width, ' '));

        screen[player.y][player.x] = player.getSymbol();
        screen[enemy.y][enemy.x] = enemy.getSymbol();

        // Clear screen (simple method)
        cout << string(50, '\n');

        for (int i = 0; i < width + 2; i++) cout << "-";
        cout << endl;

        for (int i = 0; i < height; i++) {
            cout << "|";
            for (int j = 0; j < width; j++) {
                cout << screen[i][j];
            }
            cout << "|" << endl;
        }

        for (int i = 0; i < width + 2; i++) cout << "-";
        cout << "\nScore: " << score << endl;
    }

    void input() {
        char ch;
        cout << "Move (a=left, d=right, q=quit): ";
        cin >> ch;

        if (ch == 'a') player.moveLeft();
        else if (ch == 'd') player.moveRight(width);
        else if (ch == 'q') gameOver = true;
    }

    void logic() {
        enemy.moveDown(height, width);

        if (player.x == enemy.x && player.y == enemy.y) {
            gameOver = true;
        }

        score++;
    }

    void saveScore() {
        ofstream file("score.txt", ios::app);
        file << "Score: " << score << endl;
        file.close();
    }

    void run() {
        while (!gameOver) {
            draw();
            input();
            logic();
        }

        saveScore();
        cout << "Game Over! Final Score: " << score << endl;
    }
};

// Menu
void menu() {
    int choice;

    do {
        cout << "\n--- CAR RACING GAME ---\n";
        cout << "1. Start Game\n";
        cout << "2. View Scores\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            Game game(20, 10);
            game.run();
        }
        else if (choice == 2) {
            ifstream file("score.txt");
            string line;
            cout << "\n--- Previous Scores ---\n";
            while (getline(file, line)) {
                cout << line << endl;
            }
            file.close();
        }

    } while (choice != 3);
}

// Main
int main() {
    srand(time(0));
    menu();
    return 0;
}
