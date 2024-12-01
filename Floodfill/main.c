#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "API.h"

// Global variables for current position of the robot. 
// 0,0 being top left, 4,4 being bottom right
int currRow = 15;
int currCol = 0;

// Global 2D integer array that 
// represents the flood fill algorithm
int path[16][16];
char walls[16][16];

// Max width and height of the maze
int MAX_X = 16;
int MAX_Y = 16;

// Mock sensor values. Either true or false
int wallLeft;
int wallRight;
int wallFront;

// Values to determine what way the robot is facing
// and what direction it needs to turn to.
// 0 Up, 1 Right, 2 Down, 3 Left
int goTo = 0;
int facing = 1;

/*
    This function is part of the Micromouse simulator.
    This was not written by us.
*/
void log(char* text) {
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}

/*
    This function is used to initialize the path array
    to -1 to indicate that the maze algorithm has not 
    been ran.
    
    Also called after every move to rerun the flood fill
    alogrithm
*/
void initPath() {
    
    //Sets every cell to a value of -1
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            path[x][y] = -1;
        }
    }

    return;
}

/*
    Recursive function to run the flood fill algorithm
    and take into consideration the walls of the maze.
*/
#define MAX_X 16  // Maximum rows for a 16x16 maze
#define MAX_Y 16  // Maximum columns for a 16x16 maze

/*void flood(int pos_row, int pos_col, int distance) {

    // Check to see if current row and column are outside of the bounds.
    // If so, do nothing and return
    if (pos_row > MAX_X - 1 || pos_row < 0) return;
    if (pos_col > MAX_Y - 1 || pos_col < 0) return;

    // Check to see if the current cell is a lower distance value than the current call.
    // If so, no need to change the value, return.
    if (path[pos_row][pos_col] < distance && path[pos_row][pos_col] != -1) return;

    // Set the current cell equal to the current distance from the center.
    path[pos_row][pos_col] = distance;

    // Recursive call on the cell to the right of current cell.
    if (pos_col < MAX_Y - 1 && !(walls[pos_row][pos_col] & 0b0100))
        flood(pos_row, pos_col + 1, distance + 1);

    // Recursive call on the cell to the left.
    if (pos_col > 0 && !(walls[pos_row][pos_col - 1] & 0b0100))
        flood(pos_row, pos_col - 1, distance + 1);
    
    // Recursive call on the cell to the bottom of current cell.
    if (pos_row < MAX_X - 1 && !(walls[pos_row][pos_col] & 0b0010))
        flood(pos_row + 1, pos_col, distance + 1);
    
    // Recursive call on the cell to the top.
    if (pos_row > 0 && !(walls[pos_row - 1][pos_col] & 0b0010))
        flood(pos_row - 1, pos_col, distance + 1);
    
    return;
}*/
void flood(int pos_row, int pos_col, int distance) {
    if (pos_row >= MAX_X || pos_row < 0 || pos_col >= MAX_Y || pos_col < 0) return;
    if (path[pos_row][pos_col] <= distance && path[pos_row][pos_col] != -1) return;
    path[pos_row][pos_col] = distance;

    if (pos_col < MAX_Y - 1 && !(walls[pos_row][pos_col] & 0b0100))
        flood(pos_row, pos_col + 1, distance + 1);
    if (pos_col > 0 && !(walls[pos_row][pos_col - 1] & 0b0100))
        flood(pos_row, pos_col - 1, distance + 1);
    if (pos_row < MAX_X - 1 && !(walls[pos_row][pos_col] & 0b0010))
        flood(pos_row + 1, pos_col, distance + 1);
    if (pos_row > 0 && !(walls[pos_row - 1][pos_col] & 0b0010))
        flood(pos_row - 1, pos_col, distance + 1);
}

/*
    Function to display the flood fill alogirthm onto the MicroMouse
    Simulator
*/
void print() {
   
    char text[2];
    int pathNUM;

    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 16; col++) {
            
            pathNUM = path[15 - col][ row];   
            itoa(pathNUM,text,10); //from integer to ascii converter funcrion, 10 stands for base 10
            API_setText(row, col, text);
            text[0] = ' ';
            text[1] = ' ';

        }
    }
}

void getSensors() {

    wallLeft = API_wallLeft();
    wallFront = API_wallFront();
    wallRight = API_wallRight();

}

