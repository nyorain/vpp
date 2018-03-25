tests = [
  'allocator',
  'copy',
  'framebuffer',
  'memory',
  'bufferOps',
  'image',
  'objects',
  'submit',
  'sharedBuffer',
  'pipeline',
  'trackedDescriptor'
]

foreach test : tests
  test(test, executable('test_' + test, test + '.cpp', dependencies: vpp_dep))
endforeach
