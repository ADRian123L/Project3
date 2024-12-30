// Creator: Adrian Lozada

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace b { // Namespac
    using AdjecL = std::vector<std::vector<int>>;
    class Graph {
    public:
        Graph(std::size_t size, std::size_t neighbors)
            : graph(size, std::vector<int>(neighbors, -1)), visited(size){};

        void        AddEdge(int, int, int); // Add edge to graph
        std::string path(int, int);        // Find path from start to end

    private:
        AdjecL graph;
        enum class Direction { North, South, East, West }; // Direction
        std::vector<Direction> pathE;                    // Path
        Direction              dirct(int);               // Convert int to Direction
        char                   convertoS(Direction);    // Convert Direction to char
        std::vector<bool>      visited;                // Visited nodes
        bool                   findPath(int, int);    // Find path from start to end
    };
} // namespace b

// Convert int to Direction
b::Graph::Direction b::Graph::dirct(int to) {
    switch (to) {
    case 0: {
        return Direction::East;
    }
    case 1: {
        return Direction::South;
    }
    case 2: {
        return Direction::West;
    }
    case 3: {
        return Direction::North;
    }
    default: {
        return Direction::North;
    }
    }
}

// Add edge to graph
void b::Graph::AddEdge(int from, int to, int pos) {
    graph.at(from).at(pos) = to;
}

// Convert Direction to char
char b::Graph::convertoS(Direction dirct) {
    switch (dirct) {
    case Direction::North: {
        return 'N';
    }
    case Direction::South: {
        return 'S';
    }
    case Direction::East: {
        return 'E';
    }
    case Direction::West: {
        return 'W';
    }
    default: {
        return 'N';
    }
    }
}

// Find path from start to end
std::string b::Graph::path(int from, int to) {
    std::fill(visited.begin(), visited.end(), false);
    pathE.clear();
    const auto        found = findPath(from, to);
    std::stringstream answer;
    if (found) {
        for (const auto &ite : pathE) {
            answer << convertoS(ite) << ' ';
        }
        std::string direct = answer.str();
        direct.pop_back();
        std::reverse(direct.begin(), direct.end());
        return direct;
    }
    else {
        return std::string();
    }
}

// Find path from start to end
bool b::Graph::findPath(int from, int to) {
    if (from == to) {
        return true;
    }
    visited.at(from) = true;
    for (const auto &ite : graph.at(from)) {
        if (ite != -1 && !visited.at(ite)) {
            if (findPath(ite, to)) {
                pathE.push_back(dirct(&ite - &graph.at(from).front()));
                return true;
            }
        }
    }
    return false;
}

int main() {
    int           x{}, y{}, entry{};
    b::Graph     *gra{nullptr}; // Graph
    std::ifstream input("input.txt");
    if (input.is_open()) {
        input >> x >> y;
        gra = new b::Graph(x * y, 4); // Initialize graph
        for (int i = 0; i < x * y; ++i) {
            input >> entry;
            if (entry && entry + (i % y) < y) // Right
                gra->AddEdge(i, (entry + i), 0);
            if (entry && entry + 1 + i / x <= x) // Down
                gra->AddEdge(i, (i + entry * y), 1);
            if (entry && (i % y) - entry >= 0) // Left
                gra->AddEdge(i, (i - entry), 2);
            if (entry && (i / x) - entry >= 0) // Up
                gra->AddEdge(i, (i - entry * y), 3);
        }
        input.close();
    }
    else {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }
    std::string   path = gra->path(0, x * y - 1); // Find path
    std::ofstream output("output.txt");
    if (output.is_open()) {
        output << path;
        output.flush();
        output.close();
    }
    else {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }
    delete gra;
    return 0;
}
