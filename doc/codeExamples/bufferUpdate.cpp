///This example shows how to update a vulkan buffer easily

//Constructs a new BufferUpdate object for a vulkan buffer that will be interpreted using
//the std140 layout.
BufferUpdate update(buffer, BufferPacking::std140);

//Writes a raw, manually aligned buffer. In this case decltype(myData) could be std::uint8_t
//to really write a raw buffer (in this case myData is a reference to 24 objects).
//Note that it is not enough to just pass such a buffer to the update, one has to use
//the vpp::raw wrapper to emphasize that this data will NOT be correctly aligned by default.
update.add(vpp::raw(myData, 24));

//Just adds a float to the buffer.
//Could also be done with a bool or std::int32_t/std::uint32_t.
update.add(7.f);

//Containers can be passed as well.
//The only condition is that they have begin() and end() functions and the contained objects
//have VulkanType specializations (e.g. double, raw buffer or custom specialization).
update.add({7.f, 4.f, 1230.f});
update.add({4u, 6u, 2u});
update.add(myFloatVector);

//Enables use for e.g. uniform descriptor (offset).
update.alignUniform();

//One can also add multiple objects at once.
update.add(myVec, myMat, 5, 1234.f);

//This offset function call just "moves forward" on the buffer without touching its content.
update.offset(10);

//In this case the next 10 bytes on the buffer will be overriden with 0.
update.offset(10, true);

//If you provide VulkanType specializations for your types, you can also pass them directly
//to the update.
update.add(myStruct);


//The whole update can also be done in a more convinient way.
//Note that using this method there is no option for custom aligns or offsets.
//Alternatively there is also a fill140 function.
fill430(buffer, 7.f, Vec2(4.f., 2.f), Vec3d(4., 544., 420.), 4., myStruct);



///CONCEPTS

///XXX: this is a concept not yet implemented:
//The buffer data can be retrieved in a similiar way.
//This will read the bytes from the given dataBuffer into the given objects
//references.
// readDataLayout140(dataBuffer, myVec, myFloat, myStruct);

///This function reads the given dataBuffer aligned into the given types.
// template<typename... T>
// void readDataLayout(const Range<std::uint8_t>& dataBuffer, BufferAling align, T&... types);
// void readDataLayout140(const Range<std::uint8_t>& dataBuffer, T&... types);
// void readDataLayout430(const Range<std::uint8_t>& dataBuffer, T&... types);
//problems: what about offsets & extra aligns?
//provide a class like BufferReader?
//provider offsets and align bufferfill/read objects?
//BufferUpdate class needed anymore then?
