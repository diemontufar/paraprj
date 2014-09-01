
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

static const long NUMTICKS = 2000; //Number of ticks

static const int HUMANSPEED = 2; //Human speed
static const int INCUBATIONTIME = 5;//Ticks that take the zombie disease to incubate in an infected human

static int const GRIDROWS = 20; //Number of row grids
static int const GRIDCOLUMNS = 20;//Number of column grids

static int const CELLROWSPERGRID = 10; //Number of cell rows per grid
static int const CELLCOLUMNSPERGRID = 10; //Number of coll columns per grid

static const int ZOMBIESPEED = 1; //Zombie speed
static const int DECOMPOSITIONTIME = 500;//Ticks that take the zombie to decompose

static const float BIRTHPERCENTAGE = 40; //40%
static const float HEADSHOTPERCENTAGE = 10; //40%
static const float SUCESSFULBITEPERCENTAGE = 70; //40%

#define DEBUG = true;
//#define DEBUGMODEL = true; //Enable more verbosity debug messages
//#define DEBUGGRID = false;
//#define DEBUGCELL = true;

#endif
