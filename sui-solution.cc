#include "search-strategies.h"
#include "memusage.h"
#include <deque>
#include <map>
#include <set>
#include <tuple>
#include <algorithm>

int memory_threshold = 100 * (1024^2);

std::vector<SearchAction> BreadthFirstSearch::solve(const SearchState &init_state) {
	std::vector<SearchAction> solution;
	std::deque<SearchState> open;
	std::set<SearchState> closed;
	std::map< std::shared_ptr<SearchState>, std::tuple<std::shared_ptr<SearchState>, SearchAction> > action_map;

	if (init_state.isFinal())
		return {};

	auto tmp_ptr = std::make_shared<SearchState>(init_state);
	action_map.emplace(tmp_ptr, std::make_tuple(tmp_ptr, init_state.actions()[0]));
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
			if (tmp_ptr == nullptr)
				tmp_ptr = std::make_shared<SearchState>(working_state);

			for( auto &action : actions){
				SearchState new_state(working_state);
				new_state = action.execute(new_state);
				auto new_ptr = std::make_shared<SearchState>(new_state);
				
				action_map.emplace(new_ptr, std::make_tuple(tmp_ptr, action));

				open.push_back(new_state);

				if(new_state.isFinal()){
					int tmp = 1;
					auto map_itr = action_map.find(new_ptr);
					auto prev_ptr = std::shared_ptr<SearchState>(nullptr);
					auto tmp2 = action_map.find(tmp_ptr);

					if (tmp2 == action_map.end()){
						std::cout << "Fuck_tmp" << std::endl;
					}

					while(std::get<0>(map_itr->second) != prev_ptr){ //only works once. why?
						std::cout << "Solution steps found: " << tmp << std::endl;
						tmp++;
						solution.push_back(std::get<1>(map_itr->second));
						prev_ptr = std::get<0>(map_itr->second);
						map_itr = action_map.find(new_ptr); //only god knows if this works... (probably doesnt)
						if (map_itr == action_map.end()){
							std::cout << "Fuck_everything" << std::endl;
						}
						if (std::get<0>(map_itr->second) == prev_ptr){
							std::cout << "Fuck_cycle: " << prev_ptr << std::endl;
						}
					}
					std::reverse(solution.begin(), solution.end());
					std::cout << "Solution: ";
					for (long unsigned int i = 0; i < solution.size(); i++)
					std::cout << solution[i] << "," << std::endl;
					
					return solution;
				}
			}
			tmp_ptr = nullptr;
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