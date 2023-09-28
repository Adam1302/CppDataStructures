// C++ Data Structures

#define DEBUG_MODE 1
#if DEBUG_MODE
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif

#include <iostream>
#include <queue>
#include <unordered_map>
#include <list>

template<typename T>
class Graph {
    struct Edge {
        T destinationVertex;
        int weight;

        bool operator==(const Edge& e);
    };

    struct Vertex {
        T vertexName;
        std::vector<Edge> edges;
    };

    std::unordered_map<T, Vertex*> vertices;

    void dijkstrasAlgorithm(const T& from, const T& to, std::unordered_map<Vertex*, int>& distances, std::unordered_map<Vertex*, Vertex*>& lastVertex, std::unordered_map<Vertex*, bool>& visited);
    public:
        Graph();
        Graph(const Graph& other);
        Graph(Graph&& other);
        Graph& operator=(const Graph& other);
        Graph& operator=(Graph&& other);
        void addVertex(const T& newKey);
        void addVertex(T&& newKey);
        void deleteVertex(const T& key);
        void deleteVertex(T&& key);
        void addEdge(const T& from, const T& to, int weight = 1);
        void addEdge(T&& from, const T& to, int weight = 1);
        void addEdge(const T& from, T&& to, int weight = 1);
        void addEdge(T&& from, T&& to, int weight = 1);
        int edgeWeight(const T& from, const T& to);
        int edgeWeight(const T& from, T&& to);
        int edgeWeight(T&& from, const T& to);
        int edgeWeight(T&& from, T&& to);
        void deleteEdge(const T& from, const T& to);
        void deleteEdge(const T& from, T&& to);
        void deleteEdge(T&& from, const T& to);
        void deleteEdge(T&& from, T&& to);
        std::list<T> neighbours(const T& elem);
        bool isVertex(const T& elem);
        bool areNeighbours(const T& elem1, const T& elem2);
        int shortestPathLength(const T& from, const T& to);
        std::list<T> shortestPath(const T& from, const T& to);
        void bfsSearch(const T& start);
        void bfsSearch(T&& start);
        template <typename U>
        friend std::ostream& operator<<(std::ostream& out, const Graph<U>& g);
        ~Graph();
};

template <typename T>
bool Graph<T>::Edge::operator==(const Edge& e) {
    return (destinationVertex == e.destinationVertex) && (weight == e.weight);
}

template <typename T>
void Graph<T>::dijkstrasAlgorithm(const T& from, const T& to, std::unordered_map<Vertex*, int>& distances, std::unordered_map<Vertex*, Vertex*>& lastVertex, std::unordered_map<Vertex*, bool>& visited) {
    if (!isVertex(std::move(from)) || !isVertex(std::move(to))) {
        throw std::invalid_argument("Not an existing vertex");
    }

    struct pqDistance {
        Vertex* vertex;
        int* distance;

        bool operator< (const pqDistance& other) {
            return *distance < *(other.distance);
        }

        bool operator> (const pqDistance& other) {
            return *distance > *(other.distance);
        }
    };

    std::priority_queue<pqDistance, std::vector<pqDistance>, std::greater<void>> pq;

    for (auto& [vName, vertex] : vertices) {
        if (vName == from) {
            distances[vertex] = 0;
        } else {
            distances[vertex] = INT_MAX;
        }
        lastVertex[vertex] = nullptr;

        pq.push(pqDistance{vertex, &distances[vertex]});
    }

    while (!pq.empty()) {
        Vertex* nextMinVertex = pq.top().vertex;

        visited[nextMinVertex] = true;

        for (const auto& [neighbourName, weight] : nextMinVertex->edges) {
            Vertex* neighbourVertex = vertices[neighbourName];

            if (visited[neighbourVertex]) continue;

            int tempDist = distances[nextMinVertex] + weight;

            if (tempDist < distances[neighbourVertex]) {
                distances[neighbourVertex] = tempDist;
                lastVertex[neighbourVertex] = nextMinVertex;
            }
        }

        pq.pop();
    }
}

template <typename T>
Graph<T>::Graph() : vertices{} {}

