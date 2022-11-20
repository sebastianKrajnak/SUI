#!/bin/bash

make

echo "Testing student heuristic"
./fc-sui 5 15 --easy-mode 25 --solver a_star --heuristic student
echo "Testing default heuristic"
./fc-sui 5 15 --easy-mode 25 --solver a_star --heuristic nb_not_home