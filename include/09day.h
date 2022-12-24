#ifndef DAY09_H
#define DAY09_H

#define UP 0
#define RIGHT 2
#define DOWN 4
#define LEFT 6

class RopeBridge {
    static std::string MakeKey(int x, int y);

   public:
    static int CountVisitedNodesForSizeN(
        const std::vector<std::string>& instructions, const int n);
    static int CountVisitedNodesForSize2(
        const std::vector<std::string>& instructions);
};

#endif
