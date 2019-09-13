// autocomplete.js - A autocomplete-box is a combination of a text-box and a drop-down.
// User may type into it and/or select an item from the drop-down (by clicking on the triangle at the right).
// The drop-down has sections which are toggled from a control area between the text-box and the drop-down.

/* eslint-env browser */

"use strict";

var Textfield = require("fin-hypergrid/src/cellEditors/Textfield");
var prototype = require("fin-hypergrid/src/cellEditors/CellEditor").prototype;
var Queueless = require("./queueless");
var elfor = require("./elfor");

var TOGGLE_MODE_PREFIX = "toggle-mode-";

var stateToActionMap = {
  hidden: slideDown,
  visible: slideUp
};

module.exports = function(grid) {
  var autocomplete = Textfield.extend("autocomplete", {
    initialize: function() {
      var el = this.el;

      this.input = el.querySelector("input");
      this.dropper = el.querySelector("span");
      this.options = el.querySelector("div");
      this.controls = this.options.querySelector("div");
      this.dropdown = this.options.querySelector("select");
      this.controllable = this.modes.length > 1;
      // set up a transition end controller
      this.optionsTransition = new Queueless(this.options, this);
      this.menuModesSource = this.column.menuModes || { distinctValues: true };

      // wire-ups
      this.dropper.addEventListener(
        "mousedown",
        this.toggleDropDown.bind(this)
      );
      this.dropdown.addEventListener("mousewheel", function(e) {
        e.stopPropagation();
      });
      this.dropdown.addEventListener("change", this.insertText.bind(this));

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
      '    <input type="text" lang="{{locale}}" style="{{input_style}}">',
      '    <span title="Click for options"></span>',
      "    <div>",
      "        <div></div>",
      '        <select option_items="{{option_items}}" size="10" lang="{{locale}}"></select>',
      "    </div>",
      "</div>"
    ].join("\n"),

    modes: [
      {
        name: "distinctValues",
        appendOptions: function(optgroup) {
          const formatter = this.column.getFormatter();
          const items = JSON.parse(optgroup.getAttribute("option_items"));
          const option_width = this.input.style.width;
          this.loadOptions(optgroup, items, formatter, option_width);
        }
      }
    ],

    showEditor: function() {
      // set the initial state of the mode toggles
      if (!this.built) {
        var menuModesSource = this.menuModesSource,
          menuModes = (this.menuModes = {});

        // build the proxy
        this.modes.forEach(function(mode) {
          var modeName = mode.name;
          if (modeName in menuModesSource) {
            menuModes[modeName] = menuModesSource[modeName];
          }
        });

        // wire-ups
        if (this.controllable) {
          this.controls.addEventListener("click", onModeIconClick.bind(this));
        }

        this.modes.forEach(function(mode) {
          // create a toggle
          var toggle = document.createElement("span");
          if (this.controllable) {
            toggle.className = TOGGLE_MODE_PREFIX + mode.name;
            toggle.title = "Toggle " + (mode.label || mode.name).toLowerCase();
            if (mode.tooltip) {
              toggle.title += "\n" + mode.tooltip;
            }
            toggle.textContent = mode.symbol;
          }

          this.controls.appendChild(toggle);

          // create and label a new optgroup
          if (mode.selector) {
            var optgroup = document.createElement("optgroup");
            optgroup.label = mode.label;
            optgroup.className = "submenu-" + mode.name;
            optgroup.style.backgroundColor = mode.backgroundColor;
            this.dropdown.add(optgroup);
          }

          setModeIconAndOptgroup.call(
            this,
            toggle,
            mode.name,
            menuModes[mode.name]
          );
        }, this);

        this.built = true;
      }

      prototype.showEditor.call(this);
    },

    hideEditor: function() {
      // this is where you would persist this.menuModes
      prototype.hideEditor.call(this);
    },

    toggleDropDown: function() {
      if (!this.optionsTransition.transitioning) {
        var state = window.getComputedStyle(this.dropdown).visibility;
        stateToActionMap[state].call(this);
      }
    },

    insertText: function(e) {
      // replace the input text with the drop-down text
      this.input.focus();
      this.input.value = this.dropdown.value;
      this.input.setSelectionRange(0, this.input.value.length);
      // close the drop-down
      this.toggleDropDown();
    },
    loadOptions: function(dropdown, items, formatter, width) {
      const toggleDropDown = this.toggleDropDown.bind(this);
      items.sort().forEach(function(item) {
        const val = formatter(item);
        var option = new Option(val, val);
        option.style.width = width;
        option.addEventListener("mousedown", toggleDropDown);
        dropdown.appendChild(option);
      });
    }
  });

  grid.cellEditors.add(autocomplete);
};

function onModeIconClick(e) {
  var ctrl = e.target;

  if (ctrl.tagName === "SPAN") {
    // extra ct the mode name from the toggle control's class name
    var modeClassName = Array.prototype.find.call(ctrl.classList, function(
        className
      ) {
        return className.indexOf(TOGGLE_MODE_PREFIX) === 0;
      }),
      modeName = modeClassName.substr(TOGGLE_MODE_PREFIX.length);

    // toggle mode in the filter
    var modeState = (this.menuModes[modeName] ^= 1);

    setModeIconAndOptgroup.call(this, ctrl, modeName, modeState);
  }
}

function setModeIconAndOptgroup(ctrl, name, state) {
  var style,
    optgroup,
    sum,
    display,
    dropdown = this.dropdown,
    mode = this.modes.find(function(mode) {
      return mode.name === name;
    }), // eslint-disable-line no-shadow
    selector = mode.selector;

  // set icon state (color)
  ctrl.classList.toggle("active", !!state);

  // empty the optgroup if hiding; rebuild it if showing
  if (state) {
    // rebuild it
    // show progress cursor for (at least) 1/3 second
    style = this.el.style;
    style.cursor = "progress";
    setTimeout(function() {
      style.cursor = null;
    }, 333);

    if (selector) {
      optgroup = dropdown.querySelector(selector);
      sum = mode.appendOptions.call(this, optgroup);

      // update sum
      optgroup.label = optgroup.label.replace(/ \(\d+\)$/, ""); // remove old sum
      // optgroup.label += ' (' + sum + ')';
    } else {
      sum = mode.appendOptions.call(this, dropdown);
      if (!this.controllable) {
        //ctrl.textContent = sum + ' values';
        ctrl.textContent = "";
      }
    }

    display = null;
  } else {
    display = "none";
  }

  // hide/show the group
  if (!selector) {
    selector = "option,optgroup:not([class])";
    var mustBeChildren = true; // work-around for ':scope>option,...' not avail in IE11
  }
  elfor.each(selector, iteratee, dropdown);

  function iteratee(el) {
    if (!mustBeChildren || el.parentElement === dropdown) {
      el.style.display = display;
    }
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
    "mousedown",
    (this.slideUpBound = slideUp.bind(this))
  );

  // wait for transition to end
  this.optionsTransition.begin();
}

function slideUp() {
  // stop listening to input clicks
  this.input.removeEventListener("mousedown", this.slideUpBound);

  // start the slide up effect
  this.options.style.height = 0;

  // schedule the hide to occur after the slide up effect
  this.optionsTransition.begin(function(event) {
    this.style.visibility = "hidden";
  });
}

function getFloat(el, style) {
  return parseFloat(window.getComputedStyle(el)[style]);
}