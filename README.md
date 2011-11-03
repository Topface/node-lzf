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

---
### Authors
- Ian Babrou (ibobrik@gmail.com)
