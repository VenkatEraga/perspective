/******************************************************************************
 *
 * Copyright (c) 2019, the Perspective Authors.
 *
 * This file is part of the Perspective library, distributed under the terms of
 * the Apache License 2.0.  The full license can be found in the LICENSE file.
 *
 */
#pragma once
#ifdef PSP_ENABLE_PYTHON

/******************************************************************************
 *
 * Pybind includes
 */
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h>
#include <boost/optional.hpp>


/******************************************************************************
 *
 * Perspective includes
 */
#include <perspective/first.h>
#include <perspective/raw_types.h>
#include <perspective/base.h>
#include <perspective/binding.h>
#include <perspective/exports.h>
#include <perspective/python/accessor.h>
#include <perspective/python/base.h>
#include <perspective/python/context.h>
#include <perspective/python/fill.h>
#include <perspective/python/table.h>
#include <perspective/python/utils.h>
#include <perspective/python/view.h>


namespace perspective {
namespace binding {
    // In perspective/python/*.h
}
}

/******************************************************************************
 *
 * Python binding
 */
using namespace perspective::binding;

PYBIND11_MODULE(libbinding, m)
{
    /******************************************************************************
     *
     * Table
     */
    py::class_<Table, std::shared_ptr<Table>>(m, "Table")
        .def(py::init<std::shared_ptr<t_pool>, std::vector<std::string>, std::vector<t_dtype>,
        std::uint32_t, std::string>())
        .def("size", &Table::size)
        .def("get_schema", &Table::get_schema)
        .def("unregister_gnode", &Table::unregister_gnode)
        .def("reset_gnode", &Table::reset_gnode)
        .def("get_id", &Table::get_id)
        .def("get_pool", &Table::get_pool)
        .def("get_gnode", &Table::get_gnode);

    /******************************************************************************
     *
     * View
     */
    // Bind a View for each context type

    py::class_<View<t_ctx0>, std::shared_ptr<View<t_ctx0>>>(m, "View_ctx0")
        .def(py::init<std::shared_ptr<Table>, std::shared_ptr<t_ctx0>, std::string, std::string,
            t_view_config>())
        .def("sides", &View<t_ctx0>::sides)
        .def("num_rows", &View<t_ctx0>::num_rows)
        .def("num_columns", &View<t_ctx0>::num_columns)
        .def("get_row_expanded", &View<t_ctx0>::get_row_expanded)
        .def("schema", &View<t_ctx0>::schema)
        .def("column_names", &View<t_ctx0>::column_names)
        .def("_get_deltas_enabled", &View<t_ctx0>::_get_deltas_enabled)
        .def("_set_deltas_enabled", &View<t_ctx0>::_set_deltas_enabled)
        .def("get_context", &View<t_ctx0>::get_context)
        .def("get_row_pivots", &View<t_ctx0>::get_row_pivots)
        .def("get_column_pivots", &View<t_ctx0>::get_column_pivots)
        .def("get_aggregates", &View<t_ctx0>::get_aggregates)
        .def("get_filter", &View<t_ctx0>::get_filter)
        .def("get_sort", &View<t_ctx0>::get_sort)
        .def("get_step_delta", &View<t_ctx0>::get_step_delta)
        .def("get_row_delta", &View<t_ctx0>::get_row_delta)
        .def("get_column_dtype", &View<t_ctx0>::get_column_dtype)
        .def("is_column_only", &View<t_ctx0>::is_column_only);

    py::class_<View<t_ctx1>, std::shared_ptr<View<t_ctx1>>>(m, "View_ctx1")
        .def(py::init<std::shared_ptr<Table>, std::shared_ptr<t_ctx1>, std::string, std::string,
            t_view_config>())
        .def("sides", &View<t_ctx1>::sides)
        .def("num_rows", &View<t_ctx1>::num_rows)
        .def("num_columns", &View<t_ctx1>::num_columns)
        .def("get_row_expanded", &View<t_ctx1>::get_row_expanded)
        .def("expand", &View<t_ctx1>::expand)
        .def("collapse", &View<t_ctx1>::collapse)
        .def("set_depth", &View<t_ctx1>::set_depth)
        .def("schema", &View<t_ctx1>::schema)
        .def("column_names", &View<t_ctx1>::column_names)
        .def("_get_deltas_enabled", &View<t_ctx1>::_get_deltas_enabled)
        .def("_set_deltas_enabled", &View<t_ctx1>::_set_deltas_enabled)
        .def("get_context", &View<t_ctx1>::get_context)
        .def("get_row_pivots", &View<t_ctx1>::get_row_pivots)
        .def("get_column_pivots", &View<t_ctx1>::get_column_pivots)
        .def("get_aggregates", &View<t_ctx1>::get_aggregates)
        .def("get_filter", &View<t_ctx1>::get_filter)
        .def("get_sort", &View<t_ctx1>::get_sort)
        .def("get_step_delta", &View<t_ctx1>::get_step_delta)
        .def("get_row_delta", &View<t_ctx1>::get_row_delta)
        .def("get_column_dtype", &View<t_ctx1>::get_column_dtype)
        .def("is_column_only", &View<t_ctx1>::is_column_only);

    py::class_<View<t_ctx2>, std::shared_ptr<View<t_ctx2>>>(m, "View_ctx2")
        .def(py::init<std::shared_ptr<Table>, std::shared_ptr<t_ctx2>, std::string, std::string,
            t_view_config>())
        .def("sides", &View<t_ctx2>::sides)
        .def("num_rows", &View<t_ctx2>::num_rows)
        .def("num_columns", &View<t_ctx2>::num_columns)
        .def("get_row_expanded", &View<t_ctx2>::get_row_expanded)
        .def("expand", &View<t_ctx2>::expand)
        .def("collapse", &View<t_ctx2>::collapse)
        .def("set_depth", &View<t_ctx2>::set_depth)
        .def("schema", &View<t_ctx2>::schema)
        .def("column_names", &View<t_ctx2>::column_names)
        .def("_get_deltas_enabled", &View<t_ctx2>::_get_deltas_enabled)
        .def("_set_deltas_enabled", &View<t_ctx2>::_set_deltas_enabled)
        .def("get_context", &View<t_ctx2>::get_context)
        .def("get_row_pivots", &View<t_ctx2>::get_row_pivots)
        .def("get_column_pivots", &View<t_ctx2>::get_column_pivots)
        .def("get_aggregates", &View<t_ctx2>::get_aggregates)
        .def("get_filter", &View<t_ctx2>::get_filter)
        .def("get_sort", &View<t_ctx2>::get_sort)
        .def("get_row_path", &View<t_ctx2>::get_row_path)
        .def("get_step_delta", &View<t_ctx2>::get_step_delta)
        .def("get_row_delta", &View<t_ctx2>::get_row_delta)
        .def("get_column_dtype", &View<t_ctx2>::get_column_dtype)
        .def("is_column_only", &View<t_ctx2>::is_column_only);

    /******************************************************************************
     *
     * t_view_config
     */
    py::class_<t_view_config>(m, "t_view_config")
        .def(py::init<std::vector<std::string>, std::vector<std::string>,
            tsl::ordered_map<std::string, std::string>, std::vector<std::string>,
            std::vector<std::tuple<std::string, std::string, std::vector<t_tscalar>>>,
            std::vector<std::vector<std::string>>, std::string, bool>())
        .def("add_filter_term", &t_view_config::add_filter_term);

    /******************************************************************************
     *
     * t_data_table
     */
    py::class_<t_data_table>(m, "t_data_table")
        .def("size", reinterpret_cast<unsigned long (t_data_table::*)() const>(&t_data_table::size));

    /******************************************************************************
     *
     * t_schema
     */
    py::class_<t_schema>(m, "t_schema")
        .def(py::init<>())
        .def("columns", &t_schema::columns)
        .def("types", &t_schema::types);

    /******************************************************************************
     *
     * t_gnode
     */
    py::class_<t_gnode>(m, "t_gnode")
        .def("get_id", reinterpret_cast<t_uindex (t_gnode::*)() const>(&t_gnode::get_id));

    /******************************************************************************
     *
     * t_data_slice
     */
    py::class_<t_data_slice<t_ctx0>>(m, "t_data_slice_ctx0")
        .def("get_column_slice", &t_data_slice<t_ctx0>::get_column_slice)
        .def("get_slice", &t_data_slice<t_ctx0>::get_slice)
        .def("get_column_names", &t_data_slice<t_ctx0>::get_column_names);

    py::class_<t_data_slice<t_ctx1>>(m, "t_data_slice_ctx1")
        .def("get_column_slice", &t_data_slice<t_ctx1>::get_column_slice)
        .def("get_slice", &t_data_slice<t_ctx1>::get_slice)
        .def("get_column_names", &t_data_slice<t_ctx1>::get_column_names)
        .def("get_row_path", &t_data_slice<t_ctx1>::get_row_path);

    py::class_<t_data_slice<t_ctx2>>(m, "t_data_slice_ctx2")
        .def("get_column_slice", &t_data_slice<t_ctx2>::get_column_slice)
        .def("get_slice", &t_data_slice<t_ctx2>::get_slice)
        .def("get_column_names", &t_data_slice<t_ctx2>::get_column_names)
        .def("get_row_path", &t_data_slice<t_ctx2>::get_row_path);

    /******************************************************************************
     *
     * t_ctx0
     */
    py::class_<t_ctx0>(m, "t_ctx0");

    /******************************************************************************
     *
     * t_ctx1
     */
    py::class_<t_ctx1>(m, "t_ctx1");

    /******************************************************************************
     *
     * t_ctx2
     */
    py::class_<t_ctx2>(m, "t_ctx2");


    /******************************************************************************
     *
     * t_pool
     */
    py::class_<t_pool, std::shared_ptr<t_pool>>(m, "t_pool")
        .def(py::init<>())
        .def("unregister_gnode", &t_pool::unregister_gnode)
        .def("_process", &t_pool::_process)
        // .def("set_update_delegate", &t_pool::set_update_delegate)
        ;

    /******************************************************************************
     *
     * t_tscalar
     */
    py::class_<t_tscalar>(m, "t_tscalar")
        .def(py::init<>());

    /******************************************************************************
     *
     * t_updctx
     */
    py::class_<t_updctx>(m, "t_updctx")
        .def(py::init<>())
        .def_readwrite("gnode_id", &t_updctx::m_gnode_id)
        .def_readwrite("ctx_name", &t_updctx::m_ctx);

    /******************************************************************************
     *
     * t_cellupd
     */
    py::class_<t_cellupd>(m, "t_cellupd")
        .def(py::init<>())
        .def_readwrite("row", &t_cellupd::row)
        .def_readwrite("column", &t_cellupd::column)
        .def_readwrite("old_value", &t_cellupd::old_value)
        .def_readwrite("new_value", &t_cellupd::new_value);

    /******************************************************************************
     *
     * t_stepdelta
     */
    py::class_<t_stepdelta>(m, "t_stepdelta")
        .def(py::init<>())
        .def_readwrite("rows_changed", &t_stepdelta::rows_changed)
        .def_readwrite("columns_changed", &t_stepdelta::columns_changed)
        .def_readwrite("cells", &t_stepdelta::cells);

    /******************************************************************************
     *
     * t_dtype
     */
    py::enum_<t_dtype>(m, "t_dtype")
        .value("DTYPE_NONE", DTYPE_NONE)
        .value("DTYPE_INT64", DTYPE_INT64)
        .value("DTYPE_INT32", DTYPE_INT32)
        .value("DTYPE_INT16", DTYPE_INT16)
        .value("DTYPE_INT8", DTYPE_INT8)
        .value("DTYPE_UINT64", DTYPE_UINT64)
        .value("DTYPE_UINT32", DTYPE_UINT32)
        .value("DTYPE_UINT16", DTYPE_UINT16)
        .value("DTYPE_UINT8", DTYPE_UINT8)
        .value("DTYPE_FLOAT64", DTYPE_FLOAT64)
        .value("DTYPE_FLOAT32", DTYPE_FLOAT32)
        .value("DTYPE_BOOL", DTYPE_BOOL)
        .value("DTYPE_TIME", DTYPE_TIME)
        .value("DTYPE_DATE", DTYPE_DATE)
        .value("DTYPE_ENUM", DTYPE_ENUM)
        .value("DTYPE_OID", DTYPE_OID)
        .value("DTYPE_PTR", DTYPE_PTR)
        .value("DTYPE_F64PAIR", DTYPE_F64PAIR)
        .value("DTYPE_USER_FIXED", DTYPE_USER_FIXED)
        .value("DTYPE_STR", DTYPE_STR)
        .value("DTYPE_USER_VLEN", DTYPE_USER_VLEN)
        .value("DTYPE_LAST_VLEN", DTYPE_LAST_VLEN)
        .value("DTYPE_LAST", DTYPE_LAST);

    /******************************************************************************
     *
     * t_op
     */
    py::enum_<t_op>(m, "t_op")
        .value("OP_INSERT", OP_INSERT)
        .value("OP_DELETE", OP_DELETE)
        .value("OP_CLEAR", OP_CLEAR);

    /******************************************************************************
     *
     * Perspective defs
     */
    m.def("arrow_test", &arrow_test);
    m.def("make_table", &make_table_py);
    m.def("make_computed_table", &make_computed_table_py);
    m.def("make_view_zero", &make_view_ctx0);
    m.def("make_view_one", &make_view_ctx1);
    m.def("make_view_two", &make_view_ctx2);
    m.def("get_data_slice_zero", &get_data_slice_ctx0);
    m.def("get_from_data_slice_zero", &get_from_data_slice_ctx0);
    m.def("get_data_slice_one", &get_data_slice_ctx1);
    m.def("get_from_data_slice_one", &get_from_data_slice_ctx1);
    m.def("get_data_slice_two", &get_data_slice_ctx2);
    m.def("get_from_data_slice_two", &get_from_data_slice_ctx2);
}

#endif
