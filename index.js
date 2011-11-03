try {
    module.exports = require('./build/default/lzf.node');
} catch(e) {
    module.exports = require('./build/Release/lzf.node');
}