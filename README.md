# 🐍 Retro Snake Game

## Overview
This is a **classic Snake game** developed in **C++** using **Raylib**.  
Control the snake, eat food to grow, and avoid walls or self-collision to keep playing.

---

## Features
- Grid-based snake movement  
- Snake grows only when eating food  
- Randomly generated food that never spawns on the snake  
- Collision detection with walls and self  
- Score tracking and high score saved in a file  
- Pause and resume functionality  
- Sound effects for eating and game over  

---

## Controls
- **Arrow Keys** → Move the snake (Up, Down, Left, Right)  
- **Space** → Pause / Resume the game  

---

## Game Logic
1. **Snake Movement**: Adds a new head in the current direction; removes tail unless it ate food.  
2. **Growth on Eating**: Snake grows by not removing tail when food is eaten.  
3. **Food Generation**: Randomly placed outside the snake body.  
4. **Collision Detection**: Ends game on wall or self-collision.  
5. **Game Loop**: Continuously checks input, updates game state, and renders until closed.  

---

## Technologies Used
- **C++**  
- **Raylib** (Graphics & Audio)  
- **Raymath** (Vector operations)  

---

## Project Members
- Abdullah Aziz  
- Shahyan Haider  

---

## How to Run
1. Install **Raylib** and its dependencies.  
2. Compile the project with your preferred compiler.  
3. Run the executable to play the game.  

---

## License
This project is **free to use** for learning and personal purposes.
