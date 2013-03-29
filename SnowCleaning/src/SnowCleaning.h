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
#define FOR(i,s,e) for (int i = int(s); i != int(e); i++)
#define FORALLrc FOR(r,0,L) FOR(c,0,L)
#define FORIT(i,c) for (typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define present(c,x) ((c).find(x) != (c).end())
#define cpresent(c,x) (find(all(c),x) != (c).end())
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> ii;

class Board;

class Worker {
public:
	Worker(int startR, int startC, int theID, Board *ptheBoard, double theRadius)
	: pBoard(ptheBoard)	{	r = startR;	c = startC;	id = theID;	newHire = true;	radius = theRadius;}

	string MoveR();
	string MoveL();
	string MoveU();
	string MoveD();
	double Manhattan(int otherR, int otherC) {
		return abs(otherR - r) + abs(otherC - c);
	}
	string MoveForce();

public:
	int r, c, id;
	double radius;
private:
	Board * pBoard;
	bool newHire;
};

class Board {
public:
	Board(int theLimit, map<int, Worker*> & theIdWorkers)
	: snow(theLimit), value(theLimit), idWorkers(theIdWorkers)
	{	L = theLimit;
		FOR(r, 0 , L) {
//			snow.resize(L);
//			value.resize(L);
			snow.at(r).resize(L);
			value.at(r).resize(L);
			FOR(c, 0, L) {
				snow[r][c] = false;
				value[r][c] = 0;
	}	}
		maxSnowRC.push_back(L/2);
		maxSnowRC.push_back(L/2);
	}

	int size() const {return L;}

	bool Snow(int r, int c) const {
		if(r < 0 || c < 0 || r >= L || c >= L)
			return false;
		return snow[r][c];
	}
	double Val(int r, int c) const {
		if(r < 0 || c < 0 || r >= L || c >= L)
			return 0;
		return value[r][c];
	}
	int TotalSnow() {
		int total = 0;
		FORALLrc {
			total += Snow(r, c) ? 1 : 0;
		}
		return total;
	}
	vector<int> MaxSnow() {return maxSnowRC;}

