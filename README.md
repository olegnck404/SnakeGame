# Snake Game

This is a simple console-based Snake game implemented in C++. The game is built using the ncurses library for terminal graphics and input handling.

## Prerequisites

Before compiling and running the game, make sure you have the following packages installed:

- ncurses

- g++

- make

## Compilation

To compile the game, follow these steps:

1. Open a terminal window.

2. Navigate to the directory containing the source code file (`SnakeGame.cpp`).

3. Run the following command to compile the game:

   ```bash

   g++ -o snake SnakeGame.cpp -lncurses

   ```

   This command will generate an executable file named `SnakeGame`.

## Execution

To run the game, execute the following command in the terminal:

```bash

./snake

```

## Instructions

- Use the arrow keys to control the snake's movement (up, down, left, right).

- Avoid running into the walls or the snake's own body.

- Eat the apples to grow longer and increase your score.

- Press 'q' to quit the game.

## Acknowledgements

This Snake game implementation was created as a learning exercise and is based on the classic Snake game concept.

Enjoy playing the game!
