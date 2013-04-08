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

int main( int argc, char *argv[] ) {

	SnowCleaning theCleaningObj;

	int boardSize = 0;
	int salary = 0;
	int snowFine = 0;
	cin >> boardSize;
	cin >> salary;
	cin >> snowFine;

	theCleaningObj.init(boardSize, salary, snowFine);

	if( argc != 1 ) /* argc should be 2 for correct execution */
	{
		// We print argv[0] assuming it is the program name
		//printf( "usage: filename" );
		for(int i = 1; i < argc ; i++) {
			double t;
			if(sscanf_s(argv[i], "%lf", &t) != 1) {
				//printf ("Illegal value for \"t\": %s\n", argv[i]);
				return 1;
			}
			//printf("%lf ",t);
			if(t >= 0.0)
				theCleaningObj.pBoard->setArg(i-1, t);
		}
	}

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
