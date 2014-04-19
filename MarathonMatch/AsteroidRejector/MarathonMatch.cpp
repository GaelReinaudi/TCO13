//============================================================================
// Name        : MarathonMatch
// Author      : Gael
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include "AsteroidRejector.h"

int main( int argc, char *argv[] ) {

	AsteroidRejector theObj;

	int N = 0;
	int M = 0;
	cout << "hello" << endl;
	cout.flush();
	for(int i = 0; i < 1000; ++i)
	{
		cin >> N;
		vector<int> imageData(N);
		for(int j = 0; j < N; ++j)
		{
			cin >> imageData[j];
		}
		cin >> M;
		vector<string> detections(M);
		for(int j = 0; j < M; ++j)
		{
			cin >> detections[j];
		}
        int result = theObj.trainingData(imageData, detections);
		cout << result;
		cout << endl;
		cout.flush();
	}

    for (int i=0; i < 200; i++)
    {
		cin >> N;
		vector<int> imageData(N);
		for(int j = 0; j < N; ++j)
		{
			cin >> imageData[j];
		}
		cin >> M;
		vector<string> detections(M);
		for(int j = 0; j < M; ++j)
		{
			cin >> detections[j];
		}
        int result = theObj.testingData(imageData, detections);
		cout << result;
		cout << endl;
		cout.flush();
    }
	return 0;

	//theCleaningObj.init(boardSize, salary, snowFine);

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
			{
				//theCleaningObj.pBoard->setArg(i-1, t);
			}
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
		vector<string> ret;// = theCleaningObj.nextDay(snowFalls);

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
