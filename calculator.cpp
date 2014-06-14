/**
Calculator.cpp
author: Kyle Liu
purpose: Defines the calculator class with its respective functions, given an enemy pokemon name, iterates through your team to generate the 
		 optimal move. 
**/
#include "stdafx.h" //needed for visual studio 
#include "pokemoncalculator.h"
#include <iostream> 
#include <sstream>

using namespace std;

//default calculator 
Calculator::Calculator() 
	: team_vector(), enemy(), pokemon_data_base(), moves_data_base(){}

//establishes pokemon data base/move data base and the vector for the player's team 
int Calculator::implement(ifstream &ifs) {
	ifstream pokemon_stream; 
	pokemon_stream.open("final.txt"); //our pokemon database
	if (pokemon_stream.is_open()) {
		ifstream moves_stream;
		moves_stream.open("moves.txt"); //our move database 
			if (moves_stream.is_open()) {
				string line; 
				while (getline(pokemon_stream, line)) { //read each line from the pokemon stream 
					Pokemon pokemon_to_add;
					istringstream iss(line);
					if (iss >> pokemon_to_add.name && iss >> pokemon_to_add.HP && iss >> pokemon_to_add.ATTACK && iss >> pokemon_to_add.DEFENSE && iss >> pokemon_to_add.SPATK && iss>> pokemon_to_add.SPDEF && iss>> pokemon_to_add.TYPE1) { //attempts to read each part of the line into a new Pokemon
						iss>> pokemon_to_add.TYPE2; //attempts to add second type if it exists 
						pokemon_data_base.insert(make_pair(toLowerCase(pokemon_to_add.name), pokemon_to_add)); //insert a new pair into the database <string, Pokemon>
					}
					else {
						cout << "Error: could not successfully map all pokemon" << endl;
						return PKMN_DATABASE_ERROR;
					}
				}
				while (getline(moves_stream, line)) { //read each line from move stream 
					Move move_to_add;
					istringstream iss(line);
					if (iss >> move_to_add.name && iss >> move_to_add.TYPE && iss >> move_to_add.PHYSICAL_SPECIAL && iss >> move_to_add.POWER) { //attempts to read each string from the line into a new move
						moves_data_base.insert(make_pair(toLowerCase(move_to_add.name), move_to_add)); //insert a new pair into the database <string, move>
					}
					else {
						cout << "Error: could not successfully map all moves" << endl;
						return MOVES_DATABASE_ERROR;
					}
				}
				while (getline(ifs, line)) { //read from the input .txt to generate the player's team 
					Pokemon team_pokemon;
					istringstream iss(line);
					iss >> team_pokemon.name;
					if (pokemon_data_base.find(toLowerCase(team_pokemon.name)) != pokemon_data_base.end()) { //first searches for pokemon in the database
						team_pokemon.copy(pokemon_data_base.find(toLowerCase(team_pokemon.name)) -> second); //copies stats 
					}
					else { 
						cout << "Error: could not find pokemon " << team_pokemon.name << " in database" << endl;
						return TEAM_PKMN_ERROR;
					}
					Move team_move;
					while (iss >> team_move.name) {
						if (moves_data_base.find(toLowerCase(team_move.name)) != moves_data_base.end()) { //searches for moves in database
							team_move.copy(moves_data_base.find(toLowerCase(team_move.name)) -> second); //copies moves/stats
							team_pokemon.moves_vector.push_back(team_move); //adds move to the pokemon's moveset vector
						}
						else {
							cout << "Error: could not find move " << team_move.name << " in database" << endl;
							return TEAM_MOVES_ERROR;
						}
					}
					team_vector.push_back(team_pokemon); //finally adds the pokemon to the team vector 
				}
				return SUCCESS; //after iterating through entire team txt file, we are finished 
			}
			else { //if we couldn't open the moves file
				cout << "Error: could not open move database txt file, \"move.txt\", check it is visible" << endl;
				return MOVES_FILE_ERROR;
			}
	}
	else { //if we couldn't open the pokemon file 
		cout << "Error: could not open pokemon database txt file, \"final.txt\", check it is visible" << endl;
		return PKMN_FILE_ERROR; 
	}
}

