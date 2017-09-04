tests = [
  'allocator',
  'copy',
  'memory',
]

foreach test : tests
  test(test, executable(test, test + '.cpp', dependencies: vpp_dep))
endforeach
