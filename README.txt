Wumplus World Instructions:

Alec West

Assumptions:
    *Disclaimer - These may not all necessarily be assumptions, I'm just including them to cover my bases
    1. pit, wumpus, gold, supmuw, and blockade cannot be placed in lower right 2x2, to prevent impossible game or too-easy game
    2. Blockade placement trumps everything and deletes all other content in the room.
    3. Bumps count as moves taken
    4. Arrow hits SUPMUW before WUMPUS if they're in the same room
    5. Only one arrow
    6. Only one food exists and it exists only when SUPMUW is friendly and out of a pit
    7. Only one SUPMUW and one WUMPUS
    8. Pit cannot be placed in the same location as a Wumpus
    9. Wumpus addition trumps pit addition

Citations:
    Credit for most of the file structure / world appearance goes to this github repo:
        https://github.com/YansongZang/wumpus-world.git
    UML planning and early construction of my world was organized through:
	https://repository.genmymodel.com/alecwest38/wumpus-world-cpp

Input File:
    main.cpp is set to read a text file "in.txt" from the home directory of the program.
    The input file is formatted to read one word or number per line
    1. Start with the grid size
    2. For each obstacle, put the word in the first line ("wumpus", "supmuw", "pit", "blockade", "gold"), followed by the x coordinates, then they y coordinates, each on a separate line
    3. The coordinates are zero-based. (If grid size is 10, x and y range is 0-9)

Modifications:
    If you prefer to minimize the output or walk through it all step-by-step, I've included a setPrintFrequency method that runs on four different settings
    0. Print the full board and the Agent's final board
    1. Print the full board, the Agent's intermediate steps in text, and the Agent's final board
    2. Print the full board, the board for each intermediate step, and the Agent's final board
    3. Same as 2, but with a press enter to continue between each step.

Running:
    Four scripts are included, buildrun.sh, build.sh, run.sh, and clean.sh, and each does exactly what it says.
    You should be able to just call "sh run.sh" and have the program run, but if you wish to modify main.cpp, be sure to run "sh buildrun.sh" after making any changes (i.e. switching to random terrain, or changing the print frequency)
