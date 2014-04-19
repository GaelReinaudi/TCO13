#ifndef SNOWCLEANING_H_
#define SNOWCLEANING_H_

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

#define sz(a) int((a).size())
#define all(c) (c).begin(),(c).end()
#define FOR(i,s,e) for (int i = int(s); i < int(e); i++)
#define FORALLrc FOR(r,0,L) FOR(c,0,L)
#define FORIT(i,c) for (typeof((c).begin()) i = (c).begin(); i < (c).end(); i++)
#define present(c,x) ((c).find(x) != (c).end())
#define cpresent(c,x) (find(all(c),x) != (c).end())
typedef vector<int> vi;
typedef vector<string> vs;
typedef vector<vi> vvi;
typedef pair<int,int> ii;

class AsteroidRejector
{
public:
	AsteroidRejector(void) { }
	~AsteroidRejector(void) { }

	int trainingData(vi imageData, vs detections)
	{
		return 0;
	}
	int testingData(vi imageData, vs detections)
	{
		return 0;
	}
	vi getAnswer()
	{
		vi listRet;
		return listRet;
	}
};


#endif /* SNOWCLEANING_H_ */