	void NewSnow(const vector<int> & snowFalls) {
		int falls = snowFalls.size() / 2;
		FOR(i, 0, falls) {
			int r = snowFalls[2 * i + 0];
			int c = snowFalls[2 * i + 1];
			snow[r][c] = true;
		}
	}
	void UpdateValues() {
		double maxi = 0.0;
		FORALLrc {
			value[r][c] = Snow(r, c) ? 1.0 : 0.0;
			if(Snow(r, c)) {
				value[r][c] += (Snow(r+1, c)?1:0) + (Snow(r-1, c)?1:0) + (Snow(r, c+1)?1:0) + (Snow(r, c-1)?1:0)
						+ (Snow(r+1, c+1)?1:0) + (Snow(r+1, c-1)?1:0) + (Snow(r-1, c-1)?1:0) + (Snow(r-1, c+1)?1:0);
			}
			if(value[r][c] > maxi) {
				maxSnowRC[0] = r;
				maxSnowRC[1] = c;
				maxi = value[r][c];
			}
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
		for(std::map<int, Worker*>::iterator it = idWorkers.begin(); it != idWorkers.end(); ++it) {
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
	map<int, Worker*> & idWorkers;
private:
	vector<vector<bool> > snow;
	vector<vector<double> > value;
	vector<int> maxSnowRC;
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
		radius = double(S) / double(F) * double(L) / 12;
		return 0;
	}

	vector<string> nextDay(vector<int> snowFalls) {
		pBoard->NewSnow(snowFalls);
		pBoard->UpdateValues();

		vector<string> orders;

		vector<int> maxRC = pBoard->MaxSnow();
		if(pBoard->TotalSnow() * 1.2 > (workers) * radius * radius) {
			string order;
			order = Hire(maxRC[0], maxRC[1]);
			if(order != "")
				orders.push_back(order);
		}
//		else if(pBoard->Val(maxRC[0], maxRC[1]) >= 8.0) {
//			string order;
//			order = Hire(maxRC[0], maxRC[1]);
//			if(order != "")
//				orders.push_back(order);
//		}


		FOR(i, 0, workers) {
			string order;
			order = idWorkers[i]->MoveForce();
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
	double radius;
private:
	int day;
	int workers;
	map<int, Worker*> idWorkers;
};

inline
string SnowCleaning::Hire(int r, int c) {
	if(workers >= 100)
		return "";
	Worker* pW = new Worker(r, c, workers++, pBoard, radius);
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
string Worker::MoveR() {
	if(c >= pBoard->L - 1)
		return "";
	c++;
	std::stringstream ss;	ss << "M " << id << " R";
	return ss.str();
}
inline
string Worker::MoveL() {
	if(c <= 0)
		return "";
	c--;
	std::stringstream ss;	ss << "M " << id << " L";
	return ss.str();
}
inline
string Worker::MoveU() {
	if(r <= 0)
		return "";
	r--;
	std::stringstream ss;	ss << "M " << id << " U";
	return ss.str();
}
inline
string Worker::MoveD() {
	if(r  >= pBoard->L - 1)
		return "";
	r++;
	std::stringstream ss;	ss << "M " << id << " D";
	return ss.str();
}
inline
string Worker::MoveForce() {
	if(newHire) {
		newHire = false;
		return "";
	}

	double Fx = 0.0;
	double Fy = 0.0;
	// attraction by snow like gravity
	FOR(otherR, 0, pBoard->L) {
		FOR(otherC, 0, pBoard->L) {
			double dist = Manhattan(otherR, otherC);
			if(dist == 0.0)
				continue;
			double val = pBoard->Val(otherR, otherC);
			Fx += double(otherC - c) * val / dist / dist / dist;
			Fy += double(otherR - r) * val / dist / dist / dist;
		}
	}
	double Rx = 0.0;
	double Ry = 0.0;
	// repulsion by other workers
	for(std::map<int, Worker*>::iterator it = pBoard->idWorkers.begin(); it != pBoard->idWorkers.end(); ++it) {
		Worker* pW = it->second;
		if(pW->id == id)
			continue;
		double dist = Manhattan(pW->r, pW->c);
		if(dist == 0.0)
			continue;
//		Rx -= double(pW->c - c) * exp(-dist / radius) * 10.0;
//		Ry -= double(pW->r - r) * exp(-dist / radius) * 10.0;
		Rx -= double(pW->c - c) * radius * pW->radius / dist / dist * exp(-dist / radius * 0.2);
		Ry -= double(pW->r - r) * radius * pW->radius / dist / dist * exp(-dist / radius * 0.2);
	}
	// repulsion from walls
	double wallFac = 0.7;
	Rx -= double(0-c) * radius * radius / double(c+1) / double(c+1) * exp(-double(c+1) / radius * wallFac);
	Ry -= double(0-r) * radius * radius / double(r+1) / double(r+1) * exp(-double(r+1) / radius * wallFac);
	Rx -= double(pBoard->L-c) * radius * radius / double(pBoard->L-c) / double(pBoard->L-c) * exp(-double(pBoard->L-c) / radius * wallFac);
	Ry -= double(pBoard->L-r) * radius * radius / double(pBoard->L-r) / double(pBoard->L-r) * exp(-double(pBoard->L-r) / radius * wallFac);

	Fx *= radius * radius;
	Fy *= radius * radius;
	Fx += Rx;
	Fy += Ry;
	if(Fx > 0.0 && abs(Fx) > abs(Fy)) {
		return MoveR();
	}
	if(Fx < 0.0 && abs(Fx) > abs(Fy)) {
		return MoveL();
	}
	if(Fy > 0.0 && abs(Fy) > abs(Fx)) {
		return MoveD();
	}
	if(Fy < 0.0 && abs(Fy) > abs(Fx)) {
		return MoveU();
	}

	return "";
}




#endif /* SNOWCLEANING_H_ */
