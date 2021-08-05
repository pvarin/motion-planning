#include <gtest/gtest.h>

#include <iostream>
#include <set>
#include <string>

#include "search/a_star.h"
#include "search/test/test_utils.h"

TEST(A_STAR, DEPTH_FIRST_SEARCH)
{
    auto nodes = test_graph();
    success_fn<State<double>> success = [](const State<double>& node) -> bool { return node.data() > 2.9; };
    auto path = depth_first_search(nodes[0], success);

    ASSERT_TRUE(path.has_value());
    EXPECT_NEAR(path.value()[0]->data(), 3.0, 1e-16);
}

TEST(A_STAR, BREADTH_FIRST_SEARCH)
{
    auto nodes = test_graph();
    success_fn<State<double>> is_success = [](const State<double>& node) -> bool { return node.data() > 2.9; };
    bool success = breadth_first_search(nodes[0], is_success);

    ASSERT_TRUE(success);
}
