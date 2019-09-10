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

} //namespace binding
} //namespace perspective

#endif