//calculates the best move using your team's stats, their moves, and the enemy stats
int Calculator::calculate() {
	string input; Pokemon enemy; Pokemon current_pokemon; Move current_move; string best_move = ""; string best_pokemon = ""; double damage; //set our variables
	while (true) { //repeatedly prompt user until they type in quit
		double best_damage = 0;
		cout << "Enter name of enemy pokemon or \"quit\"" << endl;
		getline(cin, input);
		input = toLowerCase(input);
		if (input == "quit") {
			return SUCCESS;
		}
		enemy.name = input;
		if (pokemon_data_base.count(toLowerCase(enemy.name)) == 1) { //search for enemy pokemon in database, and copy its stats 
			enemy = pokemon_data_base.find(toLowerCase(enemy.name))->second;
			for (unsigned i = 0; i < team_vector.size(); ++i) { //iterate through the team's pokemons
				current_pokemon = team_vector[i];
				//OPTIONAL PRINTOUT: uncomment the following line to see your pokemon's name/type and attack/spatk
				//cout << "pokemon: " << current_pokemon.name << " " << current_pokemon.TYPE1 << " " << current_pokemon.TYPE2 << " " << current_pokemon.ATTACK << " " <<current_pokemon.SPATK << endl;
				for (unsigned j = 0; j < current_pokemon.moves_vector.size(); ++j) { //iterate through the pokemon's moves 
					damage = 0;
					current_move = current_pokemon.moves_vector[j]; 
					//OPTIONAL: uncomment the following line to see the move's name/type and power
					//cout << "move: " << current_move.name << " " << current_move.POWER << " " << current_move.PHYSICAL_SPECIAL << " " << current_move.TYPE << endl;
					//Damage formula = ((((2 * Level / 5 + 2) * AttackStat * AttackPower / DefenseStat) / 50) + 2) * STAB * Weakness/Resistance * RandomNumber / 100
					if (toLowerCase(current_move.PHYSICAL_SPECIAL) == "physical") { //if the move is physical, we use attack/def stats
						damage = ((((2 * 100/5 + 2) * current_pokemon.ATTACK * (1.0*current_move.POWER)/enemy.DEFENSE)/50.0)+2);
					}
					if (toLowerCase(current_move.PHYSICAL_SPECIAL) == "special") { //if the move is special, we use spatk/spdef stats 
						damage = ((((2 * 100/5 + 2) * current_pokemon.SPATK * (1.0*current_move.POWER)/enemy.SPDEF)/50.0)+2);					
					}
					if (current_pokemon.TYPE1 == current_move.TYPE || current_pokemon.TYPE2 == current_move.TYPE) { //STAB calculation
						damage *= 1.5;
					}
					damage *= weakness_resistance(toLowerCase(current_move.TYPE), toLowerCase(enemy.TYPE1), toLowerCase(enemy.TYPE2)); //calculates weakness/resistance 
					if (damage > best_damage) { //updates based on if highest damage 
						best_damage = damage;
						best_move = current_move.name;
						best_pokemon = current_pokemon.name;
					}
				}
			}
			if (best_pokemon != "") {
				cout << "Best Choice: " << best_pokemon << " " << best_move << endl;
			}
			else {
				cout << "Couldn't find a good move to do!" << endl;
			}
		}
		else { //if we couldn't find the enemy in the database 
			cout << "Could not find enemy name, please check spelling" << endl;
		}
	}
	return UNEXPECTED_ERROR; //no logical way to reach this 
}

