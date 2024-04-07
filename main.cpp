#include <algorithm>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

using Graph = std::vector<std::vector<int>>;

std::optional<std::string>
dfs_new(int start, int end, const Graph &graph, std::vector<bool> &visited) {
    if (start == end) return std::string(" ");
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

    for (const auto &nextVertex : graph.at(start)) {
        if (nextVertex != -1 && !visited.at(nextVertex)) {
            auto path = dfs_new(nextVertex, end, graph, visited);
            if (path.has_value()) {
                auto direction = (&nextVertex - &graph.at(start).front());
                return std::to_string(direction) + path.value();
            }
        }
    }

    return std::nullopt;
}

std::optional<std::string>
dfs(int, int, const std::vector<std::vector<int>> &, std::vector<bool> &);

std::string search(int x, int y, const std::vector<std::vector<int>> &graph) {
    std::vector<bool> visited(x * y, false);
    return dfs(0, x * y - 1, graph, visited).value();
}

std::optional<std::string> dfs(int                                  start,
                               int                                  end,
                               const std::vector<std::vector<int>> &graph,
                               std::vector<bool>                   &visited) {
    if (start == end) return std::string(" ");
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
    std::vector<std::vector<int>> graph(x * y, std::vector<int>(4, -1));
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
    std::string       map    = "ESWN";
    std::string       answer = search(x, y, graph);
    std::stringstream path;
    for (auto &ptr : answer)
        if (isdigit(ptr)) path << map.at(ptr - '0') << " ";
    path.str().pop_back();

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
