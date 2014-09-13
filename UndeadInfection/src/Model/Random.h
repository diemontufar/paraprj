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
    static int random(int range);
    static bool randomBool();
    static bool randomBoolTrueBiased();
};


#endif /* RANDOM_H_ */