template <typename T>
Graph<T>::Graph(const Graph& other) : vertices{} {
    for (auto& [vName, vertex] : other.vertices) {
        vertices[vName] = new Vertex{vName, vertex->edges};
    }
}

template <typename T>
Graph<T>::Graph(Graph&& other) : vertices{} {
    for (auto& [vName, vertex] : other.vertices) {
        vertices[vName] = vertex;
        vertex = nullptr;
    }
}

template <typename T>
Graph<T>& Graph<T>::operator=(const Graph& other) {
    for (auto& [vName, vertex] : other.vertices) {
        delete vertices[vName];
        vertices[vName] = new Vertex{vName, vertex->edges};
    }
    return *this;
}

template <typename T>
Graph<T>& Graph<T>::operator=(Graph&& other) {
    for (auto& [vName, vertex] : other.vertices) {
        delete vertices[vName];
        vertices[vName] = vertex;
        vertex = nullptr;
    }
    return *this;
}

template <typename T>
void Graph<T>::addVertex(const T& newKey) {
    if (vertices.find(newKey) == vertices.end()) {
        Vertex* newVertex = new Vertex{newKey, {}};

        vertices[newKey] = newVertex;
    }
}

template <typename T>
void Graph<T>::addVertex(T&& newKey) {
    if (vertices.find(newKey) == vertices.end()) {
        Vertex* newVertex = new Vertex{newKey, {}};

        vertices[newKey] = newVertex;
    }
}

template <typename T>
void Graph<T>::deleteVertex(const T& key) {
    if (vertices.find(key) != vertices.end()) {
        std::vector<Edge>* soonGoneEdges = &vertices[key]->edges;

        for (const auto& [destination, weight] : *soonGoneEdges) {
            std::vector<Edge>* toEdges = &vertices[destination]->edges;

            toEdges->erase(std::remove(toEdges->begin(), toEdges->end(), Edge{key, weight}));
        }

        delete vertices[key];
        vertices.erase(key);
    }
}

template <typename T>
void Graph<T>::deleteVertex(T&& key) {
    if (vertices.find(key) != vertices.end()) {
        std::vector<Edge>* soonGoneEdges = &vertices[key]->edges;

        for (const auto& [destination, weight] : *soonGoneEdges) {
            std::vector<Edge>* toEdges = &vertices[destination]->edges;

            toEdges->erase(std::remove(toEdges->begin(), toEdges->end(), Edge{key, weight}));
        }

        delete vertices[key];
        vertices.erase(key);
    }
}

template <typename T>
void Graph<T>::addEdge(const T& from, const T& to, int weight) {
    if (vertices.find(from) == vertices.end()) {
        addVertex(from);
    }
    if (vertices.find(to) == vertices.end()) {
        addVertex(to);
    }

    if (areNeighbours(from, to)) {
        vertices[from]->edges.push_back(Edge{to, weight});
        vertices[to]->edges.push_back(Edge{from, weight});
    }
}

template <typename T>
void Graph<T>::addEdge(T&& from, const T& to, int weight) {
    if (vertices.find(from) == vertices.end()) {
        addVertex(from);
    }
    if (vertices.find(to) == vertices.end()) {
        addVertex(to);
    }

    if (!areNeighbours(from, to)) {
        vertices[from]->edges.push_back(Edge{to, weight});
        vertices[to]->edges.push_back(Edge{from, weight});
    }
}

template <typename T>
void Graph<T>::addEdge(const T& from, T&& to, int weight) {
    if (vertices.find(from) == vertices.end()) {
        addVertex(from);
    }
    if (vertices.find(to) == vertices.end()) {
        addVertex(to);
    }

    if (!areNeighbours(from, to)) {
        vertices[from]->edges.push_back(Edge{to, weight});
        vertices[to]->edges.push_back(Edge{from, weight});
    }
}

template <typename T>
void Graph<T>::addEdge(T&& from, T&& to, int weight) {
    if (vertices.find(from) == vertices.end()) {
        addVertex(from);
    }
    if (vertices.find(to) == vertices.end()) {
        addVertex(to);
    }

    if (!areNeighbours(from, to)) {
        vertices[from]->edges.push_back(Edge{to, weight});
        vertices[to]->edges.push_back(Edge{from, weight});
    }
}

