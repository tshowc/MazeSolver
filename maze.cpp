#include "room.h"
#include <stack>
#include <fstream>
#include <cstring>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <cctype>

using namespace std;
//Constant
const int MAXSIZE = 100;
//Function Prototypes
void loadMaze(room maze[][MAXSIZE], int currentSize, string filename);
int getMazeSize(string filename);
room loadRoom(char characterFromFile);
void printMaze(room maze[][MAXSIZE], int currentSize, int time);
void printMySolution(room maze[][MAXSIZE], int currentSize, int time);
void clearMaze(room maze[][MAXSIZE], int currentSize);
int stackSolution(room maze[][MAXSIZE], int currentSize);
int MySolution(room maze[][MAXSIZE], int currentSize);
//void getCurrentCoord(room* currpnt, room maze[][MAXSIZE], int currentSize, int &x, int&y);
void AdjacentCoord(room *currpnt, room maze[][MAXSIZE], int currentSize, int &x, int &y, int prevX, int prevY);
bool MazeSolver(room maze[][MAXSIZE], int currentSize, int startX, int startY, int cheeseX, int cheeseY); 

/*****************Debugger Notes*********************/
/* Maze1.txt is the ideal maze
 * Maze2.txt works as well with a few printing issues
 * Known Problems:
 * 	- stackSolution will get stuck in infinite loop if there are too many intersection rooms in a row
 * 		Ex: XOXXXXX
 * 		  ->X      <--
 * 		    XX XX X
 * 		    XX  CXX
 * 		- In line 638: dowhile loop will exit out prematurely and the position of Rooms.top() and current do not equal 
 * 		-Possible Reason: specific moves/rooms are not being pushed onto stack
 */		


int main()
{

		char choice;
	do{
		int sizeMaze;
		int seconds;
		int newseconds;
		string nameFile;

		cout << "What is the name of the file you wish to open? ";
		cin >> nameFile;

		int found = nameFile.find(".txt");

		if (found == -1)
		{
			cout << "Incorrect File Format. (Suggestion: Add .txt to the end of your filename)" << endl;
			return 0;
		}
	
		sizeMaze =  getMazeSize(nameFile);
		if (sizeMaze == -1)
		{
			return 0;
			
		}

		room maze[MAXSIZE][MAXSIZE];
		loadMaze(maze, sizeMaze, nameFile);
		
//		cout << "LoadMaze working" << endl;		
		seconds = stackSolution(maze, sizeMaze);
//		cout << "Back in main" << endl; 
//		cout << "Seconds Taken: " << seconds << endl;
		cout << endl;
		cout << "STACKSOLUTION: " << endl;
		printMaze(maze, sizeMaze, seconds);
		clearMaze(maze, sizeMaze);

		newseconds = MySolution(maze, sizeMaze);
		cout << "MYSOLUTION: " << endl;
		printMySolution(maze, sizeMaze, seconds);


		cout << endl;
		if (seconds < newseconds)
		{
			cout << "Stack Solution was faster than My Solution." << endl;
		}
		else if (seconds > newseconds)
		{
			cout << "My Solution was faster than Stack Solution." << endl;
		}
		else if (seconds == newseconds)
		{
			cout << "Both solutions took the equal amount of time." << endl;
		}

		
		cout << "Would you like to enter another maze? (Y or N): ";
		cin >> choice;
		
	}while((choice != 'N') && (choice != 'n'));

	return 0;
}


void loadMaze(room maze[][MAXSIZE], int currentSize, string filename)
{
	string line;
	char test;
	ifstream myfile;
	myfile.open(filename.c_str());
	getline(myfile, line); //ignore the number
	//cout << line << endl;
	for (int i = 0; i < currentSize; i++)
	{
		getline(myfile, line);//the line of maze will update every loop
		//cout << line << endl;
		for (int j = 0; j < currentSize; j++)
		{
			maze[i][j] = loadRoom(line[j]);
			//test = maze[i][j].getContents();
			//cout << "Room[" << i << "][" << j << "]: " << test << endl;

		}
	}
	
}

int getMazeSize(string filename)
{
	string digit;
	int size;
	int digitSize;
	ifstream myfile;
	myfile.open(filename.c_str());
	getline(myfile, digit);
	
	digitSize = digit.size();
	
	for (int i = 0; i < digitSize; i++)
	{
		if ((digit[i] == '0') || (digit[i] == '1') || (digit[i] == '2') || (digit[i] == '3') ||
			(digit[i] == '4') || (digit[i] == '5') || (digit[i] == '6') || (digit[i] == '7') ||
			(digit[i] == '8') || (digit[i] == '9'))
		{
			size = 1;
			
		}
		else
		{			
			cout << "Invalid File Format. Error: Can't get size of maze. " << endl;
			size = -1;
			break;
		}
	} 
			 
	if (size != -1)
	{
		size = atoi(digit.c_str());
	}
	
	return size;
		

}

