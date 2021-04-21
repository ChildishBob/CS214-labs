#include <iostream>
using namespace std;

const int MAX_N = 2100;
int t[MAX_N];
int w[MAX_N];
int dp[MAX_N][MAX_N*2];
int n;


int main() {
    cin >> n;
    int total_thick = 0;
    for (int i=1; i<=n; ++i) {
        cin >> t[i] >> w[i];
        total_thick += t[i];
    }
    for (int i=0; i<=n; ++i) {
        for (int j=0; j<=total_thick; ++j) {
            dp[i][j] = 999999;
        }
    }
    dp[0][0] = 0;
    for (int i=1; i<=n; ++i) {
        for (int j=1; j<=total_thick; ++j) {
            dp[i][j] = dp[i-1][j] + w[i];
            if (j >= t[i]) {
                dp[i][j] = min(dp[i][j], dp[i-1][j-t[i]]);
            }
        }
    }
    for (int j=1; j<=total_thick; ++j) {
        if (dp[n][j] <= j) {
            cout << "minimum bookshelf length: " << j << endl;
            break;
        }
    }
    return 0;
}
