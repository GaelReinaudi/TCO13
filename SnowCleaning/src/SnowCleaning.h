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
using namespace std;

#define sz(a) int((a).size())
#define all(c) (c).begin(),(c).end()
#define FOR(i,s,e) for (int i = int(s); i != int(e); i++)
#define FORIT(i,c) for (typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define present(c,x) ((c).find(x) != (c).end())
#define cpresent(c,x) (find(all(c),x) != (c).end())
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> ii;

class SnowCleaning;

class Worker {
public:
	Worker(int startR, int startC, int theID, int theLimit) {
		r = startR;
		c = startC;
		id = theID;
//		pClean = parent;
		L = theLimit;
	}

	string MoveNaive0() {
		static int lastLineID = -1;
		std::stringstream ss;
		ss << "M " << id << " ";
		string order = ss.str();
		if(r % 2 || lastLineID == id) {
			if(c) {
				c--;
				return order + "L";
			}//else
			if(lastLineID == id) {
				lastLineID = -1;
				return MoveNaive0();
			}
			r--;
			return order + "U";
		}
		else {
			if(c < L - 1) {
				c++;
				return order + "R";
			}//else
			if(r < L - 1) {
				r++;
				return order + "D";
			}//	else
			lastLineID = id;
			return MoveNaive0();
		}
	}

public:
	int r;
	int c;
	int id;
	int L;
//	SnowCleaning* pClean;
};

class SnowCleaning {
public:
	int init(int boardSize, int salary, int snowFine) {
		L = boardSize;
		S = salary;
		F = snowFine;
		day = 0;
		workers = 0;
		return 0;
	}

	vector<string> nextDay(vector<int> snowFalls) {
		vector<string> orders;
		if(day == 0) {
			FOR(i, 0, L) {
				string order;
				if(i % 2)
					order = Hire(i, L - 1);
				else
					order = Hire(i, 0);
				if(order != "")
					orders.push_back(order);

				order = Hire(i, L / 2);
				if(order != "")
					orders.push_back(order);
			}
		}
		else {
			FOR(i, 0, workers) {
				string order;
				order = idWorkers[i]->MoveNaive0();
				orders.push_back(order);
			}
		}
		day++;
		return orders;
	}

private:
	string Hire(int r, int c) {
		if(workers >= 100)
			return "";
		Worker* pW = new Worker(r, c, workers++, L);
		idWorkers[pW->id] = pW;
		std::stringstream ss;
		std::string s;
		ss << "H " << r << " " << c;
		s = ss.str();
		return s;
	}

public:
	int L;
	int S;
	int F;
private:
	int day;
	int workers;
	map<int, Worker*> idWorkers;
};

#endif /* SNOWCLEANING_H_ */
