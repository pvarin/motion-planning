#pragma once

#include "search/graph.h"
#include <deque>
#include <functional>
#include <memory>
#include <optional>
#include <queue>
#include <unordered_set>

template <typename State>
using success_fn = std::function<bool(const State&)>;

template <typename State>
using transition_cost_fn = std::function<double(const State&, const State&)>;

template <typename State>
using value_fn = std::function<double(const State&)>;

template <typename T>
using min_queue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

////////////////////////
// Depth First Search //
////////////////////////

template <typename State>
bool depth_first_search(std::shared_ptr<State> start, const success_fn<State>& is_success, Path<State>* path, std::unordered_set<const State*>& visited_states)
{
    if (is_success(*start)) {
        path->push_back(start);
        return true;
    }

    visited_states.insert(start.get());
    for (auto& neighbor_ptr : start->neighbors()) {
        if (visited_states.find(neighbor_ptr.get()) != visited_states.end()) {
            continue;
        }
        bool success = depth_first_search(neighbor_ptr, is_success, path, visited_states);
        if (success) {
            path->push_back(start);
            return true;
        }
    }
    return false;
}

template <typename State>
std::optional<Path<State>> depth_first_search(std::shared_ptr<State> start, const success_fn<State>& is_success)
{
    std::unordered_set<const State*> visited_states = {};
    Path<State> path;
    bool success = depth_first_search(start, is_success, &path, visited_states);
    if (success) {
        return path;
    } else {
        return std::nullopt;
    }
}

//////////////////////////
// Breadth First Search //
//////////////////////////

template <typename State>
std::optional<Path<State>> breadth_first_search(std::shared_ptr<State> start, const success_fn<State>& is_success, std::unordered_set<const State*>& visited_states)
{
    std::deque<Path<State>> path_queue;
    visited_states.insert(start.get());
    path_queue.push_back({ start });

    while (path_queue.size() > 0) {
        auto current_path = path_queue.front();
        const auto& next_state = current_path.back();
        if (is_success(*next_state)) {
            return current_path;
        }
        for (auto neighbor : next_state->neighbors()) {
            if (visited_states.find(neighbor.get()) == visited_states.end()) {
                visited_states.insert(neighbor.get());
                Path<State> next_path = current_path;
                next_path.push_back(neighbor);
                path_queue.push_back(next_path);
            }
        }
        path_queue.pop_front();
    }
    return std::nullopt;
}

template <typename State>
std::optional<Path<State>> breadth_first_search(std::shared_ptr<State> start, const success_fn<State>& is_success)
{
    std::unordered_set<const State*> visited_states = {};
    return breadth_first_search(start, is_success, visited_states);
}

//////////////
// Dijkstra //
//////////////

template <typename State>
std::optional<Path<State>> djikstra_search(std::shared_ptr<State> start, const success_fn<State>& is_success, const transition_cost_fn<State>& transition_cost, std::unordered_set<const State*>& visited_states)
{
    min_queue<std::pair<double, Path<State>>> path_queue;
    visited_states.insert(start.get());
    path_queue.push({ 0, { start } });

    while (path_queue.size() > 0) {
        auto [cost_so_far, current_path] = path_queue.top();
        const auto& next_state = current_path.back();
        if (is_success(*next_state)) {
            return current_path;
        }
        for (auto neighbor : next_state->neighbors()) {
            if (visited_states.find(neighbor.get()) == visited_states.end()) {
                visited_states.insert(neighbor.get());
                double step_cost = transition_cost(*next_state, *neighbor);
                Path<State> next_path = current_path;
                next_path.push_back(neighbor);
                path_queue.push({ step_cost + cost_so_far, next_path });
            }
        }
        path_queue.pop();
    }
    return std::nullopt;
}

template <typename State>
std::optional<Path<State>> djikstra_search(std::shared_ptr<State> start, const success_fn<State>& is_success, const transition_cost_fn<State>& transition_cost)
{
    std::unordered_set<const State*> visited_states = {};
    return djikstra_search(start, is_success, transition_cost, visited_states);
}

//////////////
// A-Star //
//////////////

template <typename State>
std::optional<Path<State>> a_star_search(std::shared_ptr<State> start, const success_fn<State>& is_success, const value_fn<State>& heuristic, const transition_cost_fn<State>& transition_cost, std::unordered_set<const State*>& visited_states)
{
    min_queue<std::pair<double, Path<State>>> path_queue;
    visited_states.insert(start.get());
    path_queue.push({ 0, { start } });

    while (path_queue.size() > 0) {
        auto [cost_so_far, current_path] = path_queue.top();
        const auto& next_state = current_path.back();
        if (is_success(*next_state)) {
            return current_path;
        }
        for (auto neighbor : next_state->neighbors()) {
            if (visited_states.find(neighbor.get()) == visited_states.end()) {
                visited_states.insert(neighbor.get());
                double step_cost = transition_cost(*next_state, *neighbor);
                double approx_cost_to_go = heuristic(*neighbor);
                Path<State> next_path = current_path;
                next_path.push_back(neighbor);
                path_queue.push({ step_cost + cost_so_far + approx_cost_to_go, next_path });
            }
        }
        path_queue.pop();
    }
    return std::nullopt;
}

template <typename State>
std::optional<Path<State>> a_star_search(std::shared_ptr<State> start, const success_fn<State>& is_success, const value_fn<State>& heuristic, const transition_cost_fn<State>& transition_cost)
{
    std::unordered_set<const State*> visited_states = {};
    return a_star_search(start, is_success, heuristic, transition_cost, visited_states);
}
