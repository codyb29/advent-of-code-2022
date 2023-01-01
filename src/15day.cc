#include "15day.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <unordered_set>

int compute_manhattan_distance(const int &x1, const int &y1, const int &x2,
                               const int &y2) {
  return std::abs(x1 - x2) + std::abs(y1 - y2);
}

std::vector<Telemetry> BeaconExclusionZone::get_telemetry_from_sensor_report(
    const std::vector<std::string> &sensor_report) {
  std::vector<Telemetry> result;

  for (std::vector<std::string>::const_iterator info = sensor_report.begin();
       info != sensor_report.end(); ++info) {
    int x_sensor;
    int y_sensor;
    int x_beacon;
    int y_beacon;
    sscanf((*info).c_str(),
           "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &x_sensor,
           &y_sensor, &x_beacon, &y_beacon);
    result.emplace_back(x_sensor, y_sensor, x_beacon, y_beacon);
  }

  return result;
}

long long
BeaconExclusionZone::get_tuning_frequency(std::vector<std::string> sensor_info,
                                          const int &grid_length) {
  std::vector<Telemetry> telemetry =
      BeaconExclusionZone::get_telemetry_from_sensor_report(sensor_info);

  long long x = 0, y = 0;
  int px = grid_length, py = grid_length;

  while (x != px || y != py) {
    px = x;
    py = y;
    for (std::vector<Telemetry>::const_iterator data = telemetry.begin();
         data != telemetry.end(); ++data) {

      int horizontal = data->distance - std::abs(y - data->y_sensor);
      if (horizontal >= 0) {
        int xMin = std::max(0, data->x_sensor - horizontal);
        int xMax = std::min(grid_length, data->x_sensor + horizontal);
        if (xMin <= x && x <= xMax) {
          x = xMax + 1;
          while (x > grid_length) {
            y += 1;
            horizontal = data->distance - std::abs(y - data->y_sensor);
            if (horizontal >= 0) {
              xMin = std::max(0, data->x_sensor - horizontal);
              xMax = std::min(grid_length, data->x_sensor + horizontal);
              if (xMin == 0) {
                x = xMax;
              } else {
                x = 0;
              }
            } else {
              x = 0;
            }
          }
        }
      }
    }
  }

  return x * 4000000LL + y;
}

int BeaconExclusionZone::no_beacon_present(std::vector<std::string> sensor_info,
                                           const int &y) {
  std::unordered_set<int> x_coordinates;
  std::vector<Telemetry> telemetry =
      BeaconExclusionZone::get_telemetry_from_sensor_report(sensor_info);
  // loop through all telemetry
  for (std::vector<Telemetry>::const_iterator data = telemetry.begin();
       data != telemetry.end(); ++data) {
    // is sensor +- distance able to reach y?
    if ((data->y_sensor < y && data->y_sensor + data->distance >= y) ||
        (data->y_sensor > y && data->y_sensor - data->distance <= y)) {
      // able to reach y, compute partial circle until < y
      int max = data->x_sensor + data->distance + 1;
      for (int i = data->x_sensor - data->distance; i < max; i++) {

        if (compute_manhattan_distance(data->x_sensor, data->y_sensor, i, y) <=
            data->distance) {
          x_coordinates.insert(i);
        }
      }

      // remove the beacon if it was added above.
      if (data->y_beacon == y &&
          x_coordinates.find(data->x_beacon) != x_coordinates.end()) {
        x_coordinates.erase(data->x_beacon);
      }
    }
    // don't do anything if the sensor can't reach y. doesn't tell us
    // anything.
  }
  return x_coordinates.size();
}