#include "Party.h"

Party::Party() {
	income = 0;
	incomeFactors.clear();
	outcomeFactors.clear();
}

int Party::GetIncome() {
	return income;
}

void Party::SetIncome(int inc) {
	income = inc > 0 ? inc : 0;
}