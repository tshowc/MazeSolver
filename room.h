#include <iostream>
#include <string>

class room
{
	public:
		room();
		room(bool v, bool I, bool R, char c);
		bool getVisited();
		void setVisited(bool newVisited);
		bool getIntersection();
		void setIntersection(bool newIntersection);
		bool getRejected();
		void setRejected(bool newRejected);

		char getContents();
		void setContents(char newcontents);
		
		bool isWall();
		bool isOpen();
		bool isCheese();


	
	private:
		char contents; //Each cell has contents (X=Wall,C=Cheese,O=Mouse, N=Nothing)
		bool visited;
		bool Intersection;
		bool Rejected;
};