template <typename T>
int Graph<T>::edgeWeight(const T& from, const T& to) {
    if (vertices.find(from) != vertices.end()) {
        for (const auto& [destination, weight] : vertices[from]->edges) {
            if (destination == to) return weight;
        }
    }
    throw std::invalid_argument("This edge does not exist");
    return 0;
}

template <typename T>
int Graph<T>::edgeWeight(const T& from, T&& to) {
    if (vertices.find(from) != vertices.end()) {
        for (const auto& [destination, weight] : vertices[from]->edges) {
            if (destination == to) return weight;
        }
    }
    throw std::invalid_argument("This edge does not exist");
    return 0;
}

template <typename T>
int Graph<T>::edgeWeight(T&& from, const T& to) {
    if (vertices.find(from) != vertices.end()) {
        for (const auto& [destination, weight] : vertices[from]->edges) {
            if (destination == to) return weight;
        }
    }
    throw std::invalid_argument("This edge does not exist");
    return 0;
}

template <typename T>
int Graph<T>::edgeWeight(T&& from, T&& to) {
    if (vertices.find(from) != vertices.end()) {
        for (const auto& [destination, weight] : vertices[from]->edges) {
            if (destination == to) return weight;
        }
    }
    throw std::invalid_argument("This edge does not exist");
    return 0;
}

template <typename T>
void Graph<T>::deleteEdge(const T& from, const T& to) {
    if (areNeighbours(from, to)) {
        int weight = edgeWeight(from, to);
        std::vector<Edge>* fromEdges = &vertices[from]->edges;
        std::vector<Edge>* toEdges = &vertices[to]->edges;

        fromEdges->erase(std::remove(fromEdges->begin(), fromEdges->end(), Edge{to, weight}));
        toEdges->erase(std::remove(toEdges->begin(), toEdges->end(), Edge{from, weight}));
    }
}

template <typename T>
void Graph<T>::deleteEdge(const T& from, T&& to) {
    if (areNeighbours(from, to)) {
        int weight = edgeWeight(from, to);
        std::vector<Edge>* fromEdges = &vertices[from]->edges;
        std::vector<Edge>* toEdges = &vertices[to]->edges;

        fromEdges->erase(std::remove(fromEdges->begin(), fromEdges->end(), Edge{to, weight}));
        toEdges->erase(std::remove(toEdges->begin(), toEdges->end(), Edge{from, weight}));
    }
}

template <typename T>
void Graph<T>::deleteEdge(T&& from, const T& to) {
    if (areNeighbours(from, to)) {
        int weight = edgeWeight(from, to);
        std::vector<Edge>* fromEdges = &vertices[from]->edges;
        std::vector<Edge>* toEdges = &vertices[to]->edges;

        fromEdges->erase(std::remove(fromEdges->begin(), fromEdges->end(), Edge{to, weight}));
        toEdges->erase(std::remove(toEdges->begin(), toEdges->end(), Edge{from, weight}));
    }
}

template <typename T>
void Graph<T>::deleteEdge(T&& from, T&& to) {
    if (areNeighbours(from, to)) {
        int weight = edgeWeight(from, to);
        std::vector<Edge>* fromEdges = &vertices[from]->edges;
        std::vector<Edge>* toEdges = &vertices[to]->edges;

        fromEdges->erase(std::remove(fromEdges->begin(), fromEdges->end(), Edge{to, weight}));
        toEdges->erase(std::remove(toEdges->begin(), toEdges->end(), Edge{from, weight}));
    }
}

template <typename T>
std::list<T> Graph<T>::neighbours(const T& elem) {
    if (vertices.find(elem) != vertices.end()) {
        std::list<T> neighbourList;
        for (const auto& [destination, weight] : vertices[elem]->edges) {
            neighbourList.push_back(destination);
        }
        return neighbourList;
    }

    return {};
}

