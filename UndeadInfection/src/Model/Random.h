/*
 * Random.h
 *
 *  Created on: Sep 3, 2014
 *      Author: diogonal
 */

#ifndef RANDOM_H_
#define RANDOM_H_



class Random
{
public:
    Random();
    virtual ~Random();
    static double random();
    static void resetSeed();
    static int random(int range);
    static int random(int start, int end);
    static bool randomBool();
    static bool randomBoolTrueBiased();
    static bool randomBoolFalseBiasedN();
    static bool randomBoolFalseBiasedZN();

};


#endif /* RANDOM_H_ */
