project(
  'imagine',
  'c',
  meson_version : '>= 1.3.0',
  version : '0.1',
  default_options : ['warning_level=3'],
)



ffmpeg = find_program('ffmpeg', required: true)

fs = import('fs')
if not fs.is_dir(meson.current_source_dir() / 'lib/printer')
  message('The printer submodule is missing!')
  message('Please run: git submodule update --init --recursive')
  message('Or download the library independently')
endif

subdir('lib/printer')

dependencies = [
  printer_dep,
]

sources = [
  'src/flagcer.c',
  'src/imagine.c',
  'src/main.c',
]

exe = executable(
  'imagine',
  sources,
  dependencies : dependencies,
  install : true,
)

test('basic', exe)
