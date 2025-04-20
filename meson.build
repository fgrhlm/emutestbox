project(
  'emutestbox',
  'c',
)

inc = include_directories('inc')

dep_jansson = dependency('jansson')
dep_cmocka = dependency('cmocka')

src_etb = files(
  'src/api.c',
  'src/loader.c',
  'src/runner.c',
  'src/debugger.c'
)

dep_etb = declare_dependency(
  dependencies : [dep_jansson, dep_cmocka]
)

lib_etb = library(
  'emutestbox',
  src_etb,
  version : '0.0.1',
  include_directories : inc,
  dependencies : [dep_jansson]
)

dep_etb_test = declare_dependency(
  link_with : lib_etb,
  include_directories : inc,
  dependencies : [dep_jansson, dep_cmocka],

)

src_test_loader = files('src/loader.c', 'tests/loader.c')
executable('test_loader', src_test_loader, dependencies : dep_etb_test,)

src_test_api = files('src/api.c', 'tests/api.c')
executable('test_api', src_test_api, dependencies : dep_etb_test, c_args : '-fno-stack-protector')

configure_file(
  input : 'tests/test.json',
  output : 'test.json',
  copy: true
)

configure_file(
  input : 'tests/run_tests.sh',
  output : 'run_tests.sh',
  copy: true
)
