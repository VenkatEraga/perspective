/******************************************************************************
 *
 * Copyright (c) 2019, the Perspective Authors.
 *
 * This file is part of the Perspective library, distributed under the terms of
 * the Apache License 2.0.  The full license can be found in the LICENSE file.
 *
 */
#ifdef PSP_ENABLE_PYTHON

#include <perspective/base.h>
#include <perspective/binding.h>
#include <perspective/python/base.h>
#include <perspective/python/utils.h>
#include <arrow/api.h>



/**
 * arrow example
 */



using arrow::DoubleBuilder;
using arrow::Int64Builder;
using arrow::ListBuilder;

// While we want to use columnar data structures to build efficient operations, we
// often receive data in a row-wise fashion from other systems. In the following,
// we want give a brief introduction into the classes provided by Apache Arrow by
// showing how to transform row-wise data into a columnar table.
//
// The data in this example is stored in the following struct:
struct data_row {
  int64_t id;
  double cost;
  std::vector<double> cost_components;
};

// Transforming a vector of structs into a columnar Table.
//
// The final representation should be an `arrow::Table` which in turn
// is made up of an `arrow::Schema` and a list of
// `arrow::ChunkedArray` instances. As the first step, we will iterate
// over the data and build up the arrays incrementally.  For this
// task, we provide `arrow::ArrayBuilder` classes that help in the
// construction of the final `arrow::Array` instances.
//
// For each type, Arrow has a specially typed builder class. For the primitive
// values `id` and `cost` we can use the respective `arrow::Int64Builder` and
// `arrow::DoubleBuilder`. For the `cost_components` vector, we need to have two
// builders, a top-level `arrow::ListBuilder` that builds the array of offsets and
// a nested `arrow::DoubleBuilder` that constructs the underlying values array that
// is referenced by the offsets in the former array.
arrow::Status VectorToColumnarTable(const std::vector<struct data_row>& rows,
                                    std::shared_ptr<arrow::Table>* table) {
  // The builders are more efficient using
  // arrow::jemalloc::MemoryPool::default_pool() as this can increase the size of
  // the underlying memory regions in-place. At the moment, arrow::jemalloc is only
  // supported on Unix systems, not Windows.
  arrow::MemoryPool* pool = arrow::default_memory_pool();

  Int64Builder id_builder(pool);
  DoubleBuilder cost_builder(pool);
  ListBuilder components_builder(pool, std::make_shared<DoubleBuilder>(pool));
  // The following builder is owned by components_builder.
  DoubleBuilder& cost_components_builder =
      *(static_cast<DoubleBuilder*>(components_builder.value_builder()));

  // Now we can loop over our existing data and insert it into the builders. The
  // `Append` calls here may fail (e.g. we cannot allocate enough additional memory).
  // Thus we need to check their return values. For more information on these values,
  // check the documentation about `arrow::Status`.
  for (const data_row& row : rows) {
    ARROW_RETURN_NOT_OK(id_builder.Append(row.id));
    ARROW_RETURN_NOT_OK(cost_builder.Append(row.cost));

    // Indicate the start of a new list row. This will memorise the current
    // offset in the values builder.
    ARROW_RETURN_NOT_OK(components_builder.Append());
    // Store the actual values. The final nullptr argument tells the underyling
    // builder that all added values are valid, i.e. non-null.
    ARROW_RETURN_NOT_OK(cost_components_builder.AppendValues(row.cost_components.data(),
                                                             row.cost_components.size()));
  }

  // At the end, we finalise the arrays, declare the (type) schema and combine them
  // into a single `arrow::Table`:
  std::shared_ptr<arrow::Array> id_array;
  ARROW_RETURN_NOT_OK(id_builder.Finish(&id_array));
  std::shared_ptr<arrow::Array> cost_array;
  ARROW_RETURN_NOT_OK(cost_builder.Finish(&cost_array));
  // No need to invoke cost_components_builder.Finish because it is implied by
  // the parent builder's Finish invocation.
  std::shared_ptr<arrow::Array> cost_components_array;
  ARROW_RETURN_NOT_OK(components_builder.Finish(&cost_components_array));

  std::vector<std::shared_ptr<arrow::Field>> schema_vector = {
      arrow::field("id", arrow::int64()), arrow::field("cost", arrow::float64()),
      arrow::field("cost_components", arrow::list(arrow::float64()))};

  auto schema = std::make_shared<arrow::Schema>(schema_vector);

  // The final `table` variable is the one we then can pass on to other functions
  // that can consume Apache Arrow memory structures. This object has ownership of
  // all referenced data, thus we don't have to care about undefined references once
  // we leave the scope of the function building the table and its underlying arrays.
  *table = arrow::Table::Make(schema, {id_array, cost_array, cost_components_array});

  return arrow::Status::OK();
}

