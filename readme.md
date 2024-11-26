# TicTacToe Agent: Reinforcement Learning with Q-Learning & Policy Gradient

This project aims to create an intelligent agent capable of playing Tic-Tac-Toe effectively against a human. The agent is built using reinforcement learning techniques, specifically **Q-Learning** and **Policy Gradient**, to learn optimal strategies over time. The project does not rely on any machine learning libraries, and all matrix operations are implemented from scratch to understand the underlying mechanics.
You can mess around with the different algorithms and their parameters to test out their strengths and (mostly) weaknesses.


## Features

- **Reinforcement Learning Approach**: The agent uses both **Q-Learning** and **Policy Gradient** methods to learn optimal strategies.
- **Matrix Operations from Scratch**: All matrix math is manually implemented for learning purposes, without relying on external libraries such as NumPy.
- **Human vs Agent Gameplay**: A simple interface for humans to play against the agent in a Tic-Tac-Toe game.
- **Continuous Experimentation**: The project is actively testing different parameters to improve agent performance.
- **Minmax algorithm**: Currently a succesful approach to play optimally.
- **qValues learning**: The success of this approach depends greatly on parameters inputed and training time but it is succesful.
- **Saving model** : every model that is trained has a method to save it and every agent can be preloaded with a neural network/qTable.

## Current Status

The agent is still in development, and while progress has been made, convergence has not yet been achieved. Despite efforts, the Policy Gradient approaches still show inconsistencies in the agent's performance. As of now, it doesn't yet play optimally or converge to a consistent strategy. I've tried policyGradient but with two different networks facing each other but this doesn't converge.
The approach is now through a minmax algorith who is effectively bruteforcing it's way to the solution. This could be used in my future code to give reward according to moves. 
Qlearning is a succesful approach but it depends a lot on the parameters so try it out as you will.


## Next Steps

- **Testing Parameters**: The next step involves fine-tuning the learning rates, exploration strategies, and other hyperparameters to try and achieve convergence.
- **Optimizing Matrix Operations**: Currently, matrix operations are implemented manually. There is a plan to rewrite the matrix math using **Eigen** for more efficient and optimized computation.
- **Future Extensions**: In the future, this project aims to implement other games, such as **Connect 4**, using the same reinforcement learning techniques.

## Installation

Clone the repository to your local machine:

```bash
git clone https://github.com/yourusername/tictactoe-agent.git
