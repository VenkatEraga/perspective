// autocomplete.js - A autocomplete-box is a combination of a text-box and a drop-down.
// User may type into it and/or select an item from the drop-down (by clicking on the triangle at the right).
// The drop-down has sections which are toggled from a control area between the text-box and the drop-down.

/* eslint-env browser */

"use strict";

var Textfield = require("fin-hypergrid/src/cellEditors/Textfield");
var prototype = require("fin-hypergrid/src/cellEditors/CellEditor").prototype;


var stateToActionMap = {
  hidden: slideDown,
  visible: slideUp
};

module.exports = function(grid) {
  var autocomplete = Textfield.extend("autocomplete", {
    initialize: function() {
      var el = this.el;

      this.input = el.querySelector("input");    
      this.options = el.querySelector("div");
      this.controls = this.options.querySelector("div");
      this.dropdown = this.options.querySelector("select");    
      this.isOptionFocused = false;
     
      this.input.addEventListener("blur", () => {
        setTimeout(() => {
          if (!this.isOptionFocused) {
            console.log(" blurred ");
            this.cancelEditing();
          } else this.isOptionFocused = false;
        });
      });

      this.dropdown.addEventListener("focus", () => {
        this.isOptionFocused = true;
      });

      this.dropdown.addEventListener("mousewheel", function(e) {
        e.stopPropagation();
      });
      this.dropdown.addEventListener("change", this.insertText.bind(this));

      const formatter = this.column.getFormatter();
      const items = JSON.parse(this.dropdown.getAttribute("option_items"));
      const option_width = this.input.style.width;
      this.loadOptions(this.dropdown, items, formatter, option_width);

      // Auto filter on keyup
      this.input.onkeyup = function(e) {
        const value = this.input.value;
        const dropdown = this.dropdown;
        const items = JSON.parse(dropdown.getAttribute("option_items"));
        var filtered_items = items;
        if (value || value.trim() !== "")
          filtered_items = items.filter(item =>
            item
              .toLowerCase()
              .trim()
              .includes(value.toLowerCase().trim())
          );
        this.dropdown.innerHTML = "";
        const formatter = this.column.getFormatter();
        const option_width = this.input.style.width;
        this.loadOptions(
          this.dropdown,
          filtered_items,
          formatter,
          option_width
        );
      }.bind(this);
    },

    template: [
      '<div class="hypergrid-combobox" title="">',
      '    <input type="text"  style="{{input_style}}">',     
      "    <div>",
      "        <div></div>",
      '        <select option_items="{{option_items}}" size="10"></select>',
      "    </div>",
      "</div>"
    ].join("\n"),

    showEditor: function() {
      prototype.showEditor.call(this);
    },

    hideEditor: function() {     
      prototype.hideEditor.call(this);
    },

    toggleDropDown: function() {    
        var state = window.getComputedStyle(this.dropdown).visibility;
        stateToActionMap[state].call(this);    
    },

    insertText: function(e) {
      // replace the input text with the drop-down text
      this.input.focus();
      this.input.value = this.dropdown.value;     
      // close the drop-down
      this.toggleDropDown();
      this.stopEditing();
    },

    loadOptions: function(dropdown, items, formatter, width) {
      items.sort().forEach(function(item) {
        const val = formatter(item);
        var option = new Option(val, val);
        option.style.width = width;
        dropdown.appendChild(option);
      });
    },
    getEditorValue,
    setBounds: setCellBounds,
    saveEditorValue
  });

  grid.cellEditors.add(autocomplete);
};

function getEditorValue(updated) {
  this._row.then(([old]) => {
    const index = old.__INDEX__;
    delete old["__INDEX__"];
    const colname = Object.keys(old)[0];
    this.table.update([{ __INDEX__: index, [colname]: updated }]);
  });
  return this.localizer.format(updated);
}

function setCellBounds(cellBounds) {
  const style = this.el.style;
  style.left = px(cellBounds.x + 4);
  style.top = px(cellBounds.y - 3);
  style.width = px(cellBounds.width - 10);
  style.height = px(cellBounds.height);
}

function px(n) {
  return n + "px";
}

function saveEditorValue(x) {
   var save = !(x && x === this.initialValue) && this.grid.fireBeforeCellEdit(this.event.gridCell, this.initialValue, x, this);
  if (save) {
  this.data[this.event.gridCell.y - 1][this.event.gridCell.x] = x;
  }
}

function slideDown() {
  // preserve the text box's current text selection, which is about to be lost
  this.selectionStart = this.input.selectionStart;
  this.selectionEnd = this.input.selectionEnd;

  // clean up the select list from last usage
  this.dropdown.selectedIndex = -1; // be kind (remove previous selection)
  this.dropdown.style.scrollTop = 0; // rewind

  // show the drop-down slide down effect
  this.options.style.visibility = "visible";
  var dropDownTopMargin = getFloat(this.dropdown, "marginTop"),
    dropDownRows = this.dropdown.size,
    optionHeight =
      Math.ceil(
        ((this.dropdown.length && getFloat(this.dropdown[0], "height")) ||
          13.1875) * 2
      ) /
        2 +
      1;
  this.options.style.height =
    dropDownTopMargin + optionHeight * dropDownRows + "px"; // starts the slide down effect

  // while in drop-down, listen for clicks in text box which means abprt
  this.input.addEventListener(
    "mouseup",
    (this.slideUpBound = slideUp.bind(this))
  );

  }

function slideUp() {
  // stop listening to input clicks
  this.input.removeEventListener("mouseup", this.slideUpBound);

  // start the slide up effect
  this.options.style.height = 0;

}

function getFloat(el, style) {
  return parseFloat(window.getComputedStyle(el)[style]);
}
