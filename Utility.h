class Node
{
public:
	int x, y;
	Node* prev;

	Node(int, int);
};

class Utility
{
public:
	/*------------------
	A star path finding funciton
	path array, row, col, from point, to point, obstacles
	-------------------*/
	static int pathFinding(int [][2], int, int, int[], int[], int, int [][2]);
private:
	static const int neigh[][2];
	static const int MAX = 1000000000;
	static int heuristicCost(int, int [][2], int[], int[]);
};