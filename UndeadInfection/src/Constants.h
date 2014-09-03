
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

static const long NUMTICKS = 4000; //Number of ticks

static const int HUMANSPEED = 2; //Human speed
static const int INCUBATIONTIME = 5;//Ticks that take the zombie disease to incubate in an infected human

static int const GRIDROWS = 10; //Number of row grids
static int const GRIDCOLUMNS = 10;//Number of column grids

static int const CELLROWSPERGRID = 10; //Number of cell rows per grid
static int const CELLCOLUMNSPERGRID = 10; //Number of coll columns per grid

static const int ZOMBIESPEED = 1; //Zombie speed
static const int MINDECOMPOSITIONTIME = 5*365;//Ticks that take the zombie to decompose
static const int MAXDECOMPOSITIONTIME = 11*365;//Ticks that take the zombie to decompose

static const float DARWINPOPDENSITY = 48;
static const float NUMBEROFZOMBIES = 10;
//static const float DARWINPOPDENSITY = 926/2;
static const float NTPOPDENSITY = 0.17;

static const float GUNDENSITY = 19.13;
static const float BIRTHPERCENTAGE = 10.75; //

static const float HEADSHOTPERCENTAGE = 20; //40%
static const float SUCESSFULBITEPERCENTAGE = 90; //40%

#define MOVE    0.1 //Probability of a move in each direction

#define DEBUG = true;
//#define DEBUGMODEL = true; //Enable more verbosity debug messages
//#define DEBUGGRID = false;
//#define DEBUGCELL = true;

#endif
