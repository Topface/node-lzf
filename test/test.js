var assert = require("assert");


var lzf = require("../index.js");


var lorem = "Lorem ipsum dolor sit amet, consectetur adipiscing elit." +
    "Curabitur volutpat, nulla nec egestas semper," +
    "ante dui tristique nibh, quis feugiat.";

var loremBuffer = new Buffer(lorem);
var loremCompressed = lzf.compress(loremBuffer);
var loremDecompressed = lzf.decompress(loremCompressed);

// check default output buffer
assert.equal(loremDecompressed.toString(), lorem);

// check minimum output buffer
assert.equal(lzf.decompress(loremCompressed, loremBuffer.length), lorem);

// check error on too small buffer
try {
    lzf.decompress(loremCompressed, loremBuffer.length - 1);
    assert.fail("exception should be thrown for too small buffer");
} catch (e) {}

console.log("test ok");
