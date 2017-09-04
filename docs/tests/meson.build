tests = [
  'allocator',
  'copy',
  'memory',
]

foreach test : tests
  test(test, executable('test_' + test, test + '.cpp', dependencies: vpp_dep))
endforeach
