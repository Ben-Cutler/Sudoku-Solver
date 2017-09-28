#include<iostream>
#include<vector>


using namespace std;
const int boxes = 3; // Subgrid size
const int options = boxes * boxes;   // Board is currently 1-9


/**
Square Class
**/
struct position{
	int row;
	int col;
};

class square{
    bool* possibilities; // Keeps track of which numbers are possible
    public:
    square(); // Constructor
    ~square(); // Destructor
    bool known; // If there is only one option in the possibilities, then it is known.
    void setVal (int key); // Sets a value in the square to false/impossible.
    void solveVal(int key); //Sets every value except the key value to true, and the rest to false. Also trips 'known' to true.
    int showNumber(); // If the value is known, this method shows the number
    bool checkForSolved(); // Check if square is known yet
    int countPossibilities(); // Counts the number of possible values
    int* numericOptions(); // Returns an array of the values that could be stored in the square
};
square ::square(){
    /*
    Purpose : Sets 'known' to false, and then sets each possible number to 'true'
    */
    known = false; // Each square starts as unknown

    possibilities = new bool[options];
    for (int i=0 ; i<options ; i++){
        *(possibilities+i) = true; // Defaults each value to true.
    }
};
square :: ~square(){
    delete possibilities;
}
void square :: setVal(int key){
     /*
    Inputs : An integer 'key', which corresponds to a number that has been ruled 'impossible' and can be eliminated
    Outputs : void
    Purpose : To eliminate a number as a possibility.
    */
    *(possibilities + key -1) = false; // We need to set key-1 to false since arrays start at 0, not 1
    return;
}
void square :: solveVal(int key){
    /*
    Inputs : An integer 'key', which corresponds to a number that has been ruled 'the only possible answer', usually as a given. If key is 0 this function does nothing.
    Outputs : void
    Purpose : To set a number as a the only possible one that could be there.
    */
    if (!key){
        return; // If the key is 0 this function does nothing. We need this because unknown values are defaulted to 0
    }
    for (int i=0 ; i<options ; ++i){
        if (i != (key)){
            setVal(i); // Otherwise, it eliminates each element that is not the key.
        }
    }
    known = true; // And the value is known.
}
int  square :: showNumber(){
    /*
    Inputs : void
    Outputs : The number that is the correct square val.
    Purpose : To see / use the number that is known for that square. If the number is not known, it returns 0
    */
    if (known){
        for (int i=0;i<options ;++i){
            if (*(possibilities +i)){ // Finds the only 'true' in the possibilities, and returns its index +1
                    return i+1;
            }
        }
    }
    return 0;
}
int  square :: countPossibilities(){
    /*
    Input: None
    Output: Returns how many numbers are unknown in the square
    */
    int counter = 0;
    for (int i=0 ; i<options ; ++i){
        if (*(possibilities +i)){ // Finds all the  'trues' in the possibilities
            counter+=1;
        }
    }
    return counter;
}
bool square :: checkForSolved(){
    /*
    Inputs: None
    Outputs: True if there's only  one value
    */
    int counter = countPossibilities();
    if (counter ==1){
        //cout << "Found one, updating! " << endl;
        known = true;
        return 1;
    }
    return 0;
}
int* square :: numericOptions(){
    /*
    Inputs: None
    Outputs: Returns a pointer to an array, that array has the numbers that are 'legal' in the square
    Purpose: This array is looped over if we need to guess the values that could be there.
    */
	int* outArray = new int[countPossibilities()];
	int counter = 0;
	for (int i=0 ; i<options ; i++){
		if (*(possibilities+i)){
			*(outArray +counter) = i+1;
			++counter;
		}

	}
	return outArray;
}


/**
Board Class
**/

