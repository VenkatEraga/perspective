/******************************************************************************
 *
 * Copyright (c) 2017, the Perspective Authors.
 *
 * This file is part of the Perspective library, distributed under the terms of
 * the Apache License 2.0.  The full license can be found in the LICENSE file.
 *
 */

import {PropsBuilder} from "@finos/perspective-viewer/dist/esm/custom_styles";
import {cloneDeep} from "lodash";

const properties = new PropsBuilder();
const title = `--hypergrid`;

properties.add_fonts({
    font: title,
    columnHeaderFont: `${title}-header`,
    rowHeaderFont: title,
    treeHeaderFont: title
});

properties.add_styles({
    treeHeaderBackgroundColor: `${title}-tree-header--background`,
    backgroundColor: `${title}--background`,
    treeHeaderColor: `${title}-tree-header--color`,
    color: [`${title}--color`, `color`],
    columnHeaderBackgroundColor: `${title}-header--background`,
    columnHeaderSeparatorColor: `${title}-separator--color`,
    columnHeaderColor: `${title}-header--color`,
    columnColorNumberPositive: `${title}-positive--color`,
    columnColorNumberNegative: `${title}-negative--color`,
    columnBackgroundColorNumberPositive: `${title}-positive--background`,
    columnBackgroundColorNumberNegative: `${title}-negative--background`,
    halign: `${title}--text-align`,
    columnHeaderHalign: `${title}--text-align`,
    hoverCellHighlight: {
        enabled: true,
        backgroundColor: `${title}-cell-hover--background`,
        color: `${title}-cell-hover--color`
    }
});

properties.add_measures({
    width: `${title}--width`,
    minimumColumnWidth: `${title}--min-width`,
    maximumColumnWidth: `${title}--max-width`
});

properties.add_styles({
    hoverRowHighlight: {
        enabled: true,
        backgroundColor: `${title}-row-hover--background`,
        color: `${title}-row-hover--color`
    }
});

export function get_styles(elem) {
    return properties.get_properties(elem);
}

export function clear_styles(elem) {
    return properties.clear_properties(elem);
}

const COLUMN_HEADER_FONT = "12px Helvetica, sans-serif";
const GROUP_LABEL_FONT = "12px Open Sans, sans-serif";

const base_grid_properties = {
    autoSelectRows: false,
    cellPadding: 5,
    cellSelection: false,
    columnSelection: false,
    rowSelection: false,
    checkboxOnlyRowSelections: false,
    columnClip: true,
    columnHeaderFont: COLUMN_HEADER_FONT,
    columnHeaderForegroundSelectionFont: '12px "Arial", Helvetica, sans-serif',
    columnsReorderable: false,
    defaultRowHeight: 24,
    editable: true,
    editOnKeydown: true,
    editorActivationKeys: ["alt", "esc"],
    enableContinuousRepaint: false,
    feedbackCount: 1000000,
    fixedColumnCount: 0,
    fixedRowCount: 0,
    fixedLinesHWidth: 1,
    fixedLinesVWidth: 1,
    font: '12px "Arial", Helvetica, sans-serif',
    foregroundSelectionFont: '12px "Arial", Helvetica, sans-serif',
    gridLinesH: false,
    gridLinesV: true, // except: due to groupedHeaderPlugin's `clipRuleLines: true` option, only header row displays these lines
    gridLinesUserDataArea: false, // restricts vertical rule line rendering to header row only
    groupedHeader: {
        paintBackground: null, // no group header label decoration
        columnHeaderLines: false, // only draw vertical rule lines between group labels
        groupConfig: [
            {
                halign: "center", // center group labels
                font: GROUP_LABEL_FONT
            }
        ]
    },
    halign: "left",
    headerTextWrapping: false,
    hoverColumnHighlight: {enabled: false},
    hoverRowHighlight: {
        enabled: true,
        backgroundColor: "#555"
    },
    hoverCellHighlight: {
        enabled: true,
        backgroundColor: "#333"
    },
    noDataMessage: "",
    minimumColumnWidth: 50,
    multipleSelections: false,
    renderFalsy: false,
    rowHeaderFont: "12px Arial, Helvetica, sans-serif",
    rowHeaderForegroundSelectionFont: '12px "Arial", Helvetica, sans-serif',
    rowResize: true,
    scrollbarHoverOff: "visible",
    rowHeaderCheckboxes: false,
    rowHeaderNumbers: false,
    showFilterRow: true,
    showHeaderRow: true,
    showTreeColumn: false,
    showRowNumbers: false,
    showCheckboxes: false,
    singleRowSelectionMode: false,
    sortColumns: [],
    sortOnDoubleClick: true,
    treeRenderer: "TreeCell",
    treeHeaderFont: "12px Arial, Helvetica, sans-serif",
    treeHeaderForegroundSelectionFont: '12px "Arial", Helvetica, sans-serif',
    useBitBlit: false,
    vScrollbarClassPrefix: "",
    voffset: 0
};

const light_theme_overrides = {
    backgroundColor: "#ffffff",
    color: "#666",
    lineColor: "#AAA",
    // font: '12px Arial, Helvetica, sans-serif',
    font: '12px "Open Sans", Helvetica, sans-serif',
    foregroundSelectionFont: "12px amplitude-regular, Helvetica, sans-serif",
    foregroundSelectionColor: "#666",
    backgroundSelectionColor: "rgba(162, 183, 206, 0.3)",
    selectionRegionOutlineColor: "rgb(45, 64, 85)",
    columnHeaderColor: "#666",
    columnHeaderHalign: "left", // except: group header labels always 'center'; numbers always 'right' per `setPSP`
    columnHeaderBackgroundColor: "#fff",
    columnHeaderForegroundSelectionColor: "#333",
    columnHeaderBackgroundSelectionColor: "#40536d",
    rowHeaderForegroundSelectionFont: "12px Arial, Helvetica, sans-serif",
    treeHeaderColor: "#666",
    treeHeaderBackgroundColor: "#fff",
    treeHeaderForegroundSelectionColor: "#333",
    treeHeaderBackgroundSelectionColor: "#40536d",
    hoverCellHighlight: {
        enabled: true,
        backgroundColor: "#eeeeee"
    },
    hoverRowHighlight: {
        enabled: true,
        backgroundColor: "#f6f6f6"
    }
};

export function default_grid_properties(overrides = light_theme_overrides) {
    return Object.assign({}, cloneDeep(base_grid_properties), cloneDeep(overrides));
}
