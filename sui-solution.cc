#include "search-strategies.h"
#include "memusage.h"
#include <deque>
#include <map>
#include <set>
#include <tuple>

int memory_limit = 100 * (1024^2);

std::vector<SearchAction> BreadthFirstSearch::solve(const SearchState &init_state) {
	/* //bool solved = false;
	std::vector<SearchAction> solution;
	std::deque< std::tuple<SearchState,SearchAction> > open;
	std::map< SearchState, std::tuple<std::shared_ptr<SearchState>, SearchAction> > closed; 

	open.push_back(init_state,);

	while(!open.empty()){
		if(getCurrentRSS() >= mem_limit_ - memory_limit){
			std::cout << "Hit mem limit" << std::endl;
			break;
		}
		
		SearchState working_state(open.front());
		open.pop_front();
		auto search = closed.find(working_state);
		auto actions = working_state.actions();
		
		if (search == closed.end()){	//std::maps end() fn goes through the entire map and returns past-the-end if key is not in map
			for( auto &action : actions){
				SearchState new_state(working_state);
				new_state = action.execute(new_state);
				if( closed.find(new_state) == closed.end() )
					open.push_back(new_state, action);

				solution.push_back(action);
				//std::cout << "Sol: " << solution.back() << std::endl;
				//std::cout << "Closed: " << closed.size() << std::endl;
				if(new_state.isFinal())
					//std::cout << "Yay found the end" << std::endl;
					return solution;

			
			}
			closed.emplace(working_state, std::make_tuple(std::make_shared<SearchState>(working_state), action));
		}	
	}
	return {}; */
	std::vector<SearchAction> solution;
	std::deque<SearchState> open;
	std::set<SearchState> closed; 

	open.push_back(init_state);

	while(true){
		if(getCurrentRSS() >= mem_limit_ - memory_limit){
			std::cout << "Hit mem limit" << std::endl;
			break;
		}

		if(!open.empty()){
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
					if( closed.find(new_state) == closed.end() ){
						if(new_state.isFinal()){
							solution.push_back(action);
							std::cout << "Found solution!" << std::endl;
							return solution;
						}
						open.push_back(new_state);
					}
				}
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