#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
//Please put this source code in the same directory with SCC.in
//And do NOT change the file name.
/*
This function computes the number of Strongly Connected Components in a graph
Args:
    n: The number of nodes in the graph. The nodes are indexed as 0~n-1
    edge: The edges in the graph. For each element (a,b) in edge, it means
          there is a directed edge from a to b
          Notice that there may exists multiple edge and self-loop
Return:
    The number of strongly connected components in the graph.
*/

void previsit(int v, int &clock, int *pre);
void postvisit(int v, int &clock, int *post);
void explore(int v, vector<pair<int,int>> &edge, int *&visited, int &clock,int *&prev,int *&post);
void explore(int v, vector<pair<int,int>> &edge, int *&visited);
void dfs(int n, vector<pair<int,int>> &edge, int *&prev, int *&post);
void delete_sink(int n, vector<pair<int,int>> &edge, int *&pre, int *&post, int &num_node);
int count_SCC(int n, vector<pair<int,int>> &edge, int &num_SCC);
int SCC(int n, vector<pair<int,int>> &edge);

//Please do NOT modify anything in main(). Thanks!
int main()
{
    int m,n;
    vector<pair<int,int>> edge;
    ifstream fin;
    ofstream fout;
    fin.open("SCC.in");
    fin>>n>>m;
    int tmp1,tmp2;
    for(int i=0;i<m;i++)
    {
        fin>>tmp1>>tmp2;
        edge.push_back(pair<int,int>(tmp1,tmp2));
    }
    fin.close();
    int ans=SCC(n,edge);
    fout.open("SCC.out");
    fout<<ans<<'\n';
    fout.close();
    return 0;
}

void previsit(int v, int &clock, int *pre)
{
    pre[v] = clock;
    ++clock;
}
void postvisit(int v, int &clock, int *post)
{
    post[v] = clock;
    ++clock;
}
void explore(int v, vector<pair<int,int>> &edge, int *&visited, int &clock,int *&prev,int *&post) {
    visited[v] = 1;

    previsit(v, clock, prev);
    
    for (int i=0; i<edge.size(); ++i) {
        if(edge.at(i).first == v) {
            int u = edge.at(i).second;
            if(visited[u] == 0) {
                explore(u, edge, visited, clock, prev, post);
            }
        }

    }
    
    postvisit(v, clock, post);
}


void explore(int v, vector<pair<int,int>> &edge, int *&visited) {
    visited[v] = 1;

    for (int i=0; i<edge.size(); ++i) {
        if(edge.at(i).first == v) {
            int u = edge.at(i).second;
            if(visited[u] == 0) {
                explore(u, edge, visited);
            }
        }
    }
}
void dfs(int n, vector<pair<int,int>> &edge, int *&prev, int *&post) {
    int *visited = new int[n];
    
    for(int i=0; i<n; ++i) {
        visited[i] = 0;
    }
    
    int clock = 1;
    for (int i=0; i<n; ++i) {
        if (visited[i] == 0) {
            explore(i, edge, visited, clock, prev, post);
        }
    }
        
    delete [] visited;
}
void delete_sink(int n, vector<pair<int,int>> &edge, int *&pre, int *&post, int &num_node) {
    int max_post = 0;
    int max_post_node=0;
    for(int i=0; i<n; ++i) {
        if(post[i] > max_post) {
            max_post = post[i];
            max_post_node = i;
        }
    }

    int *visited = new int[n];

    for(int i=0; i<n; ++i) {
        visited[i] = 0;
    }

    explore(max_post_node, edge, visited);

    for(int i=0; i<n; ++i) {
        if(visited[i]) {
            ++num_node;
            for (int j=0; j<edge.size(); ++j) {
                if(edge.at(j).first == i || edge.at(j).second == i) {
                    edge.erase(edge.begin()+j);
                }
            }
        }
    }
}
int count_SCC(int n, vector<pair<int,int>> &edge, int &num_SCC) {
    
    vector<pair<int,int>> edge_R;

    for(int i=0; i<edge.size(); ++i) {
        edge_R.push_back(pair<int,int>(edge.at(i).second, edge.at(i).first));
    }

    int *prev = new int[n];
    int *post = new int[n];
    
    dfs(n, edge_R, prev, post);

    int num_node = 0;
    delete_sink(n, edge, prev, post, num_node);
    
    delete [] prev;
    delete [] post;

    if(num_node == n) {
        return num_SCC;
    }
    else {
        ++num_SCC;
        return count_SCC(n-num_node, edge, num_SCC);
    }
}
int SCC(int n, vector<pair<int,int>> &edge) {
    int num_SCC = 0;  // # of SCC
    int res = count_SCC(n, edge, num_SCC);
    return res;
}
