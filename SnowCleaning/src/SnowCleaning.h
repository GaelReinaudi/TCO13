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
	: pBoard(ptheBoard)	{	r = startR;	c = startC;	id = theID;	newHire = true;	radius = theRadius; Ar = -1; Ac = -1;}

	string MoveR();
	string MoveL();
	string MoveU();
	string MoveD();
	string NoMove();
	string MoveRandLR();
	string MoveRandUD();
	double Manhattan(int otherR, int otherC) {
		return abs(otherR - r) + abs(otherC - c);
	}
	double Distance(int otherR, int otherC) {
		return sqrt((otherR - r)*(otherR - r) + (otherC - c)*(otherC - c));
	}
	double ValueInRadius();
	string MoveForce();
	bool AssignCell(int asr, int asc) {
		if(Ar >= 0) // already assigned
			return false;
		Ar = asr; Ac = asc;
		return true;
	}
	bool UnAssignCell() {
		Ar = -1;
		Ac = -1;
		return true;
	}
	vector<int> ClosestIsolatedSnow();

public:
	int r, c, id;
	double radius;
private:
	Board * pBoard;
	bool newHire;
	int Ar, Ac;
};

class Board {
public:
	Board(int theLimit, map<int, Worker*> & theIdWorkers)
	: snow(theLimit), value(theLimit), valueExt(theLimit), idWorkers(theIdWorkers)
	{	L = theLimit;
		FOR(r, 0 , L) {
//			snow.resize(L);
//			value.resize(L);
//			valueExt.resize(L);
			snow.at(r).resize(L);
			value.at(r).resize(L);
			valueExt.at(r).resize(L);
			FOR(c, 0, L) {
				snow[r][c] = false;
				value[r][c] = 0;
				valueExt[r][c] = 0;
	}	}
		maxSnowRC.push_back(L/2);
		maxSnowRC.push_back(L/2);
		maxSnowRCExt.push_back(L/2);
		maxSnowRCExt.push_back(L/2);
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
	double ValExt(int r, int c) const {
		if(r < 0 || c < 0 || r >= L || c >= L)
			return 0;
		return valueExt[r][c];
	}
	int TotalSnow() {
		int total = 0;
		FORALLrc {
			total += Snow(r, c) ? 1 : 0;
		}
		return total;
	}
	vector<int> MaxSnow0() {return maxSnowRC;}
	vector<int> MaxSnowExt() {return maxSnowRCExt;}

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
		double maxiExt = 0.0;
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
		FORALLrc {
			valueExt[r][c] = Val(r, c)+0.1;
			if(Snow(r, c)) {
				valueExt[r][c] += (Val(r+1, c)) + (Val(r-1, c)) + (Val(r, c+1)) + (Val(r, c-1))
						+ (Val(r+1, c+1)) + (Val(r+1, c-1)) + (Val(r-1, c-1)) + (Val(r-1, c+1));
			}
			if(valueExt[r][c] > maxiExt) {
				maxSnowRCExt[0] = r;
				maxSnowRCExt[1] = c;
				maxiExt = valueExt[r][c];
			}
		}
	}
	double ClosestWorkerDist(int fromR, int fromC) {
		double dist = 1000.0;
		for(std::map<int, Worker*>::iterator it = idWorkers.begin(); it != idWorkers.end(); ++it) {
			double Wd = it->second->Manhattan(fromR, fromC);
			if(Wd < dist)
				dist = Wd;
		}
		return dist;
	}
	Worker* ClosestWorker(int fromR, int fromC) {
		double dist = 1000.0;
		Worker* pW = 0;
		for(std::map<int, Worker*>::iterator it = idWorkers.begin(); it != idWorkers.end(); ++it) {
			double Wd = it->second->Manhattan(fromR, fromC);
			if(Wd < dist) {
				dist = Wd;
				pW = it->second;
			}
		}
		return pW;
	}
	bool HasWorker(int inR, int inC) {
		for(std::map<int, Worker*>::iterator it = idWorkers.begin(); it != idWorkers.end(); ++it) {
			int rr = it->second->r;
			int cc = it->second->c;
			if(inR == rr && inC == cc)
				return true;
		}
		return false;
	}
	// updates the snow map depending on where the cleaners are
	void Clean();

public:
	int L;
	map<int, Worker*> & idWorkers;
//private:
	vector<vector<bool> > snow;
	vector<vector<double> > value;
	vector<vector<double> > valueExt;
	vector<int> maxSnowRC;
	vector<int> maxSnowRCExt;
};


