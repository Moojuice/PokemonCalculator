Pokemon Calculator v1.0 
Kyle Liu
Last Updated 3/10/14

Purpose: 
Given your team's pokemon, their movesets, and the enemy pokemon name you are facing, this will calculate the best move according effectiveness and relative 
damage it will do to the enemy pokemon. Works up to gen 6.

Instructions: 
In the folder where your .exe is being built (typically "Debug"), insert a .txt doc of your team. 
Structure of your team should be in the format: <pokemon> <move1> <move2> <move3> <move4>
					for example: scizor metal_claw superpower aerial_ace swords_dance
In the command line, a typical input could be "pokemoncalculator.exe team.txt". 

Each pokemon should be on a different line. Names of moves/pokemon need to be connected with underscores "_", such as Mr._Mime or Mime_Jr. 
Mega Evolutions aren't considered in this version so far.

Specfic Pokemon's names -- 
Deoxys, Deoxys-A, Deoxys-D, Deoxys-S
Wormadam-P, Wormadam-T, Wormadam-S
Rotom, Rotom-W, Rotom-Fr, Rotom-Fa, Rotom-H, Rotom-M
Giratina, Giratina-O
Shaymin, Shaymin-S
Darmanitan, Darmanitan-Z
Tornadus, Tornadus-T
Thundurus, Thundurus-T
Landorus, Landorus-T
Kyurem, Kyurem-B, Kyurem-W
Meloetta, Meloetta-P
Aegislash-S, Aegislash-B
Pumpkaboo, Pumpkaboo-Sm, Pumpkaboo-Su, Pumpkaboo-L
Gourgeist, Gourgeist-Sm, Gourgeist-Su, Gourgeist-L

General information: 
As of now, mega evolutions and abilities aren't taken into consideration. So if the enemy has water absorb or levitate, it's not accounted for. 
In addition, status moves like toxic or thunder wave, and stat boosting/lowering moves like swords dance and nasty plot aren't considered. 
Levels, EVs, IVs not taken into consideration.

If you haven't noticed, this is for simplistic PvE usage, where you can just keep this running and type in the enemy pokemon for an easy way to 
tell what to do next. 

To generate "final.txt" and "moves.txt", I wrote a separate program which parsed the information from bulbapedia. Moves was a simple, one step
parse, but generating final.txt involved parsing pokemon by base stats, then pokemon by type, and then combining the text docs together. 

Was written in Visual Studio 2012. 

Couple things to note:
The coding is not my best work, I was working on this for fun, so the style is not great. Things that could be improved: I don't use constants for 
reused terms, so a lot of things are hard-encoded (though this is a quick fix). In addition, 

References:
Serebii.net was used for the damage calculation formula. 
Bulbapedia was used for finding the complete list of pokemon and moves.  