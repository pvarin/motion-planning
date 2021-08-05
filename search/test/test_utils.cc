#include "search/test/test_utils.h"

std::vector<std::shared_ptr<State<double>>> test_graph()
{
    using Node = State<double>;
    std::vector<std::shared_ptr<Node>> nodes = {
        std::make_shared<Node>(0.0),
        std::make_shared<Node>(1.0),
        std::make_shared<Node>(2.0),
        std::make_shared<Node>(3.0),
    };

    nodes[0]->neighbors().push_back(nodes[1]);
    nodes[0]->neighbors().push_back(nodes[2]);
    nodes[0]->neighbors().push_back(nodes[3]);
    nodes[1]->neighbors().push_back(nodes[0]);
    nodes[1]->neighbors().push_back(nodes[1]);
    nodes[2]->neighbors().push_back(nodes[3]);

    return nodes;
}
