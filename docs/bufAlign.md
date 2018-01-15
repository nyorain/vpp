Cases where you have to care about buffer range alignment:

- use it with a DrawIndirectCommand at the beginning (align 4)
- descriptor buffer (align min*BufferOffsetAlignment)
- more?