class SnowCleaning {
public:
	int init(int boardSize, int salary, int snowFine);

	vector<string> nextDay(vector<int> snowFalls);

Board* pBoard;

private:
	string Hire(int r, int c);
	void AssignWorkers();
	bool DiceHire0();
	bool DiceHire1();

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

	pBoard->snow[r][c] = false;
	pBoard->value[r][c] = 0.0;
	pBoard->valueExt[r][c] = 0.0;
	pBoard->UpdateValues();

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
	if(pBoard->HasWorker(r, c+1))
		return "";
	c++;
	pBoard->snow[r][c] = false;
	pBoard->value[r][c] = 0.0;
	pBoard->valueExt[r][c] = 0.0;
	std::stringstream ss;	ss << "M " << id << " R";
	return ss.str();
}
inline
string Worker::MoveL() {
	if(c <= 0)
		return "";
	if(pBoard->HasWorker(r, c-1))
		return "";
	c--;
	pBoard->snow[r][c] = false;
	pBoard->value[r][c] = 0.0;
	pBoard->valueExt[r][c] = 0.0;
	std::stringstream ss;	ss << "M " << id << " L";
	return ss.str();
}
inline
string Worker::MoveU() {
	if(r <= 0)
		return "";
	if(pBoard->HasWorker(r-1, c))
		return "";
	r--;
	pBoard->snow[r][c] = false;
	pBoard->value[r][c] = 0.0;
	pBoard->valueExt[r][c] = 0.0;
	std::stringstream ss;	ss << "M " << id << " U";
	return ss.str();
}
inline
string Worker::MoveD() {
	if(r  >= pBoard->L - 1)
		return "";
	if(pBoard->HasWorker(r+1, c))
		return "";
	r++;
	pBoard->snow[r][c] = false;
	pBoard->value[r][c] = 0.0;
	pBoard->valueExt[r][c] = 0.0;
	std::stringstream ss;	ss << "M " << id << " D";
	return ss.str();
}
inline
string Worker::NoMove() {
	pBoard->snow[r][c] = false;
	pBoard->value[r][c] = 0.0;
	pBoard->valueExt[r][c] = 0.0;
	return "";
}
inline
string Worker::MoveRandLR() {
//return NoMove();
	if(r  >= pBoard->L - 1)
		return MoveL();
	if(r  <= 0)
		return MoveR();
	if(double(rand()) / double(RAND_MAX) < 0.5)
		return MoveL();
	return MoveR();
}
inline
string Worker::MoveRandUD() {
//return NoMove();
	if(c  >= pBoard->L - 1)
		return MoveU();
	if(c  <= 0)
		return MoveD();
	if(double(rand()) / double(RAND_MAX) < 0.5)
		return MoveU();
	return MoveD();
}

inline
double Worker::ValueInRadius() {
	double val = 0.0;
	int R = radius / 2 - 1;
	FOR(y, r - R, r + R) {
		FOR(x, c - R, c + R) {
			val += pBoard->Val(x, y);
		}
	}
	return val + 1.1;
}
inline
vector<int> Worker::ClosestIsolatedSnow() {
	int valS = 1000.0;
	vector<int> toRet(2, -1);
	FOR(d,0,pBoard->L) {
		FOR(relR,-d,d) {
			FOR(pm,0,1) {
				int absR = r + relR;
				int absC = pm ? (c + (d - abs(relR))) : (c - (d - abs(relR)));
				int valtemp = pBoard->ValExt(absR, absC);
				if(valtemp > 0.5 && valtemp < valS) {
					valS = valtemp;
					toRet[0] = absR;
					toRet[1] = absC;
				}
			}
		}
		if(valS < 1000.0)
			break;
	}
	return toRet;
}

inline
int SnowCleaning::init(int boardSize, int salary, int snowFine) {
	L = boardSize;
	S = salary;
	F = snowFine;
	day = 0;
	workers = 0;
	pBoard = new Board(L, idWorkers);
	radius = sqrt(double(S) / double(F)) * 2.8;
	return 0;
}

inline
bool SnowCleaning::DiceHire0() {
	double p = double(rand()) / double(RAND_MAX);
	double t = double(day) / 1900.0;
	bool yes = p*p*p*p > t;
	return yes;
}
inline
bool SnowCleaning::DiceHire1() {
	double p = double(rand()) / double(RAND_MAX);
	double t = double(day) / 1999.0;
	double FsS = double(F) / double(S);
	double u = t * (FsS) / 2;
	double v = u*u;
	double w = v*v;
	bool yes = p < v;

	return false;
	return yes;
}

