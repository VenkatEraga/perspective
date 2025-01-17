/******************************************************************************
 *
 * Copyright (c) 2017, the Perspective Authors.
 *
 * This file is part of the Perspective library, distributed under the terms of
 * the Apache License 2.0.  The full license can be found in the LICENSE file.
 *
 */

const fs = require("fs");
const path = require("path");

const arrow = fs.readFileSync(path.join(__dirname, "..", "arrow", "test-null.arrow")).buffer;

var arrow_psp_internal_schema = [9, 10, 1, 2, 3, 4, 11, 19, 19, 12, 12, 12, 2];

module.exports = (perspective, mode) => {
    describe("Internal API", function() {
        it("is actually using the correct runtime", async function() {
            // Get the internal module;
            if (perspective.sync_module) {
                perspective = perspective.sync_module();
            }
            expect(perspective.__module__.wasmJSMethod).toEqual(mode === "ASMJS" ? "asmjs" : "native-wasm");
        });

        it("Arrow schema types are mapped correctly", async function() {
            // This only works for non parallel
            if (perspective.sync_module) {
                perspective = perspective.sync_module();
            }
            var table = perspective.table(arrow.slice());
            let schema, stypes;
            let types = [];
            try {
                schema = table._Table.get_schema();
                stypes = schema.types();

                for (let i = 0; i < stypes.size(); i++) {
                    types.push(stypes.get(i).value);
                }
                expect(arrow_psp_internal_schema).toEqual(types);
            } finally {
                if (schema) {
                    schema.delete();
                }
                if (stypes) {
                    stypes.delete();
                }
            }
            table.delete();
        });
    });
};
