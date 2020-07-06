{
  'target_defaults': {
    'include_dirs': ['src'],
    'cflags_cc': [
      '-fexceptions',
      '-Wall',
      '-Wno-cast-function-type',
      '-Wno-deprecated-declarations',
      '-O3'
    ],
    'cflags!': ['-fno-exceptions'],
    'xcode_settings': {
      'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
      'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
      'CLANG_CXX_LIBRARY': 'libc++',
      'MACOSX_DEPLOYMENT_TARGET': '10.7',
      'OTHER_CPLUSPLUSFLAGS': [
        '-fexceptions',
        '-Wall',
        '-Wno-macro-redefined',
        '-Wno-deprecated-declarations',
        '-O3'
      ]
    },
    'msvs_settings': {
      'VCCLCompilerTool': {
        'ExceptionHandling': 1,
        'DisableSpecificWarnings': ['4005', '4477', '4752']
      }
    }
  },
  'targets': [
    {
      'target_name': 'arch_specific',
      'type': 'static_library',
      'sources': ['src/highwayhash/arch_specific.cc'],
    }, {
      'target_name': 'hh_avx2',
      'dependencies': ['arch_specific'],
      'type': 'static_library',
      'sources': ['src/highwayhash/highwayhash_target.cc'],
      'defines': ['HH_TARGET_NAME=AVX2'],
      'cflags_cc': ['-mavx2'],
      'xcode_settings': {
        'OTHER_CPLUSPLUSFLAGS': ['-mavx2']
      }
    }, {
      'target_name': 'hh_sse41',
      'dependencies': ['arch_specific'],
      'type': 'static_library',
      'sources': ['src/highwayhash/highwayhash_target.cc'],
      'defines': ['HH_TARGET_NAME=SSE41'],
      'cflags_cc': ['-msse4.1'],
      'xcode_settings': {
        'OTHER_CPLUSPLUSFLAGS': ['-msse4.1']
      }
    }, {
      'target_name': 'hh_portable',
      'dependencies': ['arch_specific'],
      'type': 'static_library',
      'sources': ['src/highwayhash/highwayhash_target.cc'],
      'defines': ['HH_TARGET_NAME=Portable']
    }, {
      'target_name': 'instruction_sets',
      'dependencies': ['hh_avx2', 'hh_sse41', 'hh_portable'],
      'type': 'static_library',
      'sources': ['src/highwayhash/instruction_sets.cc']
    }, {
      'target_name': 'highwayhash',
      'dependencies': [
        'instruction_sets',
        "<!(node -p \"require('node-addon-api').gyp\")"
        ],
      'sources': ['src/bindings.cc'],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
    }
  ]
}