//our weakness/resistance calculator, pretty self-explanatory 
double Calculator::weakness_resistance(string move_type, string enemy_type1, string enemy_type2) {
	double result = 1;
	if (move_type == "normal") {
		if (enemy_type1 == "rock" || enemy_type2 == "rock") {
			result *= .5;
		}
		if (enemy_type1 == "steel" || enemy_type2 == "steel") {
			result *= .5;
		}
		if (enemy_type1 == "ghost" || enemy_type2 == "ghost") {
			result *= 0;
		}
	}
	if (move_type == "fighting") {
		if (enemy_type1 == "normal" || enemy_type2 == "normal") {
			result *= 2;
		}
		if (enemy_type1 == "flying" || enemy_type2 == "flying") {
			result *= .5;
		}
		if (enemy_type1 == "poison" || enemy_type2 == "poison") {
			result *= .5;
		}
		if (enemy_type1 == "rock" || enemy_type2 == "rock") {
			result *= 2;
		}
		if (enemy_type1 == "bug" || enemy_type2 == "bug") {
			result *= .5;
		}
		if (enemy_type1 == "ghost" || enemy_type2 == "ghost") {
			result *= 0;
		}
		if (enemy_type1 == "steel" || enemy_type2 == "steel") {
			result *= 2;
		}
		if (enemy_type1 == "psychic" || enemy_type2 == "psychic") {
			result *= .5;
		}
		if (enemy_type1 == "ice" || enemy_type2 == "ice") {
			result *= 2;
		}
		if (enemy_type1 == "dark" || enemy_type2 == "dark") {
			result *= 2;
		}
		if (enemy_type1 == "fairy" || enemy_type2 == "fairy") {
			result *= .5;
		}
	}
	if (move_type == "flying") {
		if (enemy_type1 == "fighting" || enemy_type2 == "fighting") {
			result *= 2;
		}
		if (enemy_type1 == "rock" || enemy_type2 == "rock") {
			result *= .5;
		}
		if (enemy_type1 == "bug" || enemy_type2 == "bug") {
			result *= 2;
		}
		if (enemy_type1 == "steel" || enemy_type2 == "steel") {
			result *= .5;
		}
		if (enemy_type1 == "grass" || enemy_type2 == "grass") {
			result *= 2;
		}
		if (enemy_type1 == "electric" || enemy_type2 == "electric") {
			result *= .5;
		}
	}
	if (move_type == "poison") {
		if (enemy_type1 == "poison" || enemy_type2 == "poison") {
			result *= .5;
		}
		if (enemy_type1 == "ground" || enemy_type2 == "ground") {
			result *= .5;
		}
		if (enemy_type1 == "rock" || enemy_type2 == "rock") {
			result *= .5;
		}
		if (enemy_type1 == "ghost" || enemy_type2 == "ghost") {
			result *= .5;
		}
		if (enemy_type1 == "steel" || enemy_type2 == "steel") {
			result *= 0;
		}
		if (enemy_type1 == "grass" || enemy_type2 == "grass") {
			result *= 2;
		}
		if (enemy_type1 == "fairy" || enemy_type2 == "fairy") {
			result *= 2;
		}
	}
	if (move_type == "ground") {
		if (enemy_type1 == "flying" || enemy_type2 == "flying") {
			result *= 0;
		}
		if (enemy_type1 == "poison" || enemy_type2 == "poison") {
			result *= 2;
		}
		if (enemy_type1 == "rock" || enemy_type2 == "rock") {
			result *= 2;
		}
		if (enemy_type1 == "bug" || enemy_type2 == "bug") {
			result *= .5;
		}
		if (enemy_type1 == "steel" || enemy_type2 == "steel") {
			result *= 2;
		}
		if (enemy_type1 == "fire" || enemy_type2 == "fire") {
			result *= 2;
		}
		if (enemy_type1 == "grass" || enemy_type2 == "grass") {
			result *= .5;
		}
		if (enemy_type1 == "electric" || enemy_type2 == "electric") {
			result *= 2;
		}
	}
	if (move_type == "rock") {
		if (enemy_type1 == "fighting" || enemy_type2 == "fighting") {
			result *=.5;
		}
		if (enemy_type1 == "flying" || enemy_type2 == "flying") {
			result *=2;
		}
		if (enemy_type1 == "ground" || enemy_type2 == "ground") {
			result *=.5;
		}
		if (enemy_type1 == "bug" || enemy_type2 == "bug") {
			result *=2;
		}
		if (enemy_type1 == "steel" || enemy_type2 == "steel") {
			result *=.5;
		}
		if (enemy_type1 == "fire" || enemy_type2 == "fire") {
			result *=2;
		}
		if (enemy_type1 == "ice" || enemy_type2 == "ice") {
			result *=2;
		}
	}
	if (move_type == "bug") {
		if (enemy_type1 == "fighting" || enemy_type2 == "fighting") {
			result *=.5;
		}
		if (enemy_type1 == "flying" || enemy_type2 == "flying") {
			result *=.5;
		}
		if (enemy_type1 == "poison" || enemy_type2 == "poison") {
			result *=.5;
		}
		if (enemy_type1 == "ghost" || enemy_type2 == "ghost") {
			result *=.5;
		}
		if (enemy_type1 == "steel" || enemy_type2 == "steel") {
			result *=.5;
		}
		if (enemy_type1 == "fire" || enemy_type2 == "fire") {
			result *=.5;
		}
		if (enemy_type1 == "grass" || enemy_type2 == "grass") {
			result *=2;
		}
		if (enemy_type1 == "psychic" || enemy_type2 == "psychic") {
			result *=2;
		}
		if (enemy_type1 == "dark" || enemy_type2 == "dark") {
			result *=2;
		}
		if (enemy_type1 == "fairy" || enemy_type2 == "fairy") {
			result *=2;
		}
	}
	if (move_type == "ghost") {
		if (enemy_type1 == "normal" || enemy_type2 == "normal") {
			result *=0;
		}
		if (enemy_type1 == "ghost" || enemy_type2 == "ghost") {
			result *=2;
		}
		if (enemy_type1 == "psychic" || enemy_type2 == "psychic") {
			result *=2;
		}
		if (enemy_type1 == "dark" || enemy_type2 == "dark") {
			result *=.5;
		}
	}
	if (move_type == "steel") {
		if (enemy_type1 == "rock" || enemy_type2 == "rock") {
			result *=2;
		}
		if (enemy_type1 == "steel" || enemy_type2 == "steel") {
			result *=.5;
		}
		if (enemy_type1 == "fire" || enemy_type2 == "fire") {
			result *=.5;
		}
		if (enemy_type1 == "water" || enemy_type2 == "water") {
			result *=.5;
		}
		if (enemy_type1 == "electric" || enemy_type2 == "electric") {
			result *=.5;
		}
		if (enemy_type1 == "ice" || enemy_type2 == "ice") {
			result *=2;
		}
		if (enemy_type1 == "fairy" || enemy_type2 == "fairy") {
			result *=2;
		}
	}
	if (move_type == "fire") {
		if (enemy_type1 == "rock" || enemy_type2 == "rock") {
			result *=.5;
		}
		if (enemy_type1 == "bug" || enemy_type2 == "bug") {
			result *=2;
		}
		if (enemy_type1 == "steel" || enemy_type2 == "steel") {
			result *=2;
		}
		if (enemy_type1 == "fire" || enemy_type2 == "fire") {
			result *=.5;
		}
		if (enemy_type1 == "water" || enemy_type2 == "water") {
			result *=.5;
		}
		if (enemy_type1 == "grass" || enemy_type2 == "grass") {
			result *=2;
		}
		if (enemy_type1 == "ice" || enemy_type2 == "ice") {
			result *=2;
		}
		if (enemy_type1 == "dragon" || enemy_type2 == "dragon") {
			result *=.5;
		}
	}
	if (move_type == "water") {
		if (enemy_type1 == "ground" || enemy_type2 == "ground") {
			result *=2;
		}
		if (enemy_type1 == "rock" || enemy_type2 == "rock") {
			result *=2;
		}
		if (enemy_type1 == "fire" || enemy_type2 == "fire") {
			result *=.5;
		}
		if (enemy_type1 == "water" || enemy_type2 == "water") {
			result *=.5;
		}
		if (enemy_type1 == "grass" || enemy_type2 == "grass") {
			result *=.5;
		}
		if (enemy_type1 == "dragon" || enemy_type2 == "dragon") {
			result *=.5;
		}
	}
	if (move_type == "grass") {
		if (enemy_type1 == "flying" || enemy_type2 == "flying") {
			result *=.5;
		}
		if (enemy_type1 == "poison" || enemy_type2 == "poison") {
			result *=.5;
		}
		if (enemy_type1 == "ground" || enemy_type2 == "ground") {
			result *=2;
		}
		if (enemy_type1 == "rock" || enemy_type2 == "rock") {
			result *=2;
		}
		if (enemy_type1 == "bug" || enemy_type2 == "bug") {
			result *=.5;
		}
		if (enemy_type1 == "steel" || enemy_type2 == "steel") {
			result *=.5;
		}
		if (enemy_type1 == "fire" || enemy_type2 == "fire") {
			result *=.5;
		}
		if (enemy_type1 == "water" || enemy_type2 == "water") {
			result *=2;
		}
		if (enemy_type1 == "grass" || enemy_type2 == "grass") {
			result *=.5;
		}
		if (enemy_type1 == "dragon" || enemy_type2 == "dragon") {
			result *=.5;
		}
	}
	if (move_type == "electric") {
		if (enemy_type1 == "flying" || enemy_type2 == "flying") {
			result *=2;
		}
		if (enemy_type1 == "ground" || enemy_type2 == "ground") {
			result *=0;
		}
		if (enemy_type1 == "water" || enemy_type2 == "water") {
			result *=2;
		}
		if (enemy_type1 == "grass" || enemy_type2 == "grass") {
			result *=.5;
		}
		if (enemy_type1 == "electric" || enemy_type2 == "electric") {
			result *=.5;
		}
		if (enemy_type1 == "dragon" || enemy_type2 == "dragon") {
			result *=.5;
		}
	}
	if (move_type == "psychic") {
		if (enemy_type1 == "fighting" || enemy_type2 == "fighting") {
			result *=2;
		}
		if (enemy_type1 == "poison" || enemy_type2 == "poison") {
			result *=.5;
		}
		if (enemy_type1 == "steel" || enemy_type2 == "steel") {
			result *=.5;
		}
		if (enemy_type1 == "psychic" || enemy_type2 == "psychic") {
			result *=.5;
		}
		if (enemy_type1 == "dark" || enemy_type2 == "dark") {
			result *=0;
		}
	}
	if (move_type == "ice") {
		if (enemy_type1 == "flying" || enemy_type2 == "flying") {
			result *=2;
		}
		if (enemy_type1 == "ground" || enemy_type2 == "ground") {
			result *=2;
		}
		if (enemy_type1 == "steel" || enemy_type2 == "steel") {
			result *=.5;
		}
		if (enemy_type1 == "fire" || enemy_type2 == "fire") {
			result *=.5;
		}
		if (enemy_type1 == "water" || enemy_type2 == "water") {
			result *=.5;
		}
		if (enemy_type1 == "grass" || enemy_type2 == "grass") {
			result *=2;
		}
		if (enemy_type1 == "ice" || enemy_type2 == "ice") {
			result *=.5;
		}
		if (enemy_type1 == "dragon" || enemy_type2 == "dragon") {
			result *=2;
		}
	}
	if (move_type == "dragon") {
		if (enemy_type1 == "steel" || enemy_type2 == "steel") {
			result *=.5;
		}
		if (enemy_type1 == "dragon" || enemy_type2 == "dragon") {
			result *=2;
		}
		if (enemy_type1 == "fairy" || enemy_type2 == "fairy") {
			result *=0;
		}
	}
	if (move_type == "dark") {
		if (enemy_type1 == "fighting" || enemy_type2 == "fighting") {
			result *=.5;
		}
		if (enemy_type1 == "ghost" || enemy_type2 == "ghost") {
			result *=2;
		}
		if (enemy_type1 == "psychic" || enemy_type2 == "psychic") {
			result *=2;
		}
		if (enemy_type1 == "dark" || enemy_type2 == "dark") {
			result *=.5;
		}
		if (enemy_type1 == "fairy" || enemy_type2 == "fairy") {
			result *=.5;
		}
	}
	if (move_type == "fairy") {
		if (enemy_type1 == "fighting" || enemy_type2 == "fighting") {
			result *=2;
		}
		if (enemy_type1 == "poison" || enemy_type2 == "poison") {
			result *=.5;
		}
		if (enemy_type1 == "steel" || enemy_type2 == "steel") {
			result *=.5;
		}
		if (enemy_type1 == "fire" || enemy_type2 == "fire") {
			result *=.5;
		}
		if (enemy_type1 == "dragon" || enemy_type2 == "dragon") {
			result *=2;
		}
		if (enemy_type1 == "dark" || enemy_type2 == "dark") {
			result *=2;
		}
	}
	return result;
}