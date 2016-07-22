/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 nyorain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

///\file
///\brief Defines the Connection and Connectable classes needed e.g. for callbacks.

#pragma once

#include <nytl/nonCopyable.hpp>

#include <cstdlib>
#include <memory>

namespace nytl
{

///Base class for objects that can be connected to in any way.
///This connection can then be controlled (i.e. destroyed) with a Connection object.
class Connectable
{
protected:
	friend class Connection;
	friend class ConnectionRef;

	virtual ~Connectable() = default;
    virtual void remove(size_t id) = 0;
};

///Underlaying connection data.
using ConnectionDataPtr = std::shared_ptr<std::size_t>;

///\ingroup function
///\brief The Connection class represents a Connection to a nytl::Callback slot.
///\details A Connection object is returned when a function is registered in a Callback object
///and can then be used to unregister the function and furthermore check whether
///the Callback object is still valid and the function is still registered.
class Connection
{
public:
    Connection(Connectable& call, const ConnectionDataPtr& data) noexcept
		: callback_(&call), data_(data) {}

	Connection() = default;
    ~Connection() = default;

    Connection(const Connection&) = default;
    Connection& operator=(const Connection&) = default;

    Connection(Connection&&) = default;
    Connection& operator=(Connection&&) = default;

	///Unregisters the function associated with this Connection from the Callback object.
    void destroy() { if(callback_ && connected()) callback_->remove(*data_); callback_ = nullptr; }

	///Returns whether the function is still registered and the Callback is still alive.
    bool connected() const { return (callback_) && (data_) && (*data_ != 0); }

protected:
	Connectable* callback_ {nullptr};
    ConnectionDataPtr data_ {nullptr};
};

///\ingroup function
///\brief Like Connection representing a registered function but can be used inside Callbacks.
///\details Sometimes it may be useful to unregister a Callback function while it is called
///(e.g. if the Callback function should be called only once) and there is no possibility to
///capture a Connection object inside the Callback (like e.g. with lambdas) then a ConnectionRef
///parameter can be added to the beggining of the Callbacks function signature with which the
///function can be unregistered from inside itself. A new class is needed for this since
///if Connection is used in a fucntion signature, the Callback object can not know if this
///Connection object is part of the signature or only there to get a Connection to itself.
///So there is no need for generally using this class outside a Callback function, Connection
///should be used instead since it proved the same functionality.
class ConnectionRef
{
public:
    ConnectionRef(Connectable& call, const ConnectionDataPtr& data) noexcept
		: callback_(&call), data_(data) {}

    ~ConnectionRef() = default;

    ConnectionRef(const ConnectionRef& other) = default;
    ConnectionRef& operator=(const ConnectionRef& other) = default;

    ConnectionRef(ConnectionRef&& other) = default;
    ConnectionRef& operator=(ConnectionRef&& other) = default;

	///Disconnected the Connection, unregisters the associated function.
    void destroy() const { if(callback_ && connected()) callback_->remove(*data_); }

	///Returns whether the Callback function is still registered.
    bool connected() const { return (callback_) && (*data_ != 0); }

protected:
	Connectable* callback_ {nullptr};
    ConnectionDataPtr data_ {nullptr};
};

///\ingroup function
///RAII Connection class that will disconnect automatically on destruction.
class RaiiConnection : public NonCopyable
{
public:
	RaiiConnection(const Connection& conn) : Connection_(conn) {}
	~RaiiConnection() { Connection_.destroy(); }

	RaiiConnection(RaiiConnection&& other) : Connection_(std::move(other.Connection_)) {}
	RaiiConnection& operator=(RaiiConnection&& other)
		{ release(); Connection_ = std::move(other.Connection_); return *this; }

	Connection& get() { return Connection_; }
	const Connection& get() const { return Connection_; }
	void release(){ Connection_ = {}; }

	bool connected() const { return Connection_.connected(); }
	void destroy() { Connection_.destroy(); }

protected:
	Connection Connection_ {};
};

}
