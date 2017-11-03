#include "Sudoku_Board.h"

int averageRecursiveCalls(vector<int>);

int main()
{
	vector<int> recursiveCalls;
	int calls;
	ifstream fin;
	string line;
	bool solved;

	// Read the sample grid from the file.
	string fileName = "C:/Users/Devin/Desktop/sudoku.txt";

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}
		
		while(getline(fin, line))	//Runs as long as the input is "good" and the next char is not a Z 
									//Note to instr: Can also remove the Z from the text file, and this will terminate without the if statement
		if(fin.peek() != 'Z')
		{
		board b1(SquareSize);
		b1.initialize(line);
		b1.conflictScan();
		b1.print();
		calls = b1.sudokuSolver(1);
		recursiveCalls.push_back(calls);
		b1.print();
		}
		
	averageRecursiveCalls(recursiveCalls);
}

//Math function that takes the average of all the recursive calls
int averageRecursiveCalls(vector<int> r){
	int k;
	int average;
	
	for (int i =0 ; i< r.size(); i++)
		k += r[i];
	
	average = k / (r.size());
	
	cout << "The average number of recursive calls for " << r.size() <<" boards was " << average << endl;
}

