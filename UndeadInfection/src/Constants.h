
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/*
 * Demographics Northern Territory:
 * Land Area: 1,420,970Km2
 * Population: 211,945 (2011 Census)
 * % Women: 51.7%
 * % Men: 48.3%
 * Population Density: Population/Km2-> 0.17
 * % of People in Capital (Darwin): 54%
 * % of People between 0-14 years: 23.2%
 * % of People between 15-39 years: 40.5%
 * % of People between 40-59 years: 26.7%
 * % of People between 60 and over: 9.6%
 * Birth rate: 0.025 / 365
 * Death rate: 0.015 / 365
 * Probability weapon: 0.3
 */

static const long NUMTICKS = 2000; //Number of ticks
static const long YEARTICKS = 365; //Ticks in 1 year

static const int HUMANSPEED = 2; //Human speed
static const int INCUBATIONTIME = 5;//Ticks that take the zombie disease to incubate in an infected human

static int const GRIDROWS = 10; //Number of row grids
static int const GRIDCOLUMNS = 10;//Number of column grids

static const int ZOMBIESPEED = 1; //Zombie speed
static const int MINDECOMPOSITIONTIME = 5*365;//Ticks that take the zombie to decompose
static const int MAXDECOMPOSITIONTIME = 11*365;//Ticks that take the zombie to decompose

static const float DARWINPOPDENSITY = 48;
static const float NUMBEROFZOMBIES = 10;
//static const float DARWINPOPDENSITY = 926/2;
static const float NTPOPDENSITY = 0.17;
static const float GENDERRATIO = 0.483; //Women vs Men

static const float GUNDENSITY = 0.3; //19.13
static const float BIRTHPERCENTAGE = 10.75; //

static const float HEADSHOTPERCENTAGE = 20; //40%
static const float SUCESSFULBITEPERCENTAGE = 90; //40%

#define MOVE    0.1 //Probability of a move in each direction

#define DEBUG = true;
#define DEBUGGRID = false;

#endif
