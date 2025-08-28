#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <utility>
#include <climits>
#include <cmath>
#include <numeric>
#include <bits/stdc++.h>
using namespace std;
#define inf 9999

// Name: Saumya Sadh
// PID: A17043544

///////////////////////////////////////////////////////
//////////////       QUESTION 1 HERE   ////////////////
///////////////////////////////////////////////////////
pair<int, vector<int>> myCount(vector<int> L)
{
  // helper: merges two sorted halves of L[l..mid], L[mid+1..r]
  function<long long(int,int)> sort_and_count = [&](int l, int r)->long long {
    if (l >= r) return 0;
    int m = l + (r - l) / 2;
    long long inv = 0;
    inv += sort_and_count(l, m);
    inv += sort_and_count(m + 1, r);

    // merge
    vector<int> tmp;
    tmp.reserve(r - l + 1);
    int i = l, j = m + 1;
    while (i <= m && j <= r) {
        if (L[i] <= L[j]) {
            tmp.push_back(L[i++]);
        } else {
            tmp.push_back(L[j++]);
            inv += (m - i + 1); // all remaining in left are > L[j-1]
        }
    }
    while (i <= m) tmp.push_back(L[i++]);
    while (j <= r) tmp.push_back(L[j++]);
    // copy back
    for (int k = 0; k < (int)tmp.size(); ++k) L[l + k] = tmp[k];
    return inv;
};

long long inv = sort_and_count(0, (int)L.size() - 1);
return { (int)inv, L };
  /*
    TODO:

    Write an algorithm that solves the counting inversion problem.

    Input:
        vector<int> L: a non-empty array of distinct integers
        
    return:
        pair<int, vector<int>: A pair of the following elements in the exact order:
        0: int: the number of inversions in L 
        1: vector<int>: an array containing the elements of L, but sorted
    
  */
}

/////////////////////////////////////////////////////////
//////////////       QUESTION 2 HERE   //////////////////
/////////////////////////////////////////////////////////
tuple<vector<int>, vector<int>, int> mySimplexLP(vector<vector<int>> A, vector<int> B, vector<int> C)
{
    const double EPS = 1e-12;

    int m = (int)A.size();
    int n = (int)(A.empty() ? 0 : A[0].size());
    int total_vars = n + m;           // original x plus slacks
    int rows = m + 1;                 // m constraints + 1 objective
    int cols = total_vars + 1;        // all vars + RHS

    // Build tableau: [ A | I | B ] with objective row "z - Cx = 0"
    vector<vector<double>> tab(rows, vector<double>(cols, 0.0));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) tab[i][j] = (double)A[i][j];
        tab[i][n + i] = 1.0;                 // slack si
        tab[i][cols - 1] = (double)B[i];     // RHS
    }
    // Objective row: -C under x, 0 elsewhere, RHS = 0
    for (int j = 0; j < n; ++j) tab[m][j] = -(double)C[j];

    // Initial basis = slacks
    vector<int> basis(m);
    iota(basis.begin(), basis.end(), n); // n..n+m-1

    auto entering = [&]() -> int {
        // For maximization with row "reduced costs", pick most negative
        int col = -1;
        double most_neg = -EPS; // need strictly < -EPS to improve
        for (int j = 0; j < total_vars; ++j) {
            if (tab[m][j] < most_neg) {
                most_neg = tab[m][j];
                col = j;
            }
        }
        return col; // -1 => optimal
    };

    auto leaving = [&](int col) -> int {
        int row = -1;
        double best_ratio = 0.0;
        for (int i = 0; i < m; ++i) {
            double a = tab[i][col];
            if (a > EPS) {
                double ratio = tab[i][cols - 1] / a;
                if (ratio < -EPS) continue; // negative RHS shouldn’t happen in Phase II
                if (row == -1 || ratio < best_ratio - EPS ||
                    (fabs(ratio - best_ratio) <= EPS && i < row)) {
                    best_ratio = ratio;
                    row = i;
                }
            }
        }
        return row; // -1 => unbounded
    };

    auto pivot = [&](int r, int c) {
        double p = tab[r][c];
        // Normalize pivot row
        for (int j = 0; j < cols; ++j) tab[r][j] /= p;
        // Eliminate pivot column from other rows
        for (int i = 0; i < rows; ++i) if (i != r) {
            double f = tab[i][c];
            if (fabs(f) > EPS) {
                for (int j = 0; j < cols; ++j)
                    tab[i][j] -= f * tab[r][j];
            }
        }
        basis[r] = c;
    };

    // Main simplex loop
    while (true) {
        int ent = entering();
        if (ent == -1) break; // optimal
        int lv = leaving(ent);
        if (lv == -1) {
            // Unbounded (shouldn’t occur in your tests)
            return {vector<int>(n,0), vector<int>(m,0), 0};
        }
        pivot(lv, ent);
    }

    // Extract solution
    vector<double> sol(total_vars, 0.0);
    for (int i = 0; i < m; ++i) {
        if (basis[i] >= 0 && basis[i] < total_vars)
            sol[basis[i]] = tab[i][cols - 1];
    }

    vector<int> x(n, 0), slack(m, 0);
    for (int j = 0; j < n; ++j)   x[j]    = (int)llround(sol[j]);
    for (int i = 0; i < m; ++i)   slack[i]= (int)llround(sol[n + i]);

    int value = (int)llround(tab[m][cols - 1]); // z is at RHS of objective row

    return {x, slack, value};
}