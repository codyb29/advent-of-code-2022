#include <array>
#include <iostream>
#include <queue>
#include <utility>

#include "08day.h"

int TreetopTreeHouse::SearchAllDirections(
    const int i, const int j,
    const std::vector<std::vector<int>> &tree_heights) {
    // right
    int score = 1;
    int count = 1;
    std::vector<int>::size_type side = j + 1;
    while (side < tree_heights[i].size() - 1 &&
           tree_heights[i][j] > tree_heights[i][side++]) {
        count++;
    }
    score *= count;

    // left
    side = j - 1;
    count = 1;
    while (side > 0 && tree_heights[i][j] > tree_heights[i][side--]) {
        count++;
    }
    score *= count;

    // down
    std::vector<std::vector<int>>::size_type vert = i + 1;
    count = 1;
    while (vert < tree_heights.size() - 1 &&
           tree_heights[i][j] > tree_heights[vert++][j]) {
        count++;
    }
    score *= count;

    // up
    vert = i - 1;
    count = 1;
    while (vert > 0 && tree_heights[i][j] > tree_heights[vert--][j]) {
        count++;
    }
    score *= count;

    return score;
}

std::vector<std::vector<int>> TreetopTreeHouse::ConvertToHeights(
    std::vector<std::string> string_heights) {
    std::vector<std::vector<int>> tree_heights;
    for (std::vector<std::string>::const_iterator it = string_heights.begin();
         it != string_heights.end(); ++it) {
        std::vector<int> tree_height_row;
        for (char height : *it) {
            tree_height_row.push_back(height - '0');
        }
        tree_heights.push_back(tree_height_row);
    }
    return tree_heights;
}

int TreetopTreeHouse::CountTreesVisible(
    std::vector<std::vector<int>> tree_heights) {
    if (tree_heights.size() == 0) return 0;
    std::vector<std::vector<bool>> is_visible(
        tree_heights.size(), std::vector<bool>(tree_heights[0].size(), false));

    // top/bottom row
    for (std::vector<int>::size_type i = 0; i < tree_heights[0].size(); i++) {
        std::vector<std::vector<int>>::size_type j = 0;
        int max_so_far = tree_heights[j][i];
        is_visible[j][i] = true;
        while (j < tree_heights.size() - 2) {
            if (max_so_far < tree_heights[j + 1][i]) {
                is_visible[j + 1][i] = true;
                max_so_far = tree_heights[j + 1][i];
            }
            j++;
        }

        j = tree_heights.size() - 1;
        max_so_far = tree_heights[j][i];
        is_visible[j][i] = true;
        while (j > 1) {
            if (max_so_far < tree_heights[j - 1][i]) {
                is_visible[j - 1][i] = true;
                max_so_far = tree_heights[j - 1][i];
            }
            j--;
        }
    }

    // left/right column
    for (std::vector<std::vector<int>>::size_type i = 0;
         i < tree_heights[0].size(); i++) {
        std::vector<int>::size_type j = 0;
        int max_so_far = tree_heights[i][j];
        is_visible[i][j] = true;
        while (j < tree_heights[i].size() - 2) {
            if (max_so_far < tree_heights[i][j + 1]) {
                is_visible[i][j + 1] = true;
                max_so_far = tree_heights[i][j + 1];
            }
            j++;
        }

        j = tree_heights[i].size() - 1;
        max_so_far = tree_heights[i][j];
        is_visible[i][j] = true;
        while (j > 1) {
            if (max_so_far < tree_heights[i][j - 1]) {
                is_visible[i][j - 1] = true;
                max_so_far = tree_heights[i][j - 1];
            }
            j--;
        }
    }

    int visible = 0;
    for (std::vector<std::vector<bool>>::const_iterator it = is_visible.begin();
         it != is_visible.end(); ++it) {
        for (std::vector<bool>::const_iterator jt = it->begin();
             jt != it->end(); ++jt) {
            if (*jt) {
                visible++;
            }
        }
    }

    return visible;
}

int TreetopTreeHouse::HighestScenicScore(
    std::vector<std::vector<int>> tree_heights) {
    int high_score = 0;

    for (std::vector<std::vector<int>>::size_type i = 1;
         i < tree_heights.size() - 1; i++) {
        for (std::vector<int>::size_type j = 1; j < tree_heights[i].size() - 1;
             j++) {
            high_score = std::max(
                high_score,
                TreetopTreeHouse::SearchAllDirections(i, j, tree_heights));
        }
    }

    return high_score;
}

