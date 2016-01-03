#include "Utility.h"
#include <vector>
#include <stack>

using namespace std;

Node::Node(int px, int py) {
	x = px;
	y = py;
	prev = NULL;
}

const int Utility::neigh[][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } };

/*------------------
Path finding with A* algorithm
-------------------*/
int Utility::pathFinding(int path[][2], int row, int col, int from[], int to[], int num_obstacles, int obstacle[][2]) {
	// initialize cells
	double **realCost = new double*[row];
	for (int i = 0; i < row; ++i) {
		realCost[i] = new double[col];
		for (int j = 0; j < col; ++j) {
			realCost[i][j] = MAX;
		}
	}

	// blocks with obstacle is FALSE
	for (int i = 0; i < num_obstacles; ++i) {
		realCost[obstacle[i][0]][obstacle[i][1]] = -1;
	}

	int cur[2] = { from[0], from[1] };
	realCost[cur[0]][cur[1]] = 0;

	vector<int> cand[2];
	cand[0].push_back(cur[0]);
	cand[1].push_back(cur[1]);
	Node*** node = new Node**[row];
	for (int i = 0; i < row; ++i) {
		node[i] = new Node*[col];
		for (int j = 0; j < col; ++j) {
			node[i][j] = new Node(i, j);
		}
	}

	while (true) {
		// find the best candidate among the list
		double min = MAX;
		int nth = 0;
		for (int i = 0; i < cand[0].size(); ++i) {
			int tmp[2] = { cand[0][i], cand[1][i] };
			if (min > realCost[cand[0][i]][cand[1][i]] + heuristicCost(num_obstacles, obstacle, tmp, to)) {
				min = realCost[cand[0][i]][cand[1][i]] + heuristicCost(num_obstacles, obstacle, tmp, to);
				nth = i;
			}
		}
		cur[0] = cand[0][nth];
		cur[1] = cand[1][nth];

		if (cur[0] == to[0] && cur[1] == to[1]) {
			break;
		}
		else {
			cand[0].erase(cand[0].begin() + nth);
			cand[1].erase(cand[1].begin() + nth);
		}

		// expand node
		for (int k = 0; k < 8; ++k) {
			int r = cur[0] + neigh[k][0];
			int c = cur[1] + neigh[k][1];

			if (!(r < 0 || c < 0 || r >= row || c >= col)) {
				if (realCost[r][c] > realCost[cur[0]][cur[1]] + sqrt(neigh[k][0] * neigh[k][0] + neigh[k][1] * neigh[k][1])) {
					realCost[r][c] = realCost[cur[0]][cur[1]] + sqrt(neigh[k][0] * neigh[k][0] + neigh[k][1] * neigh[k][1]);
					cand[0].push_back(r);
					cand[1].push_back(c);

					node[r][c]->prev = node[cur[0]][cur[1]];
				}
			}
		}
	}

	// Resolve path
	Node* p = node[to[0]][to[1]];
	int l, length = 0;
	while (p != NULL) {
		p = p->prev;
		length++;
	}
	p = node[to[0]][to[1]];
	l = length - 1;
	while (p != NULL) {
		path[l][0] = p->x;
		path[l][1] = p->y;
		p = p->prev;
		l--;
	}

	return length;
}

/*-------------------
Calculate Euclidean distance as our heuristic estimate function h(n)
moving direction is up-right
found up-down, left-right, up-right, left-down connected obstacles
--------------------*/
int Utility::heuristicCost(int num_obstacles, int obstacle[][2], int cur[], int dst[]) {
	int margin[2][2] = { { cur[0], cur[1] }, { dst[0], dst[1] } };
	bool *visited = new bool[num_obstacles]();
	stack<int> s;
	for (int i = 0; i < num_obstacles; ++i) {
		if (!visited[i]) {
			bool u = false;
			bool d = false;
			bool l = false;
			bool r = false;

			visited[i] = true;
			s.push(i);
			while (!s.empty()) {
				int e = s.top();
				s.pop();

				if (obstacle[e][0] == cur[0]) d = true;
				if (obstacle[e][0] == dst[0]) u = true;
				if (obstacle[e][1] == cur[1]) l = true;
				if (obstacle[e][1] == dst[1]) r = true;

				for (int j = 0; j < num_obstacles; ++j) {
					for (int k = 0; k < 8; ++k) {
						if (!visited[j] && (obstacle[j][0] == obstacle[e][0] + neigh[k][0]) && (obstacle[j][1] == obstacle[e][1] + neigh[k][1])) {
							visited[j] = true;
							s.push(j);
						}
					}
				}
			}

			if ((u && d) || (l && r) || (u && r) || (l && d))
				return MAX;
		}
	}

	return abs(dst[0] - cur[0]) + abs(dst[1] - cur[1]);
}