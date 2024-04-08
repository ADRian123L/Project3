#include <algorithm>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

using Graph  = std::vector<std::vector<int>>;
using Vertex = Graph::const_iterator;
using INT    = std::numeric_limits<int>;

std::optional<std::size_t> heuristic(std::size_t        x,
                                     std::size_t        y,
                                     Vertex             vertex,
                                     const Graph       &graph,
                                     std::vector<bool> &visited) {
    std::vector<int> neighbors;
    int              dx, dy;
    for (const auto &it : *vertex) {
        dx = dy = INT::max();
        if (it != -1 && !visited.at(it)) {
            dx = (graph.size() - (it + 1)) / x;
            dy = (graph.size() - (it + 1)) % y;
            neighbors.push_back(dx + dy);
        }
    }
    auto ite = std::min_element(neighbors.begin(), neighbors.end());

    return std::distance(neighbors.begin(), ite);
}

std::optional<std::string>
dfs_n(std::size_t next, const Graph &graph, std::vector<bool> &visited) {
    if (next == graph.size() - 1)
        return std::string(" ");
    visited.at(next) = true;
    for (const auto &it : graph.at(next)) {
        if (it != -1 && !visited.at(next)) {
            auto path = dfs_n(it, graph, visited);
            if (path.has_value()) {
                auto neighboor = &it - &graph.at(next).front();
                return std::to_string(neighboor) + path.value();
            }
        }
    }
    return std::nullopt;
}

std::optional<std::string>
dfs(Vertex vertex, const Graph &graph, std::vector<bool> &visited) {
    auto pos = std::distance(graph.cbegin(), vertex);
    if (pos == graph.size() - 1)
        return std::string(" ");
    visited.at(pos) = true;
    for (const auto &it : *vertex) {
        if (it != -1 && !visited.at(it)) {
            auto path = dfs(graph.cbegin() + it, graph, visited);
            if (path.has_value()) {
                auto distance = &it - &(*vertex).front();
                return std::to_string(distance) + path.value();
            }
        }
    }
    return std::nullopt;
}

std::optional<std::string>
dfs_new(int start, int end, const Graph &graph, std::vector<bool> &visited) {
    if (start == end)
        return std::string(" ");
    visited.at(start) = true;
    for (auto it{graph.at(start).begin()}; it != graph.at(start).end(); ++it) {
        if (*it != -1 && !visited.at(*it)) {
            auto path = dfs_new(*it, end, graph, visited);
            if (path.has_value()) {
                auto direction{std::distance(graph.at(start).begin(), it)};
                return std::to_string(direction) + path.value();
            }
        }
    }
    return std::nullopt;
}

std::optional<std::string> dfs(int, int, const Graph &, std::vector<bool> &);

std::string search(int x, int y, const std::vector<std::vector<int>> &graph) {
    std::vector<bool> visited(x * y, false);
    return dfs(graph.begin(), graph, visited).value();
}

std::optional<std::string>
dfs(int start, int end, const Graph &graph, std::vector<bool> &visited) {
    if (start == end)
        return std::string(" ");
    visited.at(start) = true;
    for (int i = 0; i < 4; ++i) {
        if (graph.at(start).at(i) != -1 && !visited.at(graph.at(start).at(i))) {
            auto returned = dfs(graph.at(start).at(i), end, graph, visited);
            if (returned.has_value())
                return std::to_string(i) + returned.value();
        }
    }
    return std::nullopt;
}

int main() {
    int x, y, entry;
    std::cin >> x >> y;
    Graph graph(x * y, std::vector<int>(4, -1));
    for (int i = 0; i < x * y; ++i) {
        std::cin >> entry;
        if (entry && entry + (i % y) < y) // Right
            graph.at(i).front() = (entry + i);
        if (entry && entry + 1 + i / x <= x) // Down
            graph.at(i).at(1) = (i + entry * y);
        if (entry && (i % y) - entry >= 0) // Left
            graph.at(i).at(2) = (i - entry);
        if (entry && (i / x) - entry >= 0) // Up
            graph.at(i).back() = (i - entry * y);
    }

    /*
    int counter{};
    for (auto const &vertex : graph) {
        std::cout << "At " << counter << ": [";
        for (auto const &neigh : vertex) {
            std::cout << neigh << ", ";
        }
        std::cout << "] ";
        std::cout << std::endl;
        counter++;
    }
    */
    std::string map    = "ESWN";
    std::string answer = search(x, y, graph);
    std::cout << answer << std::endl;
    std::cout << "1" << std::endl;
    std::stringstream path;
    for (auto &ptr : answer)
        if (isdigit(ptr))
            path << map.at(ptr - '0') << " ";
    path.str().pop_back();
    std::cout << "2" << std::endl;
    std::cout << path.str() << std::endl;

    std::ofstream output("output.txt");
    if (output.is_open()) {
        output << path.str();
        output.close();
    }
    else {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    return 0;
}
