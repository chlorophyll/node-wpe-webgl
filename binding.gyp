{
  'targets': [
    {
      'target_name': 'gles2',
      'defines': [
        'VERSION=0.0.1'
      ],
      'variables': {
        'platform': '<(OS)',
        'has_glfw': '<!(pkg-config glfw3 --libs || true | grep glfw)',
        'has_nexus': '<!(pkg-config glesv2 egl --libs || true | grep nexus)',
        'has_bcm': '<!(pkg-config glesv2 egl --libs || true | grep bcm)'
      },
      'include_dirs': [
        "<!(node -e \"require('nan')\")",
        '<(module_root_dir)/deps/include',
        '/opt/vc/include'
      ],
      'conditions': [
        ['OS=="linux" and has_glfw!=""', {
          'sources': [
            'src/glew/gles2glewimpl.cc',
            'src/bindings.cc',
            'src/gles2platform.cc',
            'src/interface/webgl.cc'
          ],
          'libraries': ['<!@(pkg-config --libs glfw3 glew)'],
          'defines': ['IS_GLEW']
        }],
        ['OS=="linux" and has_glfw=="" and has_nexus!=""', {
          'sources': [
            'src/nexus/gles2nexusimpl.cc',
            'src/bindings.cc',
            'src/gles2platform.cc',
            'src/interface/webgl.cc'
          ],
          'libraries': ['<!@(pkg-config --libs egl glesv2)'],
          'include_dirs': [ '<!@(pkg-config egl glesv2 --cflags-only-I | sed s/-I//g)' ]
        }],
        ['OS=="linux" and has_glfw=="" and has_nexus=="" and has_bcm!=""', {
          'sources': [
            'src/rpi/gles2rpiimpl.cc',
            'src/bindings.cc',
            'src/gles2platform.cc',
            'src/interface/webgl.cc'
          ],
          'libraries': ['<!@(pkg-config --libs egl glesv2)'],
          'include_dirs': [ '<!@(pkg-config egl glesv2 --cflags-only-I | sed s/-I//g)']
        }],
        ['OS=="mac"', {
          'sources': [
            'src/glew/gles2glewimpl.cc',
            'src/bindings.cc',
            'src/gles2platform.cc',
            'src/interface/webgl.cc'
          ],
          'include_dirs': [ '<!@(pkg-config glfw3 glew --cflags-only-I | sed s/-I//g)'],
          'libraries': [ '<!@(pkg-config --libs glfw3 glew)', '-framework OpenGL'],
          'library_dirs': ['/usr/local/lib'],
          'defines': ['IS_GLEW']
        }],
        ['OS=="win"', {
          'sources': [
            'src/glew/gles2glewimpl.cc',
            'src/bindings.cc',
            'src/gles2platform.cc',
            'src/interface/webgl.cc'
          ],
          'include_dirs': ['<(module_root_dir)/deps/include'],
          'library_dirs': ['<(module_root_dir)/deps/windows/lib/<(target_arch)'],
          'libraries': ['glew32.lib','opengl32.lib'],
          'defines' : ['IS_GLEW','WIN32_LEAN_AND_MEAN','VC_EXTRALEAN'],
          'msvs_settings' : {
            'VCCLCompilerTool' : {
              'AdditionalOptions' : ['/O2','/Oy','/GL','/GF','/Gm-','/EHsc','/MT','/GS','/Gy','/GR-','/Gd']
            },
            'VCLinkerTool' : {
              'AdditionalOptions' : ['/OPT:REF','/OPT:ICF','/LTCG']
            },
          }
        }]
      ]
    }
  ]
}
