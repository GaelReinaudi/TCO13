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

class Board;

class Worker {
public:
	Worker(int startR, int startC, int theID, Board *ptheBoard)
	: pBoard(ptheBoard)	{	r = startR;		c = startC;		id = theID;		newHire = true;	}

	string MoveNaive1();

public:
	int r, c, id;
private:
	Board * pBoard;
	bool newHire;
};

class Board {
public:
	Board(int theLimit, map<int, Worker*> & theIdWorkers)
	: snow(theLimit), idWorkers(theIdWorkers)
	{	L = theLimit;
		FOR(r, 0 , L) {
			snow.resize(L);
			FOR(c, 0, L) {
				snow[r][c] = false;
	}	}	}

	int size() {return L;}

	void NewSnow(const vector<int> & snowFalls) {
		int falls = snowFalls.size() / 2;
		FOR(i, 0, falls) {
			int r = snowFalls[2 * i + 0];
			int c = snowFalls[2 * i + 1];
			snow[r][c] = true;
		}
	}
	int FirstSnowOnRow(int r) {
		FOR(i,0,L){
			if(snow[r][i])
				return i;
		}
		return -1;
	}
	bool HasWorkerOnRow(int r) {
		for(std::map<int, Worker*>::iterator it = idWorkers.begin(); it != idWorkers.end(); ++it)
		{
			Worker* pW = it->second;
			if(pW->r == r)
				return true;
		}
		return false;
	}
	// updates the snow map depending on where the cleaners are
	void Clean();

public:
	int L;
	vector<vector<bool> > snow;
	map<int, Worker*> & idWorkers;
};


class SnowCleaning {
public:
	int init(int boardSize, int salary, int snowFine) {
		L = boardSize;
		S = salary;
		F = snowFine;
		day = 0;
		workers = 0;
		pBoard = new Board(L, idWorkers);
		return 0;
	}

	vector<string> nextDay(vector<int> snowFalls) {
		pBoard->NewSnow(snowFalls);

		vector<string> orders;

		FOR(r, 0, L) {
			string order;
			int firstSnowRow = pBoard->FirstSnowOnRow(r);
			bool workerRow = pBoard->HasWorkerOnRow(r);
			if(firstSnowRow >= 0 && !workerRow) {
				order = Hire(r, firstSnowRow);
			}

			if(order != "")
				orders.push_back(order);
		}


		FOR(i, 0, workers) {
			string order;
			order = idWorkers[i]->MoveNaive1();
			if(order != "")
				orders.push_back(order);
		}

		pBoard->Clean();

		day++;
		return orders;
	}

Board* pBoard;

private:
	string Hire(int r, int c);

public:
	int L;
	int S;
	int F;
private:
	int day;
	int workers;
	map<int, Worker*> idWorkers;
};

inline
string SnowCleaning::Hire(int r, int c) {
	if(workers >= 100)
		return "";
	Worker* pW = new Worker(r, c, workers++, pBoard);
	idWorkers[pW->id] = pW;

	std::stringstream ss;
	std::string s;
	ss << "H " << r << " " << c;
	s = ss.str();
	return s;
}

inline
void Board::Clean() {
	for(std::map<int, Worker*>::iterator it = idWorkers.begin(); it != idWorkers.end(); ++it)
	{
		Worker* pW = it->second;
		int rW = pW->r;
		int cW = pW->c;
		snow[rW][cW] = false;
	}
}


inline
string Worker::MoveNaive1() {
	if(newHire) {
		newHire = false;
		return "";
	}
	std::stringstream ss;
	ss << "M " << id << " ";
	string order = ss.str();
	int rSn = pBoard->FirstSnowOnRow(r);
	if(rSn < c && rSn >= 0) {
		c--;
		return order + "L";
	}
	else if(rSn > c) {
		c++;
		return order + "R";
	}
	return "";
}




#endif /* SNOWCLEANING_H_ */