template <typename T>
bool Graph<T>::isVertex(const T& elem) {
    return vertices.find(elem) != vertices.end();
}

template <typename T>
bool Graph<T>::areNeighbours(const T& elem1, const T& elem2) {
    if (vertices.find(elem1) != vertices.end()) {
        for (const auto& [destination, weight] : vertices[elem1]->edges) {
            if (destination == elem2) return true;
        }
    }

    return false;
}

template <typename T>
int Graph<T>::shortestPathLength(const T& from, const T& to) {
    // DIJKSTRA's ALGORITHM
    std::unordered_map<Vertex*, int> distances;
    std::unordered_map<Vertex*, Vertex*> lastVertex;
    std::unordered_map<Vertex*, bool> visited;

    dijkstrasAlgorithm(from, to, distances, lastVertex, visited);

    return distances[vertices[to]];
}

template <typename T>
std::list<T> Graph<T>::shortestPath(const T& from, const T& to) {
    // DIJKSTRA's ALGORITHM
    std::unordered_map<Vertex*, int> distances;
    std::unordered_map<Vertex*, Vertex*> lastVertex;
    std::unordered_map<Vertex*, bool> visited;

    dijkstrasAlgorithm(from, to, distances, lastVertex, visited);

    std::list<T> thePath;

    Vertex* last = vertices[to];
    while (last != vertices[from]) {
        thePath.push_front(last->vertexName);
        last = lastVertex[last];
    }
    thePath.push_front(from);

    return thePath;
}

template <typename T>
void Graph<T>::bfsSearch(const T& start) {
    if (!vertices.count(start)) {
        std::cerr << "ERROR: " << start << " is not a key" << "\n";
        return;
    }

    std::queue<Vertex*> Q;
    std::unordered_map<Vertex*, Vertex*> parents;
    std::unordered_map<Vertex*, int> level;

    for (auto& [t, vertex] : vertices) {
        parents[vertex] = nullptr;
        level[vertex] = -1;
    }

    Q.push(vertices[start]);
    level[vertices[start]] = 0;
    parents[vertices[start]] = vertices[start];

    while (!Q.empty()) {
        Vertex* v = Q.front();
        Q.pop();

        for (auto& edge : v->edges) {
            Vertex* w = vertices[edge.destinationVertex];
            if (!parents[w]) {
                Q.push(w);
                parents[w] = v;
                level[w] = level[v] + 1;
            }
        }
    }
}

