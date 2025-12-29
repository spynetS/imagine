project(
  'imagine',
  'c',
  meson_version : '>= 1.3.0',
  version : '0.1',
  default_options : ['warning_level=3'],
)



ffmpeg = find_program('ffmpeg', required: true)

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
