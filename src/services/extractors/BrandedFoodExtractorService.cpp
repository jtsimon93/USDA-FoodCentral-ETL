#include "services/extractors/BrandedFoodExtractorService.h"
#include "csv-parser.h"
#include "models/usda/BrandedFood.h"
#include <optional>

BrandedFoodExtractorService::BrandedFoodExtractorService(
    const std::string &branded_food_input_file)
    : branded_food_input_file(branded_food_input_file) {}

std::vector<USDA::BrandedFood> &
BrandedFoodExtractorService::GetBrandedFoodEntries() {
  ExtractBrandedFoodEntries();
  return branded_food_entries;
}

void BrandedFoodExtractorService::ExtractBrandedFoodEntries() {
  csv::CSVReader reader(branded_food_input_file);

  for (csv::CSVRow &row : reader) {
    USDA::BrandedFood branded_food;

    try {
      branded_food.fdc_id = row[0].get<int>();
      branded_food.brand_owner =
          row[1].is_null() ? std::nullopt
                           : std::make_optional(row[1].get<std::string>());
      branded_food.brand_name =
          row[2].is_null() ? std::nullopt
                           : std::make_optional(row[2].get<std::string>());
      branded_food.subbrand_name =
          row[3].is_null() ? std::nullopt
                           : std::make_optional(row[3].get<std::string>());
      branded_food.gtin_upc =
          row[4].is_null() ? std::nullopt
                           : std::make_optional(row[4].get<std::string>());
      branded_food.ingredients =
          row[5].is_null() ? std::nullopt
                           : std::make_optional(row[5].get<std::string>());
      branded_food.not_a_significant_source_of =
          row[6].is_null() ? std::nullopt
                           : std::make_optional(row[6].get<std::string>());
      branded_food.serving_size = row[7].is_null()
                                      ? std::nullopt
                                      : std::make_optional(row[7].get<float>());
      branded_food.serving_size_unit =
          row[8].is_null() ? std::nullopt
                           : std::make_optional(row[8].get<std::string>());
      branded_food.household_serving_fulltext =
          row[9].is_null() ? std::nullopt
                           : std::make_optional(row[9].get<std::string>());
      branded_food.branded_food_category =
          row[10].is_null() ? std::nullopt
                            : std::make_optional(row[10].get<std::string>());
      branded_food.data_source =
          row[11].is_null() ? std::nullopt
                            : std::make_optional(row[11].get<std::string>());
      branded_food.package_weight =
          row[12].is_null() ? std::nullopt
                            : std::make_optional(row[12].get<std::string>());

      auto parse_date = [](const std::string &s)
          -> std::optional<std::chrono::year_month_day> {
        if (s.size() >= 10) {
          try {
            int y = std::stoi(s.substr(0, 4));
            int m = std::stoi(s.substr(5, 2));
            int d = std::stoi(s.substr(8, 2));

            return std::chrono::year_month_day{
                std::chrono::year{y},
                std::chrono::month{static_cast<unsigned int>(m)},
                std::chrono::day{static_cast<unsigned int>(d)}};

          } catch (...) {
            return std::nullopt;
          }
        }
        return std::nullopt;
      };

      branded_food.modified_date = row[13].is_null()
                                       ? std::nullopt
                                       : parse_date(row[13].get<std::string>());
      branded_food.available_date =
          row[14].is_null() ? std::nullopt
                            : parse_date(row[14].get<std::string>());
      branded_food.market_country =
          row[15].is_null() ? std::nullopt
                            : std::make_optional(row[15].get<std::string>());
      branded_food.discontinued_date =
          row[16].is_null() ? std::nullopt
                            : parse_date(row[16].get<std::string>());
      branded_food.preparation_state_code =
          row[17].is_null() ? std::nullopt
                            : std::make_optional(row[17].get<std::string>());
      branded_food.trade_channel =
          row[18].is_null() ? std::nullopt
                            : std::make_optional(row[18].get<std::string>());
      branded_food.short_description =
          row[19].is_null() ? std::nullopt
                            : std::make_optional(row[19].get<std::string>());
      branded_food.material_code =
          row[20].is_null() ? std::nullopt
                            : std::make_optional(row[20].get<std::string>());

      branded_food_entries.push_back(branded_food);
    } catch (const std::exception &e) {
      std::cerr << "Failed to parse branded food row: " << e.what() << "\n";
    }
  }
}
