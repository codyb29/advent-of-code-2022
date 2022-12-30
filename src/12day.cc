#include "12day.h"

#include <iostream>
#include <queue>

const int HillClimbingAlgorithm::direction[][2] = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1}};
std::vector<std::string>::size_type
HillClimbingAlgorithm::GetShortestPath(std::vector<std::string> elevation) {
  std::string ans = "";
  // find starting point
  for (std::vector<std::string>::size_type i = 0; i < elevation.size(); i++) {
    for (std::string::size_type j = 0; j < elevation[i].length(); j++) {

      // DFS() for E
      std::vector<std::string> elevation_map(elevation);
      if (elevation_map[i][j] == 'S' || elevation_map[i][j] == 'a') {
        std::vector<std::vector<std::string>> sequence_map(
            elevation_map.size(),
            std::vector<std::string>(elevation_map[i].length()));
        std::queue<Node *> nodes;

        nodes.push(new Node(i, j, 'a', ""));
        elevation_map[i][j] = '\0';

        while (!nodes.empty()) {
          // get first in the queue
          Node *curr = nodes.front();
          nodes.pop();
          // add all children to the queue
          for (int k = 0; k < 4; k++) {
            int x = curr->i + direction[k][0];
            int y = curr->j + direction[k][1];
            if (x >= 0 && y >= 0 &&
                x < static_cast<int>(elevation_map.size()) &&
                y < static_cast<int>(elevation_map[x].length())) {

              if (elevation_map[x][y] != 'E') {
                if (curr->letter + 1 >= elevation_map[x][y] &&
                    elevation_map[x][y] != '\0') {
                  nodes.push(
                      new Node(x, y, elevation_map[x][y], curr->sequence));
                  // visit node.
                  elevation_map[x][y] = '\0';
                }
              } else if (curr->letter >= 'y' &&
                         (ans.length() == 0 ||
                          curr->sequence.length() + 1 < ans.length())) {
                ans = curr->sequence + 'E';
              }
            }
          }
        }
      }
    }
  }

  // S was not found
  return ans.length() - 1;
}

std::string HillClimbingAlgorithm::GetKey(int a, int b) {
  return std::to_string(a) + "," + std::to_string(b);
}

// Here lies the dead code that doesn't work. Implements DFS...

// I have now learned that a DFS does not guarantee that the shortest path
// will be returned... post order traversal std::string
// HillClimbingAlgorithm::DFS(
//     std::vector<std::string> &elevation_map, std::vector<std::string> ref,
//     std::unordered_set<std::string> &deadends,
//     std::vector<std::vector<std::string>> &sequence_map, int i, int j) {

//   char letter = elevation_map[i][j];
//   elevation_map[i][j] = '\0';

//   std::vector<int> children;
//   std::string curr_route = std::string(1, letter);
//   std::string best_route = "";
//   for (int k = 0; k < 4; k++) {
//     int x = i + direction[k][0];
//     int y = j + direction[k][1];
//     std::string possible_route = "";
//     if (x >= 0 && y >= 0 && x < static_cast<int>(elevation_map.size()) &&
//         y < static_cast<int>(elevation_map[x].length())) {
//       if (elevation_map[x][y] == 'E' && letter >= 'y') {
//         children.push_back(k);
//         possible_route = "E";
//       } else if (elevation_map[x][y] != '\0' && elevation_map[x][y] != 'E'
//       &&
//                  letter + 1 >= elevation_map[x][y] &&
//                  deadends.find(GetKey(i, j, x, y)) == deadends.end()) {
//         if (sequence_map[x][y].length() == 0) {
//           children.push_back(k);
//           possible_route =
//               DFS(elevation_map, ref, deadends, sequence_map, x, y);
//         } else {
//           possible_route = sequence_map[x][y];
//         }

//         // deadend
//         if (possible_route.length() == 0) {
//           deadends.insert(GetKey(i, j, x, y));
//         }
//       }
//     }
//     if (possible_route.length() > 0 &&
//         (best_route.length() == 0 ||
//          possible_route.length() < best_route.length())) {
//       best_route = possible_route;
//     }
//   }

