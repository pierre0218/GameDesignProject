#include "FlyWin32.h"
#include <vector>

using namespace std;

class Party
{
private:
	int income;

	/* TODO: add units and other stuffs in here */

public:
	vector<FnObject> incomeFactors;
	vector<FnObject> outcomeFactors;

	Party();

	int GetIncome();
	void SetIncome(int);
};

