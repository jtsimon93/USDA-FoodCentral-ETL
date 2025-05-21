#include "services/loaders/SQLiteLoaderService.h"
#include <iostream>
#include <sstream>

SQLiteLoaderService::SQLiteLoaderService(const std::string &dbPath) {
  int rc = sqlite3_open(dbPath.c_str(), &db);
  if (rc != SQLITE_OK) {
    std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    db = nullptr;
  }
}

SQLiteLoaderService::~SQLiteLoaderService() {
  if (db) {
    if (transactionActive) {
      rollbackTransaction();
    }
    sqlite3_close(db);
    db = nullptr;
  }
}

bool SQLiteLoaderService::Initialize() {
  if (!db) {
    std::cerr << "Database connection not established" << std::endl;
    return false;
  }

  return createTables();
}

bool SQLiteLoaderService::createTables() {
  if (!db)
    return false;

  // Create foods table if it doesn't exit
  if (!tableExists("foods")) {
    const char *sql = R"SQL(
            CREATE TABLE foods (
                fdc_id INTEGER PRIMARY KEY,
                data_type TEXT,
                description TEXT,
                food_category_id TEXT,
                publication_date TEXT
            ))SQL";

    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
      std::cerr << "Error creating foods table: " << errMsg << std::endl;
      sqlite3_free(errMsg);
      return false;
    }
  }

  // Create branded_foods table if it doesn't exist
  if (!tableExists("branded_foods")) {
    const char *sql = R"SQL(
            CREATE TABLE branded_foods (
                fdc_id INTEGER PRIMARY KEY,
                brand_owner TEXT,
                brand_name TEXT,
                subbrand_name TEXT,
                gtin_upc TEXT,
                ingredients TEXT,
                not_a_significant_source_of TEXT,
                serving_size REAL,
                serving_size_unit TEXT,
                household_serving_fulltext TEXT,
                branded_food_category TEXT,
                data_source TEXT,
                package_weight TEXT,
                modified_date TEXT,
                available_date TEXT,
                discontinued_date TEXT,
                market_country TEXT,
                preparation_state_code TEXT,
                trade_channel TEXT,
                short_description TEXT,
                material_code TEXT
            )
        )SQL";

    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
      std::cerr << "Error creating branded_foods table: " << errMsg
                << std::endl;
      sqlite3_free(errMsg);
      return false;
    }
  }

  return true;
}

bool SQLiteLoaderService::tableExists(const std::string &tableName) {
  if (!db)
    return false;

  std::string sql =
      "SELECT name FROM sqlite_master WHERE type='table' AND name=?";
  sqlite3_stmt *stmt;

  int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    logError("Preparing table existence check");
    return false;
  }

  rc = sqlite3_bind_text(stmt, 1, tableName.c_str(), -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    logError("Binding table name parameter");
    sqlite3_finalize(stmt);
    return false;
  }

  bool exists = false;
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    exists = true;
  }

  sqlite3_finalize(stmt);
  return exists;
}

void SQLiteLoaderService::beginTransaction() {
  if (!db)
    return;

  char *errMsg = nullptr;
  int rc = sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &errMsg);

  if (rc != SQLITE_OK) {
    std::cerr << "Error beginning transaction: " << errMsg << std::endl;
    sqlite3_free(errMsg);
  } else {
    transactionActive = true;
  }
}

void SQLiteLoaderService::commitTransaction() {
  if (!db || !transactionActive)
    return;

  char *errMsg = nullptr;
  int rc = sqlite3_exec(db, "COMMIT", nullptr, nullptr, &errMsg);

  if (rc != SQLITE_OK) {
    std::cerr << "Error committing transaction: " << errMsg << std::endl;
    sqlite3_free(errMsg);
  } else {
    transactionActive = false;
  }
}

void SQLiteLoaderService::rollbackTransaction() {
  if (!db || !transactionActive)
    return;

  char *errMsg = nullptr;
  int rc = sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &errMsg);

  if (rc != SQLITE_OK) {
    std::cerr << "Error rolling back transaction: " << errMsg << std::endl;
    sqlite3_free(errMsg);
  } else {
    transactionActive = false;
  }
}

sqlite3_stmt *SQLiteLoaderService::prepareInsertStatement(
    const std::string &table, const std::vector<std::string> &columns) {

  if (!db)
    return nullptr;

  std::stringstream sql;
  sql << "INSERT INTO " << table << " (";

  // Add column names
  for (size_t i = 0; i < columns.size(); ++i) {
    sql << columns[i];
    if (i < columns.size() - 1) {
      sql << ", ";
    }
  }

  sql << ") VALUES (";

  // Add placeholders
  for (size_t i = 0; i < columns.size(); ++i) {
    sql << "?";
    if (i < columns.size() - 1) {
      sql << ", ";
    }
  }

  sql << ")";

  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    logError("Preparing insert statement");
    return nullptr;
  }

  return stmt;
}

bool SQLiteLoaderService::finalizeStatement(sqlite3_stmt *stmt) {
  if (!stmt)
    return false;

  int rc = sqlite3_finalize(stmt);
  if (rc != SQLITE_OK) {
    logError("Finalizing statement");
    return false;
  }

  return true;
}

