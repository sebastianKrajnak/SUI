#include "search-strategies.h"
#include "memusage.h"
#include <deque>
#include <map>
#include <set>
#include <tuple>

int memory_threshold = 100 * (1024^2);

std::vector<SearchAction> BreadthFirstSearch::solve(const SearchState &init_state) {
	//bool solved = false;
	std::vector<SearchAction> solution;
	std::deque<SearchState> open;
	std::map< SearchState, std::tuple<std::shared_ptr<SearchState>, SearchAction> > closed;

	if (init_state.isFinal())
		return {};

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
		//auto action = actions[0]; //not used?
		if (search == closed.end()){	//std::maps end() fn goes through the entire map and returns past-the-end if key is not in map
			for( auto &action : actions){
				SearchState new_state(working_state);
				new_state = action.execute(new_state);
				closed.emplace(new_state, std::make_tuple(std::make_shared<SearchState>(working_state), action));

				open.push_back(new_state);

				if(new_state.isFinal()){
					int tmp = 1;
					//solution.push_back(action);
					auto solution_state = closed.find(new_state);
					while(solution_state != closed.end()){
						std::cout << "Solution steps found: " << tmp << std::endl;
						tmp++;
						solution.push_back(std::get<1>(solution_state->second));
						solution_state = closed.find(*(std::get<0>(solution_state->second).get())); //only god knows if this works... (probably doesnt)
					}
					std::cout << "Solution: ";
					for (long unsigned int i = 0; i < solution.size(); i++)
					std::cout << solution[i] << "," << std::endl;
					
					return solution;
				}
			}
		}	
	}

	std::cout << "Open_size = " << open.size() << std::endl;
	std::cout << "Closed_size = " << closed.size() << std::endl;

	return {};

	/* while(!solved){
		if(solved == working_state.isFinal())
			break;

		if(getCurrentRSS() < this.mem_limit - memory_threshold)
			break;

		for(auto &action : working_state.actions()){
			SearchState new_state(working_state);
			new_state = action.execute(new_state);
			opened.push_back(new_state);
		}
	}
	if(solved)
		return solution;
	else
		return {}; */
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