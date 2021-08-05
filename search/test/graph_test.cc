#include <gtest/gtest.h>
#include <memory>

#include "search/graph.h"
#include "search/test/test_utils.h"

TEST(GRAPH, GRAPH)
{
    auto nodes = test_graph();

    EXPECT_EQ(nodes[0]->neighbors().size(), 3);
    EXPECT_EQ(nodes[1]->neighbors().size(), 2);
    EXPECT_EQ(nodes[2]->neighbors().size(), 1);
    EXPECT_EQ(nodes[3]->neighbors().size(), 0);

    EXPECT_EQ(nodes[0]->neighbors()[0], nodes[1]);
    EXPECT_EQ(nodes[0]->neighbors()[1], nodes[2]);
    EXPECT_EQ(nodes[0]->neighbors()[2], nodes[3]);

    EXPECT_EQ(nodes[1]->neighbors()[0], nodes[0]);
    EXPECT_EQ(nodes[1]->neighbors()[1], nodes[1]);

    EXPECT_EQ(nodes[2]->neighbors()[0], nodes[3]);
}
