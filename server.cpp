#include "std_lib_facilities.h"
#include <thread>
#include <mutex>
#include <unordered_map>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <queue>
#include <unistd.h>

struct Edge {
    int src, dest;
};

vector<Edge> readGraphFromFile(const string& filename) {
    vector<Edge> edges;
ifstream file(filename);
if (!file) {
    cerr << "Unable to open file: " << filename << endl;
}
int src, dest;
while (file >> src >> dest) {
    edges.push_back({src, dest});
}
    return edges;
}

vector<int> bfs(const vector<Edge>& edges, int src, int dest) {
    unordered_map<int, vector<int>> adj;
    for (const Edge& e : edges) {
        adj[e.src].push_back(e.dest);
        adj[e.dest].push_back(e.src); 
    }

    vector<int> parent(adj.size(), -1);
    queue<int> q;
    q.push(src);
    parent[src] = src;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u == dest) break;

        for (int v : adj[u]) {
            if (parent[v] == -1) {
                parent[v] = u;
                q.push(v);
            }
        }
    }

    vector<int> path;
    if (parent[dest] != -1) {
        for (int v = dest; v != src; v = parent[v])
            path.push_back(v);
        path.push_back(src);
        reverse(path.begin(), path.end());
    }

    return path;
}

unordered_map<string, vector<int>> cache;
mutex mtx;

vector<int> processRequest(const vector<Edge>& edges, int src, int dest) {
    string key = to_string(src) + "-" + to_string(dest);
    lock_guard<mutex> lck(mtx);
    auto it = cache.find(key);
    if (it != cache.end()) return it->second;

    vector<int> path = bfs(edges, src, dest);
    cache[key] = path;
    if (cache.size() > 10) cache.erase(cache.begin());
    return path;
}

void handleClient(int clientSocket, const vector<Edge>& edges) {
    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) return;
    buffer[bytesRead] = '\0';

    int src, dest;
    sscanf(buffer, "%d %d", &src, &dest);

    vector<int> path = processRequest(edges, src, dest);
    string response = accumulate(path.begin(), path.end(), string(), [](string s, int v) { return s + to_string(v) + " "; });
    response += "\n";
    send(clientSocket, response.c_str(), response.length(), 0);
    close(clientSocket);
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
    cerr << "Usage: " << argv[0] << " <filename> <port>" << endl;
    return 1;
}

    string filename = argv[1];
    int port = atoi(argv[2]);

    vector<Edge> edges = readGraphFromFile(filename);

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    listen(serverSocket, 5);
    vector<thread> threads;
    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        threads.push_back(thread(handleClient, clientSocket, ref(edges)));
    }

    for (thread& t : threads)
        t.join();
}