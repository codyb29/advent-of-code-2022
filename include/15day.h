#ifndef DAY15_H
#define DAY15_H

#include <climits>
#include <cmath>
#include <string>
#include <vector>

int compute_manhattan_distance(const int &x1, const int &y1, const int &x2,
                               const int &y2);

struct Telemetry {
  int x_sensor;
  int y_sensor;
  int x_beacon;
  int y_beacon;
  int distance;

  Telemetry(int x_sensor, int y_sensor, int x_beacon, int y_beacon) {
    this->x_sensor = x_sensor;
    this->y_sensor = y_sensor;
    this->x_beacon = x_beacon;
    this->y_beacon = y_beacon;
    this->distance =
        compute_manhattan_distance(x_sensor, y_sensor, x_beacon, y_beacon);
  }
};

class BeaconExclusionZone {
  static std::vector<Telemetry> get_telemetry_from_sensor_report(
      const std::vector<std::string> &sensor_report);

public:
  static long long get_tuning_frequency(std::vector<std::string> sensor_info,
                                        const int &grid_length);
  static int no_beacon_present(std::vector<std::string> sensor_info,
                               const int &y);
};

#endif