inline
vector<string> SnowCleaning::nextDay(vector<int> snowFalls) {
	pBoard->NewSnow(snowFalls);
	pBoard->UpdateValues();

	vector<string> orders;

	double FsS = double(F) / double(S);
	vector<int> maxRC = pBoard->MaxSnowExt();
	if(0*pBoard->TotalSnow() > (workers + 1) * radius * radius * 1.0) {
		if(DiceHire0()) {
			string order;
			order = Hire(maxRC[0], maxRC[1]);
			if(order != "")
				orders.push_back(order);
			maxRC = pBoard->MaxSnowExt();
			if(true) {
				order = Hire(maxRC[0], maxRC[1]);
				if(order != "")
					orders.push_back(order);
			}
		}
	}
	if(pBoard->TotalSnow() > (double(workers) + double(3.0 - 2 * day / 2000.0)) * radius * radius * 1.0) {
		if(DiceHire0()) {
			maxRC = pBoard->MaxSnowExt();
			string order;
			order = Hire(maxRC[0], maxRC[1]);
			if(order != "")
				orders.push_back(order);
			maxRC = pBoard->MaxSnowExt();
			if(true) {
				order = Hire(maxRC[0], maxRC[1]);
				if(order != "")
					orders.push_back(order);
			}
		}
	}
	maxRC = pBoard->MaxSnowExt();
	double valTrig = 8.0;
	double distTrig = (3.0 + sqrt(1/(FsS))) * 20.0 / 9;//pBoard->Val(maxRC[0], maxRC[1]);
	double valTrigExt = 70.0;
	double distTrigExt = (radius) * 130.0 / pBoard->ValExt(maxRC[0], maxRC[1]);
	if(day < 111600 && pBoard->Val(maxRC[0], maxRC[1]) >= valTrig)
	{
		if(pBoard->ClosestWorkerDist(maxRC[0], maxRC[1]) > distTrig) {
			string order;
			order = Hire(maxRC[0], maxRC[1]);
			if(order != "")
				orders.push_back(order);
			maxRC = pBoard->MaxSnowExt();
			if(false) {
				order = Hire(maxRC[0], maxRC[1]);
				if(order != "")
					orders.push_back(order);
			}
		}
	}
	if(day < 111200 && pBoard->Val(maxRC[0], maxRC[1]) >= valTrig*1.0)
	{
		if(pBoard->ClosestWorkerDist(maxRC[0], maxRC[1]) > distTrig) {
			string order;
			order = Hire(maxRC[0], maxRC[1]);
			if(order != "")
				orders.push_back(order);
			maxRC = pBoard->MaxSnowExt();
			if(false) {
				order = Hire(maxRC[0], maxRC[1]);
				if(order != "")
					orders.push_back(order);
			}
		}
	}

	FOR(i, 0, workers) {
		string order;
		order = idWorkers[i]->MoveForce();
		if(order != "")
			orders.push_back(order);
	}

	AssignWorkers();

	if(S < F && day >= 1999 - F / S){
		FORALLrc {
			if(pBoard->Snow(r, c)) {
				string order;
				order = Hire(r, c);
				if(order != "")
					orders.push_back(order);
			}
		}
	}

	pBoard->Clean();

	day++;
	return orders;
}

inline
void SnowCleaning::AssignWorkers() {
	for(std::map<int, Worker*>::iterator it = pBoard->idWorkers.begin(); it != pBoard->idWorkers.end(); ++it) {
		Worker* pW = it->second;
		vector<int> cloSnow = pW->ClosestIsolatedSnow();
		// closest worker of this isolated snow
		if(cloSnow[0] >= 0) {
			Worker* pClosWorkColSonwo = pBoard->ClosestWorker(cloSnow[0], cloSnow[1]);
			if(pClosWorkColSonwo == pW) {
				bool didAssign = pClosWorkColSonwo->AssignCell(cloSnow[0], cloSnow[1]);
			}
		}
	}
return;
	FORALLrc {
		if(pBoard->Snow(r, c)) {
			Worker* pCloW = pBoard->ClosestWorker(r, c);
			if(pCloW) {
				bool didAssign = pCloW->AssignCell(r, c);
			}
		}
	}
}

