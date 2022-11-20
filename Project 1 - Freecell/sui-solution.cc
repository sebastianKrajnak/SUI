#include "search-strategies.h"
#include "memusage.h"
#include <deque>
#include <map>
#include <memory>
#include <set>
#include <tuple>

int memory_limit = 100 * (1024^2);

std::vector<SearchAction> FindSolution(const SearchState &init_state, SearchState &goal, std::map< SearchState, std::tuple<SearchState, SearchAction> > action_map){
	auto tmp_iterator = action_map.find(goal);
	auto tmp_tuple = tmp_iterator->second;
	auto tmp_state = std::get<0>(tmp_tuple);
	auto tmp_sol_move = std::get<1>(tmp_tuple);
	
	std::vector<SearchAction> solution;
	solution.push_back(tmp_sol_move);

	while(true){
		if(init_state == tmp_state){
			return solution;
		}
		else{
			tmp_iterator = action_map.find(tmp_state);
			tmp_tuple = tmp_iterator->second;
			tmp_state = std::get<0>(tmp_tuple);
			tmp_sol_move = std::get<1>(tmp_tuple);

			solution.push_back(tmp_sol_move);
		}
	}
}

std::vector<SearchAction> BreadthFirstSearch::solve(const SearchState &init_state) {

	std::vector<SearchAction> solution;
	std::deque<SearchState> open;
	std::set<SearchState> closed; 
	// where first state is new_state and the tuple consists of parent_state and action that lead from parent to new
	//std::map< std::shared_ptr<SearchState>, std::tuple<std::shared_ptr<SearchState>, SearchAction> > action_map;
	std::map< SearchState, std::tuple<SearchState, SearchAction> > action_map;

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
				/* auto state_ptr = std::make_shared<SearchState>(new_state);
				action_map.emplace(
					state_ptr,
					std::make_tuple(std::make_shared<SearchState>(working_state), action)
					); */
				action_map.emplace(
					new_state,
					std::make_tuple(working_state, action));
 
				if(new_state.isFinal()){
					auto tmp_iterator = action_map.find(new_state);
					auto tmp_tuple = tmp_iterator->second;
					auto tmp_state = std::get<0>(tmp_tuple);
					auto tmp_sol_move = std::get<1>(tmp_tuple);

					solution.push_back(action);
					std::cout << "Found solution!" << std::endl;
					std::cout << "Parent is: " << tmp_state << " and move is: " << tmp_sol_move << std::endl;
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