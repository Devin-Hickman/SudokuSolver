// Declarations and functions for project #4
#ifndef BOARD
#define BOARD

#include <iostream>
#include <limits.h>
#include "matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>
#include<stack>

using namespace std;

struct cell {
	int conflictCounter;
	int indexRow;
	int indexCol;
	int indexSquare;
};

typedef int ValueType; // The type of the value in a cell
const int Blank = -1;  // Indicates that a cell is blank

const int SquareSize = 3;  //  The number of cells in a small square
						   //  (usually 3).  The board has
						   //  SquareSize^2 rows and SquareSize^2
						   //  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

class board
	// Stores the entire Sudoku board
{
public:
	board(int);
	void clear();
	void initialize(string &line);
	void print();
	
	void setCell(int, int, int);
	void clearCell(int, int);
	
	void clearConflicts(int, int);
	void conflictScan();
	void sortConflictVector();
	void printConflicts();
	
	int conflictCounter(int,int);
	int squareNumber(int,int);
	int sudokuSolver(int);
	
	bool canPlaceNumber(int, int, int);
	bool isBlank(int, int);
	bool findConflicts(int, int, int);
	bool isSolved();

	ValueType getCell(int, int);

private:
	
	vector<cell> numConflictsInCell;	//Vector that tracks the information related to a cell's amount of conflicts and its location in the grid
	matrix<ValueType> value;
	vector<vector<bool> > conflictRow;	//Stores whether a digit has been placed in a Row.
	vector<vector<bool> > conflictCol;	//Stores whether a digit has been placed in a Col
	vector<vector<bool> > conflictSquare;	//Stores whether a digit has been placed in a Square
	int recursiveCount = 0;
};

board::board(int sqSize)
	: value(BoardSize + 1, BoardSize + 1)
	// Board constructor
{
	clear();
	conflictRow.resize(10);
	conflictCol.resize(10);
	conflictSquare.resize(10);
	
	for (int k = 0; k < 10; k++) {
		conflictRow[k].resize(10, false);
		conflictCol[k].resize(10, false);
		conflictSquare[k].resize(10, false);
	}
}

// Mark all possible values as legal for each board entry
void board::clear()
{
	for (int i = 1; i <= BoardSize; i++)
		for (int j = 1; j <= BoardSize; j++)
		{
			value[i][j] = Blank;
		}
}

//Checks to see if the index and value can be assigned. Returns true if it can, false otherwise
bool board::canPlaceNumber(int i, int j, int ch){
	if (findConflicts(i,j,ch))
		return true;
	else
		return false;
}

//Sets a cell to the value passed
void board::setCell(int i, int j, int ch) {
	value[i][j] = ch;
	findConflicts(i,j,ch);
	conflictScan();
}

//Sets a cell to blank
void board::clearCell(int i, int j) {
	clearConflicts(i, j);
	value[i][j] = -1;
}

// Read a Sudoku board from the input file.
void board::initialize(string &line)
{
	char ch;

	clear();
	int k =0;
	for (int i = 1; i <= BoardSize; i++)
		for (int j = 1; j <= BoardSize; j++)
		{
			ch = line[k];
			// If the read char is not Blank
			if (ch != '.')
				setCell(i, j, ch - '0');   // Convert char to int
				k++	;					   // cout <<  i << " " << j << " "<<value[i][j] << "\n";
		}
}
//Scans the board for empty cells. Creates and sorts a vector of cells based on the number of conflicts
// (values that cannot be placed in this empty cell) from least -> greatest
void board::conflictScan()
{
	while(numConflictsInCell.size() != 0){
		numConflictsInCell.pop_back();
	}
	int cellNumber = 0;
	for(int i = 1; i <= BoardSize; i++){
		for(int j = 1; j <= BoardSize; j++){
			if (isBlank(i,j)==true){
				int	conflictNumber = conflictCounter(i,j);
				numConflictsInCell.push_back(cell());
				numConflictsInCell[k].conflictCounter =  conflictNumber;
				numConflictsInCell[k].indexRow = i;
				numConflictsInCell[k].indexCol = j;
				numConflictsInCell[k].indexSquare = squareNumber(i,j);
				cellNumber++;
			}
		}
	}
	sortConflictVector();
}

//Counts the number of conflicts for a cell; used in ConflictScan
int board::conflictCounter(int indexRow, int indexCol){
	int conflictingCells;
	int indexSquare = squareNumber(indexRow, indexCol);

	for (int i = 1; i <= 9; i ++){
		if(conflictRow[indexRow][i] == true)
			conflictingCells++;
		if(conflictCol[indexCol][i] == true)
			conflictingCells++;
		if(conflictCol[indexCol][i]== false && conflictRow[indexRow][i] == false && conflictSquare[indexSquare][i] == true)
			conflictingCells++;
	}

	return conflictingCells;
	
}

//Sorts the vector of cells; used in conflictScan (Selection Sort)
void board::sortConflictVector(){
	for(int i = 0; i< numConflictsInCell.size() ; i++){
		for (int j = i+1; j < numConflictsInCell.size() ; j++){
				if(numConflictsInCell[i].conflictCounter > numConflictsInCell[j].conflictCounter) {
					swap(numConflictsInCell[i], numConflictsInCell[j]);
				}
			}					
		}	
	}

