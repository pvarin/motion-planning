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
using transition_reward_fn = std::function<double(const State&, const State&)>;

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
std::optional<Path<State>> djikstra_search(std::shared_ptr<State> start, const success_fn<State>& is_success, const transition_reward_fn<State>& transition_reward, std::unordered_set<const State*>& visited_states)
{
    std::priority_queue<std::pair<double, Path<State>>> path_queue;
    visited_states.insert(start.get());
    path_queue.push({ 0, { start } });

    while (path_queue.size() > 0) {
        auto [reward_so_far, current_path] = path_queue.top();
        const auto& next_state = current_path.back();
        if (is_success(*next_state)) {
            return current_path;
        }
        for (auto neighbor : next_state->neighbors()) {
            if (visited_states.find(neighbor.get()) == visited_states.end()) {
                visited_states.insert(neighbor.get());
                double step_reward = transition_reward(*next_state, *neighbor);
                Path<State> next_path = current_path;
                next_path.push_back(neighbor);
                path_queue.push({ step_reward + reward_so_far, next_path });
            }
        }
        path_queue.pop();
    }
    return std::nullopt;
}

template <typename State>
std::optional<Path<State>> djikstra_search(std::shared_ptr<State> start, const success_fn<State>& is_success, const transition_reward_fn<State>& transition_reward)
{
    std::unordered_set<const State*> visited_states = {};
    return djikstra_search(start, is_success, transition_reward, visited_states);
}
