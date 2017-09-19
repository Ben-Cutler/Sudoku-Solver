#include<iostream>
#include<vector>


using namespace std;
const int boxes = 3; // Subgrid
const int options = boxes * boxes;   // Board is currently 1-9
/**
Square Class
**/

class square{
    bool* possibilities; // Keeps track of which numbers are possible
    public:
    bool known; // If there is only one option in the possibilities, then it is known.
    void setVal (int key); // Sets a value in the square to false/impossible.
    void solveVal(int key); //Sets every value except the key value to true, and the rest to false. Also trips 'known' to true.
    square(); // Constructor
    int showNumber(); // If the value is known, this method shows the number
    bool checkForSolved();
    int countPossibilities();

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
void square :: setVal(int key){
     /*
    Inputs : An integer 'key', which corresponds to a number that has been ruled 'impossible' and can be eliminated
    Outputs : void
    Purpose : To eliminate a number as a possibility.
    */
    *(possibilities + key -1) = false; // Sets the 'key-1' item in the possibility array to false;
    return;
}
void square :: solveVal(int key){
    /*
    Inputs : An integer 'key', which corresponds to a number that has been ruled 'the only possible answer', usually as a given. If key is 0 this function does nothing.
    Outputs : void
    Purpose : To set a number as a the only possible one that could be there.
    */
    if (!key){
        return; // If the key is 0 this function does nothing.
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
    Outputs: True if there's only

    */
    int counter = countPossibilities();
    if (counter ==1){
        //cout << "Found one, updating! " << endl;
        known = true;
        return 1;
    }
    return 0;
}


/**
Board Class
**/

class board {
    square** sudoku;
    void printANumFreeRow();
    void printANumRow(int);
    bool crossHatch(int,int);

public:
    board();
    void fillBoard();
    void displayboard();
    bool totalCrossHatch();
    bool subGridCrossHatch(int, int);
    bool totalGridCrossHatch();
    bool done();
    board operator =(board);
    void solveBoard();
    bool hasError();

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

    cout << "+-------+-------+-------+" << endl;

}
void board :: printANumRow(int num){
    /*
    Inputs : Takes in no data, but references the board
    Outputs : Displays a row which has numbers in it.
    Purpose : To display rows which have numbers in them
    */
    for (int i = 0; i<options ; i++){
       if (!( i%boxes )){
             cout << "| "; // There should be a bar before the 0th value, the 3rd value, the 6th integer
       }
        cout << (sudoku[num][i]).showNumber() << ' '; // prints the board and a space char
    }
    cout << '|' << endl; // There needs to be a bar at the very end of the row
}
bool board :: crossHatch( int row, int col){
    /*
    add comments lat3r
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
    bool foundOne = false;
    for (int i=0 ; i<options ; ++i){
        for (int j=0 ; j<options; ++j){
           if (sudoku[i][j].known){
                foundOne += crossHatch(i,j);
           }
        }
    }
    return foundOne;
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
    bool foundSome = false;
    for (int i=0 ; i<boxes ; ++i){
        for (int j=0 ; j<boxes ; ++j){
            foundSome += subGridCrossHatch(i,j);
        }
    }
    return foundSome;
}
bool board :: done(){
    bool done = true;
    for (int i=0 ; i<options ; i++){
        for (int j=0 ; j<options ; j++){
            if (! (sudoku[i][j].known)){
                done = false;
            }
        }
    }
    return done;
}
void board :: solveBoard(){
    while (totalCrossHatch() || totalGridCrossHatch() ){}
    if ((!done()) && (!hasError())){
        cout << "Ohh no!" << endl;
        board testBoard = *this;

    }
}
bool board :: hasError(){
    for (int i=0 ; i<options ;i++){
        for (int j = 0 ; j<options ; j++){
            if (!(sudoku[i][j].countPossibilities())){
                return true;
            }
        }
    }
    return false;
}
board board :: operator = (board rightBoard){ //Makes a DEEP COPY
    board outBoard;
    for (int i=0;i<options;++i){
        for(int j=0 ; j<options ; ++j){
            if ((rightBoard.sudoku[i][j]).known){
                outBoard.sudoku[i][j].solveVal(rightBoard.sudoku[i][j].showNumber() );
            }
        }
    }
    while (outBoard.totalCrossHatch() || outBoard.totalGridCrossHatch()){}
    return outBoard;
}

int main(){
    board myBoard;
    myBoard.fillBoard();
    myBoard.solveBoard();
    if (myBoard.done()){
        cout <<"DONE! " << endl;
    }
    myBoard.displayboard();
    return 0;
}
