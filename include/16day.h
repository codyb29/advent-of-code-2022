#ifndef DAY16_H
#define DAY16_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class ProboscideaVolcanium {
  struct Valve {
    long id;
    std::string label;
    int flow_rate;
    std::vector<Valve *> neighbors;

    Valve() {}

    Valve(std::string label) { this->label = label; }
  };

  struct ValveMonitor {
    Valve *valve;
    Valve *help_valve;
    int pressure_released;
    int flow_rate;
    int current_time;
    long open_valves;

    ValveMonitor(Valve &valve, int pressure_released, int flow_rate,
                 int current_time, long open_valves,
                 Valve *help_valve = nullptr) {
      this->valve = &valve;
      this->help_valve = help_valve;
      this->pressure_released = pressure_released;
      this->flow_rate = flow_rate;
      this->current_time = current_time;
      this->open_valves = open_valves;
    }
  };

  static std::string make_label(std::string label1, std::string label2);

  static std::string make_key(std::string label, int minute);

  static ProboscideaVolcanium::Valve *
  get_valves(const std::vector<std::string> &scan_output);

  static int maximum_pressure_release(ValveMonitor root,
                                      std::vector<int> *pressure,
                                      Valve *has_second);
  // static int DFS(ProboscideaVolcanium::ValveMonitor current,
  //                std::vector<std::vector<int>> &cache);

public:
  static int
  maximum_pressure_release_solo(const std::vector<std::string> &scan_output);
  static int maximum_pressure_release_with_elephant(
      const std::vector<std::string> &scan_output);
  // static int maximum_pressure_release_with_elephant(
  //     const std::vector<std::string> &scan_output);
};

#endif