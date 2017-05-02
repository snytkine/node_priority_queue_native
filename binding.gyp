{
  "targets": [
    {
      "target_name": "priorityqueue_native",
      "sources": [ "src/priorityqueue_native.cpp", "src/ObjectHolder.cpp", "src/index.d.ts"],
      "cflags": ["-Wall", "-std=c++11"],
      'xcode_settings': {
        'OTHER_CFLAGS': [
          '-std=c++11'
        ],
      },
      "conditions": [
        [ 'OS=="mac"', {
          "xcode_settings": {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
            'OTHER_LDFLAGS': ['-stdlib=libc++'],
            'MACOSX_DEPLOYMENT_TARGET': '10.7' }
        }
        ]
      ]
    }
  ]
}
