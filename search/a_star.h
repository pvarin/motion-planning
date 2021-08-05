#pragma once

#include "search/graph.h"
#include <deque>
#include <functional>
#include <memory>
#include <optional>
#include <unordered_set>

template <typename State>
using success_fn = std::function<bool(const State&)>;

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
bool breadth_first_search(std::shared_ptr<State> start, const success_fn<State>& is_success, std::unordered_set<const State*>& visited_states)
{
    std::deque<std::shared_ptr<State>> state_queue;
    visited_states.insert(start.get());
    state_queue.push_back(start);

    while (state_queue.size() > 0) {
        auto next_state = state_queue.front();
        if (is_success(*next_state)) {
            return true;
        }
        for (auto neighbor : next_state->neighbors()) {
            if (visited_states.find(neighbor.get()) == visited_states.end()) {
                visited_states.insert(neighbor.get());
                state_queue.push_back(neighbor);
            }
        }
        state_queue.pop_front();
    }
    return false;
}

template <typename State>
bool breadth_first_search(std::shared_ptr<State> start, const success_fn<State>& is_success)
{
    std::unordered_set<const State*> visited_states = {};
    return breadth_first_search(start, is_success, visited_states);
}
