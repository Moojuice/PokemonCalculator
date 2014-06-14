/**
Moves.cpp
author: Kyle Liu
purpose: defines the moves struct, with a copy function 
**/
#include "stdafx.h" //needed for visual studio 
#include "pokemoncalculator.h"

using namespace std;

//default constructor 
Move::Move()
	: name(""), POWER(0), PHYSICAL_SPECIAL("no_dmg_type"),TYPE("no_type"){}

//takes in another move in the parameter and copies its stats 
void Move::copy(Move& toCopy) {
	name = toCopy.name;
	POWER = toCopy.POWER;
	PHYSICAL_SPECIAL = toCopy.PHYSICAL_SPECIAL;
	TYPE = toCopy.TYPE;
}