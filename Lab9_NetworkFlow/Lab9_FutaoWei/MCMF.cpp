#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define INT_MAX 2147483647

int total_flow = 0;
int total_cost = 0;
int n, m, source, sink;

vector<vector<bool>> graph_matrix;
vector<vector<int>> capacity_matrix;
vector<vector<int>> cost_matrix;

void MCMF() {
    while (true) {
        vector<int> M(n+1, INT_MAX);
        vector<int> M_history(n+1, INT_MAX);
        vector<int> predecessor(n+1, -1);
        
        M[source] = 0;
        
        // find the least cost path
        for (int i=1; i<=n-1; ++i) {
            bool flag = false;
            for (int j=1; j<=n; ++j) {
                if (M[j] != M_history[j]) {
                    for (int k=1; k<=n; ++k) {
                        if (graph_matrix[j][k] && M[k]>M[j]+cost_matrix[j][k]) {
                            M_history[k] = M[k];
                            M[k] = M[j] + cost_matrix[j][k];
                            predecessor[k] = j;
                            flag = true;
                        }
                    }
                }
            }
            if (!flag) break;
        }
        
        if (M[sink] == INT_MAX) break;
        
        // find the bottleneck capacity
        int temp = sink;
        int bottleneck_capacity = INT_MAX;
        while (temp != source) {
            int pred = predecessor[temp];
            if (graph_matrix[pred][temp] && capacity_matrix[pred][temp]<bottleneck_capacity) {
                bottleneck_capacity = capacity_matrix[pred][temp];
            }
            temp = pred;
        }
        
        // augment flow along the least cost path
        temp = sink;
        while (temp != source) {
            int pred = predecessor[temp];
            if (graph_matrix[pred][temp]) {
                graph_matrix[temp][pred] = true;
                capacity_matrix[temp][pred] += bottleneck_capacity;
                cost_matrix[temp][pred] = -1 * cost_matrix[pred][temp];
                
                capacity_matrix[pred][temp] -= bottleneck_capacity;
                if (capacity_matrix[pred][temp] == 0) {
                    graph_matrix[pred][temp] = false;
                }
                
                total_cost += bottleneck_capacity * cost_matrix[pred][temp];
            }
            temp = pred;
        }
        total_flow += bottleneck_capacity;
    }
}

int main() {
    ifstream infile("MCMF.in");
    
    infile >> n >> m >> source >> sink;
    
    for (int i=0; i<=n; ++i) {
        vector<bool> temp1(n+1, false);
        graph_matrix.push_back(temp1);
        vector<int> temp2(n+1, 0);
        capacity_matrix.push_back(temp2);
        cost_matrix.push_back(temp2);
    }
    
    for (int i=0; i<m; ++i) {
        int u, v, c, a;
        infile >> u >> v >> c >> a;
        graph_matrix[u][v] = true;
        capacity_matrix[u][v] = c;
        cost_matrix[u][v] = a;
    }
    
    infile.close();
    
    MCMF();
    
    cout << '\n' << total_flow << ' ' << total_cost << endl;
    
    return 0;
}
