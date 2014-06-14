/**
Pokemon.cpp
author: Kyle Liu
purpose: defines the Pokemon struct, with a copy function 
**/
#include "stdafx.h" //needed for visual studio 
#include "pokemoncalculator.h"

using namespace std;

//default constructor 
Pokemon::Pokemon() 
	: name(""), ATTACK(0), DEFENSE(0), SPATK(0), SPDEF(0), SPEED(0), TYPE1("no_type"), TYPE2("no_type"), moves_vector(){}

//copy function 
void Pokemon::copy(Pokemon & toCopy) {
	name = toCopy.name;
	ATTACK = toCopy.ATTACK;
	DEFENSE = toCopy.DEFENSE;
	SPATK =	toCopy.SPATK;
	SPDEF = toCopy.SPDEF;
	SPEED = toCopy.SPEED;
	TYPE1 = toCopy.TYPE1;
	TYPE2 = toCopy.TYPE2;
	moves_vector = toCopy.moves_vector;
}