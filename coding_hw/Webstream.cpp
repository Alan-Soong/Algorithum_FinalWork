#include<iostream>
#include<vector>
#include<queue>
#include<climits>
using namespace std;

class Graph {
    int V;
    int E;
    vector<int> graph[200];
    vector<vector<int>> capacity;
    int level[404], ptr[404];
    vector<vector<int>> allPaths;
public:
    Graph(int v, int e) {
        V = v;
        E = e;
        capacity.resize(404, vector<int>(404, 0));
    }
    void append(int start, int end);
    bool bfs(int source, int sink);
    int dfs(int u, int sink, int flow);
    int maxFlow();
    void findPaths();
    void printPaths();
};

void Graph::append(int start, int end) {
    int u = start - 1;
    int v = end - 1;
    graph[u].push_back(v);
}

bool Graph::bfs(int source, int sink) {
    for (int i = 0; i < 2 * V + 2; i++) {
        level[i] = -1;
    }
    queue<int> q;
    level[source] = 0;
    q.push(source);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v = 0; v < 2 * V + 2; v++) {
            if (level[v] < 0 && capacity[u][v] > 0) {
                level[v] = level[u] + 1;
                q.push(v);
            }
        }
    }
    return level[sink] >= 0;
}

int Graph::dfs(int u, int sink, int flow) {
    if (u == sink) {
        return flow;
    }
    for (int& i = ptr[u]; i < 2 * V + 2; i++) {
        if (level[i] == level[u] + 1 && capacity[u][i] > 0) {
            int f = dfs(i, sink, min(flow, capacity[u][i]));
            if (f > 0) {
                capacity[u][i] -= f;
                capacity[i][u] += f;
                return f;
            }
        }
    }
    return 0;
}

int Graph::maxFlow() {
    int s = 2 * V;
    int t = 2 * V + 1;

    // 源点到左部点 (0 到 V-1)
    for (int i = 0; i < V; i++) {
        capacity[s][i] = 1;
    }

    // 右部点到汇点 (V 到 2V-1)
    for (int i = V; i < 2 * V; i++) {
        capacity[i][t] = 1;
    }

    // 原图中的边 (u -> v) 转化为 u -> v+V
    for (int u = 0; u < V; u++) {
        for (int v : graph[u]) {
            capacity[u][v + V] = 1;
        }
    }

    int flow = 0;
    while (bfs(s, t)) {
        for (int i = 0; i < 2 * V + 2; i++) {
            ptr[i] = 0;
        }
        int f;
        while ((f = dfs(s, t, INT_MAX)) > 0) {
            flow += f;
        }
    }
    return flow;
}

void Graph::findPaths() {
    vector<int> next(V, -1);       // 每个点的下一个点
    vector<bool> isPrev(V, false); // 是否有前驱

    // 检查所有可能的边，找到匹配
    for (int u = 0; u < V; u++) {
        for (int v : graph[u]) {
            if (capacity[u][v + V] == 0) { // 有流量经过
                next[u] = v;
                isPrev[v] = true;
            }
        }
    }

    vector<bool> visited(V, false);

    // 从没有前驱的顶点开始构建路径
    for (int i = 0; i < V; i++) {
        if (!isPrev[i] && !visited[i]) {
            vector<int> path;
            int cur = i;
            while (cur != -1 && !visited[cur]) {
                visited[cur] = true;
                path.push_back(cur + 1); // 转换为1-based索引
                cur = next[cur];
            }
            allPaths.push_back(path);
        }
    }

    // 处理独立顶点（没有前驱也没有后继的顶点）
    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            allPaths.push_back({ i + 1 });
        }
    }
}

void Graph::printPaths() {
    for (const auto& path : allPaths) {
        cout << path.size();
        for (int num : path) {
            cout << " " << num;
        }
        cout << endl;
    }
}

int main() {
    int v, e;
    cin >> v >> e;
    Graph g(v, e);
    int start, end;
    for (int i = 0; i < e; i++) {
        cin >> start >> end;
        g.append(start, end);
    }

    int maxF = g.maxFlow();
    int minPathCover = v - maxF;
    cout << minPathCover << endl;
    g.findPaths();
    g.printPaths();
    return 0;
}