//   for (int child : children) {
//     int x = i + direction[child][0];
//     int y = j + direction[child][1];
//     elevation_map[x][y] = ref[x][y];
//   }

//   if (best_route.length() > 0) {
//     best_route = curr_route + best_route;

//     if (sequence_map[i][j].length() == 0 ||
//         best_route.length() < sequence_map[i][j].length()) {
//       sequence_map[i][j] = best_route;
//     }
//   }

//   return best_route;
// }

// post order traversal
// std::vector<std::string>::size_type
// HillClimbingAlgorithm::DFS(std::vector<std::string> elevation_map,
//                            std::vector<std::string>::size_type i,
//                            std::string::size_type j) {
//   elevation_map[i][j] = 'a';
//   std::vector<std::vector<std::string>> sequence_map(
//       elevation_map.size(),
//       std::vector<std::string>(elevation_map[i].length()));
//   std::stack<Node *> st;
//   Node *root = new Node(i, j, elevation_map[i][j], nullptr);

//   st.push(root);
//   while (!st.empty()) {
//     std::stack<Node *>::size_type orig_size = st.size();
//     do {
//       orig_size = st.size();
//       Node *parent = st.top();
//       int k = 0, dead_end = 0;
//       do {
//         int x = parent->i + direction[k][0];
//         int y = parent->j + direction[k][1];
//         // check bounds
//         if (x >= 0 && y >= 0 && x < static_cast<int>(elevation_map.size())
//         &&
//             y < static_cast<int>(elevation_map[x].length())) {
//           // not visited
//           // we can step without busting out the climbing gear
//           if (elevation_map[x][y] != '\0' &&
//               parent->letter + 1 >= elevation_map[x][y] &&
//               elevation_map[x][y] >= 'a') {

//             // if we determined an optimal sequence for a given spot,
//             attempt to
//             // use. don't re-traverse what we already know. Could also be a
//             dead
//             // end.
//             if (sequence_map[x][y].length() > 0 && sequence_map[x][y] !=
//             "X")
//             {
//               // use it if it beats other potential paths.
//               if (sequence_map[parent->i][parent->j].length() == 0 ||
//                   sequence_map[parent->i][parent->j] == "X" ||
//                   sequence_map[x][y].length() + 1 <
//                       sequence_map[parent->i][parent->j].length()) {
//                 sequence_map[parent->i][parent->j] =
//                     parent->letter + sequence_map[x][y];
//               }
//               // traverse since we don't know what this path will bring.
//             } else if (sequence_map[x][y] != "X") {
//               st.push(new Node(x, y, elevation_map[x][y], parent));
//             } else {
//               dead_end++;
//             }
//             // Special case, if above is invalid, it could be because we
//             found E
//             // make sure that we can reach E by being at least height y.
//           } else if (elevation_map[parent->i][parent->j] >= 'y' &&
//                      elevation_map[x][y] == 'E') {
//             // pop out any other children since they can't possibly beat a
//             // direct link. should only be one link from parent to E. Not
//             // possible to have more than 1 since no diagonal.
//             while (!st.empty() && parent != st.top()) {
//               st.pop();
//             }
//             // set base case.
//             sequence_map[x][y] = 'E';
//             sequence_map[parent->i][parent->j] = parent->letter;
//             sequence_map[parent->i][parent->j] += 'E';
//             break;
//           } else {
//             dead_end++;
//           }
//         } else {
//           dead_end++;
//         }
//       } while (++k < 4);
//       // mark it as visited if we need to go deeper into the tree
//       if (orig_size < st.size()) {
//         elevation_map[parent->i][parent->j] = 255;
//         // otherwise, no need to go deeper, pop it out, we already know the
//         // optimal from here.
//       } else {
//         // make sure we don't go here again since it's a dead end.
//         if (dead_end == 4) {
//           sequence_map[parent->i][parent->j] = "X";
//         }
//         st.pop();
//         if (!st.empty()) {
//           // Allow to be read again
//           elevation_map[st.top()->i][st.top()->j] = st.top()->letter;
//         }
//       }
//     } while (orig_size < st.size());
//   }

//   std::cout << sequence_map[i][j] << std::endl;

//   return sequence_map[i][j].length() - 1;
// }
