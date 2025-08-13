#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int N = 5005;

class Graph {
public:
	int V, E;
	vector<int> inDegree;
	vector<vector<int>> matrix;

	Graph(int v, int e) {
		V = v;
		E = e;
		inDegree.resize(v, 0);
		matrix.resize(v);
	}
	void append_edge(int a, int b, bool& flag) {
		if (a == b) flag = true; // 标记是否存在自环
		else {
			matrix[b].push_back(a); // 逆向建图
			//matrix[a].push_back(b); // 逆向建图
			inDegree[a]++;
			//inDegree[b]++;
		}
	}
};

void ts_DFS(Graph& graph, vector<int>& result, bool& flag) {
	if (flag) return; // 自环直接返回

	priority_queue<int, vector<int>, less<int>> find_order; // 最大堆
	vector<bool> visited(graph.V, false);	//是否被访问

	// 找到所有入度为 0 的点
	for (int i = 0; i < graph.V; i++) {
		if (graph.inDegree[i] == 0) {
			find_order.push(i);
		}
	}

	// 主程序
	while (!find_order.empty()) {
		int tmp = find_order.top();
		find_order.pop();

		if (!visited[tmp]) {
			result.push_back(tmp);
			visited[tmp] = true;
		}

		for (int j : graph.matrix[tmp]) { // 不知道个数有多少，所以使用这种遍历
			if (visited[j]) continue; // 跳过已访问节点
			graph.inDegree[j]--;
			if (graph.inDegree[j] == 0) {
				find_order.push(j);
			}
		}
	}

	int p = visited.size();
	for (int i = 0; i < p; i++) {
		if (!visited[i]) {
			flag = true; //存在环路（1->2, 2->1）
		}
	}
}

void PrintList(vector<int>& result, bool flag) {
	if (!result.empty() && !flag) {
		for (int i = result.size() - 1; i >= 0; i--) { // 直接倒序输出
			cout << result[i] + 1 << " ";
		}
		cout << endl;
	}
	else {
		cout << "Impossible!" << endl;
	}
}

int main() {
	int size;
	cin >> size;
	while (size--) {
		int V, E;
		cin >> V >> E;
		Graph graph(V, E);
		vector<int> result;
		bool flag = false;

		for (int j = 0; j < E; j++) {
			int start, end;
			cin >> start >> end;
			graph.append_edge(start - 1, end - 1, flag);
		}
		ts_DFS(graph, result, flag);
		PrintList(result, flag);
	}
	return 0;
}