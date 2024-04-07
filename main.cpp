#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

std::string dfs(int                                  start,
                int                                  end,
                const std::vector<std::vector<int>> &graph,
                std::vector<int>                    &dp);

std::string search(int x, int y, const std::vector<std::vector<int>> &graph) {
    std::vector<int> traveled(x * y, 0);
    return dfs(0, x * y - 1, graph, traveled);
}

std::string dfs(int                                  start,
                int                                  end,
                const std::vector<std::vector<int>> &graph,
                std::vector<int>                    &travelled) {
    if (start == end)
        return std::string(" ");
    travelled.at(start) = 1;
    std::string returned;
    for (int i = 0; i < 4; ++i)
        if (graph.at(start).at(i) != -1 && !travelled.at(graph.at(start).at(i)))
            if ((returned =
                     dfs(graph.at(start).at(i), end, graph, travelled)) != "")
                return std::to_string(i) + returned;
    return std::string();
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
    std::cout << "Searching: " << std::endl;
    std::cout << std::boolalpha;
    std::string hash   = "ESWN";
    std::string answer = search(x, y, graph);
    for (auto &ptr : answer)
        if (isdigit(ptr))
            ptr = hash.at(ptr - '0');
    std::cout << answer << std::endl;
    return 0;
}