class board {
    square** sudoku;
    void printANumFreeRow();
    void printANumRow(int);
    bool crossHatch(int,int);
    bool hasError(); // Checks if the board has an error (such as a duplicate)
    void deepCopy(board); // Makes a deep copy of a board
    position findHighPossibilitySpots(); // Finds spots that are unknown, but have very few (usually 2) potential values.
    bool totalCrossHatch(); // Looks at all rows and columns to find numbers by process of elimination
    bool subGridCrossHatch(int, int); // looks at a specific sub grid, and checks for numbers by process of elimination
    bool totalGridCrossHatch(); // Looks at all subgrids
    bool done(); // Check if the board is done or not
public:
    board(); // Constructor
    void fillBoard(); // Fills the board with data from user
    void displayboard(); // prints the board
    void solveBoard(); // 'Main' function, solves the board by process of elimination and some guess and check

};
board :: board(){

    sudoku = new square*[options]; // Set the sudoku pointer pointer to an array of sudoku pointers
    for(int i = 0; i < options; ++i)
        sudoku[i] = new square[options]; // for each sudoku pointer - point it at a new 1xN array
};
void board :: fillBoard(){
    /*
    Purpose : To fill board with the user's numbers.
    */
    int userNum;
    for (int i=0 ; i<options ;++i){
        for (int j = 0 ; j<options ; ++j){
            cin >> userNum; // Get the user's number
            (sudoku[i][j]).solveVal( userNum); // Solve the square with it.
        }
    }
    //cout << "Success!" << endl;
    return;
}
void board :: displayboard(){
    /*
    Inputs : The user enters no data, and this changes no data
    Outputs : This script prints rows which have numbers, or are dividers of 3x3 boxes
    Purpose : To display the data in an aesthetically pleasing manner
     */
     for (int i=0; i<boxes; i++){
        printANumFreeRow();  // This displays the nice bar on the top of the sudoku board

        for(int j = 0; j<boxes;j++){
            printANumRow(boxes*i +j); // This displays the rows which have numbers in them.
            }
        }
    printANumFreeRow(); // We have a bar on the bottom of the board, so we print a final one.
}
void board :: printANumFreeRow() {
    /*
    Input <none>
    Output : A bar of text
    Purpose :This just prints a short row which is a delimiter between 3x3 grids.
    */
    cout << '+';
    for (int i=0 ; i<=boxes ; i++){

        for (int i=0 ; i<2 * boxes -1 ; i++){
                cout << '-';
        }
        cout << '+';
    }
    cout << endl;


    //cout << "+-------+-------+-------+" << endl;

}
void board :: printANumRow(int num){
    /*
    Inputs : Takes in a num, corresponding to the row that's being printed [0-8]
    Outputs : Displays a row which has numbers in it.
    Purpose : To display rows which have numbers in them
    */
    for (int i = 0; i<options ; i++){
       if (!( i%boxes )){
             cout << "| "; // There should be a bar before the 0th value, the 3rd value, the 6th integer
       }
       if ((sudoku[num][i]).showNumber() > 10){
            cout << char ( (sudoku[num][i]).showNumber() ) << ' '; // prints the board and a space char
       }else{
        cout << (sudoku[num][i]).showNumber() << ' '; // prints the board and a space char
       }
    }
    cout << '|' << endl; // There needs to be a bar at the very end of the row
}
bool board :: crossHatch( int row, int col){
    /*
    Inputs: A numbered Row and a numbered column.
    Outputs: If a value was updated via crosshatch, then a true
    Purpose: To look through the rows and column, and eliminate the possibility that two numbers share a row and a column.
    */
    bool foundSome = false;
    int key = (sudoku[row][col]).showNumber();
    for (int i=0 ; i< options ; ++i){
        if(!(sudoku[i][col].known )){
            (sudoku[i][col]).setVal(key);
            foundSome += (sudoku[i][col]).checkForSolved();
        }

        if(!(sudoku[row][i].known )){
            (sudoku[row][i]).setVal(key);
            foundSome += (sudoku[row][i]).checkForSolved();
        }
    }
    return foundSome;
}
bool board :: totalCrossHatch(){
    /*
    Inputs : None
    Outputs: True if a 'CrossHatch' method turns up a value.
    Purpose: To look through each row and each column, if one of the rows/Cols returns that a value was found, then it returns true (To reset)
    */

    for (int i=0 ; i<options ; ++i){
        for (int j=0 ; j<options; ++j){
           if (sudoku[i][j].known){
                if (crossHatch(i,j)){
                    return true ;
                }
           }
        }
    }
    return false;
}
bool board :: subGridCrossHatch(int gridRow, int gridCol){
    /*
    Inputs : The grid rows and columns, numbers between 0 and 'boxes'
    Outputs: True if a number was found, false otherwise
    Purpose: To determine if a subgrid (a boxes x boxes grid) has any potential 'openings' and if so, fill them.
    This is a 3 step process:
    Step 1) Find out what numbers are known in the sudoku grid
    Step 2) For each number in step 1, rule it out as a candidate for each unknown number
    Step 3) Check each box in the sub grid to check for a solved box.
    */
    vector<int> numbersInBox;
    bool foundSome = false;

    for (int l = 0 ; l<3 ; l++){
        for (int i= gridRow*boxes ; i< gridRow*boxes + boxes ; ++i){
            for (int j= gridCol*boxes ; j< gridCol*boxes + boxes ; ++j){
                if (l==0){
                    if (sudoku[i][j].known){
                        numbersInBox.push_back(sudoku[i][j].showNumber()); // For each square in the subgrid, if it's known, then we add it to a vector.
                    }
                }else if (l==1){
                    if (!(sudoku[i][j].known)){
                        for (int k=0 ; k<numbersInBox.size() ; k++){
                            (sudoku[i][j]).setVal(numbersInBox[k]); // For each number in the vector, we can rule it out for the unknown squares
                        }
                    }
                }else if (l==2){
                    if (!(sudoku[i][j]).known)
                    foundSome += (sudoku[i][j]).checkForSolved(); // We finally check each square to see if there's a solved one.
                }
            }
        }
    }

    return foundSome;
}
bool board :: totalGridCrossHatch(){
    /*
    Inputs : None
    Outputs: True if a 'subGridCrossHatch' method turns up a value.
    Purpose: To look through each subgrid, if one of the subgrids returns that a value was found, then it returns true (To reset)
    */
    for (int i=0 ; i<boxes ; ++i){
        for (int j=0 ; j<boxes ; ++j){
            if (subGridCrossHatch(i,j)){
                return true;
            }
        }
    }
    return false;
}
bool board :: done(){
    /*
    Inputs: None
    Outputs: Returns true if there are no unknown values in the sudoku board.
    Purpose: To determine if there are any unknown values in the sudoku board.

    */

    for (int i=0 ; i<options ; i++){
        for (int j=0 ; j<options ; j++){
            if (! (sudoku[i][j].known)){ // Loop through the sudoku board and check the known value
                return false;
            }
        }
    }
    return true;
}
void board :: solveBoard(){
    /*
    Inputs: None (relies on sudoku instance having a few values though)
    Outputs: None
    Purpose: To eliminate possibilities via crosshatching, then make an "educated guess" and recursively solve the puzzle with these guesses and checks.
    */
    while (totalCrossHatch() || totalGridCrossHatch() ){} // Try to solve it via crosshatching alone. Sometimes this alone is enough.
    if ((!done()) && (!hasError())){ // If it exhausts crosshatching and it's not done
        board testBoard; // Make a board that we can 'test out'
        position potentialSpots = findHighPossibilitySpots(); // find a spot of where to guess
        int *guesses = sudoku[potentialSpots.row][potentialSpots.col].numericOptions(); // An array of numbers that count be the solution, and will be guessed
        int amountOfGuesses = sudoku[potentialSpots.row][potentialSpots.col].countPossibilities(); // Size of the 'guesses array'
        for (int i=0 ; (i<amountOfGuesses) ; i++){ // Loop through guesses array
        	testBoard.deepCopy( *this); // Make a deep copy of 'this' into the test board
        	testBoard.sudoku[potentialSpots.row][potentialSpots.col].solveVal(*(guesses+i)); // Apply the guess to the test board
        	testBoard.solveBoard(); // run the solve board method on the test board
        	if (testBoard.done() && !testBoard.hasError()){ // If the  testboard is done & error free, we are finished
        		(*this).deepCopy( testBoard); // we make 'this' a deep copy of the testboard
        		return;
        	}
        	// If this guess wasn't right, the for loop goes onto the other guesses. Ultimately, one of the guesses has to be right.
        }

    }
};
bool board::hasError(){
    /*
    Inputs : An Int 'rowOrColNum' which will be [0,8]. It will be either a row or a column, which is number 'rowORColNum'. The string 'rowORCol' tells is of the number corresponds to a row or a col
    Outputs: Returns a bool, 'failed' . If it returns 0 then it didn't find a duplicate value in the row or column If it returns 1 it also displays the repeated value, and column/row
    Purpose: To test a row or column for duplicate values.
    */


    for (int k=0 ; k<options ; k++){
	    for (int i=0;i<options ; i++){
	    	for (int j=i+1 ; j<options ; j++){
	    		if ((sudoku[i][k].showNumber() == sudoku[j][k].showNumber()) && sudoku[j][k].known && sudoku[i][k].known){
	    			return true;
	    		}
	    		if ((sudoku[k][i].showNumber() == sudoku[k][j].showNumber())&& sudoku[k][j].known && sudoku[k][i].known){
	    			return true;
	    		}
	    	}
	    }
	}
	return false;
} // End of Method
void board :: deepCopy(board rightBoard){
    /*
    Inputs: A board class
    Outputs: void
    Purpose: To make 'this' a DEEP COPY of the 'rightboard'.
    */
    board outBoard;
    for (int i=0;i<options;++i){
        for(int j=0 ; j<options ; ++j){
            if ((rightBoard.sudoku[i][j]).known){
                // This loops through the right board, and uses the 'solveVal' method on each of the numbers that are known on the rightboard
                outBoard.sudoku[i][j].solveVal(rightBoard.sudoku[i][j].showNumber() );
            }
        }
    }
    while (outBoard.totalCrossHatch() || outBoard.totalGridCrossHatch()){} // Then, it figures out which numbers in the square are impossible
    sudoku =  outBoard.sudoku; // Finaly, "this"'s sudoku becomes the board we modified

}
position board :: findHighPossibilitySpots(){
    /*
    Inputs : None
    Outputs: A 'position class' which has the coordinates of a square with the smallest number of possible outcomes.
    Purpose: To find a square which has the lowest number of possible values that could occupy that square (usually 2), and return the coordinates of that square.
    */

	int minSoFar =options; // Default minimum is an empty square.
	position *bestspot;    //The position that gets returned
	for (int k=0 ; k<2 ; k++){
		for (int i=0 ; i<options ; i++){
			for (int j=0 ; j<options ; j++){
				if (!(sudoku[i][j].known )){
					if (k==0){ // On the first iteration the method finds the lowest number of possibilities.
						if (sudoku[i][j].countPossibilities() < minSoFar)
							minSoFar = sudoku[i][j].countPossibilities();
					} else if (k==1){ // On the second iteration, it returns the first square it finds with that number of possibilities.
						if (sudoku[i][j].countPossibilities() == minSoFar){
							// If it's a min square, it returns it
							bestspot = new position;
							bestspot -> row = i;
							bestspot -> col = j;
							return *bestspot;
						}
					}
				}
			}
		}
	}
}


int main(){
    board myBoard;
    myBoard.fillBoard();
    myBoard.solveBoard();
    myBoard.displayboard();

    return 0;
}
