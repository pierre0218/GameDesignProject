#include "Party.h"
#include <vector>

using namespace std;

class EconomyManager
{
	static EconomyManager *s_instance;

private:
	vector<Party> all_parties;

public:
	static EconomyManager* instance() {
		if (!s_instance) {
			s_instance = new EconomyManager;
		}
		return s_instance;
	}

	void Start();
	void Update(int);
	void GainIncome(Party, FnObject);
	void LoseIncome(Party, FnObject);
};