void updateWalls() {
    int maxIndex = 15; // Maximum index for a 16x16 maze

    // If the mouse is facing up
    if (facing == 0) { 

        // Wall to the left condition
        if (wallLeft && currCol != 0) {
            walls[currRow][currCol - 1] |= 0b0100;
            API_setWall(currCol, maxIndex - currRow, 'w');
        }

        // Wall to the right condition
        if (wallRight && currCol != maxIndex) {
            walls[currRow][currCol] |= 0b0100;
            API_setWall(currCol, maxIndex - currRow, 'e');
        }

        // Wall to the top condition
        if (wallFront && currRow != 0) {
            walls[currRow - 1][currCol] |= 0b0010;
            API_setWall(currCol, maxIndex - currRow, 'n');
        }

        return;
    }

    // If the mouse is facing right
    if (facing == 1) {

        // Wall is up condition
        if (wallLeft && currRow != 0) {
            walls[currRow - 1][currCol] |= 0b0010;
            API_setWall(currCol, maxIndex - currRow, 'n');
        }

        // Wall is down condition
        if (wallRight && currRow != maxIndex) {
            walls[currRow][currCol] |= 0b0010;
            API_setWall(currCol, maxIndex - currRow, 's');
        }

        // Wall is right condition
        if (wallFront && currCol != maxIndex) {
            walls[currRow][currCol] |= 0b0100;
            API_setWall(currCol, maxIndex - currRow, 'e');
        }

        return;
    }

    // If the mouse is facing down
    if (facing == 2) {

        // Wall is right condition
        if (wallLeft && currCol != maxIndex) {
            walls[currRow][currCol] |= 0b0100;
            API_setWall(currCol, maxIndex - currRow, 'e');
        }

        // Wall is left condition
        if (wallRight && currCol != 0) {
            walls[currRow][currCol - 1] |= 0b0100;
            API_setWall(currCol, maxIndex - currRow, 'w');
        }

        // Wall is down condition
        if (wallFront && currRow != maxIndex) {
            walls[currRow][currCol] |= 0b0010;
            API_setWall(currCol, maxIndex - currRow, 's');
        }

        return;
    }

    // If the mouse is facing left
    if (facing == 3) {

        // Wall is down condition
        if (wallLeft && currRow != maxIndex) {
            walls[currRow][currCol] |= 0b0010;
            API_setWall(currCol, maxIndex - currRow, 's');
        }

        // Wall is up condition
        if (wallRight && currRow != 0) {
            walls[currRow - 1][currCol] |= 0b0010;
            API_setWall(currCol, maxIndex - currRow, 'n');
        }

        // Wall is left condition
        if (wallFront && currCol != 0) {
            walls[currRow][currCol - 1] |= 0b0100;
            API_setWall(currCol, maxIndex - currRow, 'w');
        }

        return;
    }
}

void findMove() {
    int left, right, down, up;
    int maxIndex = 15; // Maximum index for a 16x16 maze

    // Check to see if the robot is on an edge row or column.
    if (currCol != 0) {
        left = path[currRow][currCol - 1];
    } else {
        left = 99;
    }

    if (currCol != maxIndex) {
        right = path[currRow][currCol + 1];
    } else { 
        right = 99;
    }

    if (currRow != 0) {
        up = path[currRow - 1][currCol];
    } else { 
        up = 99;
    }

    if (currRow != maxIndex) {
        down = path[currRow + 1][currCol];
    } else { 
        down = 99;
    }

    // Check for walls
    if (currRow > 0 && (walls[currRow - 1][currCol] & 0b0010)) {
        up = 99;
    }    

    if (currCol > 0 && (walls[currRow][currCol - 1] & 0b0100)) {
        left = 99;
    }

    if (currCol < maxIndex && (walls[currRow][currCol] & 0b0100)) {
        right = 99;
    }

    if (currRow < maxIndex && (walls[currRow][currCol] & 0b0010)) {
        down = 99;
    }
    
    // Determine the direction with the lowest path value
    if (up <= right && up <= down && up <= left) {
        goTo = 0;
        return;
    }
    
    if (left <= right && left <= down && left <= up) {
        goTo = 3;
        return;
    }
    
    if (right <= left && right <= down && right <= up) {
        goTo = 1;
        return;
    }
    
    if (down <= right && down <= left && down <= up) {
        goTo = 2;
        return;
    }       
}