arrow::Status ColumnarTableToVector(const std::shared_ptr<arrow::Table>& table,
                                    std::vector<struct data_row>* rows) {
  // To convert an Arrow table back into the same row-wise representation as in the
  // above section, we first will check that the table conforms to our expected
  // schema and then will build up the vector of rows incrementally.
  //
  // For the check if the table is as expected, we can utilise solely its schema.
  std::vector<std::shared_ptr<arrow::Field>> schema_vector = {
      arrow::field("id", arrow::int64()), arrow::field("cost", arrow::float64()),
      arrow::field("cost_components", arrow::list(arrow::float64()))};
  auto expected_schema = std::make_shared<arrow::Schema>(schema_vector);

  if (!expected_schema->Equals(*table->schema())) {
    // The table doesn't have the expected schema thus we cannot directly
    // convert it to our target representation.
    return arrow::Status::Invalid("Schemas are not matching!");
  }

  // As we have ensured that the table has the expected structure, we can unpack the
  // underlying arrays. For the primitive columns `id` and `cost` we can use the high
  // level functions to get the values whereas for the nested column
  // `cost_components` we need to access the C-pointer to the data to copy its
  // contents into the resulting `std::vector<double>`. Here we need to be care to
  // also add the offset to the pointer. This offset is needed to enable zero-copy
  // slicing operations. While this could be adjusted automatically for double
  // arrays, this cannot be done for the accompanying bitmap as often the slicing
  // border would be inside a byte.

  auto ids =
      std::static_pointer_cast<arrow::Int64Array>(table->column(0)->chunk(0));
  auto costs =
      std::static_pointer_cast<arrow::DoubleArray>(table->column(1)->chunk(0));
  auto cost_components =
      std::static_pointer_cast<arrow::ListArray>(table->column(2)->chunk(0));
  auto cost_components_values =
      std::static_pointer_cast<arrow::DoubleArray>(cost_components->values());
  // To enable zero-copy slices, the native values pointer might need to account
  // for this slicing offset. This is not needed for the higher level functions
  // like Value(…) that already account for this offset internally.
  const double* ccv_ptr = cost_components_values->data()->GetValues<double>(1);

  for (int64_t i = 0; i < table->num_rows(); i++) {
    // Another simplification in this example is that we assume that there are
    // no null entries, e.g. each row is fill with valid values.
    int64_t id = ids->Value(i);
    double cost = costs->Value(i);
    const double* first = ccv_ptr + cost_components->value_offset(i);
    const double* last = ccv_ptr + cost_components->value_offset(i + 1);
    std::vector<double> components_vec(first, last);
    rows->push_back({id, cost, components_vec});
  }

  return arrow::Status::OK();
}

#define EXIT_ON_FAILURE(expr)                      \
  do {                                             \
    ::arrow::Status status_ = (expr);                \
    if (!status_.ok()) {                           \
      std::cerr << status_.message() << std::endl; \
    }                                              \
  } while (0);



namespace perspective {
namespace binding {

/******************************************************************************
 *
 * Date Parsing
 */
t_date
pythondate_to_t_date(t_val date) {
    return t_date(date.attr("year").cast<std::int32_t>(),
        date.attr("month").cast<std::int32_t>(),
        date.attr("day").cast<std::int32_t>());
}

t_dtype type_string_to_t_dtype(std::string value, std::string name){
    auto type = t_dtype::DTYPE_STR;

    // TODO consider refactor
    if (value == "int") {
        // Python int
        type = t_dtype::DTYPE_INT64;
    } else if (value == "int8") {
        // Numpy int8
        type = t_dtype::DTYPE_INT8;
    } else if (value == "int16") {
        // Numpy int16
        type = t_dtype::DTYPE_INT16;
    } else if (value == "int32") {
        // Numpy int32
        type = t_dtype::DTYPE_INT32;
    } else if (value == "int64") {
        // Numpy int64
        type = t_dtype::DTYPE_INT64;
    } else if (value == "float") {
        // Python float
        type = t_dtype::DTYPE_FLOAT64;
    } else if (value == "float16") {
        // TODO
        // Numpy float16
        // type = t_dtype::DTYPE_FLOAT16;
        type = t_dtype::DTYPE_FLOAT32;
    } else if (value == "float32") {
        // Numpy float32
        type = t_dtype::DTYPE_FLOAT32;
    } else if (value == "float64") {
        // Numpy float64
        type = t_dtype::DTYPE_FLOAT64;
    } else if (value == "float128") {
        // TODO
        // Numpy float128
        type = t_dtype::DTYPE_FLOAT64;
    } else if (value == "str") {
        // Python unicode str
        type = t_dtype::DTYPE_STR;
    } else if (value == "bool") {
        // Python bool
        type = t_dtype::DTYPE_BOOL;
    } else if (value == "bool_") {
        // Numpy bool
        type = t_dtype::DTYPE_BOOL;
    } else if (value == "bool8") {
        // Numpy bool8
        type = t_dtype::DTYPE_BOOL;
    } else if (value == "datetime") {
        // Python datetime
        // TODO inheritance
        type = t_dtype::DTYPE_TIME;
    } else if (value == "datetime64") {
        // Numpy datetime64
        type = t_dtype::DTYPE_TIME;
    } else if (value == "Timestamp") {
        // Pandas timestamp
        type = t_dtype::DTYPE_TIME;
    } else if (value == "date") {
        // Python date
        // TODO inheritance
        type = t_dtype::DTYPE_DATE;
    } else {
        CRITICAL("Unknown type '%s' for key '%s'", value, name);
    }
    return type;
}

t_dtype type_string_to_t_dtype(py::str type, py::str name){
    return type_string_to_t_dtype(type.cast<std::string>(), name.cast<std::string>());
}


void arrow_test() {
    std::cout << "here" << std::endl;
    //from arrow example
    std::vector<data_row> rows = {
      {1, 1.0, {1.0}}, {2, 2.0, {1.0, 2.0}}, {3, 3.0, {1.0, 2.0, 3.0}}};
    std::shared_ptr<::arrow::Table> table;
    EXIT_ON_FAILURE(VectorToColumnarTable(rows, &table));
    std::vector<data_row> expected_rows;
    EXIT_ON_FAILURE(ColumnarTableToVector(table, &expected_rows));
    assert(rows.size() == expected_rows.size());
    std::cout << "here2" << std::endl;

}

} //namespace binding
} //namespace perspective

#endif