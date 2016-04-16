/**
* \example for the usage of setup command buffers.
* SetupCommandBuffers allow to easily execute commands needed for setup (like copying image/buffer
* data or inserting some barriers) on the device.
**/

{

//Retrieve a setupCommandBuffer from the vpp device.
auto buffer = device.setupCommandBuffer();

//Get a (forward) executionState object.
//Does only have any effect after the buffer is queued for execution.
//The state could also be retrieved after setting the buffer was set up for execution, but since
//this will often be implicitly done by buffer destruction, it is shown here that it can be done
//before it as well.
auto state = buffer.executionState();

//record some example commands on the image.
//The reason the buffer object can be used directly here in a vulkan command, is that it
//implementes an implicit conversion operator to a vk::CommandBuffer.
vk::cmdCopyImage(buffer, srcImage, srcImageLayout, dstImage, dstImageLayout, 1, &region);

//In many cases, barriers are also needed here in the command buffer, see the vulkan specification
//for details on that.

//finish recording on the buffer and set it up for execution.
//this would also be automatically done when the buffer object is deconstructed (when scrope is left)
//after this call it is undefined, whether the commands were executed or are waiting for execution
//in a queue.
buffer.execute();

//The state object received above from the buffer can now be used to determine whether the
//commandBuffer has been executed or to wait for his execution.

//In this case the comandBuffer was already executed in the meantime
if(state.finished())
{
	std::cout << "No waiting needed. Buffer was executed \n";
}

//Here, the commandBuffer was not (fully) executed and therefore we explicitly wait for it.
else
{
	std::cout << "Starting to wait for buffer... ";
	state.wait();
	std::cout << "finished! \n";
}

//If one does not care about the execution state of individual command buffers, but just want to
//assure that all are executed when the setup (on the host) is finished one could also just signal
//the device to wait for all queued setup buffers.
device.finishSetup();

}
