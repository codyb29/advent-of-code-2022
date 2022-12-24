#include <iostream>
#include <numeric>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>

#include "07day.h"

// recursive dfs
long NoSpaceLeftOnDevice::GetSumOfTotalSize(DirectoryNode* node, long limit,
                                            long& limit_total) {
    if (node == nullptr) {
        return 0;
    }

    // accumulate total size of files
    long total = std::accumulate(
        node->files.begin(), node->files.end(), 0,
        [](const long previous, const std::pair<const std::string, int>& p) {
            return previous + p.second;
        });

    // Sum up the totals of all it's sub directories as well
    for (std::unordered_map<std::string, DirectoryNode*>::const_iterator it =
             node->directories.begin();
         it != node->directories.end(); ++it) {
        total += this->GetSumOfTotalSize(it->second, limit, limit_total);
    }

    if (total <= limit) {
        limit_total += total;
    }

    return total;
}

// Try to figure this out iteratively if possible.
long NoSpaceLeftOnDevice::GetSumOfTotalSizeWithLowerBound(
    DirectoryNode* node, long limit, long& smallest_so_far) {
    if (node == nullptr) {
        return 0;
    }

    // accumulate total size of files
    long total = std::accumulate(
        node->files.begin(), node->files.end(), 0,
        [](const long previous, const std::pair<const std::string, int>& p) {
            return previous + p.second;
        });

    // Sum up the totals of all it's sub directories as well
    for (std::unordered_map<std::string, DirectoryNode*>::const_iterator it =
             node->directories.begin();
         it != node->directories.end(); ++it) {
        total += this->GetSumOfTotalSizeWithLowerBound(it->second, limit,
                                                       smallest_so_far);
    }

    if (total >= limit) {
        smallest_so_far = std::min(smallest_so_far, total);
    }

    return total;
}

void NoSpaceLeftOnDevice::BuildDirectoryTree(
    std::vector<std::string> commands) {
    DirectoryNode* node = &root;
    std::string mode = "ls";
    for (std::vector<std::string>::const_iterator it = commands.begin() + 1;
         it != commands.end(); ++it) {
        // determine if command or output
        if ((*it)[0] == '$') {
            mode = it->substr(2, 2);
            // cd case
            if (mode == "cd") {
                std::string dir_name = it->substr(5);
                node = dir_name == ".." ? node->parent
                                        : node->directories[it->substr(5)];
            }
            // ls case
        } else {
            // determine if file or directory to create
            if (it->substr(0, 3) == "dir") {
                node->directories[it->substr(4)] = new DirectoryNode(node);
            } else {
                std::string::size_type separatorIdx = it->find(" ");
                node->files[it->substr(separatorIdx + 1)] =
                    std::stoi(it->substr(0, separatorIdx));
            }
        }
    }
}

long NoSpaceLeftOnDevice::GetSumOfTotalUnderSize(long size, long& total) {
    long total_under_size = 0;
    DirectoryNode* node = &root;
    total = this->GetSumOfTotalSize(node, size, total_under_size);
    return total_under_size;
}

long NoSpaceLeftOnDevice::SmallestEligibleDirectoryToDelete(long lower_bound,
                                                            long upper_bound) {
    long smallest_to_lower_bound = upper_bound;
    DirectoryNode* node = &root;
    this->GetSumOfTotalSizeWithLowerBound(node, lower_bound,
                                          smallest_to_lower_bound);

    return smallest_to_lower_bound;
}

// iterative dfs (too hard. Maybe if you have time, come back.)
// long SmallestEligibleDirectoryToDelete(long lower_bound, long
// upper_bound)
// {
//   std::cout << lower_bound << std::endl;
//   long smallest_so_far = upper_bound;
//   std::stack<std::pair<DirectoryNode*, long>> directory;
//   DirectoryNode* node;
//   while (!directory.empty() || node != nullptr) {
//     while (node != nullptr) {
//       for (std::unordered_map<std::string,
//       DirectoryNode*>::const_iterator
//                it = node->directories.begin();
//            it != node->directories.end(); ++it) {
//         long size_of_files = std::accumulate(
//             it->second->files.begin(), it->second->files.end(), 0,
//             [](const long previous, const std::pair<std::string, int>&
//             p) {
//               return previous + p.second;
//             });
//         directory.push(std::make_pair(it->second, size_of_files));
//       }
//     }
//     directory.pop();
//       }
//   return smallest_so_far;
// }

