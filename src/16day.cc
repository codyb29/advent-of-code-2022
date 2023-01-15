#include "16day.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_map>

ProboscideaVolcanium::Valve *
ProboscideaVolcanium::get_valves(const std::vector<std::string> &scan_output) {
  std::unordered_map<std::string, Valve *> valves;
  long id = 1;
  for (std::vector<std::string>::const_iterator it = scan_output.begin();
       it != scan_output.end(); ++it) {
    std::stringstream ss(*it);

    std::string label;
    int flow_rate;
    std::vector<std::string> tunnels;

    ss >> label >> label;
    std::string flow_rate_str;
    std::getline(ss, flow_rate_str, ';');
    flow_rate_str.erase(0, flow_rate_str.find('=') +
                               1); // Extract the flow rate value
    flow_rate =
        std::stoi(flow_rate_str); // Convert the flow rate value to an integer
    // Parse the tunnels
    std::string tunnel;
    while (ss >> tunnel) {
      if (tunnel == "valves" || tunnel == "valve") {
        break;
      }
    }
    // Read the remaining tunnels
    while (ss >> tunnel) {
      if (tunnel[tunnel.size() - 1] == ',') {
        tunnels.push_back(tunnel.substr(0, tunnel.size() - 1));
      } else {
        tunnels.push_back(tunnel);
      }
    }

    std::vector<Valve *> neighbors;
    for (std::vector<std::string>::iterator label = tunnels.begin();
         label != tunnels.end(); ++label) {
      if (valves.find(*label) == valves.end()) {
        valves[*label] = new Valve(*label);
      }
      neighbors.push_back(valves[*label]);
    }

    Valve *new_valve = new Valve(label);
    if (valves.find(label) != valves.end()) {
      new_valve = valves[label];
    }

    new_valve->flow_rate = flow_rate;
    new_valve->label = label;
    new_valve->neighbors = neighbors;
    valves[label] = new_valve;
  }

  std::vector<Valve *> sorted_valves;
  for (std::pair<std::string, Valve *> pair : valves) {
    sorted_valves.push_back(pair.second);
  }
  std::sort(sorted_valves.begin(), sorted_valves.end(),
            [](const Valve *a, const Valve *b) {
              return a->flow_rate > b->flow_rate;
            });

  for (Valve *valve : sorted_valves) {
    valve->id = id;
    id <<= 1;
  }
  return valves["AA"];
}

std::string ProboscideaVolcanium::make_key(std::string label, int minute) {
  return label + "," + std::to_string(minute);
}

std::string ProboscideaVolcanium::make_label(std::string label1,
                                             std::string label2) {
  std::string label = label1 + label2;
  // unify the labels so XXYY is the same as YYXX
  // no need to delimiter the labels together, valves must be of size 2.
  std::sort(label.begin(), label.end());
  return label;
}

// int ProboscideaVolcanium::maximum_pressure_release(
//     const std::vector<std::string> &scan_output) {
//   Valve *root = get_valves(scan_output);
//   // Keep track of a maximum pressure release
//   // int pressure_release = 0;
//   // initialize stack with new ValveMonitor -> AA
//   std::queue<ValveMonitor> monitors;
//   monitors.push(
//       ValveMonitor(*root, 0, 0, 1, new std::unordered_set<std::string>()));
//   int max_pressure_release = 0;
//   std::unordered_map<std::string, int> visited;
//   while (!monitors.empty()) {
//     ValveMonitor top = monitors.front();
//     monitors.pop();

//     if (top.current_time < 30) {
//       int increase_pressure_released = top.pressure_released + top.flow_rate;
//       // leave the valve closed/already opened, e.g. just move
//       int increase_minute = top.current_time + 1;
//       for (std::vector<Valve *>::iterator curr =
//       top.valve->neighbors.begin();
//            curr != top.valve->neighbors.end(); ++curr) {
//         std::string key = make_key((*curr)->label, increase_minute);
//         if (visited.find(key) == visited.end() ||
//             visited[key] <= increase_pressure_released) {
//           ValveMonitor explore_path(**curr, increase_pressure_released,
//                                     top.flow_rate, increase_minute,
//                                     top.open_valves);

//           monitors.push(ValveMonitor(explore_path));
//           visited[key] = increase_pressure_released;
//         }
//       }