void turnRobot() {

    // Robot is facing up
    if (facing == 0) {

        // Return if already facing correct way
        if (goTo == 0) {
            return;
        }

        // Turn robot to face right
        if (goTo == 1) {
            API_turnRight();
            facing = 1;
            return;
        }

        // Turn robot to face down
        if (goTo == 2) {
            facing = 2;
            API_turnRight();
            API_turnRight();
            return;
        }
        // Turn robot to face left
        if (goTo == 3) {
            facing = 3;
            API_turnLeft();
            return;
        }
    }
    
    // If robot is facing the right
    if (facing == 1) {
        
        // Turn robot to face up
        if (goTo == 0) {
            facing = 0;
            API_turnLeft();
            return;
        }

        // Return if facing correct way
        if (goTo == 1) {
            facing = 1;
            return;
        }

        // Turn robot to face down
        if (goTo == 2) {
            facing = 2;
            API_turnRight();
            return;
        }

        // Turn robot to face left
        if (goTo == 3) {
            facing = 3;
            API_turnRight();
            API_turnRight();
            return;
        }
    }

    // If robot is facing down
    if (facing == 2) {
        
        //Turn robot to face up
        if (goTo == 0) {
            facing = 0;
            API_turnRight();
            API_turnRight();
           
            return;
        }

        // Turn robot to face right
        if (goTo == 1) {
            facing = 1;
            API_turnLeft();
            return;
        }

        // Return if facing correct way
        if (goTo == 2) {
            facing = 2;
            return;
        }

        // Turn robot to face left
        if (goTo == 3) {
            facing = 3;
            API_turnRight();
            return;
        }
    }

    // Robot is facing left
    if (facing == 3) {
         
         // Turn robot to face up
         if (goTo == 0) {
            facing = 0;
            API_turnRight();
            return;
        }

        // Turn robot to face right
        if (goTo == 1) {
            facing = 1;
            API_turnRight();
            API_turnRight();
            return;
        }
        
        // Turn robot of face down
        if (goTo == 2) {
            facing = 2;
            API_turnLeft();
            return;
        }

        // Return if facing correct way
        if (goTo == 3) {
            facing = 3;
            return;
        }
    }
}

void moveRobot() {
    
    // Make sure there is not a wall in front
    if (wallFront) {
        return;
    }
  
    // Update current row and column depending on what
    // direction the mouse is facing and update accordingly
    if (facing == 0 && currRow > 0) {  // Add boundary check for currRow
        currRow = currRow - 1;
        API_moveForward();
        return;
    }

    if (facing == 1 && currCol < 15) { // Add boundary check for currCol
        currCol = currCol + 1;
        API_moveForward();
        return;
    }

    if (facing == 2 && currRow < 15) { // Add boundary check for currRow
        currRow = currRow + 1;
        API_moveForward();
        return;
    }

    if (facing == 3 && currCol > 0) {  // Add boundary check for currCol
        currCol = currCol - 1;
        API_moveForward();
        return;
    }
}


void printPOS() {
    
    char text[3];  // Updated size to hold two digits and a null terminator
    int num = currRow;
    itoa(num, text, 10);  
    API_setText(0, 5, text);  // Assuming these coordinates are valid
    num = currCol;
    itoa(num, text, 10); 
    API_setText(1, 5, text); 
    num = facing;
    itoa(num, text, 10); 
    API_setText(2, 5, text); 
    num = goTo;
    itoa(num, text, 10); 
    API_setText(3, 5, text); 
}


/*
    Main function for running code
*/
int main(int argc, char* argv[]) {
    
    log("Running...");

    // Initialize maze of be empty
    initPath();
    
    //Orientate Mouse to be facing right
    API_turnRight();

    // Initial flood flill
    flood(7, 7, 0); //Initialize the flood fill for the cell at (7, 7)
    flood(7, 8, 0); //Initialize the flood fill for the cell at (7, 8)
    flood(8, 7, 0); //Initialize the flood fill for the cell at (8, 7)
    flood(8, 8, 0); //Initialize the flood fill for the cell at (8, 8)
    
    // Set current position
    currRow = 15;
    currCol = 0;
    //END OF INIT

    // Run while the robot is not at the center
    while (currRow != 7 || currCol != 7) {
        
        initPath();// Clears path array
        getSensors();// Checks for walls
        updateWalls();// Updates walls array based on information

        //flood(2,2,0);// Updates Algorithm
        flood(7, 7, 0); //Initialize the flood fill for the cell at (7, 7)
        flood(7, 8, 0); //Initialize the flood fill for the cell at (7, 8)
        flood(8, 7, 0); //Initialize the flood fill for the cell at (8, 7)
        flood(8, 8, 0); //Initialize the flood fill for the cell at (8, 8)

//also I need to update the flood fucntion

        print();// Update simulator to display values
        findMove();// Determine next move
        turnRobot();// Turn robot to face correct way

        // I reached here
        printPOS();// Debugging print
        moveRobot();// Move robot for

        // Break once maze is sovled
        /*if (currRow == 7 && currCol == 7 || 
            currRow == 7 && currCol == 8 ||
            currRow == 8 && currCol == 7 ||
            currRow == 8 && currCol == 8) {
            log("Terget Reached.");
            break;
        }*/
        
    }
}

