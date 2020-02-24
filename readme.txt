***	Mustafa Taner - 335 CB - Homework 1 EGC	***

***	FlooFloo Bird	***

==	Instructions	==
 - <R> = reset / restart the game;
 - <P> = pause / unpause the game;
 - <SPACE> = make the bird jump;
 - <Z> = restart the game with one less pipe per screen;
 - <X> = restart the game with one more pipe per screen.


==	Implementation	==
 - the bird:
	- initial position in the first quarter of the screen, middle height;
	- jumps with inertion;
	- falls with gravitation;
	(physics)
	- its wing makes floo-floo continuosly.
 - the pipes:
	- moving continuosly (there is a set number of pipes continuosly rendered
	  to make it seem like there is an infinite number of them);
	- colored different randomly in each level;
	- random number of pipes scale continuosly;
	- random length and space between upper and lower pipes (with rule to have
	  enough space for the bird);
	- with each passed level, the minimum possible space between upper and lower pipes
	  decreases, down to a minimum which permits the bird to pass;
	- the space between 2 different pipes on the same level randomly varies.
 - the clouds:
	- moving continuosly (there are 3 clouds continuosly rendered to make it
	  seem like there is an infinite number of them).
 - the sun:
	- moves very slow.

 - the gameplay:
	- level: - the level increases each 10 seconds elapsed in the game;
		 - resets when the game is reset.
	- score: - continuosly increases with the elapsed time in the game;
		 - with each passed level, the score increases with a bigger bonus;
		 - resets when the game is reset.
	- speed: - the initial speed increases level by level;
		 - resets when the game is reset.
	- space: - minimum space between pipes decreases level by level;
		 - it stops decreasing when the space is the minimum possible which
		   permits the bird to jump;
		 - resets when the game is reset.
	- finish:- when the game is over, the whole screen flickers;
		 - the player can reset the game and keep playing from level 1.