room loadRoom(char characterFromFile)
{

	room newRoom;

	if (characterFromFile == 'X')
	{
		newRoom.setContents('X');
		newRoom.setRejected(true);
		return newRoom;
	}
	else if (characterFromFile == ' ')
	{
		newRoom.setContents('N');
		return newRoom;
	}
	else if (characterFromFile == 'O')
	{
		newRoom.setContents('O');
		return newRoom;
	}
	else if (characterFromFile == 'C')
	{
		newRoom.setContents('C');
		return newRoom;
	}
		

}

void printMaze(room maze[][MAXSIZE], int currentSize, int time)
{

	char contents; 
	for (int i = 0; i < currentSize; i++)
	{
		for (int j = 0; j < currentSize; j++)
		{
			contents = maze[i][j].getContents();
			if (contents == 'N')
			{
				if (maze[i][j].getVisited())
				{	
					contents = '*';
				}
				else
				{
					contents = ' ';
				}
			}
			
			cout << contents;
		}
			cout << "\n";
	}
		cout << "Seconds Taken: " << time << endl;
	

}

void printMySolution(room maze[][MAXSIZE], int currentSize, int time)
{
	
	char contents;

	for (int i = 0; i < currentSize; i++)
	{
		for (int j = 0; j < currentSize; j++)
		{
			contents = maze[i][j].getContents();
			if (maze[i][j].getContents() == 'N')
			{
				cout << ' ';
			}
			else
			{
				cout << contents;
			}
		}
		cout << "\n";
	}
	cout << "Seconds Taken: " << time << endl;
}

void clearMaze(room maze[][MAXSIZE], int currentSize)
{	
	for (int i = 0; i < currentSize; i++)
	{
		for (int j = 0; j < currentSize; j++)
		{
			if (maze[i][j].getVisited())
			{
				maze[i][j].setVisited(false);
			}
			if (maze[i][j].getIntersection())
			{
				maze[i][j].setIntersection(false);
			}
			if (maze[i][j].getRejected())
			{
				maze[i][j].setRejected(false);
			}
		}
	}
	cout << endl;
	cout << "MAZE CLEARED" << endl << endl;
}



void AdjacentCoord( room *currpnt, room maze[][MAXSIZE], int currentSize, int &x, int &y, int prevX, int prevY)
{
	
	room *temp = NULL;
	//cout << "IN CURRENTCOORD" << endl;
	//TESTING CURRPNT
	char test;
	bool V;
	bool I;
	//cout << "TESTING CURRPNT" << endl;;
	//cout << "CURRPNTCONTENTS: " << test << " Visited: " << V << endl;
	
	for (int i = 0; i < currentSize; i++)
	{
		for (int j = 0; j < currentSize; j++)
		{
			temp = &maze[i][j];
	//		if ((i == 8) && (j == 19))
	//		{
	//			test = maze[i][j].getContents();
	//			V = maze[i][j].getVisited();
	//			cout << "TESTING: " << test << " VISITED: " << V <<  endl;
	//		}
			//TESTING TEMP
			//test = temp->getContents();
			//V = temp->getVisited();
			//I = temp->getIntersection();
			//cout << "TEMPCONTENTS[" << i << "][" << j << "]" << ": " << test << " Visited: " << V << " INTERSECTION: " << I << endl;
			if (currpnt == temp)
			{
//				cout << "IN AC EQUAL" << endl;
				x = i;
				y = j;
				temp = currpnt;
				goto end;
			}		
			else if ((temp->getIntersection())&& (!temp->getRejected()))
			{
//				cout << "IN AC IF" << endl;
				x = i;
				y = j;
			}
			else if (temp->getVisited())
			{
				if (i == prevX-1)
				{
					if ((i+1 == prevX) && (j == prevY))
					{
						x = i;
						y = j;
						goto end;
					}
					else if ((i == prevX) && (j+1 == prevY))
					{
						x = i;
						y = j;
						goto end;
					}
				}

//				cout << "IN AC VISIT" << endl;
				x = i;
				y = j;
			}
			
		
		}
	}	
	end:;
//	cout << "NEW ADJCOORDS - X: " << x << " Y: " << y << endl;
} 

