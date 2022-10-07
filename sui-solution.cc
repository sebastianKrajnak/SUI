#include "search-strategies.h"
#include "memusage.h"
#include <deque>
#include <map>
#include <set>
#include <tuple>

int memory_limit = 100 * (1024^2);

std::vector<SearchAction> BreadthFirstSearch::solve(const SearchState &init_state) {
	//bool solved = false;
	std::vector<SearchAction> solution;
	std::deque<SearchState> open;
	std::map< SearchState, std::tuple<std::shared_ptr<SearchState>, SearchAction> > closed; 

	open.push_back(init_state);

	while(!open.empty()){
		if(getCurrentRSS() >= mem_limit_ - memory_limit){
			break;
		}
		SearchState working_state(open.back());
		open.pop_back();
		auto search = closed.find(working_state);
		auto actions = working_state.actions();
		
		if(working_state.isFinal())
			return solution;
		std::cout << "Hi wroking state is: " << working_state << std::endl;
		auto action = actions[0];
		if (search == closed.end()){	//std::maps end() fn goes through the entire map and returns past-the-end if key is not in map
			for( auto &action : actions){
				SearchState new_state(working_state);
				new_state = action.execute(new_state);
				closed.emplace(new_state, std::make_tuple(std::make_shared<SearchState>(working_state), action));
				solution.push_back(action);

				if(new_state.isFinal())
					return solution;

				open.push_back(new_state);
			}
		}	
	}
	return {};

	/* while(!solved){
		if(solved == working_state.isFinal())
			break;

		if(getCurrentRSS() < this.mem_limit - memory_limit)
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