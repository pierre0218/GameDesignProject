#include "EconomyManager.h"
#include <vector>

using namespace std;

void EconomyManager::Start() {

}

void EconomyManager::Update(int skip) {
	for (vector<Party>::iterator it = all_parties.begin(); it != all_parties.end(); ++it) {
		int income = it->GetIncome();
		for (vector<FnObject>::iterator inc = it->incomeFactors.begin(); inc != it->incomeFactors.end(); ++inc) {

		}
		for (vector<FnObject>::iterator out = it->outcomeFactors.begin(); out != it->outcomeFactors.end(); ++out) {

		}
		it->SetIncome(income);
	}
}

void EconomyManager::GainIncome(Party party, FnObject obj) {

}

void EconomyManager::LoseIncome(Party party, FnObject obj) {

}