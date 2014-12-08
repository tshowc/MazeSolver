#include "room.h"

room::room()
{
	contents = 'N';
	visited = false;
	Intersection = false;
	Rejected = false;

}
room::room(bool v, bool I, bool R, char c)
{
	contents = c;
	visited = v;;
	Intersection = I;
	Rejected = R;
}

bool room::getVisited()
{
	return visited;
}

void room::setVisited(bool newVisited)
{
	visited = newVisited;
}

bool room::getIntersection()
{
	return Intersection;
}

void room::setIntersection(bool newIntersection)
{
	Intersection = newIntersection;
}

bool room::getRejected()
{
	return Rejected;
}

void room::setRejected(bool newRejected)
{
	Rejected = newRejected;
}

char room::getContents()
{
	return contents;
}

void room::setContents(char newcontents)
{
	contents = newcontents;
}

bool room::isWall()
{
	if (contents == 'X')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool room::isOpen()
{
	if ((contents == 'N') || (contents == 'C'))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool room::isCheese()
{
	if (contents == 'C')
	{
		return true;
	}
	else
	{
		return false;
	}
}
	
