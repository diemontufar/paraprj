/*
 * Random.h
 *
 *  Created on: Sep 3, 2014
 *      Author: diogonal
 */

#ifndef RANDOMCLASS_H_
#define RANDOMCLASS_H_



class RandomClass
{
private:
	unsigned int seed;
public:
    RandomClass(unsigned int threadNumber);
    virtual ~RandomClass();

    double random();
    int random(int range);
    int random(int start, int end);
    bool randomBool();
    bool randomBoolTrueBiased();
    bool randomBoolFalseBiasedN();
    bool randomBoolFalseBiasedZN();

    //-static double random();
    //-static void resetSeed();
    //-static int random(int range);
    //-static int random(int start, int end);
    //-static bool randomBool();
    //-static bool randomBoolTrueBiased();
    //-static bool randomBoolFalseBiasedN();
    //-static bool randomBoolFalseBiasedZN();

};


#endif /* RANDOM_H_ */
