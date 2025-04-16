project(
  'emutestbox',
  'c',
)

inc = include_directories('inc')

dep_jansson = dependency('jansson')
dep_cmocka = dependency('cmocka')

src_etb = files(
  'src/loader.c',
  'src/runner.c',
  'src/api.c',
  'src/debugger.c'
)

dep_etb = declare_dependency(
  dependencies : [dep_jansson, dep_cmocka]
)

lib_etb = library(
  'emutestbox',
  src_etb,
  version : '0.0.1',
  dependencies : [dep_jansson]
)

src_etb_test = files(
  'src/loader.c',
  'tests/loader.c',
  'src/runner.c',
  'tests/runner.c',
  'src/api.c',
  'tests/api.c',
  'src/debugger.c',
  'tests/debugger.c'
)

dep_etb_test = declare_dependency(
  link_with : lib_etb,
  include_directories : inc,
  dependencies : [dep_jansson, dep_cmocka]
)

executable('emutestbox_test', src_etb_test, dependencies : dep_etb_test)

configure_file(
  input : 'tests/test.json',
  output : 'test.json',
  configuration : configuration_data()
)
