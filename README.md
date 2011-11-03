## node-lzf

[LZF](http://oldhome.schmorp.de/marc/liblzf.html) compression library for nodejs.

LZF advantages:

* Small code size (less then 500 lines including header files and docs).
* Very fast compression speeds, rivaling a straight copy loop, especially for decompression which is basically at (unoptimized) memcpy-speed. Compression speed can be increased by 20% by sacrificing a few percent of compression ratio.
* Mediocre compression ratios - you can usually expect about 40-50% compression for typical binary data
* Easy to use (just two functions, no state attached)
* Highly portable (written in C)
* Tunable, see the file lzfP.h in the distribution, to tailor liblzf to your needs. The generated compressed blocks can be decompressed by any liblzf version regardless of the options used to compress.
* Freely usable (BSD-type-license)

### Usage

```javascript
var lzf = require("lzf");


var lorem = "Lorem ipsum dolor sit amet, consectetur adipiscing elit." +
    "Curabitur volutpat, nulla nec egestas semper," +
    "ante dui tristique nibh, quis feugiat.";

// create buffer of your data
var loremBuffer = new Buffer(lorem);

// compress your buffer and get another buffer
var loremCompressed = lzf.compress(loremBuffer);

// decompress compressed buffer
var loremDecompressed = lzf.decompress(loremCompressed);

```

### Benchmarks

Benchmarks against [compress-buffer](https://github.com/egorFiNE/node-compress-buffer) library. Just to be short: it's faster.

```
Testing compression at 100 bytes
lzf-compress x 139,495 ops/sec ±11.88% (58 runs sampled)
zlib-compress x 29,879 ops/sec ±6.88% (88 runs sampled)
Fastest is lzf-compress

Testing decompression at 100 bytes
lzf-decompress x 36,574 ops/sec ±7.74% (87 runs sampled)
zlib-decompress x 26,153 ops/sec ±6.92% (89 runs sampled)
Fastest is lzf-compress

Testing compression at 1000 bytes
lzf-compress x 57,345 ops/sec ±9.49% (81 runs sampled)
zlib-compress x 11,828 ops/sec ±3.93% (60 runs sampled)
Fastest is lzf-compress

Testing decompression at 1000 bytes
lzf-decompress x 29,221 ops/sec ±7.06% (89 runs sampled)
zlib-decompress x 19,351 ops/sec ±4.77% (92 runs sampled)
Fastest is lzf-compress

Testing compression at 10000 bytes
lzf-compress x 10,512 ops/sec ±3.26% (92 runs sampled)
zlib-compress x 1,850 ops/sec ±0.20% (73 runs sampled)
Fastest is lzf-compress

Testing decompression at 10000 bytes
lzf-decompress x 12,487 ops/sec ±3.85% (91 runs sampled)
zlib-decompress x 6,939 ops/sec ±3.14% (93 runs sampled)
Fastest is lzf-compress

Testing compression at 100000 bytes
lzf-compress x 1,628 ops/sec ±1.50% (96 runs sampled)
zlib-compress x 132 ops/sec ±0.13% (93 runs sampled)
Fastest is lzf-compress

Testing decompression at 100000 bytes
lzf-decompress x 2,472 ops/sec ±1.09% (91 runs sampled)
zlib-decompress x 1,430 ops/sec ±0.73% (95 runs sampled)
Fastest is lzf-compress
```

---
### Authors
- Ian Babrou (ibobrik@gmail.com)
