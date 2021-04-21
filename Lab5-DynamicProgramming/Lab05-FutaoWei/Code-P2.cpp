#include <iostream>
#include <string>
#include <stack>

using namespace std;

string seqA;
string seqB;
int cost_matrix['T'+1]['T'+1];

struct node {
    int x;
    int y;
    node(int xx=0, int yy=0) {
        x = xx;
        y = yy;
    }
};

stack<node> node_stack;

int get_min_in_three_num(int a, int b, int c) {
    int min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    return min;
}

void Divide_and_Conquer_Alignment(int start1, int end1, int start2, int end2) {
    if (end2 - start2 == 1) {
        int **dp = new int* [end1 - start1 + 1];
        for (int i=0; i <= end1-start1; ++i) {
            dp[i] = new int [2];
        }
        
        dp[0][0] = 0;
        for (int i=1; i <= end1-start1; i++) {
            dp[i][0] = dp[i-1][0] + cost_matrix[seqA[start1+i]]['-'];
        }
        
        dp[0][1] = dp[0][0] + cost_matrix[seqB[start2+1]]['-'];
        for (int i=1; i <= end1-start1; i++) {
            int match_cost = dp[i-1][0] + cost_matrix[seqA[start1+i]][seqB[start2+1]];
            int unmatchA_cost = dp[i-1][1] + cost_matrix[seqA[start1+i]]['-'];
            int unmatchB_cost = dp[i][0] + cost_matrix[seqB[start2+1]]['-'];
            dp[i][1] = get_min_in_three_num(unmatchB_cost, match_cost, unmatchA_cost);
        }
        
        int col_num = 1;
        int row_num = end1 - start1;
        while (true) {
            if (row_num > 0 && col_num > 0) {
                if (dp[row_num][1] == dp[row_num][0] + cost_matrix[seqB[start2+1]]['-']) {
                    col_num = 0;
                    node temp;
                    temp.x = row_num+start1;
                    temp.y = start2;
                    node_stack.push(temp);
                }
                else if (dp[row_num][1] == dp[row_num - 1][1] + cost_matrix[seqA[start1+row_num]]['-']) {
                    --row_num;
                    node temp;
                    temp.x = row_num+start1;
                    temp.y = start2+1;
                    node_stack.push(temp);
                }
                else {
                    --row_num;
                    col_num = 0;
                    if (row_num != 0) {
                        node temp;
                        temp.x = row_num + start1;
                        temp.y = start2;
                        node_stack.push(temp);
                    }
                    else break;
                }
            }

            if (col_num == 0 && row_num > 0) {
                while (row_num > 1) {
                    --row_num;
                    node temp;
                    temp.x = row_num + start1;
                    temp.y = start2;
                    node_stack.push(temp);
                }
                break;
            }
            else if (row_num == 0) {
                break;
            }
        }
        
        for (int i=0; i <= end1-start1; ++i) {
            delete [] dp[i];
        }
        delete [] dp;
        
        return;
    }
    else {
        int **dp1 = new int* [end1 - start1 + 1];
        for (int i=0; i <= end1-start1; ++i) {
            dp1[i] = new int [2];
        }
        
        dp1[0][0] = 0;
        for (int i=1; i <= end1-start1; ++i) {
            dp1[i][0] = dp1[i-1][0] + cost_matrix[seqA[start1+i]]['-'];
        }

        for (int i=1; i <= (start2 + end2) / 2-start2; ++i) {
            dp1[0][1] = dp1[0][0] + cost_matrix[seqB[start2+i]]['-'];
            for (int j=1; j <= end1-start1; ++j) {
                int match_cost = dp1[j-1][0] + cost_matrix[seqA[start1 + j]][seqB[start2 + i]];
                int unmatchA_cost = dp1[j-1][1] + cost_matrix[seqA[start1 + j]]['-'];
                int unmatchB_cost = dp1[j][0] + cost_matrix['-'][seqB[start2 + i]];
                dp1[j][1] = get_min_in_three_num(match_cost, unmatchA_cost, unmatchB_cost);
            }
            for (int j=0; j <= end1-start1; ++j) {
                dp1[j][0] = dp1[j][1];
            }
        }

        int **dp2 = new int* [end1 - start1 + 1];
        for (int i=0; i <= end1-start1; ++i) {
            dp2[i] = new int [2];
        }
        dp2[0][0] = 0;
        for (int i=1; i <= end1-start1; i++) {
            dp2[i][0] = dp2[i-1][0] + cost_matrix[seqA[end1-i+1]]['-'];
        }

        for (int i = end2-(start2+end2)/2; i>=1; --i) {
            dp2[0][1] = dp2[0][0] + cost_matrix[seqB[(start2+end2)/2 + i]]['-'];
            for (int j=1; j <= end1-start1; ++j) {
                int match_cost = dp2[j - 1][0] + cost_matrix[seqA[end1-j+1]][seqB[(start2+end2)/2 + i]];
                int unmatchA_cost = dp2[j - 1][1] + cost_matrix[seqA[end1-j+1]]['-'];
                int unmatchB_cost = dp2[j][0] + cost_matrix['-'][seqB[(start2+end2)/2+i]];
                dp2[j][1] = get_min_in_three_num(match_cost, unmatchA_cost, unmatchB_cost);
            }
            for (int j=0; j <= end1-start1; ++j) {
                dp2[j][0] = dp2[j][1];
            }
        }
        
        int row_num = 0;
        int min = 999999999;
        for (int i=0; i <= end1-start1; ++i) {
            if (dp1[i][1] + dp2[end1 - start1 - i][1] < min) {
                min = dp1[i][1] + dp2[end1 - start1 - i][1];
                row_num = i + start1;
            }
        }
        
        node temp;
        temp.x= row_num;
        temp.y = (start2+end2) / 2;
        node_stack.push(temp);
        
        Divide_and_Conquer_Alignment(start1, row_num, start2, (start2+end2) / 2);
        Divide_and_Conquer_Alignment(row_num, end1, (start2+end2) / 2, end2);
        
        for (int i=0; i <= end1-start1; ++i) {
            delete [] dp1[i];
        }
        delete [] dp1;
        
        for (int i=0; i <= end1 - start1; ++i) {
            delete [] dp2[i];
        }
        delete [] dp2;
    }
}