template <typename T>
void Graph<T>::bfsSearch(T&& start) {
    if (!vertices.count(start)) {
        std::cerr << "ERROR: " << start << " is not a key" << "\n";
        return;
    }

    std::queue<Vertex*> Q;
    std::unordered_map<Vertex*, Vertex*> parents;
    std::unordered_map<Vertex*, int> level;

    for (auto& [t, vertex] : vertices) {
        parents[vertex] = nullptr;
        level[vertex] = -1;
    }

    Q.push(vertices[start]);
    level[vertices[start]] = 0;
    parents[vertices[start]] = vertices[start];

    while (!Q.empty()) {
        Vertex* v = Q.front();
        Q.pop();

        for (auto& edge : v->edges) {
            Vertex* w = vertices[edge.destinationVertex];
            if (!parents[w]) {
                Q.push(w);
                parents[w] = v;
                level[w] = level[v] + 1;
            }
        }
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Graph<T>& g) {
    out << "\n";
    for (const auto& [name, vertex] : g.vertices) {
        out << name << ":";
        for (auto& edge : vertex->edges) {
            out << "  ";
            out << edge.destinationVertex << "[" << edge.weight << "]";
        }
        out << "\n";
    }
    return out;
}

template <typename T>
Graph<T>::~Graph() {
    for (auto& [vName, vertex] : vertices) delete vertex;
}

template<typename T>
Graph<T> getGraph() {
    Graph<std::string> canadianMap;

    canadianMap.addVertex("Toronto");
    canadianMap.addVertex("Montreal");
    canadianMap.addVertex("Ottawa");
    canadianMap.addVertex("Edmonton");
    canadianMap.addVertex("Vancouver");
    canadianMap.addVertex("Winnipeg");

    canadianMap.addEdge("Toronto", "Ottawa", 200);
    canadianMap.addEdge("Toronto", "Montreal", 300);
    canadianMap.addEdge("Toronto", "Winnipeg", 470);
    canadianMap.addEdge("Toronto", "Edmonton", 600);

    return canadianMap;
}

void testGraph() {
    Graph<std::string> canadianMap;

    canadianMap.addVertex("Toronto");
    canadianMap.addVertex("Montreal");
    canadianMap.addVertex("Ottawa");
    canadianMap.addVertex("Edmonton");
    canadianMap.addVertex("Vancouver");
    canadianMap.addVertex("Winnipeg");

    std::cout << canadianMap << std::endl;

    canadianMap.addEdge("Toronto", "Ottawa", 200);
    canadianMap.addEdge("Toronto", "Montreal", 300);
    canadianMap.addEdge("Toronto", "Winnipeg", 470);
    canadianMap.addEdge("Toronto", "Edmonton", 600);
    std::cout << canadianMap << std::endl;

    canadianMap.addEdge("Saskatoon", "Swift Current", 150);
    canadianMap.addEdge("Saskatoon", "Swift Current", 150);
    std::cout << canadianMap << std::endl;

    canadianMap.addEdge("Toronto", "Vancouver", 1200);
    std::cout << canadianMap << std::endl;
    canadianMap.deleteEdge("Toronto", "Vancouver");
    std::cout << canadianMap << std::endl;

    for (auto& it : canadianMap.neighbours("Toronto")) {
        LOG(it)
    }

    LOG(canadianMap.areNeighbours("Toronto", "Ottawa"))
    LOG(canadianMap.areNeighbours("Toronto", "Vancouver"))
    LOG(canadianMap.edgeWeight("Toronto", "Ottawa"))

    LOG(canadianMap.isVertex("Toronto"))

    canadianMap.deleteVertex("Toronto");
    std::cout << canadianMap << std::endl;

    LOG(canadianMap.isVertex("Toronto"))

    LOG(canadianMap.shortestPathLength("Saskatoon", "Swift Current"));

    Graph<char> dijkstraTestingGraph;

    dijkstraTestingGraph.addVertex('A');
    dijkstraTestingGraph.addVertex('B');
    dijkstraTestingGraph.addVertex('C');
    dijkstraTestingGraph.addVertex('D');
    dijkstraTestingGraph.addVertex('E');

    dijkstraTestingGraph.addEdge('A', 'B', 11);
    dijkstraTestingGraph.addEdge('A', 'C', 2);
    dijkstraTestingGraph.addEdge('A', 'D', 9);
    dijkstraTestingGraph.addEdge('B', 'D', 13);
    dijkstraTestingGraph.addEdge('B', 'E', 21);
    dijkstraTestingGraph.addEdge('C', 'D', 3);
    dijkstraTestingGraph.addEdge('D', 'E', 5);

    LOG(dijkstraTestingGraph.shortestPathLength('A', 'E'))
    for (auto& node : dijkstraTestingGraph.shortestPath('A', 'E')) {
        LOG(node)
    }

    Graph<std::string> canadianMap2 = canadianMap;
    std::cout << canadianMap << std::endl;
    std::cout << canadianMap2 << std::endl;
    Graph<std::string> canadianMap3;
    canadianMap3 = canadianMap;
    std::cout << canadianMap << std::endl;
    std::cout << canadianMap3 << std::endl;

    Graph<std::string> canadianMap4{getGraph<std::string>()};
    Graph<std::string> canadianMap5;
    canadianMap5 = getGraph<std::string>();
    std::cout << canadianMap4 << std::endl;
    std::cout << canadianMap5 << std::endl;

    std::cout << canadianMap << std::endl;

    std::string city = "Toronto";
    canadianMap.bfsSearch(city);
    canadianMap.bfsSearch("Toronto");

    std::cout << canadianMap5 << std::endl;
    canadianMap5.bfsSearch(city);
    canadianMap5.bfsSearch("Toronto");
}

int main() {
    testGraph();
}
