///This example shows how to update a vulkan buffer easily
BufferUpdate update(buffer, BufferPacking::std140);
update.add(vpp::DataBuffer(&myData, 24)); //writes a raw, manually aligned buffer
update.add(7.f);
update.alignUniform(); //enables use for e.g. uniform descriptor (offset)
update.add(myVec, myMat);
update.offset(10); //just offsets without overriding
update.offset(10, true); //offsets and override with 0
update.add(myStruct);

//short way
buffer.fill140(7.f, Vec2(4.f., 2.f), Vec3d(4. 544.), 4.);
