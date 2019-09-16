/* eslint-env browser */

'use strict';
var load_autocomplete_editor = require("./autocomplete-editor.js");
var load_time_editor = require("./time-editor.js");
module.exports = function(grid, column_schema) {    

        load_autocomplete_editor(grid);
        load_time_editor(grid);

    // Override to assign the the cell editors.
    grid.behavior.dataModel.getCellEditorAt = function(columnIndex, rowIndex, declaredEditorName, cellEvent) { 
               
        const column =  grid.behavior.getColumn(columnIndex);
        const column_name = column.header;
        const column_width = column.properties["width"];

        const column_config = column_schema.filter(item=>item.name ===column_name)[0];       
        const editorName =  column_config.editor;
         if(editorName=="autocomplete"){
             const items = column_config.option_items;
             if(items){
               cellEvent.option_items = JSON.stringify(items);
               cellEvent.input_style="width:"+column_width+"px;height:21px";
             }
         }
        var cellEditor = grid.cellEditors.create(editorName, cellEvent);        
        cellEditor.data = this.data;
        cellEditor.table = this._table;
        const offset = grid.renderer.dataWindow.top;
        const args = {
            start_row: rowIndex + offset - 1,
            end_row: rowIndex + offset,
            start_col: columnIndex,
            end_col: columnIndex + 1,
            index: true
        };
        cellEditor._row = this._view.to_json(args);
        cellEditor.el.addEventListener("blur", () => setTimeout(() => cellEditor.cancelEditing()));
        return cellEditor;
    };
};