//       std::string open_valve_key = make_key(top.valve->label,
//       increase_minute);
//       // turn the valve on when positive flow and valve closed.
//       // also ensure a better cousin isn't queued
//       increase_pressure_released += top.valve->flow_rate;
//       if (top.valve->flow_rate > 0 &&
//           top.open_valves->find(top.valve->label) == top.open_valves->end()
//           && (visited.find(open_valve_key) == visited.end() ||
//            visited[open_valve_key] <= increase_pressure_released)) {
//         std::unordered_set<std::string> *opened_valves =
//             new std::unordered_set<std::string>(*top.open_valves);
//         ValveMonitor open_monitor(*top.valve, increase_pressure_released,
//                                   top.flow_rate + top.valve->flow_rate,
//                                   increase_minute, opened_valves);
//         opened_valves->insert(top.valve->label);
//         monitors.push(open_monitor);
//         visited[open_valve_key] = increase_pressure_released;
//       }
//       // we've reached are target 30 min
//     } else if (top.current_time == 30 &&
//                max_pressure_release < top.pressure_released) {
//       max_pressure_release = top.pressure_released;
//     }
//     // don't do anything if we exceeded 30 min.
//   }

//   return max_pressure_release;
// }

// int ProboscideaVolcanium::maximum_pressure_release_solo(
//     const std::vector<std::string> &scan_output) {
//   int time = 30;
//   Valve *root = get_valves(scan_output);
//   std::vector<std::vector<std::vector<int>>> pressure(
//       time, std::vector<std::vector<int>>((1 << (scan_output.size() + 1)) -
//       1,
//                                           std::vector<int>(20, -1)));

//   std::queue<ValveMonitor> monitors;
//   ValveMonitor seed(*root, 0, 0, time, 0);
//   monitors.push(seed);
//   int max_pressure_release = 0;

//   while (!monitors.empty()) {
//     ValveMonitor top = monitors.front();
//     monitors.pop();

//     if (top.current_time > 0) {
//       // leave the valve closed/already opened, e.g. just move
//       int elapse_min = top.current_time - 1;
//       for (std::vector<Valve *>::iterator me = top.valve->neighbors.begin();
//            me != top.valve->neighbors.end(); ++me) {
//         int next_node = std::log2((*me)->id);
//         if (pressure[elapse_min][top.open_valves][next_node] <
//             top.pressure_released) {
//           ValveMonitor move_valve(**me, top.pressure_released, top.flow_rate,
//                                   elapse_min, top.open_valves);
//           monitors.push(move_valve);
//           pressure[elapse_min][top.open_valves][next_node] =
//               top.pressure_released;
//         }
//       }

//       // turn the valve on when positive flow and valve closed.
//       // also ensure a better cousin isn't queued
//       if (top.valve->flow_rate > 0 && (top.open_valves & top.valve->id) == 0)
//       {
//         int next_node = std::log2(top.valve->id);
//         int new_pressure_release =
//             top.pressure_released + (top.valve->flow_rate * elapse_min);
//         long update_open_valves = top.open_valves | top.valve->id;
//         if (pressure[elapse_min][update_open_valves][next_node] <
//             new_pressure_release) {

//           ValveMonitor open_monitor(*top.valve, new_pressure_release,
//                                     top.flow_rate + top.valve->flow_rate,
//                                     elapse_min, update_open_valves);
//           monitors.push(open_monitor);
//           pressure[elapse_min][update_open_valves][next_node] =
//               new_pressure_release;
//         }
//       }

//       // we've reached our targeted min
//     } else if (top.current_time == 0 &&
//                max_pressure_release < top.pressure_released) {
//       max_pressure_release = top.pressure_released;
//     }
//     // don't do anything if we exceeded targeted min.
//   }

//   return max_pressure_release;
// }

// int ProboscideaVolcanium::maximum_pressure_release(
//     Valve *root, int time,
//     std::vector<std::vector<std::vector<std::vector<int>>>> &pressure,
//     int is_second) {
//   std::queue<ValveMonitor> monitors;
//   ValveMonitor seed(*root, 0, 0, time, 0);
//   monitors.push(seed);
//   int max_pressure_release = 0;

//   while (!monitors.empty()) {
//     ValveMonitor top = monitors.front();
//     monitors.pop();
//     // if (top.current_time == 1) {
//     //   std::cout << "Hello world!" << std::endl;
//     // }

