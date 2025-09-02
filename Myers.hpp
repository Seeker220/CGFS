#ifndef MYERS_HPP
#define MYERS_HPP

#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <sstream>
#include <iostream>

struct DiffLine {
    // struct for line and status
    char c;
    std::string line;

    DiffLine(int i, const std::string &line) {
        if (i == 0) {
            c = ' ';
        } else if (i == 1) {
            c = '+';
        } else {
            c = '-';
        }
        this->line = line;
    }
};

inline std::vector<DiffLine> getdifflines(const std::vector<std::string> &A, const std::vector<std::string> &B,
                                          const std::vector<std::vector<std::pair<int, int> > > &parent) {
    // returns vector of difflines when end is reached
    int m = A.size();
    int n = B.size();
    int i = n;
    int j = m;
    std::vector<DiffLine> difflines;
    while (i > 0 || j > 0) {
        if (parent[i][j].first == i - 1 && parent[i][j].second == j - 1) {
            difflines.push_back({0, B[i - 1]});
        } else if (parent[i][j].first == i && parent[i][j].second == j - 1) {
            difflines.push_back({2, A[j - 1]});
        } else if (parent[i][j].first == i - 1 && parent[i][j].second == j) {
            difflines.push_back({1, B[i - 1]});
        }
        std::pair<int, int> temp = parent[i][j];
        i = temp.first;
        j = temp.second;
    }
    return std::vector<DiffLine>(difflines.rbegin(), difflines.rend());
}

inline std::vector<DiffLine> diff(const std::vector<std::string> &A, const std::vector<std::string> &B) {
    // Implementation of Myers Algorithm using BFS. https://blog.jcoglan.com/2017/02/12/the-myers-diff-algorithm-part-1/
    int m = A.size();
    int n = B.size();
    std::vector<DiffLine> difflines;
    if (m == 0 && n == 0) {
        return difflines;
    }
    if (m == 0) {
        for (int i = 0; i < n; i++) {
            difflines.push_back({1, B[i]});
        }
        return difflines;
    }
    if (n == 0) {
        for (int i = 0; i < m; i++) {
            difflines.push_back({2, A[i]});
        }
        return difflines;
    }
    std::queue<std::pair<int, int> > q;
    std::vector<std::vector<std::pair<int, int> > > parent(n + 1, std::vector<std::pair<int, int> >(m + 1, {-1, -1}));
    q.push({0, 0});
    while (!(q.empty())) {
        std::pair<int, int> c = q.front();
        q.pop();
        if (c == std::pair<int, int>(0, 0)) {
            int i = 0;
            int j = 0;
            while (i + 1 <= n && j + 1 <= m) {
                if (A[j] == B[i]) {
                    parent[i + 1][j + 1] = {i, j};
                    if (i + 1 == n && j + 1 == m) {
                        return getdifflines(A, B, parent);
                    }
                    q.push({i + 1, j + 1});
                    i++;
                    j++;
                } else {
                    break;
                }
            }
        }
        if (c.second + 1 <= m && parent[c.first][c.second + 1] == std::pair<int, int>(-1, -1)) {
            parent[c.first][c.second + 1] = {c.first, c.second};
            if (c.first == n && c.second + 1 == m) {
                return getdifflines(A, B, parent);
            }

            q.push({c.first, c.second + 1});
            int i = c.first;
            int j = c.second + 1;
            while (i + 1 <= n && j + 1 <= m) {
                if (A[j] == B[i] && parent[i + 1][j + 1] == std::pair<int, int>(-1, -1)) {
                    parent[i + 1][j + 1] = {i, j};
                    if (i + 1 == n && j + 1 == m) {
                        return getdifflines(A, B, parent);
                    }
                    q.push({i + 1, j + 1});
                    i++;
                    j++;
                } else {
                    break;
                }
            }
        }
        if (c.first + 1 <= n && parent[c.first + 1][c.second] == std::pair<int, int>(-1, -1)) {
            parent[c.first + 1][c.second] = {c.first, c.second};
            if (c.first + 1 == n && c.second == m) {
                return getdifflines(A, B, parent);
            }
            q.push({c.first + 1, c.second});
            int i = c.first + 1;
            int j = c.second;
            while (i + 1 <= n && j + 1 <= m) {
                if (A[j] == B[i] && parent[i + 1][j + 1] == std::pair<int, int>(-1, -1)) {
                    parent[i + 1][j + 1] = {i, j};
                    if (i + 1 == n && j + 1 == m) {
                        return getdifflines(A, B, parent);
                    }
                    q.push({i + 1, j + 1});
                    i++;
                    j++;
                } else {
                    break;
                }
            }
        }
    }
    return difflines;
}

inline std::vector<std::string> getStVec(const std::string &input) {
    // get vector of strings from multiline string
    std::vector<std::string> lines;
    std::istringstream iss(input);
    std::string line;

    while (std::getline(iss, line)) {
        lines.push_back(line);
    }

    return lines;
}


inline void printdiff(const std::string &A, const std::string &B) {
    // prints diff from diffline array
    std::vector<DiffLine> outs = diff(getStVec(A), getStVec(B));
    for (const DiffLine &l: outs) {
        switch (l.c) {
            case '+':
                std::cout << "\033[1;32;47m" << l.c << " " << l.line << "\n";
                break;
            case '-':
                std::cout << "\033[1;31;47m" << l.c << " " << l.line << "\n";
                break;
            default:
                std::cout << "\033[1;34;47m" << l.c << " " << l.line << "\n";
                break;
        }
    }
    std::cout << "\033[0m" << "\n";
}

#endif
