
#ifndef PARAMETERS_H_
#define PARAMETERS_H_

/*
 * http://www.indexmundi.com/australia/demographics_profile.html
 * Demographics Northern Territory:
 * Land Area: 1,420,970Km2
 * Population: 211,945 (2011 Census) / 235,179 (2012)
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
static const int NUMTHREADS = 8;
//static const long NUMTICKS = 45625; //Number of ticks 125*365
static const long NUMTICKS = 3650; //Number of ticks 125*365
//static const long NUMTICKS = 10; //Number of ticks 125*365
static const long YEARTICKS = 365; //Ticks in 1 year 365
static const long NUMTICKSPRINT = 360; //No. Ticks to print

static const int INCUBATIONTIME = 5;//Ticks that take the zombie disease to incubate in an infected human
static float const SCALE = 1.0/6.0; //Proportion of rows/columns of NT covered in the simulation
static int const GRIDROWS = 500; //Number of row grids
static int const GRIDCOLUMNS =500;//Number of column grids
static float const TOTALGRIDCELLS = GRIDROWS * GRIDCOLUMNS; //Number of cells grids
static const float NUMBEROFZOMBIES = 10;

static const int MINFERTILITYAGE = 15;
static const int MAXFERTILITYAGE = 45;
static const float PROBTOHAVEBABYWHENPAIRED = 0.0035; //Prob. of birth when paired

static const int MINDECOMPOSITIONTIME = 29;//Ticks that take the zombie to decompose
static const int MAXDECOMPOSITIONTIME = 30;//Ticks that take the zombie to decompose

static const int MINLIFEEXPECTANCY = 65;//minimum human life expectancy
static const int MAXLIFEEXPECTANCY= 75;//maximum human life expectancy

static const float GENDERRATIO = 0.48;

static const float AUSPOP = 23000000.0;
static const float NTPOP = 235182.0; //2012 census
static const float NTPOPDENSITY = 0.17/2.0;
//static const float DARWINPOPDENSITY = 50.0*50.0*NTPOPDENSITY;
static const float DARWINPOPDENSITY = 500.0*500.0*NTPOPDENSITY;
static const float DEATHSNT= 1038.0; //http://www.abs.gov.au/AUSSTATS/abs@.nsf/DetailsPage/3302.02012?OpenDocument
static const float DEATHRATENT = DEATHSNT/365.0/6;


static const float GUNDENSITY = 0.2; //19.13

static const float HEADSHOTPERCENTAGE = 0.05; //40%
static const float SUCESSFULBITEPERCENTAGE =50; //40%

#define MOVE    0.25 //Probability of a move in each direction
#define DEBUG = true;
#define DEBUGGRID = true;


//-static const float DEATHRATE = 403.0/23000000.0; //Used in inmmaculate conception
//-static const float BIRTHSPERDAYAU = 300000/365; //~ 300,000 births per year in Australia
//-static const float BIRTHSPERDAYNT = 12; // With a growth of 3.8% per year in NT ->http://www.abs.gov.au/ausstats/abs@.nsf/latestProducts/3301.0Media%20Release12012 sum 3 years and divide by 3
//-static const float BIRTHSPERWOMAN = 2;

#endif