//     if (top.current_time > 0) {
//       // leave the valve closed/already opened, e.g. just move
//       int elapse_min = top.current_time - 1;
//       for (std::vector<Valve *>::iterator me = top.valve->neighbors.begin();
//            me != top.valve->neighbors.end(); ++me) {
//         int next_node = std::log2((*me)->id);
//         if (pressure[elapse_min][top.open_valves][next_node][is_second] <
//             top.pressure_released) {
//           ValveMonitor move_valve(**me, top.pressure_released, top.flow_rate,
//                                   elapse_min, top.open_valves);
//           monitors.push(move_valve);
//           pressure[elapse_min][top.open_valves][next_node][is_second] =
//               top.pressure_released;
//         }
//       }

//       // turn the valve on when positive flow and valve closed.
//       // also ensure a better cousin isn't queued
//       if (top.valve->flow_rate > 0 && (top.open_valves & top.valve->id) == 0)
//       {
//         int next_node = std::log2(top.valve->id);
//         int new_pressure_release =
//             top.pressure_released + (top.valve->flow_rate * elapse_min);
//         long update_open_valves = top.open_valves | top.valve->id;
//         if (pressure[elapse_min][update_open_valves][next_node][is_second] <
//             new_pressure_release) {
//           ValveMonitor open_monitor(*top.valve, new_pressure_release,
//                                     top.flow_rate + top.valve->flow_rate,
//                                     elapse_min, update_open_valves);
//           if (is_second &&
//               pressure[elapse_min][update_open_valves][next_node][0] != -1) {
//             open_monitor =
//                 ValveMonitor(*top.valve, top.pressure_released,
//                 top.flow_rate,
//                              elapse_min, update_open_valves);
//           } else if (is_second &&
//                      pressure[elapse_min][update_open_valves][next_node][0]
//                      ==
//                          -1) {
//             std::cout << "Hello world!" << std::endl;
//           }
//           monitors.push(open_monitor);
//           pressure[elapse_min][update_open_valves][next_node][is_second] =
//               new_pressure_release;
//         }
//       }

//       // we've reached our targeted min
//     } else if (top.current_time == 0 &&
//                max_pressure_release < top.pressure_released) {
//       max_pressure_release = top.pressure_released;
//     }
//     // don't do anything if we exceeded targeted min.
//   }

//   return max_pressure_release;
// }

int ProboscideaVolcanium::maximum_pressure_release(ValveMonitor root,
                                                   std::vector<int> *pressure,
                                                   Valve *has_second) {
  int is_second = has_second == nullptr ? 1 : 0;
  if (root.current_time == 0) {
    return is_second == 0
               ? maximum_pressure_release(
                     ValveMonitor(*has_second, 0, 0, 26, root.open_valves),
                     pressure, nullptr)
               : 0;
  }

  // leave the valve closed/already opened, e.g. just move
  int elapse_min = root.current_time - 1;
  int next_node = std::log2(root.valve->id);
  long key = root.open_valves * 58 * 31 * 2 + next_node * 31 * 2 +
             elapse_min * 2 + is_second;
  if ((*pressure)[key] != -1) {
    return (*pressure)[key];
  }

  int max_pressure_release = 0;
  if ((root.open_valves & root.valve->id) == 0 && root.valve->flow_rate > 0) {
    long update_open_valves = root.open_valves | root.valve->id;
    ValveMonitor next = root;
    next.open_valves = update_open_valves;
    next.current_time = elapse_min;
    max_pressure_release = (elapse_min * root.valve->flow_rate) +
                           maximum_pressure_release(next, pressure, has_second);
  }

  for (std::vector<Valve *>::iterator me = root.valve->neighbors.begin();
       me != root.valve->neighbors.end(); ++me) {
    ValveMonitor next = root;
    next.valve = *me;
    next.current_time = elapse_min;
    max_pressure_release =
        std::max(max_pressure_release,
                 maximum_pressure_release(next, pressure, has_second));
  }

  (*pressure)[key] = max_pressure_release;

  return max_pressure_release;
}

int ProboscideaVolcanium::maximum_pressure_release_with_elephant(
    const std::vector<std::string> &scan_output) {
  int time = 26;
  Valve *root = get_valves(scan_output);
  ValveMonitor me(*root, 0, 0, time, 0);
  // ValveMonitor elephant(*get_valves(scan_output), 0, 0, time, 0);
  std::vector<int> *pressure =
      new std::vector<int>((1 << 15) * scan_output.size() * 31 * 2, -1);
  // std::vector<std::vector<std::vector<std::vector<int>>>> pressure(
  //     time, std::vector<std::vector<std::vector<int>>>(
  //               (1 << (scan_output.size() + 1)) - 1,
  //               std::vector<std::vector<int>>(scan_output.size(),
  //                                             std::vector<int>(2, -1))));
  return maximum_pressure_release(me, pressure, root);
}

