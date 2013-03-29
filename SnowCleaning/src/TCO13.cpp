//============================================================================
// Name        : TCO13.cpp
// Author      : Gael
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include "SnowCleaning.h"

int main() {
	SnowCleaning theCleaningObj;
	int boardSize = 0;
	int salary = 0;
	int snowFine = 0;
	cin >> boardSize;
	cin >> salary;
	cin >> snowFine;

	theCleaningObj.init(boardSize, salary, snowFine);

//	for(int ex = 0; ex < 2; ex++) {
	for(int t = 0; t < 2000; t++) {
		int snowCnt = -1;
		cin >> snowCnt;
		vector<int> snowFalls(2*snowCnt);
		for(int i = 0; i < 2 * snowCnt; i++) {
			int tempi = -1;
			cin >> tempi;
			snowFalls[i] = tempi;
		}
		vector<string> ret = theCleaningObj.nextDay(snowFalls);

		cout << ret.size() << endl;
		for(int i = 0; i < int(ret.size()); i++) {
			cout << ret[i];
			cout << endl;
		}
		cout.flush();
	}
//	}

	return 0;
}
