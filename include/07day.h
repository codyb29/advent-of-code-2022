#ifndef DAY07_H
#define DAY07_H

#define CREATE "ls"
#define MOVE "cd"
#define MAX_CAPACITY 70000000
#define FREE_SPACE_REQUIRED 30000000

struct DirectoryNode {
  std::unordered_map<std::string, int> files;
  std::unordered_map<std::string, DirectoryNode*> directories;
  DirectoryNode* parent;
  DirectoryNode(DirectoryNode* parent) : parent(parent) {}
};

class NoSpaceLeftOnDevice {
  DirectoryNode root = new DirectoryNode(NULL);

  // recursive dfs
  long GetSumOfTotalSize(DirectoryNode* node, long limit, long& limit_total);
  long GetSumOfTotalSizeWithLowerBound(DirectoryNode* node, long limit,
                                       long& smallest_so_far);
 public:
  void BuildDirectoryTree(std::vector<std::string> commands);
  long GetSumOfTotalUnderSize(long size, long& total);
  long SmallestEligibleDirectoryToDelete(long lower_bound, long upper_bound);
  };
#endif