int main() {
    getline(cin, seqA);
    getline(cin, seqB);
    
    cost_matrix['-']['A'] = cost_matrix['-']['G'] = cost_matrix['A']['-'] = cost_matrix['A']['T'] = cost_matrix['A']['C'] = 1;
    cost_matrix['T']['A'] = cost_matrix['T']['C'] = cost_matrix['G']['-'] = cost_matrix['G']['C'] = cost_matrix['C']['A'] = cost_matrix['C']['T'] = cost_matrix['C']['G'] = 1;
    cost_matrix['T']['-'] = cost_matrix['-']['T'] = 2;
    cost_matrix['-']['C'] = cost_matrix['C']['-'] = 3;
    cost_matrix['-']['-'] = cost_matrix['A']['A'] = cost_matrix['T']['T'] = cost_matrix['G']['G'] = cost_matrix['C']['C'] = 0;
    cost_matrix['A']['G'] = cost_matrix['G']['A'] = 5;
    cost_matrix['T']['G'] = cost_matrix['G']['T'] = 9;
    
    seqA = '0' + seqA;
    seqB = '0' + seqB;
    node start_node(0, 0);
    node_stack.push(start_node);
    node dst_node(seqA.size()-1, seqB.size()-1);
    node_stack.push(dst_node);
    
    Divide_and_Conquer_Alignment(0, seqA.size()-1, 0, seqB.size()-1);
    
    int node_num = node_stack.size();
    node* node_ary = new node[node_num];
    int temp = 0;
    while (!node_stack.empty()) {
        node_ary[temp] = node_stack.top();
        node_stack.pop();
        ++temp;
    }

    for (int i=0; i < temp-1; ++i) {
        for (int j=0; j < temp-i-1; ++j) {
            if (node_ary[j].x > node_ary[j+1].x) {
                swap(node_ary[j].x, node_ary[j+1].x);
                swap(node_ary[j].y, node_ary[j+1].y);
            }
            else if (node_ary[j].x == node_ary[j+1].x) {
                if (node_ary[j].y > node_ary[j + 1].y) {
                    swap(node_ary[j].x, node_ary[j+1].x);
                    swap(node_ary[j].y, node_ary[j+1].y);
                }
            }
        }
    }
    
    string seq1_prime;
    string seq2_prime;
    int edit_dist = 0;
    for (int i=0; i < node_num-1; ++i) {
        int row = node_ary[i+1].x - node_ary[i].x;
        int col = node_ary[i+1].y - node_ary[i].y;
        if (row == 0 && col == 1) {
            seq1_prime += '-';
            seq2_prime += seqB[node_ary[i+1].y];
            edit_dist += cost_matrix[seqB[node_ary[i+1].y]]['-'];
        }
        else if (row == 1 && col == 0) {
            seq1_prime += seqA[node_ary[i+1].x];
            seq2_prime += '-';
            edit_dist += cost_matrix[seqA[node_ary[i+1].x]]['-'];
        }
        else if (row == 1 && col == 1) {
            seq1_prime += seqA[node_ary[i+1].x];
            seq2_prime += seqB[node_ary[i+1].y];
            edit_dist += cost_matrix[seqA[node_ary[i + 1].x]][seqB[node_ary[i + 1].y]];
        }
    }
    cout << "Sequence A'：" << seq1_prime << "\nSequence B'：" << seq2_prime << "\nEdit distance: " << edit_dist << endl;
    
    delete [] node_ary;
    return 0;
}