//Clears any conflicts associated with this cell
void board::clearConflicts(int indexRow, int indexCol) {
	int indexSquare = squareNumber(indexRow, indexCol);
	int digit = getCell(indexRow, indexCol);

	conflictRow[indexRow][digit] = false;
	conflictCol[indexCol][digit] = false;
	conflictSquare[indexSquare][digit] = false;
}

//Determines if conflicts exist with the value to be set in a cell
bool board::findConflicts(int indexRow, int indexCol, int value) {

	int indexSquare = squareNumber(indexRow, indexCol);
	int conflictsCounter = 0;
	if (conflictRow[indexRow][value] == false)
		conflictsCounter++;
	else
		return false;

	if (conflictCol[indexCol][value] == false)
		conflictsCounter++;
	else
		return false;
		
	if(conflictSquare[indexSquare][value] == false)
		conflictsCounter++;
	else
		return false;
	
	if (conflictsCounter == 3) {
		conflictSquare[indexSquare][value] = true;
		conflictRow[indexRow][value] = true;
		conflictCol[indexCol][value] = true;
		return true;
	}
	else
		return false;
}
int board::squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
	// Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
	// coordinates of the square that i,j is in.  

	return SquareSize * ((i - 1) / SquareSize) + (j - 1) / SquareSize + 1;
}

ostream &operator<<(ostream &ostr, const cell &cell)
// Overloaded output operator for cell struct.
{
	ostr << "Cell Index: " << cell.indexRow << "," << cell.indexCol << "\n"
		 << "Cell Square: " << cell.indexSquare << "\n"
		 <<"Cell Conflict Count " << cell.conflictCounter << endl;
		 return ostr;
}

// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
ValueType board::getCell(int i, int j)
{
	if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
		return value[i][j];
	else
		throw rangeError("bad value in getCell");
}

// Returns true if cell i,j is blank, and false otherwise.
bool board::isBlank(int i, int j)
{
	if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
		throw rangeError("bad value in setCell");

	return (getCell(i, j) == Blank);
}

//Prints the amount of numbers that cannot be placed in every empty cell. Does not print the numbers that cannot be placed
void board::printConflicts() {
	
	for (int i = 0; i < numConflictsInCell.size(); i++){
		cout << "CELL " << numConflictsInCell[i].indexRow << "," << numConflictsInCell[i].indexCol << " HAS " << numConflictsInCell[i].conflictCounter << " CONFLICTS WITH CELLS\n";
	}
}

//Checks to see if the board is solved
bool board::isSolved() {
	int counter = 0;
	int conflictcounter = 0;
	for (int i = 1; i <= BoardSize; i++) {
		for (int j = 1; j <= BoardSize; j++) {
			if (value[i][j] > 0)
				counter++;
		}
	}
	if (counter == 81) {
		return true;
	}
	else {
		return false;
	}
}

// Prints the current board.
void board::print()
{
	for (int i = 1; i <= BoardSize; i++)
	{
		if ((i - 1) % SquareSize == 0)
		{
			cout << " -";
			for (int j = 1; j <= BoardSize; j++)
				cout << "---";
			cout << "-";
			cout << endl;
		}
		for (int j = 1; j <= BoardSize; j++)
		{
			if ((j - 1) % SquareSize == 0)
				cout << "|";
			if (!isBlank(i, j))
				cout << " " << getCell(i, j) << " ";
			else
				cout << "   ";
		}
		cout << "|";
		cout << endl;
	}

	cout << " -";
	for (int j = 1; j <= BoardSize; j++)
		cout << "---";
	cout << "-";
	cout << endl;
}

//Solves the sudoku puzzle
int board::sudokuSolver(int testval)
 {
	recursiveCount++;
	
	if (isSolved() == true) 
	{
		cout << "The board is solved\n";
		cout << "There were " << recursiveCount << " recursive function calls to solve this sudoku puzzle.\n";
		return recursiveCount;
	}	
	else
	{
		//Gets the indexes of the most constrained cell
		//The vector is sorted such that the most constrained cell is the last-most index of the vector
		int i = numConflictsInCell.back().indexRow;
		int j = numConflictsInCell.back().indexCol;
		
		//Iterate for potential values to be placed in the cell 1-9
		for (int x = testval; x <= 9; x++) 
		{			
			if(canPlaceNumber(i,j,x) == true)
			{		
				//Once we find a value that can be placed in the cell 
				//we remove that cell from the vector that tracks the conflicts
				//set the cell to the value and re-run the solver
				numConflictsInCell.pop_back();
				setCell(i,j,x);
				sudokuSolver(1);
				
				//Return from a potential solution: If the board is solved, return through the branch that solved it
				//If the board is not solved we clear the value of the cell that we just placed above, and try a different value
				//The for loop continues iterating from the last value it tested, and the values, i/j, are saved from when
				//they are initially set above the for loop
				if(isSolved())	
					return recursiveCount;
				else
					clearCell(i,j);
			}
		}
	}
}



#endif

