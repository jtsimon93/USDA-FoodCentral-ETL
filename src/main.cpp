#include "services/FoodExtractorService.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

int main() {
  constexpr const char *input_locations_file_path = "input_locations.txt";
  std::unordered_map<std::string, std::string> input_map;

  std::ifstream input_file(input_locations_file_path);
  if (!input_file) {
    std::cerr << "Error opening file: " << input_locations_file_path
              << std::endl;
    return 1;
  }

  std::string line;
  while (std::getline(input_file, line)) {
    if (line.empty() || line[0] == '#')
      continue;

    std::istringstream iss(line);
    std::string key, value;

    if (std::getline(iss, key, '=') && std::getline(iss, value)) {
      input_map[key] = value;
    }
  }

  FoodExtractorService food_extractor_service(input_map["food_input_file"]);
  const auto &food_entries = food_extractor_service.GetFoodEntries();

  std::cout << "Parsed " << food_entries.size() << " food entries:\n";

  return 0;
}
