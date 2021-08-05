#pragma once
#include <memory>

template <typename Data_>
struct State {
public:
    using Data = Data_;
    State(const State& state)
        : data_(new Data(*state.data_))
        , neighbors_(state.neighbors_)
    {
    }
    State(State&& state) = default;
    State(const Data& data)
        : data_(new Data(data))
    {
    }

    std::unique_ptr<Data> data() { return data_; };
    const std::unique_ptr<const Data> data() const { return data_; };
    std::vector<std::shared_ptr<State>>& neighbors() { return neighbors_; }
    const std::vector<const std::shared_ptr<const State>>& neighbors() const { return neighbors_; }

private:
    std::unique_ptr<Data> data_;
    std::vector<std::shared_ptr<State>> neighbors_;
};
