tests = [
  'allocator',
  'copy',
  'framebuffer',
  'memory',
  'image',
  'objects',
  'submit',
  'sharedBuffer',
  'pipeline',
  'trackedDescriptor',
  'address',
]

foreach test : tests
  test(test, executable('test_' + test, test + '.cpp', dependencies: vpp_dep))
endforeach