inline
string Worker::MoveForce() {
	if(newHire) {
		newHire = false;
		return "";
	}

	double Fx = 0.0;
	double Fy = 0.0;
	int l = pBoard->L;
	// for assigned workers
	if(Ar >= 0) {
		// if no snow, no more assigned
		if(!pBoard->Snow(Ar, Ac)) {
			UnAssignCell();
		}
		else {
			double AssignedForce = 10.0;
			if(Manhattan(Ar, Ac) <= 10.5 && pBoard->Snow(Ar, Ac)) {
				Fx += double(Ac - c) * AssignedForce;
				Fy += double(Ar - r) * AssignedForce;
			}
		}
	}
	// if unassigned
//	if(Ar < 0)
	{

	int range = l;
	// attraction by snow like gravity
	FOR(otherR, r-range, r+range) {
		if(otherR < 0 || otherR >= l)
			continue;
		FOR(otherC, c-range, c+range) {
			if(otherC < 0 || otherC >= l)
				continue;
			double dist = Manhattan(otherR, otherC);
			if(dist == 0.0)
				continue;
			double val = pBoard->Val(otherR, otherC);
			double oneOverDist = 1 / dist;
			double tothe3 = oneOverDist * oneOverDist * oneOverDist;
			Fx += double(otherC - c) * val * tothe3;
			Fy += double(otherR - r) * val * tothe3;
			// short range force
			double shortRange = 100.0;
			if(dist <= 1.5 && pBoard->Snow(otherR, otherC)) {
				Fx += double(otherC - c) * shortRange;
				Fy += double(otherR - r) * shortRange;
			}
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
		double fac = 0.9;
		double valSnowWorker = pW->ValueInRadius() * fac;// * 0.9;
		double oneOverDist = 1 / dist;
		double tothe3 = oneOverDist * oneOverDist * oneOverDist;
		Rx -= double(pW->c - c) * tothe3 * valSnowWorker;
		Ry -= double(pW->r - r) * tothe3 * valSnowWorker;
		Rx -= double(pW->c - c) * radius * pW->radius * oneOverDist * exp(-dist / radius * 0.58);
		Ry -= double(pW->r - r) * radius * pW->radius * oneOverDist * exp(-dist / radius * 0.58);
	}
	// repulsion from walls
	double wallFac = 0.7 * sqrt(2.2317495/radius);
	Rx -= double(-1-c) * radius * radius / double(c+1) / double(c+1) * exp(-double(c+1) / radius * wallFac);
	Ry -= double(-1-r) * radius * radius / double(r+1) / double(r+1) * exp(-double(r+1) / radius * wallFac);
	Rx -= double(pBoard->L-c) * radius * radius / double(pBoard->L-c) / double(pBoard->L-c) * exp(-double(pBoard->L-c) / radius * wallFac);
	Ry -= double(pBoard->L-r) * radius * radius / double(pBoard->L-r) / double(pBoard->L-r) * exp(-double(pBoard->L-r) / radius * wallFac);

	Fx *= radius * radius;
	Fy *= radius * radius;
	Fx += Rx;
	Fy += Ry;

	}

	if(pBoard->Snow(r, c))
		return NoMove();


	if(abs(Fx) > abs(Fy) && (Fx > 0.0 && !pBoard->Snow(r, c+1) || Fx < 0.0 && !pBoard->Snow(r, c-1))) {
		if(Fy > 0.0 && pBoard->Snow(r+1, c))
			return MoveD();
		if(Fy < 0.0 && pBoard->Snow(r-1, c))
			return MoveU();
	}
	if(abs(Fy) > abs(Fx) && (Fy > 0.0 && !pBoard->Snow(r+1, c) || Fy < 0.0 && !pBoard->Snow(r-1, c))) {
		if(Fx > 0.0 && pBoard->Snow(r, c+1))
			return MoveR();
		if(Fx < 0.0 && pBoard->Snow(r, c-1))
			return MoveL();
	}

	double trig = 0.0;
	if(Fx > trig && abs(Fx) > abs(Fy)) {
		string order = MoveR();
//		if(order == "")
//			order = MoveRandUD();
		return order;
	}
	if(Fx < -trig && abs(Fx) > abs(Fy)) {
		string order = MoveL();
//		if(order == "")
//			order = MoveRandUD();
		return order;
	}
	if(Fy > trig && abs(Fy) > abs(Fx)) {
		string order = MoveD();
//		if(order == "")
//			order = MoveRandLR();
		return order;
	}
	if(Fy < -trig && abs(Fy) > abs(Fx)) {
		string order = MoveU();
//		if(order == "")
//			order = MoveRandLR();
		return order;
	}

	if(rand() < RAND_MAX / 2)
		return MoveRandLR();
	return MoveRandUD();
}




#endif /* SNOWCLEANING_H_ */
