#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "09day.h"

std::string RopeBridge::MakeKey(int x, int y) {
    return std::to_string(x) + "," + std::to_string(y);
}

int RopeBridge::CountVisitedNodesForSizeN(
    const std::vector<std::string>& instructions, const int n) {
    int visited_nodes = 1;
    std::unordered_map<std::string, bool> visited;
    visited["0,0"] = true;
    std::vector<std::vector<int>> chain(n + 1, std::vector<int>(2, 0));

    for (std::vector<std::string>::const_iterator it = instructions.begin();
         it != instructions.end(); ++it) {
        int move = std::stoi(it->substr(2));

        for (int i = 0; i < move; i++) {
            // test the waters
            switch ((*it)[0]) {
                case 'U':
                    chain[n][1] += 1;
                    break;
                case 'D':
                    chain[n][1] -= 1;
                    break;
                case 'L':
                    chain[n][0] -= 1;
                    break;
                case 'R':
                    chain[n][0] += 1;
            };
            // create the node if it hasn't been made yet.
            std::string key = RopeBridge::MakeKey(chain[n][0], chain[n][1]);
            if (visited.find(key) == visited.end()) {
                visited[key] = false;
            }

            chain[n - 1][0] = chain[n][0];
            chain[n - 1][1] = chain[n][1];
            std::vector<std::vector<int>>::size_type j = chain.size() - 2;
            while (j > 0) {
                int xDiff = chain[j][0] - chain[j - 1][0],
                    yDiff = chain[j][1] - chain[j - 1][1];
                int x = 0, y = 0;
                if (std::abs(xDiff) > 1 || std::abs(yDiff) > 1) {
                    x = xDiff < 0 ? std::floor(xDiff / 2.0)
                                  : std::ceil(xDiff / 2.0);
                    y = yDiff < 0 ? std::floor(yDiff / 2.0)
                                  : std::ceil(yDiff / 2.0);
                }

                chain[j - 1][0] += x;
                chain[j - 1][1] += y;
                std::string key2 =
                    RopeBridge::MakeKey(chain[j - 1][0], chain[j - 1][1]);
                if (visited.find(key2) == visited.end()) {
                    visited[key2] = false;
                }
                if (j - 1 == 0) {
                    if (!visited.at(key2)) {
                        visited_nodes += 1;
                        visited[key2] = true;
                    }
                }
                j -= 1;
            }

            // printing tool
            // int k = 0;
            // for (int i = 15; i > -6; i--) {
            //   for (int j = -11; j < 15; j++) {
            //     std::string key = RopeBridge::MakeKey(i, j);
            //     std::find_if(chain.begin(), chain.end(),
            //                  [i, j](const std::vector<int>& pair) {
            //                    return pair[0] == j && pair[1] == i;
            //                  });
            //     std::vector<std::vector<int>>::iterator pair =
            //         std::find_if(chain.begin(), chain.end(),
            //                      [i, j](const std::vector<int>& pair) {
            //                        return pair[0] == j && pair[1] == i;
            //                      });
            //     if (pair != chain.end()) {
            //       std::cout << k++;
            //     } else {
            //       std::cout << '.';
            //     }
            //   }
            //   std::cout << std::endl;
            // }
            // std::cout << std::endl << std::endl;
        }
    }

    return visited_nodes;
}

int RopeBridge::CountVisitedNodesForSize2(
    const std::vector<std::string>& instructions) {
    int visited_nodes = 1;
    std::unordered_map<std::string, bool> visited;
    visited["0,0"] = true;
    int hx = 0, hy = 0, tx = 0, ty = 0, x = 0, y = 0;

    for (std::vector<std::string>::const_iterator it = instructions.begin();
         it != instructions.end(); ++it) {
        int move = std::stoi(it->substr(2));
        for (int i = 0; i < move; i++) {
            // test the waters
            switch ((*it)[0]) {
                case 'U':
                    y += 1;
                    break;
                case 'D':
                    y -= 1;
                    break;
                case 'L':
                    x -= 1;
                    break;
                case 'R':
                    x += 1;
            };
            // create the node if it hasn't been made yet.
            std::string key = RopeBridge::MakeKey(x, y);
            if (visited.find(key) == visited.end()) {
                visited[key] = false;
            }

            // check if tx, ty is still a neighbor of x, y
            if (std::abs(tx - x) > 1 || std::abs(ty - y) > 1) {
                tx = hx;
                ty = hy;
                std::string key = RopeBridge::MakeKey(tx, ty);
                if (!visited.at(key)) {
                    visited_nodes += 1;
                    visited[key] = true;
                }
            }
            hx = x;
            hy = y;
        }
    }

    return visited_nodes;
}