int stackSolution(room maze[][MAXSIZE], int currentSize)
{
//	cout << "In StackSolution" << endl;
	stack<room> Rooms;
 //	cout << "Stack Created" << endl;
	room *current;
	room *tempcurrent;
	room *prev;
	room *cheese;
	room *right;
	room *left;
	room *top;
	room *bottom;
	room *intersection;
	char test;
	int currentX;
	int currentY;
	int cheeseX;
	int cheeseY;
	//cout << "Variable defined" << endl;
	
	for (int i = 0; i < currentSize; i++)
	{
		for (int j = 0; j < currentSize; j++)
		{
			if (maze[i][j].getContents() == 'O')
			{
	//			cout << "Before *current" << endl; 
				current = &maze[i][j];
	//			cout << "After current" << endl;
				current->setVisited(true);
				currentX = i;
				currentY = j;
			}
			if (maze[i][j].getContents() == 'C')
			{
				cheese = &maze[i][j];
//				cout << "Cheese- X: " << i << " Y: " << j << endl;
				cheeseX = i;
				cheeseY = j;
				test = cheese->getContents();
//				cout << "TESTING 4: " << test << endl;
			}
		}
	}
	//START TIMER
//	cout << "Starting Timer" << endl;
	unsigned long now = time(0);
//	cout << "TIME" << now << endl;
	while (current->getContents() != 'C' )
	{

//		cout << "In while" << endl;
		if (current->getIntersection())
		{
//			cout << endl;
//			cout << "IN CURRENT INTERSECT" << endl;
			prev = &maze[currentX][currentY];;
			AdjacentCoord(current, maze, currentSize, currentX, currentY, 0, 0);	
		}
		else
		{
//			cout << endl;
//			cout << "IN CURRENT NORM" << endl;
			AdjacentCoord(current, maze, currentSize, currentX, currentY, 0, 0);	
			prev = &maze[currentX][currentY];
			current = &maze[currentX][currentY];	
		}	
		right = &maze[currentX][currentY+1];
		left = &maze[currentX][currentY-1];
		top = &maze[currentX-1][currentY];
		bottom = &maze[currentX+1][currentY];
		
		int flag = 0;
		
		if ((right->isOpen()) && !(right->getVisited()) && !(right->getRejected()) && (currentY != currentSize-1) )
		{
//			cout << "In right if" << endl;
			flag++;
			if (flag == 1)
			{
//				cout << "PUSHED RIGHT" << endl;
				current = right;
				current->setVisited(true);
				maze[currentX][currentY+1].setVisited(true);
				tempcurrent = &maze[currentX][currentY+1];
				Rooms.push(maze[currentX][currentY+1]);
				if (prev->getIntersection())
				{
//					cout << "IN RIGHT REJECT" << endl;
					maze[currentX][currentY+1].setRejected(true);
				}
			}
			if (flag >= 2)
			{
				prev->setIntersection(true);
				intersection = prev;
				Rooms.pop();
				if(Rooms.empty())
				{
					Rooms.push(*intersection);
					Rooms.push(*tempcurrent);
					tempcurrent->setRejected(true);
				}
				else
				{
					Rooms.pop();
					Rooms.push(*intersection);
					Rooms.push(*tempcurrent);
					tempcurrent->setRejected(true);
//					cout << "CURRENT REJECTED" << endl;
				}
			}
				
				
				
		}
		if ((left->isOpen()) && !(left->getVisited()) && !(left->getRejected()) && (currentY != 0))
		{
//			cout << "In left if" << endl;
			flag++;
			if (flag == 1)
			{
				current = left;
				current->setVisited(true);
				maze[currentX][currentY-1].setVisited(true);
				tempcurrent = &maze[currentX][currentY-1];
				Rooms.push(maze[currentX][currentY-1]);
//				cout << "PUSHED LEFT" << endl;
				if (prev->getIntersection())
				{
//					cout << "IN LEFT  REJECT" << endl;
					maze[currentX][currentY-1].setRejected(true);
				}
			}
			if (flag >= 2)
			{
				prev->setIntersection(true);
				intersection = prev;
				Rooms.pop();
				if(Rooms.empty())
				{
					Rooms.push(*intersection);
					Rooms.push(*tempcurrent);
					tempcurrent->setRejected(true);
				}
				else
				{
					Rooms.pop();
					Rooms.push(*intersection);
					Rooms.push(*tempcurrent);
					tempcurrent->setRejected(true);
//					cout << "CURRENT REJECTED" << endl;
				}
			}
		}
		if ((top->isOpen()) && !(top->getVisited()) && !(top->getRejected()) && (currentX != 0))
		{
//			cout << "In top if" << endl;
			flag++;
			if (flag == 1)
			{
				current = top;
				current->setVisited(true);
				maze[currentX-1][currentY].setVisited(true);
				tempcurrent = &maze[currentX-1][currentY]; 
				Rooms.push(maze[currentX-1][currentY]);
//				cout << "PUSHED TOP" << endl;
				//getCurrentCoord(current, maze, currentSize, currentX, currentY);
				//cout << "NewCoors: X: " << currentX << " Y: " << currentY << endl;
				if (prev->getIntersection())
				{
//					cout << "IN TOP REJECT" << endl;
					maze[currentX-1][currentY].setRejected(true);
				}
			}
			if (flag >= 2)
			{
				prev->setIntersection(true);
				intersection = prev;
				Rooms.pop();
				if(Rooms.empty())
				{
					Rooms.push(*intersection);
					Rooms.push(*tempcurrent);
					tempcurrent->setRejected(true);
				}
				else
				{
					Rooms.pop();
					Rooms.push(*intersection);
					Rooms.push(*tempcurrent);
					tempcurrent->setRejected(true);
//					cout << "CURRENT REJECTED" << endl;
				}
			}
		}
		if ((bottom->isOpen() || bottom->isCheese())  && !(bottom->getVisited()) && !(bottom->getRejected()) && (currentX != currentSize-1))
		{
//			cout << "In bottom if" << endl;
			flag++;
			if (flag == 1)
			{
		//		test = bottom->getContents();
		//		bool V = bottom->getVisited();
		//		bool I = bottom->getIntersection();
		//		bool R = bottom->getRejected();
//				cout << "BOTTOM CONTENTS: " << test << " VISITED: " << V << " INTERSECTION: " << I << " REJECTED: " << R << endl;
				current = bottom;
				current->setVisited(true);
				maze[currentX+1][currentY].setVisited(true);
				//test = current->getContents();
				//V = current->getVisited();
				//I = current->getIntersection();
				//R = current->getRejected();
//				cout << "BOTTOM CONTENTS: " << test << " VISITED: " << V << " INTERSECTION: " << I << " REJECTED: " << R << endl;
				//getCurrentCoord(current, maze, currentSize, currentX, currentY);
//				cout << "NewCoors: X: " << currentX << " Y: " << currentY << end;
				tempcurrent = &maze[currentX+1][currentY];
				Rooms.push(maze[currentX+1][currentY]);
//				cout << "PUSHED BOTTOM" << endl;
				if (prev->getIntersection())
				{
//					cout << "IN BOTTOM REJECT" << endl;
					maze[currentX+1][currentY].setRejected(true);
				}
			}
			if (flag >= 2)
			{
				prev->setIntersection(true);
				intersection = prev;
				Rooms.pop();
				if(Rooms.empty())
				{
					Rooms.push(*intersection);
					Rooms.push(*tempcurrent);
					tempcurrent->setRejected(true);
				}
				else
				{
					Rooms.pop();
					Rooms.push(*intersection);
					Rooms.push(*tempcurrent);
					tempcurrent->setRejected(true);
//					cout << "CURRENT REJECTED" << endl;
				}
			}
				
		}	

		if (flag == 0)//No way to go
		{
//			cout << "IN ELSE" << endl;
		//	room *tempintersect;
		//	tempintersect = intersection;
		//	intersection->setIntersection(false);
		//	intersection->setVisited(false);
			if (current->getIntersection())//If Intersection has no where to go
			{
//				cout << "IN INTERSECTION FALSE AT " << currentX << " " << currentY << endl;
				current->setRejected(true);
			}
		
			
			do{
				
//				cout << "In DO " << endl;
//				cout << "Stack Size" << Rooms.size() << endl;
		//		char test = Rooms.top().getContents();
		//		bool V = Rooms.top().getVisited();
		//		bool I = Rooms.top().getIntersection();
		//		cout << "TESTING CONTENTS: " << test << endl;
		//		cout << "TESTING VISITED: " << V << endl;
		//		cout << "TESTING INTERSECTION: " << I << endl; 
		//		getCurrentCoord(current, maze, currentSize, currentX, currentY);
		//		I = current->getIntersection();
		//		cout << "INTERSECTION AT - X: " << currentX << " Y: " << currentY << " - is: " << I << endl;
				if (Rooms.empty())
				{
					cout << "No Path Exists" << endl;
					return 0;
				}
//				bool V;
				current = &Rooms.top();
//				V = maze[currentX][currentY].getVisited();
//				cout << "CURRENT" << currentX << currentY << " VISITED: " << V << endl;
				maze[currentX][currentY].setVisited(false);
				Rooms.pop();
//				cout << "AFTER POPPING" << endl;
//				V = maze[currentX][currentY].getVisited();
//				cout << "CURRENT" << currentX << currentY << " VISITED: " << V << endl;
				AdjacentCoord(current, maze, currentSize, currentX, currentY, currentX, currentY);
			}while((!(Rooms.top().getIntersection()) && !(current->getIntersection())) && !(Rooms.empty()));
		//	intersection = tempintersect;
//			bool R;
//			bool I;
//			I = Rooms.top().getIntersection();
//			cout << "OUT DO WHILE. I: " << I  << endl;
//			I = current->getIntersection();
//			cout << "CURRENT I: " << I << endl;
			AdjacentCoord(current, maze, currentSize, currentX, currentY, 0, 0);
			current = &maze[currentX][currentY];
//			I = current->getIntersection();
//			R = current->getRejected();
//			cout << "X: " << currentX <<  " Y: " << currentY << endl;
//			cout << "I: " << I << " R: " << R << endl;
//			cout << endl;
		//	AdjacentCoord(current, maze, currentSize, currentX, currentY);
		//	cout<<endl;
		//	cout << "AFTER POPPING COORDS: X: " << currentX << " Y: " << currentY << endl;
		}
	
	}//end while

//	cout << "OUT WHILE " << endl;

	Rooms.push(maze[currentX][currentY]);
	//STOP TIMER
//	cout << "TIMER STOPPED" << endl;
	unsigned long then = time(0);
//	cout << "TIME: " << then << endl;
	unsigned long elasped = then - now;

//	cout << "ELASPED" <<  elasped << endl;
	return elasped;
}

