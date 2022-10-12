#!/bin/bash

make
echo "Testing bfs"
./fc-sui 3 420 --easy-mode 25 --solver bfs 
echo "Testing dfs no limit"
./fc-sui 3 420 --easy-mode 25 --solver dfs 
echo "Testing student heuristic"
./fc-sui 5 15 --easy-mode 25 --solver a_star --heuristic student
echo "Testing default heuristic"
./fc-sui 5 15 --easy-mode 25 --solver a_star --heuristic nb_not_home