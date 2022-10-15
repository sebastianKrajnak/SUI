#include "search-strategies.h"
#include "memusage.h"
#include <deque>
#include <queue>
#include <map>
#include <set>
#include <tuple>
#include <algorithm>

typedef std::pair<double, SearchState> Pair;

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
	// Heuristics src: https://www.human-competitive.org/sites/default/files/elyasaf-hauptmann-sipper-paper.pdf
	/* 
	SumOfBottomCards. Take the highest possible sum of
	cards in the bottom of cascades (e.g. 100), and subtract the sum of values
	of cards actually located there.
	 */

	/* double actual_sum = 0;
	for (const auto &cascade : state.stacks) {
        auto top_card = cascade.topCard();
        if (top_card.has_value())
            actual_sum += top_card->value;
    }
	 return 100 - actual_sum; */

	/*  
	DifferenceFromTop. The average value of the top cards
	in cascades, minus the average value of the top cards in
	home piles.
	*/

	double cascade_sum = 0;
	double home_sum = 0;

	for (const auto &cascade : state.stacks) {
        auto top_card = cascade.topCard();
        if (top_card.has_value())
            cascade_sum += top_card->value;
    }

	for (const auto &home : state.homes) {
        auto top_card = home.topCard();
        if (top_card.has_value())
            home_sum += top_card->value;
    }

	 return cascade_sum/8 - home_sum/4;

	/*
	 Heineman’s Staged Deepening Heusirtic (HSDH). This is
	the heuristic used by the HSD algorithm: For each home pile, locate within the cascade piles the next
	card that should be placed there, and count the cards found on top of it. The returned value is the sum of 
	this count for all foundations. This number is multiplied by 2 if there are no free FreeCells or empty 
	foundation piles (reflecting the fact that freeing the next card is harder in this case).
	*/
	
	/* /for (const auto& home : state.homes) {
		auto home_card = home.topCard();
		if(home_card.has_value())
			auto home_card_value = home_card->value;
		
		std::cout << "Home card is: " << std::endl;
		for (const auto& cascade : state.stacks) {
			
			 // I don't fuken kno how to get a list of all of card so I can compare them
		} 
	}
	return 0; */
}

std::vector<SearchAction> AStarSearch::solve(const SearchState &init_state) {
	//this works but not necessarily a_star
	/* std::vector<SearchAction> solution;
	std::map<double, SearchState> open;
	std::set<SearchState> closed;
	std::map<SearchState, std::tuple<std::shared_ptr<SearchState>, SearchAction> > action_map;
	double cost = 0;	// g(n) part of the f(n) to calculate cost, ie. trips from source

	if (init_state.isFinal())
		return {};

	auto state_ptr = std::make_shared<SearchState>(init_state);
	action_map.emplace(init_state, std::make_tuple(nullptr, init_state.actions()[0]));
	open.emplace(cost, init_state);

	while(!open.empty()){
		if(getCurrentRSS() >= mem_limit_ - memory_threshold){
			std::cout << "Out of memory." << std::endl;
			break;
		}

		SearchState working_state(open.begin()->second);
		open.erase(open.begin());
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
				open.emplace(f_n, new_state);

				if(new_state.isFinal()){
					auto map_itr = action_map.find(new_state);
					auto prev_ptr = std::shared_ptr<SearchState>(nullptr);

					//std::cout << "Found final state !" << std::endl;

					solution = FindSolution(map_itr, action_map, prev_ptr);
					action_map.clear();				
					return solution;
				}
			}
			cost++;
			state_ptr = nullptr;
		}	
	}
	action_map.clear();
	return {}; */

	std::vector<SearchAction> solution;
	std::priority_queue<std::pair<double, SearchState>> open;
	std::map<SearchState, double> g_scores;
	std::map<SearchState, double> f_scores;
	std::set<SearchState> closed;
	std::set<SearchState> open_checklist;
	std::map<SearchState, std::tuple<std::shared_ptr<SearchState>, SearchAction> > action_map;
	double cost = 0;	// g(n) part of the f(n) to calculate cost, ie. number of trips from source
	bool tentative_is_better = false;

	if (init_state.isFinal())
		return {};

	auto state_ptr = std::make_shared<SearchState>(init_state);
	action_map.emplace(init_state, std::make_tuple(nullptr, init_state.actions()[0]));
	open.emplace(std::make_pair(cost, init_state));
	open_checklist.insert(init_state);
	g_scores.emplace(init_state, cost);
	f_scores.emplace(init_state, compute_heuristic(init_state, *heuristic_));

	while(!open.empty()){
		if(getCurrentRSS() >= mem_limit_ - memory_threshold){
			std::cout << "Out of memory." << std::endl;
			break;
		}

		auto& top = open.top();
		auto& a = top.second;

		SearchState working_state(a);
		open.pop();
		open_checklist.erase(working_state);
		closed.insert(working_state);

		if(working_state.isFinal()){
			auto map_itr = action_map.find(working_state);
			auto prev_ptr = std::shared_ptr<SearchState>(nullptr);

			solution = FindSolution(map_itr, action_map, prev_ptr);

			action_map.clear();				
			return solution;
		}
		
		auto actions = working_state.actions();

		for( auto &action : actions){
			SearchState new_state(working_state);
			new_state = action.execute(new_state);

			auto search_closed = closed.find(new_state);
			auto search_open = open_checklist.find(new_state);

			if(search_closed != closed.end())
				break;
			
			auto current_g = g_scores.find(working_state);
			auto new_g = g_scores.find(new_state);
			auto tentative_score = current_g->second + 1;
			auto h_n = compute_heuristic(new_state, *heuristic_);
			auto f_n = new_g->second + h_n;

			if (search_open == closed.end())
			{
				open.emplace(std::make_pair(f_n, new_state));
				open_checklist.insert(new_state);
				tentative_is_better = true;

				if (state_ptr == nullptr)
					state_ptr = std::make_shared<SearchState>(working_state);
			}
			else if(tentative_score < new_g->second){
				tentative_is_better = true;
			}
			else{
				tentative_is_better = false;
			}

			if(tentative_is_better){
				action_map.emplace(new_state, std::make_tuple(state_ptr, action));
				g_scores.emplace(new_state, tentative_score);
				f_scores.emplace(new_state, f_n);
			}
		}
		cost++;
		state_ptr = nullptr;
	}
	action_map.clear();
	return {};
}