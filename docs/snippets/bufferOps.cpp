// simple example for the size/fill/read api:
// does not use the features for which the more comlex OO-api is needed such as
// custom alignments and offsets.

// query the size needed for a buffer in layout std140 with the given objects.
// note that for all of these calls you have to specialize the VulkanType template correectly
// for the given types (except the rawBuffer if gotten from vpp::raw and the container).
auto size = vpp::neededBufferSize140(dev, myvec2, myvec3, mymat3, myvector, myrawdata);
auto buffer = createBuffer(dev, size);

// alternatively, the size for constant size objects can be computed at compile time
// note that this of course only works for static sized types and not for containers
constexpr auto size2 = vpp::neededBufferSize140<Vec2, Vec3, Mat3, Vec4, Vec4>();
auto buffer2 = createBuffer(dev, size2);

//fill the data with the buffer
//Usually one would one finish this work in place but batch it together with other work
//can be done using a vpp::WorkManager.
auto work = vpp::fill140(dev, myvec2, myvec3, mymat3, myvector, myrawdata);

// the fill and retrieve calls will return a work implementation
// we can use it to control execution status or e.g. finish it
work->finish(); // will also be done at destruction

// read the buffer data back into the variables
// the given references must stay valid until the returned work is finished
vpp::read140(dev, data, myvec2, myvec3, mymat3, myvector, myrawdata)->finish();