// int ProboscideaVolcanium::maximum_pressure_release_with_elephant(
//     const std::vector<std::string> &scan_output) {
//   Valve *root = get_valves(scan_output);
//   std::queue<ValveMonitor> monitors;
//   ValveMonitor seed(*root, 0, 0, 26, "", root);
//   monitors.push(seed);
//   int max_pressure_release = 0;
//   std::vector<std::vector<int>> pressure =
//       std::vector(26, std::vector((1 << (scan_output.size() + 1)) - 1, 0));

//   // std::unordered_map<std::string, int> visited;
//   while (!monitors.empty()) {
//     ValveMonitor top = monitors.front();
//     monitors.pop();

//     if (top.current_time > 1) {
//       // int increase_pressure_released = top.pressure_released +
//       top.flow_rate;
//       // leave the valve closed/already opened, e.g. just move
//       int elapse_min = top.current_time - 1;
//       for (std::vector<Valve *>::iterator me = top.valve->neighbors.begin();
//            me != top.valve->neighbors.end(); ++me) {
//         for (std::vector<Valve *>::iterator helper =
//                  top.help_valve->neighbors.begin();
//              helper != top.help_valve->neighbors.end(); ++helper) {
//           std::string key_label = make_label((*me)->label, (*helper)->label);
//           std::string key = make_key(key_label, elapse_min);
//           if (visited.find(key) == visited.end() ||
//               visited[key] < top.pressure_released) {
//             ValveMonitor move_valve(**me, top.pressure_released,
//             top.flow_rate,
//                                     elapse_min, top.open_valves, *helper);
//             monitors.push(move_valve);
//             visited[key] = top.pressure_released;
//           }
//         }
//       }

//       // turn the valve on when positive flow and valve closed.
//       // also ensure a better cousin isn't queued
//       bool can_enable_my_valve =
//           top.valve->flow_rate > 0 &&
//           top.open_valves->find(top.valve->label) == top.open_valves->end();
//       if (can_enable_my_valve) {

//         int new_pressure_release =
//             top.pressure_released + (top.valve->flow_rate * elapse_min);
//         for (std::vector<Valve *>::iterator helper =
//                  top.help_valve->neighbors.begin();
//              helper != top.help_valve->neighbors.end(); ++helper) {
//           std::string key_label =
//               make_label(top.valve->label, (*helper)->label);
//           std::string open_valve_key = make_key(key_label, elapse_min);
//           if (visited.find(open_valve_key) == visited.end() ||
//               (visited[open_valve_key]
//                    .visited[open_valve_key]
//                    .pressure_released < new_pressure_release)) {

//             ValveMonitor open_monitor(
//                 *top.valve, new_pressure_release,
//                 top.flow_rate + top.valve->flow_rate, elapse_min,
//                 new std::unordered_set<std::string>(*top.open_valves),
//                 *helper);
//             open_monitor.open_valves->insert(top.valve->label);
//             monitors.push(open_monitor);
//             visited[open_valve_key] = open_monitor;
//           }
//         }
//       }

//       bool can_enable_helper_valve =
//           top.help_valve->flow_rate > 0 &&
//           top.open_valves->find(top.help_valve->label) ==
//               top.open_valves->end();
//       if (can_enable_helper_valve) {

//         int new_pressure_release =
//             increase_pressure_released + top.help_valve->flow_rate;
//         for (std::vector<Valve *>::iterator helper =
//                  top.valve->neighbors.begin();
//              helper != top.valve->neighbors.end(); ++helper) {
//           std::string key_label =
//               make_label(top.valve->label, (*helper)->label);
//           std::string open_valve_key = make_key(key_label, elapse_min);
//           if (visited.find(open_valve_key) == visited.end() ||
//               visited[open_valve_key].pressure_released <
//                   new_pressure_release) {

//             ValveMonitor open_monitor(
//                 **helper, new_pressure_release,
//                 top.flow_rate + top.help_valve->flow_rate, elapse_min,
//                 new std::unordered_set<std::string>(*top.open_valves),
//                 top.help_valve);
//             open_monitor.open_valves->insert(top.help_valve->label);
//             monitors.push(open_monitor);
//             visited[open_valve_key] = open_monitor;
//           }
//         }
//       }
//       // we've reached our targeted min
//     } else if (top.current_time == 1 &&
//                max_pressure_release < top.pressure_released) {
//       max_pressure_release = top.pressure_released;
//     }
//     // don't do anything if we exceeded targeted min.
//   }

//   return max_pressure_release;
// }