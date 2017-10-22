tests = [
  'allocator',
  'copy',
  'memory',
  'bufferOps',
  'image',
  'submit',
  'objects'
]

foreach test : tests
  test(test, executable('test_' + test, test + '.cpp', dependencies: vpp_dep))
endforeach
