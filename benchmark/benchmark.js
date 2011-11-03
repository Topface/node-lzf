var lzf  = require("../index.js");
var zlib = require("compress-buffer");
var fs   = require("fs");
var path = require("path");


var Benchmark = require('benchmark');


function testCompression(length) {
    var data  = fs.readFileSync(path.join(__dirname, "data", length + ".txt"));
    var suite = new Benchmark.Suite();

    console.log("Testing compression at " + length + " bytes");

    suite.add('lzf-compress', function() {
        lzf.compress(data);
    });

    suite.add('zlib-compress', function() {
        zlib.compress(data);
    })

    suite.on('cycle', function(event, bench) {
        console.log(String(bench));
    });

    suite.on('complete', function() {
        console.log('Fastest is ' + this.filter('fastest').pluck('name'));
        console.log();
    });

    suite.run();
}

function testDecompression(length) {
    var data           = fs.readFileSync(path.join(__dirname, "data", length + ".txt"));
    var suite          = new Benchmark.Suite();
    var lzfCompressed  = lzf.compress(data);
    var zlibCompressed = zlib.compress(data);

    console.log("Testing decompression at " + length + " bytes");

    suite.add('lzf-decompress', function() {
        lzf.decompress(lzfCompressed);
    });

    suite.add('zlib-decompress', function() {
        zlib.uncompress(zlibCompressed);
    })

    suite.on('cycle', function(event, bench) {
        console.log(String(bench));
    });

    suite.on('complete', function() {
        console.log('Fastest is ' + this.filter('fastest').pluck('name'));
        console.log();
    });

    suite.run();
}

for (var i = 0; i < 4; i++) {
    var length = Math.pow(10, i + 2);
    testCompression(length);
    testDecompression(length);
}

