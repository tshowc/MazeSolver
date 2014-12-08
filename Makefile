Maze: maze.o room.o
	g++ maze.o room.o -o maze
maze.o: maze.cpp
	g++ -c -g maze.cpp
room.o: room.cpp
	g++ -c -g room.cpp
clean:
	rm -rf *.o Maze