int SQLiteLoaderService::logError(const std::string &context) {
  if (!db)
    return SQLITE_ERROR;

  int errorCode = sqlite3_errcode(db);
  const char *errorMsg = sqlite3_errmsg(db);

  std::cerr << "SQLite error in " << context << ": " << errorCode << " - "
            << errorMsg << std::endl;

  return errorCode;
}

bool SQLiteLoaderService::LoadFoods(const std::vector<USDA::Food> &foods) {
  if (!db || foods.empty()) {
    return false;
  }

  // Define the columns for the insert statement
  std::vector<std::string> columns = {"fdc_id", "data_type", "description",
                                      "food_category_id", "publication_date"};

  // Prepare the insert statement
  sqlite3_stmt *stmt = prepareInsertStatement("foods", columns);
  if (!stmt) {
    return false;
  }

  // Begin transaction for better performance
  beginTransaction();

  bool success = true;
  int count = 0;
  const int BATCH_SIZE = 10000;

  for (const auto &food : foods) {
    // Bind parameters to the statement
    int idx = 1;
    sqlite3_bind_int(stmt, idx++, food.fdc_id);
    sqlite3_bind_text(stmt, idx++,
                      food.data_type == USDA::FoodDataType::Foundation
                          ? "foundation_food"
                          : "branded_food",
                      -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, idx++, food.description.c_str(), -1, SQLITE_STATIC);
    food.food_category_id
        ? sqlite3_bind_text(stmt, idx++, food.food_category_id->c_str(), -1,
                            SQLITE_STATIC)
        : sqlite3_bind_null(stmt, idx++);
    std::string dateStr =
        std::to_string(static_cast<int>(food.publication_date.year())) + "-" +
        std::to_string(static_cast<unsigned>(food.publication_date.month())) +
        "-" +
        std::to_string(static_cast<unsigned>(food.publication_date.day()));
    sqlite3_bind_text(stmt, idx++, dateStr.c_str(), -1, SQLITE_STATIC);

    // Execute the statement
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
      logError("Inserting food record");
      success = false;
      break;
    }

    // Reset the statement for the next iteration
    sqlite3_reset(stmt);

    // Commit in batches to avoid excessive memory usage
    count++;
    if (count % BATCH_SIZE == 0) {
      commitTransaction();
      beginTransaction();
      std::cout << "Inserted " << count << " foods of " << foods.size()
                << " food records..." << std::endl;
    }
  }

  // Finalize the statement
  finalizeStatement(stmt);

  if (success) {
    commitTransaction();
  } else {
    rollbackTransaction();
  }

  return success;
}

