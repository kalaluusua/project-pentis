# Pentis

This tile-matching project is a classic Tetris with a possibility to play either Tetris or Pentis. Tetris is a well known tile-matching puzzle video game where the players objective is to guide pieces falling from the upper edge of the screen and try to keep the construction as low as possible. When a row is complete with tiles, it disappears.
The game becomes more difficult as it progresses, and ends when the construction reaches the top of the screen. The pieces follow arrow-keys and can be dropped instantly by pressing 'space'.

The game rewards the player basic points for successfully landing a block, one point for each square the block contains. Elimination points are awarded based on the size of the elimination — the more rows eliminated simultaneously, the better. Points are also awarded for every instant drop, according to the height the tile is dropped from.

If the player reached top-10 in the leaderboard, the player has the option to submit his/hers high score and view leaderboard standings. This information is stored locally in a text file. The speed of the game increases specifically as a function of the amount of eliminations and not of the amount of eliminated rows. If the player didn't reach top-10, the current top-10 leaderboard is shown.

Pentis-mode of play introduces 1, 2, 3, and 5-tile-size pieces into the game. The drop-rate of these special blocks is also a function of the amount of eliminations. Pentis is considered more challenging mode of play. Tetris is faster that Pentis, but only has 7 different blocks. Pentis has 29 different pieces overall. The point-award system and game mechanics are identical to Tetris. However, the speed-up on Pentis is slower since it is already more challenging. A separate top-10 leaderboard is allocated for the Pentis game-mode.

## Instructions for building and using

These instructions will get the project up and running on your local machine.

### Software prerequisites

Pentis is designed to be run on a Linux operating system.

### Compiling the program

Following programs and libraries are required to compile the project:
```
• make (4.1-9.1ubuntu1)
• g++ (4:7.3.0-3ubuntu2)
• libsfml-dev (2.4.2+dfsg-4)
```
To compile the program, make sure you have the provided directories from project git.
   1. On terminal go to the project directory (Directory should contain directories: ```doc```, ```plan```, ```res```, ```src``` and files ```Makefile```, ```README.md```.
   2. In the directory, use the following command: 
    ``` make ```
   3. To run the program, use the following command: 
    ```./main```
   4. After the compiled files are no longer needed, you can delete them using command:
    ```make clean```

## Authors

- Software architecture and UI design: [Rustam Latypov](mailto:rustam.latypov@aalto.fi)
- OS compatibility and UI design: [Visa Lintunen](mailto:visa.lintunen@aalto.fi)
- Game mechanics and [SRS](https://tetris.wiki/SRS) integrity: [Kalle Alaluusua](mailto:kalle.alaluusua@aalto.fi)
- Game mechanics and UX design: [Juho Heimonen](mailto:juho.heimonen@aalto.fi)

