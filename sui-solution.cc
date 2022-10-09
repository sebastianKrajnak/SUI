#include "search-strategies.h"
#include "memusage.h"
#include <deque>
#include <map>
#include <set>
#include <tuple>
#include <algorithm>

int memory_threshold = 100 * (1024^2);

std::vector<SearchAction> FindSolution(
	std::map<SearchState, std::tuple<std::shared_ptr<SearchState>, SearchAction>>::iterator &map_itr,
	std::map<SearchState, std::tuple<std::shared_ptr<SearchState>, SearchAction> > &action_map,
	std::shared_ptr<SearchState> &prev_ptr) {

	std::vector<SearchAction> solution;

	while(std::get<0>(map_itr->second) != nullptr){ 
		solution.push_back(std::get<1>(map_itr->second));
		prev_ptr = std::get<0>(map_itr->second);

		map_itr = action_map.find(*prev_ptr.get()); 						
	}

	std::reverse(solution.begin(), solution.end());
	return solution;
}


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

		if (search == closed.end()){	//std::maps end() fn goes through the entire map and returns past-the-end if key is not in map
			closed.insert(working_state);
			if (state_ptr == nullptr)
				state_ptr = std::make_shared<SearchState>(working_state);

			for( auto &action : actions){
				SearchState new_state(working_state);
				new_state = action.execute(new_state);
				
				action_map.emplace(new_state, std::make_tuple(state_ptr, action));
				open.push_back(new_state);

				if(new_state.isFinal()){
					auto map_itr = action_map.find(new_state);
					auto prev_ptr = std::shared_ptr<SearchState>(nullptr);

					solution = FindSolution(map_itr, action_map, prev_ptr);

					action_map.clear();				
					return solution;
				}
			}
			state_ptr = nullptr;
		}	
	}
	action_map.clear();	
	return {};
}

std::vector<SearchAction> DepthFirstSearch::solve(const SearchState &init_state) {
	std::vector<SearchAction> solution;
	std::deque<SearchState> open;
	std::set<SearchState> closed;
	std::map<SearchState, std::tuple<std::shared_ptr<SearchState>, SearchAction> > action_map; 
	//(current search state, (pointer parent state, action from parent to current))

	double depth = 0;
	
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
		if(depth > depth_limit_){
			std::cout << "Reached maxed allowed depth" << std::endl;
			break;
		}

		SearchState working_state(open.back());
		open.pop_back();
		auto search = closed.find(working_state);
		auto actions = working_state.actions();

		if (search == closed.end()){	//std::sets end() fn goes through the entire set and returns past-the-end if key is not in set
			closed.insert(working_state);
			if (state_ptr == nullptr)
				state_ptr = std::make_shared<SearchState>(working_state);

			for( auto &action : actions){
				SearchState new_state(working_state);
				new_state = action.execute(new_state);
				
				action_map.emplace(new_state, std::make_tuple(state_ptr, action));
				open.push_back(new_state);

				if(new_state.isFinal()){
					auto map_itr = action_map.find(new_state);
					auto prev_ptr = std::shared_ptr<SearchState>(nullptr);

					solution = FindSolution(map_itr, action_map, prev_ptr);
					action_map.clear();				
					return solution;
				}
			}
			depth++;
			state_ptr = nullptr;
		}	
	}
	action_map.clear();	
	return {};
}

double StudentHeuristic::distanceLowerBound(const GameState &state) const {
    return 0;
}

std::vector<SearchAction> AStarSearch::solve(const SearchState &init_state) {
	/* std::vector<SearchAction> solution;
	std::map<SearchState, double> open;
	std::set<SearchState> closed;
	std::map<SearchState, std::tuple<std::shared_ptr<SearchState>, SearchAction> > action_map;
	double cost = 0;	// g(n) part of the f(n) to calculate cost, ie. trips from source

	if (init_state.isFinal())
		return {};

	auto state_ptr = std::make_shared<SearchState>(init_state);
	action_map.emplace(init_state, std::make_tuple(nullptr, init_state.actions()[0]));
	open.emplace(init_state, cost);
	
	while(!open.empty()){
		if(getCurrentRSS() >= mem_limit_ - memory_threshold){
			std::cout << "Out of memory." << std::endl;
			break;
		}

		// TODO add a way to return key of min value from open!!!
		SearchState working_state;
		open.erase(working_state);

		auto search = closed.find(working_state);
		auto actions = working_state.actions();
		
		if (search == closed.end()){	//std::maps end() fn goes through the entire map and returns past-the-end if key is not in map
			closed.insert(working_state);
			if (state_ptr == nullptr)
				state_ptr = std::make_shared<SearchState>(working_state);

			for( auto &action : actions){
				SearchState new_state(working_state);
				new_state = action.execute(new_state);
				
				auto h_n = compute_heuristic(new_state, *heuristic_);
				auto f_n = cost + h_n;

				action_map.emplace(new_state, std::make_tuple(state_ptr, action));
				open.emplace(new_state, f_n);

				if(new_state.isFinal()){
					auto map_itr = action_map.find(new_state);
					auto prev_ptr = std::shared_ptr<SearchState>(nullptr);

					std::cout << "Found final state !" << std::endl;

					solution = FindSolution(map_itr, action_map, prev_ptr);
					action_map.clear();				
					return solution;
				}
			}
			cost++;
			state_ptr = nullptr;
		}	
	}
	action_map.clear();	 */
	return {};
}