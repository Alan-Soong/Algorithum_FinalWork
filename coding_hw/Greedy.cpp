#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef long long int ll;

//重命名结点结构体
struct Node {
	ll start;
	ll end;
	ll weight;
	ll order;
	Node(ll a, ll b, ll c, ll d) {
		start = a;
		end = b;
		weight = c;
		order = d;
	}
	//重载运算符
	bool operator<(const Node& a)const
	{
		if (weight == a.weight) return order < a.order;
		return weight < a.weight;
	}
	bool operator>(const Node& a)const
	{
		if (weight == a.weight) return order > a.order;
		return weight > a.weight;
	}
	bool operator==(const Node& a)const
	{
		return weight == a.weight;
	}
};

//图结构
class Graph {
public:
	ll V, E;
	vector<Node> weight;
	vector<int> father_DSU;	//并查集数组

	Graph(ll v, ll e) {
		V = v;
		E = e;
		for (ll i = 0; i < V; i++) {
			father_DSU.push_back(i);
		}
	}
	void append_edge(ll a, ll b, ll c, ll d) {
		if (a == b) return;
		else {
			Node tmp(a, b, c, d);
			weight.push_back(tmp);
		}
	}

	//并查集相关
	ll find(ll x) {
		if (x != father_DSU[x]) {
			father_DSU[x] = find(father_DSU[x]);
		}
		return father_DSU[x];
	}
	bool father_DSU_mst(ll a, ll b) {
		ll fa = find(a);
		ll fb = find(b);
		if (fa != fb) {
			father_DSU[fb] = fa;
			return true;
		}
		else {
			return false;
		}
	}
};

ll V, E;	//图的结点和边

//构建优先队列（最小堆）
priority_queue<Node, vector<Node>, greater<Node>> min_Node(vector<Node> weight) {
	priority_queue<Node, vector<Node>, greater<Node>> weight_pre;
	for (ll i = 0; i < weight.size(); i++) {
		weight_pre.push(weight[i]);
	}
	return weight_pre;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> V >> E;
	Graph graph(V, E);
	for (ll i = 0; i < E; i++) {
		ll a, b, c;
		cin >> a >> b >> c;
		graph.append_edge(a - 1, b - 1, c, i);
	}

	ll sum = 0, xr = 0, visited = 0;
	priority_queue<Node, vector<Node>, greater<Node>> tmp = min_Node(graph.weight);
	
	while (!tmp.empty() && visited < V - 1) {
		Node curr = tmp.top();
		
		//两节点相同祖先，可以插入这条边
		if (graph.father_DSU_mst(curr.start, curr.end)) {
			sum += curr.weight;
			xr ^= (curr.order + 1);
			visited++;
		}

		tmp.pop();
	}

	//每个点都已经访问到
	if (visited == V - 1) {
		cout << sum << " " << xr << endl;
	}
	else {
		cout << -1 << endl;
	}
	return 0;
}