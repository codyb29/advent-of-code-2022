#include <iostream>
#include <unordered_map>
#include <vector>

#include "01day.h"
#include "02day.h"
#include "03day.h"
#include "04day.h"
#include "05day.h"
#include "06day.h"
#include "07day.h"
#include "08day.h"
#include "09day.h"
#include "10day.h"
#include "11day.h"
#include "12day.h"
#include "13day.h"
#include "14day.h"
#include "15day.h"
#include "16day.h"
#include "17day.h"
#include "18day.h"
#include "19day.h"
#include "20day.h"
#include "21day.h"
#include "22day.h"
#include "23day.h"
#include "24day.h"
#include "25day.h"
#include "utilities.h"

int main(int argc, char *argv[]) {
  // Check that an argument was provided
  if (argc < 2) {
    std::cerr << "Error: No argument provided" << std::endl;
    return 1;
  }

  // Convert the argument to an integer
  char *endptr = nullptr;
  long day = std::strtol(argv[1], &endptr, 10);

  // Check for errors in the conversion
  if (*endptr != '\0') {
    std::cerr << "Error: Invalid argument '" << argv[1] << "'" << std::endl;
    return 1;
  }
  if (day < 1 || day > 25) {
    std::cerr << "Error: Day must be between 1 and 25" << std::endl;
    return 1;
  }

  // Do a switch case statement on the argument
  switch (day) {
  case 1: {
    std::vector<std::string> input_buffer =
        ConvertInputFileToVector("static/01day_test.txt");
    std::cout << "Elf with most calories total: "
              << ElfWithMostCalories(input_buffer) << std::endl;

    std::cout << "Top three elves' calories total: "
              << ElvesWithMostCalories(input_buffer) << std::endl;
    break;
  }
  case 2: {
    const std::vector<std::pair<char, char>> encrypted_strategy_guide =
        InputFileToPairs("static/02day_test.txt");
    int score = DecryptStrategyGuideIncorrectly(encrypted_strategy_guide);
    std::cout << "Estimated Total Score: " << score << std::endl;

    score = DecryptStrategyGuide(encrypted_strategy_guide);
    std::cout << "Actual Total Score: " << score << std::endl;

    break;
  }
  case 3: {
    const std::vector<std::string> rucksacks =
        RucksackOrganization::ConvertInputIntoRucksacks(
            "static/03day_test.txt");
    const std::vector<std::pair<std::string, std::string>> rucksack_pairs =
        RucksackOrganization::ConvertRucksackIntoPairs(rucksacks);
    std::cout << "Sum of common items: "
              << RucksackOrganization::SumOfPriorities(rucksack_pairs)
              << std::endl;
    std::cout << "Sum of badges: "
              << RucksackOrganization::SumOfBadges(rucksacks) << std::endl;
  } break;
  case 4: {
    const std::vector<std::string> assignments =
        Utilities::ProcessInputFile("static/04day_test.txt");
    std::cout << "Completely Overlapping Pairs: "
              << CleanCamp::CountOverlappingPairs(assignments,
                                                  CleanCamp::IsFullyOverlapping)
              << std::endl;
    std::cout << "Partially Overlapping Pairs: "
              << CleanCamp::CountOverlappingPairs(
                     assignments, CleanCamp::IsPartiallyOverlapping)
              << std::endl;
    break;
  }
  case 5: {
    const std::vector<std::string> puzzle_input =
        Utilities::ProcessInputFile("static/05day_test.txt");
    const std::vector<std::stack<char>> crates =
        SupplyStacks::ProcessCrates(puzzle_input);
    const std::vector<std::vector<int>> instructions =
        SupplyStacks::ProcessInstructions(puzzle_input);
    std::cout << "On top of each stack for CrateMover 9000: "
              << SupplyStacks::SimulateCrateMover9000(crates, instructions)
              << std::endl;
    std::cout << "On top of each stack for CrateMover 9001: "
              << SupplyStacks::SimulateCrateMover9001(crates, instructions)
              << std::endl;
    break;
  }
  case 6: {
    std::vector<std::string> puzzle_input =
        Utilities::ProcessInputFile("static/06day_test.txt");
    std::string datastream_buffer = puzzle_input.front();
    std::cout << "First marker after 4 characters: "
              << TuningTrouble::GetStartOfPacket(datastream_buffer, 4)
              << std::endl;
    std::cout << "First marker after 14 characters: "
              << TuningTrouble::GetStartOfPacket(datastream_buffer, 14)
              << std::endl;
    break;
  }
  case 7: {
    NoSpaceLeftOnDevice noSpaceLeftOnDevice;
    std::vector<std::string> commands =
        Utilities::ProcessInputFile("static/07day_test.txt");
    noSpaceLeftOnDevice.BuildDirectoryTree(commands);

    long total = 0;
    std::cout << "Sum of all directories with a total size of at most "
                 "100000: "
              << noSpaceLeftOnDevice.GetSumOfTotalUnderSize(100000, total)
              << std::endl;
    std::cout << "Smallest directory eligible to be deleted: "
              << noSpaceLeftOnDevice.SmallestEligibleDirectoryToDelete(
                     FREE_SPACE_REQUIRED - (MAX_CAPACITY - total), total)
              << std::endl;
    break;
  }
  case 8: {
    std::vector<std::string> puzzle_input =
        Utilities::ProcessInputFile("static/08day_test.txt");
    std::vector<std::vector<int>> tree_heights =
        TreetopTreeHouse::ConvertToHeights(puzzle_input);
    std::cout << TreetopTreeHouse::CountTreesVisible(tree_heights) << std::endl;
    std::cout << TreetopTreeHouse::HighestScenicScore(tree_heights)
              << std::endl;
    break;
  }
  case 9: {
    std::vector<std::string> instructions =
        Utilities::ProcessInputFile("static/09day_test.txt");
    std::cout << "Visited T Spots for 2: "
              << RopeBridge::CountVisitedNodesForSizeN(instructions, 2)
              << std::endl;

    std::cout << "Visited T spots for 10: "
              << RopeBridge::CountVisitedNodesForSizeN(instructions, 10)
              << std::endl;
    break;
  }
  case 10: {
    std::vector<std::string> puzzle_input =
        Utilities::ProcessInputFile("static/10day_test.txt");
    std::cout << "total: " << CathodeRayTube::GetSignalStrength(puzzle_input)
              << std::endl;

    CathodeRayTube::DecodeMessage(puzzle_input);
    break;
  }
  case 11: {
    std::vector<std::string> puzzle_input =
        Utilities::ProcessInputFile("static/11day_test.txt");
    std::vector<Monkey> monkeys = MonkeyInTheMiddle::MakeMonkeys(puzzle_input);
    std::cout << MonkeyInTheMiddle::GetMonkeyBusinessLevelForRounds(monkeys,
                                                                    10000)
              << std::endl;
    break;
  }
  case 12: {
    std::vector<std::string> puzzle_input =
        Utilities::ProcessInputFile("static/12day_test.txt");
    std::cout << "fewest steps to E: "
              << HillClimbingAlgorithm::GetShortestPath(puzzle_input)
              << std::endl;
    break;
  }
  case 13: {
    std::vector<std::string> puzzle_input =
        Utilities::ProcessInputFile("static/13day_test.txt");
    std::cout << "Correct order pairs' sum: "
              << DistressSignal::sum_of_right_pairs(puzzle_input) << std::endl;
    std::cout << "Get decoder key: "
              << DistressSignal::get_decoder_key(puzzle_input) << std::endl;
    break;
  }
  case 14: {
    std::vector<std::string> puzzle_input =
        Utilities::ProcessInputFile("static/14day_test.txt");
    std::cout << "Units of sand at rest: "
              << RegolithReservoir::count_resting_sand(puzzle_input)
              << std::endl;
    std::cout << "Units of sand at rest with floor: "
              << RegolithReservoir::count_resting_sand_with_floor(puzzle_input)
              << std::endl;
    break;
  }
  case 15: {
    const std::vector<std::string> puzzle_input =
        Utilities::ProcessInputFile("static/15day_test.txt");
    int y = 2000000;
    int grid_length = 4000000;
    std::cout << "positions where beacon is not present at y= " << y << ": "
              << BeaconExclusionZone::no_beacon_present(puzzle_input, y)
              << std::endl;

    std::cout << "tuning frequency: "
              << BeaconExclusionZone::get_tuning_frequency(puzzle_input,
                                                           grid_length)
              << std::endl;
    break;
  }
  case 16: {
    std::vector<std::string> puzzle_input =
        Utilities::ProcessInputFile("static/16day_test.txt");
    // std::cout << "Most pressure released: "
    //           << ProboscideaVolcanium::maximum_pressure_release_solo(
    //                  puzzle_input)
    //           << std::endl;
    std::cout << "Most pressure released with elephant assistance: "
              << ProboscideaVolcanium::maximum_pressure_release_with_elephant(
                     puzzle_input)
              << std::endl;
    break;
  }
  case 17: {
    std::vector<std::string> puzzle_input =
        Utilities::ProcessInputFile("static/17day_test.txt");
    const std::vector<signed char> jet_pattern =
        get_jet_pattern(puzzle_input.front());
    std::cout << "rock height after 2022: "
              << get_rock_tower_height(jet_pattern, 2022L) << std::endl;
    std::cout << "rock height after 1000000000000: "
              << get_rock_tower_height(jet_pattern, 1000000000000L)
              << std::endl;
    break;
  }
  case 18: {
    std::vector<std::string> puzzle_input =
        Utilities::ProcessInputFile("static/18day_test.txt");
    std::cout << "Surface area of scanned lava: "
              << get_lava_surface_area_pt1(puzzle_input) << std::endl;
    std::cout << "Surface area of scanned lava without air pockets: "
              << get_lava_surface_area_pt2(puzzle_input) << std::endl;
    break;
  }
  case 19: {
    std::vector<std::string> puzzle_input =
        Utilities::ProcessInputFile("static/19day_input.txt");
    std::cout << "Sum of blueprint quality level: "
              << get_sum_of_all_blueprint_quality(puzzle_input, 24)
              << std::endl;
    std::cout << "product of top 3 blueprints: "
              << get_product_of_top_3(puzzle_input, 32) << std::endl;
    break;
  }
  case 20:
  case 21:
  case 22:
  case 23:
  case 24:
  case 25:
  default:
    std::cout << "Not solved yet..." << std::endl;
  }

  return 0;
}
