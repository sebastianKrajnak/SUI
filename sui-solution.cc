#include "search-strategies.h"
#include "memusage.h"
#include <deque>
#include <map>
#include <set>
#include <tuple>
#include <algorithm>

class Node{
	public:
		SearchState state;
		std::shared_ptr<SearchState> parent = nullptr;
		SearchAction action;
};


int memory_threshold = 100 * (1024^2);

std::vector<SearchAction> BreadthFirstSearch::solve(const SearchState &init_state) {
	std::vector<SearchAction> solution;
	std::deque<SearchState> open;
	std::set<SearchState> closed;
	std::map<SearchState, std::tuple<std::shared_ptr<SearchState>, SearchAction> > action_map; 
	//(current search state, (pointer parent state, action from parent to current))

	if (init_state.isFinal())
		return {};

	auto state_ptr = std::make_shared<SearchState>(init_state);
	action_map.emplace(init_state, std::make_tuple(nullptr, init_state.actions()[0]));
	open.push_back(init_state);

	while(!open.empty()){
		if(getCurrentRSS() >= mem_limit_ - memory_threshold){
			std::cout << "Out of memory." << std::endl;
			break;
		}
		SearchState working_state(open.front());
		open.pop_front();
		auto search = closed.find(working_state);
		auto actions = working_state.actions();

		std::cout << "Hi wroking state is: " << working_state << std::endl;

		if (search == closed.end()){	//std::maps end() fn goes through the entire map and returns past-the-end if key is not in map
			closed.insert(working_state);
			if (state_ptr == nullptr)
				state_ptr = std::make_shared<SearchState>(working_state); //creates new pointer that doesnt correspond to the one in action_map in previous cycles

			for( auto &action : actions){
				SearchState new_state(working_state);
				new_state = action.execute(new_state);
				//auto new_ptr = std::make_shared<SearchState>(new_state);
				
				action_map.emplace(new_state, std::make_tuple(state_ptr, action));
				open.push_back(new_state);

				if(new_state.isFinal()){
					int tmp = 1;
					auto map_itr = action_map.find(new_state);
					auto prev_ptr = std::shared_ptr<SearchState>(nullptr);
					auto tmp2 = action_map.find(working_state);

					if (tmp2 == action_map.end()){
						std::cout << "Fuck_tmp" << std::endl;
					}

					while(std::get<0>(map_itr->second) != nullptr && tmp < 10){ //only works once. why?
						std::cout << "Solution steps found: " << tmp << std::endl;
						tmp++;
						solution.push_back(std::get<1>(map_itr->second));
						prev_ptr = std::get<0>(map_itr->second);

						/*if (prev_ptr.get() == std::get<0>(map_itr->second).get())
						{
							std::cout << prev_ptr << " ?= " << std::get<0>(map_itr->second).get() << std::endl;
							std::cout << action_map.size() << std::endl;
							for (auto &t : action_map)
							{
								std::cout << t.first << std::get<0>(t.second).get() << std::endl;
							}
							std::cout << *prev_ptr.get() << std::endl;
							auto tt = action_map.find(*prev_ptr.get());
							std::cout << map_itr->first << "FUCK" << std::endl
									  << tt->first << std::endl;
							break;
						}*/

						map_itr = action_map.find(*prev_ptr.get()); //only god knows if this works... (probably doesnt)
						if (map_itr == action_map.end()){
							std::cout << "Fuck_everything" << std::endl;
							break;
						}
						if (std::get<0>(map_itr->second) == nullptr){
							std::cout << "Fuck_cycle" << std::endl;
						}
						
					}
					std::reverse(solution.begin(), solution.end());
					std::cout << "Solution: ";
					for (long unsigned int i = 0; i < solution.size(); i++)
					std::cout << solution[i] << "," << std::endl;
					
					return solution;
				}
			}
			state_ptr = nullptr;
		}	
	}

	std::cout << "Open_size = " << open.size() << std::endl;
	std::cout << "Closed_size = " << closed.size() << std::endl;

	return {};
}

std::vector<SearchAction> DepthFirstSearch::solve(const SearchState &init_state) {
	return {};
}

double StudentHeuristic::distanceLowerBound(const GameState &state) const {
    return 0;
}

std::vector<SearchAction> AStarSearch::solve(const SearchState &init_state) {
	return {};
}