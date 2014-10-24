{
  "targets": [
    {
      "target_name": "lzf",
      "sources": [
        "src/lzf.cc",
        "src/lzf/lzf_c.cc",
        "src/lzf/lzf_d.cc",
        "src/lzf/lzf.h",
        "src/lzf/lzfP.h"
      ],
      'conditions': [
        [ 'OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {
          'cflags': ['-O2']
        }],
        ['OS=="mac"', {
          'xcode_settings': {
            'OTHER_CFLAGS': ['-O2']
          }
        }]
      ],
      "include_dirs": [ '<!(node -e "require(\'nan\')")' ],
    }
  ]
}