int MySolution(room maze[][MAXSIZE], int currentSize)
{

	room *current;
	room *cheese;
	int startX;
	int startY;
	int cheeseX;
	int cheeseY;
	
	for (int i = 0; i < currentSize; i++)
	{
		for (int j = 0; j < currentSize; j++)
		{
			if (maze[i][j].getContents() == 'O')
			{
	//			cout << "Before *current" << endl; 
				current = &maze[i][j];
	//			cout << "After current" << endl;
				current->setVisited(true);
				startX = i;
				startY = j;
			}
			if (maze[i][j].getContents() == 'C')
			{
				cheese = &maze[i][j];
//				cout << "Cheese- X: " << i << " Y: " << j << endl;
				cheeseX = i;
				cheeseY = j;
	//			test = cheese->getContents();
//				cout << "TESTING 4: " << test << endl;
			}
		}
	}
	//START TIMER
//	cout << "Starting Timer" << endl;
	unsigned long now = time(0);
//	cout << "TIME" << now << endl;

	if (MazeSolver(maze, currentSize, startX, startY, cheeseX, cheeseY))
	{
//		cout << "SUCCESS" << endl;
	}
	else
	{
		cout << "Path not found. Try Again." << endl;
	}
	
	unsigned long then = time(0);
	return then - now;
	

}

