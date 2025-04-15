project(
  'emutestbox',
  'c',
)

inc = include_directories('inc')
src = files(
  'src/loader.c',
  'src/runner.c',
  'src/api.c',
  'src/debugger.c'
)

library('emutestbox', src, version: '0.0.1', soversion: '1')
