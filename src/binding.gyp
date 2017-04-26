{
  "targets": [
    {
      "target_name": "mypq",
      "sources": [
        "cpp_pq.h",
        "cpp_pq.cpp",
        "ObjectHolder.cpp",
        "ObjectHolder.h"
      ],
      "cflags": ["-Wall", "-std=c++11"],
      "conditions": [
        [ 'OS=="mac"',
          {
            "xcode_settings": {
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
              'OTHER_CPLUSPLUSFLAGS' :
              ['-std=c++11','-stdlib=libc++'],
              'OTHER_LDFLAGS': ['-stdlib=libc++'],
              'MACOSX_DEPLOYMENT_TARGET': '10.7' }
          }
        ]
      ],
      "include_dirs": []
    }
  ]
}