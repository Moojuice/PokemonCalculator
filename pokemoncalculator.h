/**
PokemonCalculator.h
author: Kyle Liu
purpose: defines the structs and classes used to calculate the most effective move.
**/
#ifndef POKEMONCALCULATOR_H
#define POKEMONCALCULATOR_H

#include <vector>
#include <string>
#include <map>
#include <fstream> 

enum return_values{SUCCESS, BAD_ARGS, BAD_FILE_NAME, UNEXPECTED_ERROR, PKMN_FILE_ERROR, MOVES_FILE_ERROR, PKMN_DATABASE_ERROR, MOVES_DATABASE_ERROR, TEAM_PKMN_ERROR, TEAM_MOVES_ERROR};

//function we use for case-sensitivity removal 
std::string toLowerCase(std::string &);

//our move struct
struct Move {
	Move();
	void copy (Move&);
	std::string name;
	unsigned POWER;
	std::string PHYSICAL_SPECIAL;
	std::string TYPE;
};

//our pokemon struct
struct Pokemon {
	Pokemon(); // default constructor 
	void copy (Pokemon&);
	std::string name;
	unsigned ATTACK;
	unsigned DEFENSE;
	unsigned SPATK;
	unsigned SPDEF;
	unsigned SPEED;
	std::string TYPE1;
	std::string TYPE2;
	std::vector<Move> moves_vector;
};

//our calculator class, which defines most of our functions 
class Calculator {
public:
	Calculator();
	int implement(std::ifstream &);
	int calculate();
	bool done();
	bool stalemate();
	int prompt();
	int turn();
	int play();
private:
	double weakness_resistance(std::string, std::string, std::string);
	std::vector<Pokemon> team_vector; 
	Pokemon enemy; 
	std::map<std::string,Pokemon> pokemon_data_base;
	std::map<std::string,Move> moves_data_base; 
};

#endif