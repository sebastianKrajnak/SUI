#include "search-strategies.h"
#include "memusage.h"
#include <deque>
#include <map>
#include <set>
#include <tuple>

int memory_limit = 100 * (1024^2);

std::vector<SearchAction> BreadthFirstSearch::solve(const SearchState &init_state) {
	/* std::vector<SearchAction> solution;
	std::deque<SearchState> open;
	std::set<SearchState> closed;
	std::map< std::shared_ptr<SearchState>, std::tuple<std::shared_ptr<SearchState>, SearchAction> > action_map; 

	open.push_back(init_state);

	while(!open.empty()){
		if(getCurrentRSS() >= mem_limit_ - memory_limit){
			std::cout << "Hit mem limit" << std::endl;
			break;
		}
		
		SearchState working_state(open.front());
		open.pop_front();
		auto actions = working_state.actions();
		
		for( auto &action : actions){
			SearchState new_state(working_state);
			new_state = action.execute(new_state);
			if( closed.find(*working_state) == closed.end()){	//current working_state is NOT in closed
				closed.emplace(
					std::make_shared<SearchState>(new_state), 
					std::make_tuple(std::make_shared<SearchState>(working_state), action)
				);
				if( closed.find(new_state) == closed.end() ){
					solution.push_back(action);
					if(new_state.isFinal()){
						std::cout << "Found solution!" << std::endl;
						return solution;
					}
					open.push_back(new_state);
				}
			}
		}
	}
	return {};
}  */

	std::vector<SearchAction> solution;
	std::deque<SearchState> open;
	std::set<SearchState> closed; 
	// where first state is new_state and the tuple consists of parent_state and action that lead from parent to new
	std::map< std::shared_ptr<SearchState>, std::tuple<std::shared_ptr<SearchState>, SearchAction> > action_map;


	open.push_back(init_state);

	while(!open.empty()){
		if(getCurrentRSS() >= mem_limit_ - memory_limit){
			std::cout << "Hit mem limit" << std::endl;
			break;
		}

		SearchState working_state(open.front());
		open.pop_front();


		auto actions = working_state.actions();
		if(working_state.isFinal())
			return {};
		else if( closed.find(working_state) == closed.end()){
			closed.insert(working_state);

			for( auto &action : actions){
				SearchState new_state(working_state);
				new_state = action.execute(new_state);
				action_map.emplace(
					std::make_shared<SearchState>(new_state),
					std::make_tuple(std::make_shared<SearchState>(working_state), action)
					);
				if(new_state.isFinal()){
					
					solution.push_back(action);
					std::cout << "Found solution!" << std::endl;
					return solution;
				}
				open.push_back(new_state);
			}
		}
	}
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