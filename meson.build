project(
  'emutestbox',
  'c',
)

inc = include_directories('inc')

inc_ref_1 = include_directories('tests/6502-emu')
inc_ref_2 = include_directories('tests/f6502/include')

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

src_test_debugger = files('tests/6502-emu/6502.c', 'tests/debugger.c')
executable(
  'test_debugger',
  src_test_debugger,
  dependencies : dep_etb_test,
  c_args : '-pg',
  link_args : '-pg',
  include_directories : inc_ref_1
)

src_test_loader = files('src/loader.c', 'tests/loader.c')
executable(
  'test_loader',
  src_test_loader,
  dependencies : dep_etb_test,
  c_args : '-pg',
  link_args : '-pg'
)

src_test_api = files('src/api.c', 'tests/api.c')
executable(
  'test_api',
  src_test_api,
  dependencies : dep_etb_test,
  c_args : ['-fno-stack-protector', '-pg'],
  link_args : '-pg'
)

src_test_ref_1 = files(
  'tests/6502-emu/6502.c',
  'tests/ref_1.c',
  'src/api.c',
  'src/loader.c',
  'src/runner.c',
  'src/debugger.c'
)

executable(
  'test_ref_1',
  src_test_ref_1,
  dependencies : dep_etb_test,
  c_args : ['-fno-stack-protector', '-pg'],
  link_args : '-pg',
  include_directories : [inc, inc_ref_1]
)

src_test_ref_2 = files(
  'src/api.c',
  'src/loader.c',
  'src/runner.c',
  'src/debugger.c',
  'tests/ref_2.c',
  'tests/f6502/src/cpu.c',
  'tests/f6502/src/addr.c',
  'tests/f6502/src/mem.c',
  'tests/f6502/src/instr.c',
  'tests/f6502/src/instr_arith.c',
  'tests/f6502/src/instr_branch.c',
  'tests/f6502/src/instr.c',
  'tests/f6502/src/instr_ctrl.c',
  'tests/f6502/src/instr_flags.c',
  'tests/f6502/src/instr_inc.c',
  'tests/f6502/src/instr_load.c',
  'tests/f6502/src/instr_logic.c',
  'tests/f6502/src/instr_shift.c',
  'tests/f6502/src/instr_stack.c',
  'tests/f6502/src/instr_trans.c',
  'tests/f6502/src/bin.c',
)

executable(
  'test_ref_2',
  src_test_ref_2,
  dependencies : dep_etb_test,
  c_args : ['-fno-stack-protector', '-pg'],
  link_args : '-pg',
  include_directories : [inc, inc_ref_2]
)

configure_file(
  input : 'tests/run_tests.sh',
  output : 'run_tests.sh',
  copy: true
)

run_command('cp', '-r', 'tests/dummy_tests', 'build/dummy_tests')
