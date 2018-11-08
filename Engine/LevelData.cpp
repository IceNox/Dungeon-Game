#include "LevelData.h"

#include <fstream>

LevelData::LevelData(std::string levelFileName)
{
	/*
	std::string directory = "Content/Game/Levels/" + levelFileName;
	std::ifstream in(directory);

	in >> levelName;
	in >> width >> height;
	in >> playerStartX >> playerStartY;
	in >> exitX >> exitY;

	in >> wallCount;
	for (int i = 0; i < wallCount; i++) {
		int wx;
		int wy;
		std::string wt;
		bool wa;
		bool wtg;

		in >> wx >> wy >> wt >> wa >> wtg;

		wallX.push_back(wx);
		wallY.push_back(wy);
		wallType.push_back(wt);
		wallActivatable.push_back(wa);
		wallToggleable.push_back(wtg);
	}

	in >> waterCount;
	for (int i = 0; i < waterCount; i++) {
		int wx;
		int wy;

		in >> wx >> wy;

		waterX.push_back(wx);
		waterY.push_back(wy);
	}

	in >> lavaCount;
	for (int i = 0; i < lavaCount; i++) {
		int lx;
		int ly;

		in >> lx >> ly;

		lavaX.push_back(lx);
		lavaY.push_back(ly);
	}

	in >> baseFloorType;

	in >> floorCount;
	for (int i = 0; i < floorCount; i++) {
		int fx;
		int fy;
		std::string ft;

		in >> fx >> fy >> ft;

		floorX.push_back(fx);
		floorY.push_back(fy);
		floorType.push_back(ft);
	}

	in >> wireCount;
	for (int i = 0; i < wireCount; i++) {
		int wx;
		int wy;
		std::string wc;
		bool wg;
		std::string wgt;
		std::string wf;

		in >> wx >> wy >> wc >> wg >> wgt >> wf;

		wireX.push_back(wx);
		wireY.push_back(wy);
		wireColor.push_back(wc);
		wireGate.push_back(wg);
		wireGateType.push_back(wgt);
		wireFacing.push_back(wf);
	}

	in >> crateCount;
	for (int i = 0; i < crateCount; i++) {
		int cx;
		int cy;

		in >> cx >> cy;

		crateX.push_back(cx);
		crateY.push_back(cy);
	}

	in >> pressurePlateCount;
	for (int i = 0; i < pressurePlateCount; i++) {
		int px;
		int py;
		std::string pt;

		in >> px >> py >> pt;

		plateX.push_back(px);
		plateY.push_back(py);
		plateType.push_back(pt);
	}

	in >> lampCount;
	for (int i = 0; i < lampCount; i++) {
		int lx;
		int ly;
		std::string lt;
		bool ltg;

		in >> lx >> ly >> lt >> ltg;

		lampX.push_back(lx);
		lampY.push_back(ly);
		lampType.push_back(lt);
		lampToggleable.push_back(ltg);
	}

	in >> doorCount;
	for (int i = 0; i < doorCount; i++) {
		int dx;
		int dy;
		std::string df;

		in >> dx >> dy >> df;

		doorX.push_back(dx);
		doorY.push_back(dy);
		doorFacing.push_back(df);
	}

	in >> signCount;
	for (int i = 0; i < signCount; i++) {
		int sx;
		int sy;
		std::string st;

		in >> sx >> sy;
		getline(in, st);

		signX.push_back(sx);
		signY.push_back(sy);
		signText.push_back(st);
	}

	in >> shooterCount;
	for (int i = 0; i < shooterCount; i++) {
		std::string st;
		int sx;
		int sy;
		int so;
		std::string sf;
		bool sa;

		in >> st >> sx >> sy >> so >> sf >> sa;

		shooterType.push_back(st);
		shooterX.push_back(sx);
		shooterY.push_back(sy);
		shooterOffset.push_back(so);
		shooterFacing.push_back(sf);
		shooterActive.push_back(sa);
	}

	in >> barbarianCount;
	for (int i = 0; i < barbarianCount; i++) {
		int bx;
		int by;
		std::string bt;

		in >> bx >> by >> bt;

		barbarianX.push_back(bx);
		barbarianY.push_back(by);
		barbarianType.push_back(bt);
	}

	in >> wizardCount;
	for (int i = 0; i < wizardCount; i++) {
		int wx;
		int wy;
		std::string wt;

		in >> wx >> wy >> wt;

		wizardX.push_back(wx);
		wizardY.push_back(wy);
		wizardType.push_back(wt);
	}

	in >> paladinCount;
	for (int i = 0; i < paladinCount; i++) {
		int px;
		int py;
		std::string pt;

		in >> px >> py >> pt;

		paladinX.push_back(px);
		paladinY.push_back(py);
		paladinType.push_back(pt);
	}

	in >> zombieCount;
	for (int i = 0; i < zombieCount; i++) {
		int zx;
		int zy;
		std::string zt;

		in >> zx >> zy >> zt;

		zombieX.push_back(zx);
		zombieY.push_back(zy);
		zombieType.push_back(zt);
	}

	in >> lichCount;
	for (int i = 0; i < lichCount; i++) {
		int lx;
		int ly;
		std::string lt;

		in >> lx >> ly >> lt;

		lichX.push_back(lx);
		lichY.push_back(ly);
		lichType.push_back(lt);
	}

	in >> wraithCount;
	for (int i = 0; i < wraithCount; i++) {
		int wx;
		int wy;
		std::string wt;

		in >> wx >> wy >> wt;

		wraithX.push_back(wx);
		wraithY.push_back(wy);
		wraithType.push_back(wt);
	}
	*/
}