bool MazeSolver(room maze[][MAXSIZE], int currentSize, int startX, int startY, int cheeseX,  int cheeseY)
{
	//Using Recursion to solve the maze

	maze[startX][startY].setContents('*');
	maze[cheeseX][cheeseY].setContents('N');
	
	if ((startX == cheeseX) && (startY == cheeseY))
	{
		maze[cheeseX][cheeseY].setContents('*');
		return true;
	}
	if ((startX > 0) && (maze[startX-1][startY].getContents() == 'N') && (MazeSolver(maze, currentSize, startX-1, startY, cheeseX, cheeseY)))
	{
		return true;
	}
	if ((startX < currentSize) && (maze[startX+1][startY].getContents() == 'N') && MazeSolver(maze, currentSize, startX+1, startY, cheeseX, cheeseY))
	{
		return true;
	}
	if ((startY > 0) && (maze[startX][startY-1].getContents() == 'N') && (MazeSolver(maze, currentSize, startX, startY-1, cheeseX, cheeseY)))
	{
		return true;
	}
	if ((startY < currentSize) && (maze[startX][startY+1].getContents() == 'N') && (MazeSolver(maze, currentSize, startX, startY+1, cheeseX, cheeseY)))
	{
		return true;
	}
//            PRINT OUT MAZE	
//	char contents;
//
//	for (int i = 0; i < currentSize; i++)
//	{
//		for (int j = 0; j < currentSize; j++)
//		{
//			contents = maze[i][j].getContents();
//			if (maze[i][j].getContents() == 'N')
//			{
//				cout << ' ';
//			}
//			else
//			{
//				cout << contents;
//			}
//		}
//		cout << "\n";
//	}
//	cout << endl;

	maze[startX][startY].setContents('N');
	return false;
}
	
