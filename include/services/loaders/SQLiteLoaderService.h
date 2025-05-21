#pragma once

/**
 * @file SQLiteLoaderService.h
 * @brief Service for loading USDA FoodData Central datasets into SQLite
 * database
 *
 * This service handles the "Load" phase of the ETL process, managing:
 * - Database schema creation and maintenance
 * - Transaction management
 * - Data insertion with prepared statements for performance
 * - Error handling during the loading process
 */

#include "models/usda/BrandedFood.h"
#include "models/usda/Food.h"
#include "models/usda/FoodCategory.h"
#include "sqlite/sqlite3.h"
#include <string>
#include <vector>

class SQLiteLoaderService {
public:
  /**
   * @brief Constructs a SQLiteLoaderService with specified database path
   *
   * @param dbPath Path to the SQLite database file (will be created if doesn't
   * exist)
   */
  SQLiteLoaderService(const std::string &dbPath);

  /**
   * @brief Destructor ensures database connection is properly closed
   */
  ~SQLiteLoaderService();

  /**
   * @brief Initialize the database connection and schema
   *
   * Creates necessary tables if they don't exist and prepares
   * the database for data loading operations.
   *
   * @return true if initialization succeeded, false otherwise
   */
  bool Initialize();

  /**
   * @brief Loads the food data into the database
   *
   * Handles the insertion of the food records into the SQLite database,
   * managing the transaction and handling potential errors during insertion.
   *
   * @param foods Vector of Food objects to insert into the database
   * @return true if loading succeeded, false if errors occurred
   */
  bool LoadFoods(const std::vector<USDA::Food> &foods);

  /**
   * @brief Loads branded food data into the database
   *
   * Handles the insertion of branded food records into the SQLite database,
   * managing the transaction and handling potential errors during insertion.
   *
   * @param branded_foods Vector of BrandedFood objects to insert into the
   * database
   * @return true if loading succeeded, false if errors occurred
   */
  bool LoadBrandedFood(const std::vector<USDA::BrandedFood> &branded_foods);

  /**
   * @brief Loads the food categories into the database
   *
   * Handles the insertion of food categories into the SQLite database,
   * managing the transaction and handling potential errors during insertion.
   *
   * @param food_categories Vector of FoodCategory objects to insert into the
   * database
   * @return true if loading succeeded, false if errors occurred
   */
  bool LoadFoodCategory(const std::vector<USDA::FoodCategory> &food_categories);

private:
  /**
   * @brief Creates all required tables in the database
   *
   * Defines the schema for branded foods and any related tables.
   * This method is called during initialization if tables don't exist.
   *
   * @return true if tables were created successfully, false otherwise
   */
  bool createTables();

  /**
   * @brief Begins an SQLite transaction for batch operations
   *
   * Transactions significantly improve insertion performance by
   * grouping multiple operations into a single commit.
   */
  void beginTransaction();

  /**
   * @brief Commits the current transaction
   *
   * Finalizes all changes made within the current transaction.
   */
  void commitTransaction();

  /**
   * @brief Rolls back the current transaction
   *
   * Reverts all changes made within the current transaction,
   * used when errors occur during data loading.
   */
  void rollbackTransaction();

  /**
   * @brief Prepares an SQLite statement for inserting data
   *
   * Creates and returns a prepared statement for the given table and columns,
   * improving performance for batch insertions.
   *
   * @param table Name of the table to insert into
   * @param columns Vector of column names for the insert statement
   * @return Pointer to prepared SQLite statement, nullptr if preparation failed
   */
  sqlite3_stmt *prepareInsertStatement(const std::string &table,
                                       const std::vector<std::string> &columns);

  /**
   * @brief Checks if a table exists in the database
   *
   * @param tableName Name of the table to check
   * @return true if the table exists, false otherwise
   */
  bool tableExists(const std::string &tableName);

  /**
   * @brief Logs an SQLite error with context information
   *
   * @param context Description of the operation that failed
   * @return SQLite error code
   */
  int logError(const std::string &context);

  /**
   * @brief Finalizes a prepared statement and handles any errors
   *
   * @param stmt Pointer to the statement to finalize
   * @return true if finalization was successful, false otherwise
   */
  bool finalizeStatement(sqlite3_stmt *stmt);

  /** SQLite database connection handle */
  sqlite3 *db = nullptr;

  /** Flag indicating if a transaction is currently active */
  bool transactionActive = false;
};
