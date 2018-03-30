/*
 * Copyright (c) 2016 PolySync
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * \example Publish.cpp
 *
 * Publish/Subscribe Example.
 *
 * Shows how to use publish/subscribe communication model, and the Messaging API
 * routines. This half of the example code populates and publishes a message
 * to the PolySync bus.
 *
 * The second half of the example - Subscribe.cpp - runs as a seperate node,
 * and subscribes to the messages to access the data buffer.
 *
 * The example uses the standard PolySync node template and state machine.
 * Send the SIGINT (control-C on the keyboard) signal to the node/process to do 
 * a graceful shutdown.
 *
 */

#include <iostream>
#include <PolySyncNode.hpp>
#include <PolySyncDataModel.hpp>

using namespace std;

/**
 * Node flags to be OR'd with node flags during the build.
 *
 */
#ifndef NODE_FLAGS_VALUE
#define NODE_FLAGS_VALUE (0) 
#endif

/**
 * @brief PublisherSubscriberNode class
 *
 * 
 */
    int count;
class PublisherSubscriberNode : public polysync::Node
{

private:
    const string node_name = "virtual-car-msg";
    //const string platform_motion_msg_name = "ps_platform_motion_msg";
    const string byte_array_msg_name = "ps_byte_array_msg";
    ps_msg_type _messageType;
    
public:
    
    PublisherSubscriberNode()
    {
        setNodeType( PSYNC_NODE_TYPE_API_USER );
        setDomainID( PSYNC_DEFAULT_DOMAIN );
        setSDFID( PSYNC_SDF_ID_INVALID );
        setFlags( NODE_FLAGS_VALUE | PSYNC_INIT_FLAG_STDOUT_LOGGING );
        setNodeName( node_name );
    }
    
    ~PublisherSubscriberNode()
    {
        
    }

    void initStateEvent() override
    {
        _messageType = getMessageTypeByName( byte_array_msg_name );

    }
    
    void releaseStateEvent() override
    {
        // do nothing, sleep for 10 milliseconds
    }
    
    void errorStateEvent() override
    {
        // do nothing, sleep for 10 milliseconds
        polysync::sleepMicro( 10000 );
    }
    
    void fatalStateEvent() override
    {
        // do nothing, sleep for 10 milliseconds
        polysync::sleepMicro( 10000 );
    }
    
    void warnStateEvent() override
    {
        // do nothing, sleep for 10 milliseconds
        polysync::sleepMicro( 10000 );
    }
    
    /**
     * Override the base class functionality to send messages when the node
     * reaches the "ok" state. This is the state where the node is in its
     * normal operating mode.
     */
    void okStateEvent() override
    {

        //MARK Temp for now, should actually be subscribing to messages for this info
        float distBetwCars = 15.0f;
        float veloVirtualCar = 5.0f;
        float rateChangeBetwCars = 0.5f;

        // Create a message
        //polysync::datamodel::PlatformMotionMessage message( *this );
        polysync::datamodel::ByteArrayMessage virtMessage(*this);

        // Set message data

        std::vector< unsigned char > msg;
        char msgBuf[128];
        msg.clear();
        msg.reserve(128);

        sprintf(msgBuf,"DISTBETWCARS%f,VELOVIRTUALCAR%f,RATECHANGEBETWCARS%f,END",
        distBetwCars,
        veloVirtualCar,
        rateChangeBetwCars);

        msg.clear();
        for (int i = 0; i < 128; ++i)
        {
            msg.push_back( (unsigned char)msgBuf[i]);
            if (msgBuf[i] == '\0')
                break;
        }

        virtMessage.setBytes(msg);
        virtMessage.setHeaderTimestamp( polysync::getTimestamp() );
        virtMessage.setHeaderSrcGuid (36839);
        virtMessage.publish();
        //cout << msgBuf<<endl;
        //virtMessage.print();

        // The ok state is called periodically by the system, sleep to reduce
        // the number of messages sent
        polysync::sleepMicro( 1000000 );
    }
};


/**
 * Entry point for this node
 * The "connectToPolySync" is a blocking call, users must use Ctrl-C to quit
 * the execution loop.
 */
int main()
{
    //psync_init();
    // Create an instance of the PublisherSubscriberNode and connect it to 
    // PolySync
    PublisherSubscriberNode publisherSubscriberNode;

    // When the node has been created, it will cause an initStateEvent to be
    // sent and then proceed into the okState.  connectToPolySync does not
    // return, use Ctrl-C to exit
    publisherSubscriberNode.connectPolySync();
    return 0;
}
