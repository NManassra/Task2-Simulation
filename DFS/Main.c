#include <stdio.h>

#include "API.h"

#define MAX_SIZE 32

// Directions: 0=North, 1=East, 2=South, 3=West
int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};

// Visited array and stack
int visited[MAX_SIZE][MAX_SIZE] = {0};
int stack[MAX_SIZE * MAX_SIZE][2];
int top = -1;

// Stack helper functions
void push(int x, int y) {
    stack[++top][0] = x;
    stack[top][1] = y;
}

void pop(int *x, int *y) {
    *x = stack[top][0];
    *y = stack[top][1];
    top--;
}

int isValid(int x, int y, int width, int height) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

// Rotate to face a specific direction
void turnToDirection(int currentDir, int targetDir) {
    int turnSteps = (targetDir - currentDir + 4) % 4;
    if (turnSteps == 1) {
        API_turnRight();
    } else if (turnSteps == 2) {
        API_turnRight();
        API_turnRight();
    } else if (turnSteps == 3) {
        API_turnLeft();
    }
}

int main() {
    int mazeWidth = API_mazeWidth();
    int mazeHeight = API_mazeHeight();

    int targetX = 7, targetY = 7; // Define the target cell

    int x = 0, y = 0; // Start at (0, 0)
    int dir = 0;      // Start facing North (0=North, 1=East, 2=South, 3=West)
    visited[x][y] = 1;
    push(x, y);

    while (top >= 0) {
        // Check if the target is reached
        if (x == targetX && y == targetY) {
            API_setText(x, y, "Target"); // Optional: Mark the target visually
            API_setColor(x, y, 'G');     // Optional: Highlight the target cell
            break; // Stop the search
        }

        int moved = 0;
        for (int i = 0; i < 4; i++) {
            int newDir = (dir + i) % 4;
            int nx = x + dx[newDir];
            int ny = y + dy[newDir];

            if (isValid(nx, ny, mazeWidth, mazeHeight) && !visited[nx][ny]) {
                // Check if there's no wall in the direction
                if (newDir == dir && !API_wallFront()) {
                    moved = 1;
                } else if ((newDir == (dir + 1) % 4) && !API_wallRight()) {
                    turnToDirection(dir, newDir);
                    moved = 1;
                } else if ((newDir == (dir + 3) % 4) && !API_wallLeft()) {
                    turnToDirection(dir, newDir);
                    moved = 1;
                }

                if (moved) {
                    visited[nx][ny] = 1;
                    push(nx, ny);
                    API_moveForward();
                    x = nx;
                    y = ny;
                    dir = newDir;
                    break;
                }
            }
        }

        if (!moved) {
            // Backtrack
            int prevX, prevY;
            pop(&prevX, &prevY);

            // Determine the direction to the previous cell
            int backDir = -1;
            for (int i = 0; i < 4; i++) {
                if (prevX == x + dx[i] && prevY == y + dy[i]) {
                    backDir = i;
                    break;
                }
            }

            if (backDir != -1) {
                turnToDirection(dir, backDir);
                API_moveForward();
                x = prevX;
                y = prevY;
                dir = backDir;
            }
        }
    }

    return 0;
}