bool SQLiteLoaderService::LoadBrandedFood(
    const std::vector<USDA::BrandedFood> &branded_foods) {

  if (!db || branded_foods.empty()) {
    return false;
  }

  // Define the columns for the insert statement
  std::vector<std::string> columns = {"fdc_id",
                                      "brand_owner",
                                      "brand_name",
                                      "subbrand_name",
                                      "gtin_upc",
                                      "ingredients",
                                      "not_a_significant_source_of",
                                      "serving_size",
                                      "serving_size_unit",
                                      "household_serving_fulltext",
                                      "branded_food_category",
                                      "data_source",
                                      "package_weight",
                                      "modified_date",
                                      "available_date",
                                      "discontinued_date",
                                      "market_country",
                                      "preparation_state_code",
                                      "trade_channel",
                                      "short_description",
                                      "material_code"};

  // Prepare the insert statement
  sqlite3_stmt *stmt = prepareInsertStatement("branded_foods", columns);
  if (!stmt) {
    return false;
  }

  // Begin transaction for better performance
  beginTransaction();

  bool success = true;
  int count = 0;
  const int BATCH_SIZE = 10000;

  for (const auto &food : branded_foods) {
    // Bind parameters to the statement
    int idx = 1;
    sqlite3_bind_int(stmt, idx++, food.fdc_id);
    food.brand_owner ? sqlite3_bind_text(stmt, idx++, food.brand_owner->c_str(),
                                         -1, SQLITE_TRANSIENT)
                     : sqlite3_bind_null(stmt, idx++);
    food.brand_name ? sqlite3_bind_text(stmt, idx++, food.brand_name->c_str(),
                                        -1, SQLITE_TRANSIENT)
                    : sqlite3_bind_null(stmt, idx++);
    food.subbrand_name
        ? sqlite3_bind_text(stmt, idx++, food.subbrand_name->c_str(), -1,
                            SQLITE_TRANSIENT)
        : sqlite3_bind_null(stmt, idx++);
    food.gtin_upc ? sqlite3_bind_text(stmt, idx++, food.gtin_upc->c_str(), -1,
                                      SQLITE_TRANSIENT)
                  : sqlite3_bind_null(stmt, idx++);
    food.ingredients ? sqlite3_bind_text(stmt, idx++, food.ingredients->c_str(),
                                         -1, SQLITE_TRANSIENT)
                     : sqlite3_bind_null(stmt, idx++);
    food.not_a_significant_source_of
        ? sqlite3_bind_text(stmt, idx++,
                            food.not_a_significant_source_of->c_str(), -1,
                            SQLITE_TRANSIENT)
        : sqlite3_bind_null(stmt, idx++);

    // Handle optional numeric fields
    if (food.serving_size) {
      sqlite3_bind_double(stmt, idx++, *food.serving_size);
    } else {
      sqlite3_bind_null(stmt, idx++);
    }

    food.serving_size_unit
        ? sqlite3_bind_text(stmt, idx++, food.serving_size_unit->c_str(), -1,
                            SQLITE_TRANSIENT)
        : sqlite3_bind_null(stmt, idx++);
    food.household_serving_fulltext
        ? sqlite3_bind_text(stmt, idx++,
                            food.household_serving_fulltext->c_str(), -1,
                            SQLITE_TRANSIENT)
        : sqlite3_bind_null(stmt, idx++);
    food.branded_food_category
        ? sqlite3_bind_text(stmt, idx++, food.branded_food_category->c_str(),
                            -1, SQLITE_TRANSIENT)
        : sqlite3_bind_null(stmt, idx++);
    food.data_source ? sqlite3_bind_text(stmt, idx++, food.data_source->c_str(),
                                         -1, SQLITE_TRANSIENT)
                     : sqlite3_bind_null(stmt, idx++);
    food.package_weight
        ? sqlite3_bind_text(stmt, idx++, food.package_weight->c_str(), -1,
                            SQLITE_TRANSIENT)
        : sqlite3_bind_null(stmt, idx++);

    // Handle date fields by converting year_month_day to string
    if (food.modified_date) {
      std::string dateStr =
          std::to_string(static_cast<int>(food.modified_date->year())) + "-" +
          std::to_string(static_cast<unsigned>(food.modified_date->month())) +
          "-" +
          std::to_string(static_cast<unsigned>(food.modified_date->day()));
      sqlite3_bind_text(stmt, idx++, dateStr.c_str(), -1, SQLITE_TRANSIENT);
    } else {
      sqlite3_bind_null(stmt, idx++);
    }

    if (food.available_date) {
      std::string dateStr =
          std::to_string(static_cast<int>(food.available_date->year())) + "-" +
          std::to_string(static_cast<unsigned>(food.available_date->month())) +
          "-" +
          std::to_string(static_cast<unsigned>(food.available_date->day()));
      sqlite3_bind_text(stmt, idx++, dateStr.c_str(), -1, SQLITE_TRANSIENT);
    } else {
      sqlite3_bind_null(stmt, idx++);
    }

    if (food.discontinued_date) {
      std::string dateStr =
          std::to_string(static_cast<int>(food.discontinued_date->year())) +
          "-" +
          std::to_string(
              static_cast<unsigned>(food.discontinued_date->month())) +
          "-" +
          std::to_string(static_cast<unsigned>(food.discontinued_date->day()));
      sqlite3_bind_text(stmt, idx++, dateStr.c_str(), -1, SQLITE_TRANSIENT);
    } else {
      sqlite3_bind_null(stmt, idx++);
    }

    food.market_country
        ? sqlite3_bind_text(stmt, idx++, food.market_country->c_str(), -1,
                            SQLITE_TRANSIENT)
        : sqlite3_bind_null(stmt, idx++);
    food.preparation_state_code
        ? sqlite3_bind_text(stmt, idx++, food.preparation_state_code->c_str(),
                            -1, SQLITE_TRANSIENT)
        : sqlite3_bind_null(stmt, idx++);
    food.trade_channel
        ? sqlite3_bind_text(stmt, idx++, food.trade_channel->c_str(), -1,
                            SQLITE_TRANSIENT)
        : sqlite3_bind_null(stmt, idx++);
    food.short_description
        ? sqlite3_bind_text(stmt, idx++, food.short_description->c_str(), -1,
                            SQLITE_TRANSIENT)
        : sqlite3_bind_null(stmt, idx++);
    food.material_code
        ? sqlite3_bind_text(stmt, idx++, food.material_code->c_str(), -1,
                            SQLITE_TRANSIENT)
        : sqlite3_bind_null(stmt, idx++);

    // Execute the statement
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
      logError("Inserting branded food record");
      success = false;
      break;
    }

    // Reset the statement for the next record
    sqlite3_reset(stmt);

    // Commit in batches to avoid excessive memory usage
    count++;
    if (count % BATCH_SIZE == 0) {
      commitTransaction();
      beginTransaction();
      std::cout << "Inserted " << count << " of " << branded_foods.size()
                << " branded food records..." << std::endl;
    }
  }

  // Finalize the statement
  finalizeStatement(stmt);

  // Commit or rollback the transaction based on success
  if (success) {
    commitTransaction();
    std::cout << "Successfully loaded " << count << " branded food records"
              << std::endl;
  } else {
    rollbackTransaction();
    std::cout << "Failed to load branded foods. Rolling back transaction."
              << std::endl;
  }

  return success;
}
