/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Jan Kelling
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
///\brief Defines the Callback and Connection classes.

#pragma once

#include <nytl/nonCopyable.hpp>
#include <nytl/compFunc.hpp>

#include <functional>
#include <vector>
#include <utility>
#include <memory>
#include <atomic>

namespace nytl
{

class Connection;
class ConnectionRef;

template <class Signature> class Callback;

namespace detail
{

class CallbackBase
{

friend class ::nytl::Connection;
friend class ::nytl::ConnectionRef;

protected:
    virtual void remove(size_t id) = 0;
};

struct ConnectionData
{
    ConnectionData(size_t i) : id(i) {}
    std::atomic<size_t> id {0}; //if id == 0, the Connection is not connected
};

}

///\ingroup function
///\brief The Connection class represents a Connection to a nytl::Callback slot.
///\details A Connection object is returned when a function is registered in a Callback object
///and can then be used to unregister the function and furthermore check whether 
///the Callback object is still valid and the function is still registered.
class Connection
{
protected:
    template<class T> friend class Callback;

	detail::CallbackBase* callback_ {nullptr};
    std::shared_ptr<detail::ConnectionData> data_ {nullptr};

    Connection(detail::CallbackBase& call, std::shared_ptr<detail::ConnectionData> data) 
		: callback_(&call), data_(data) {}

public:
	Connection() = default;
    ~Connection() = default;

    Connection(const Connection&) = default;
    Connection& operator=(const Connection&) = default;

    Connection(Connection&&) = default;
    Connection& operator=(Connection&&) = default;

	///Unregisters the function associated with this Connection from the Callback object.
    void destroy()
		{ if(callback_ && connected()) callback_->remove(data_->id); callback_ = nullptr; }

	///Returns whether the function is still registered and the Callback is still alive.
    bool connected() const 
		{ return (callback_) && (data_) && (data_->id.load() != 0); }
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
protected:
    template<class T> friend class Callback;

	detail::CallbackBase* callback_ {nullptr};
    std::shared_ptr<detail::ConnectionData> data_ {nullptr};

    ConnectionRef(detail::CallbackBase& call, std::shared_ptr<detail::ConnectionData> data) 
		: callback_(&call), data_(data) {}
public:
    ~ConnectionRef() = default;

    ConnectionRef(const ConnectionRef& other) = default;
    ConnectionRef& operator=(const ConnectionRef& other) = default;

    ConnectionRef(ConnectionRef&& other) = default;
    ConnectionRef& operator=(ConnectionRef&& other) = default;

	///Disconnected the Connection, unregisters the associated function.
    void destroy() const
		{ if(callback_ && connected()) callback_->remove(data_->id); }

	///Returns whether the Callback function is still registered.
    bool connected() const 
		{ return (callback_) && (data_->id.load() != 0); }
};

///\ingroup function
///RAII Connection class that will disconnect autoMatically on destruction.
class RaiiConnection
{
protected:
	Connection Connection_ {};

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
};


//TODO make Callback threadsafe using a lockfree list as container. There are already shared
//pointers anyway so it should not be that expensive.

///\brief Represents a Callback for which listener functions can be registered.
///\ingroup function
///
///\details It is used for registering functions that should be called when
///the Callback is triggered. This is intented as more lightweight, easier, more dynmaic and 
///macro-free options to the signal-slot mechanism used by many c++ libraries.
///The temaplte parameter Signature indicated the return types registered fucntions should have
///and the possible parameter they can get.
///
///Registering a Callback function returns a Connection object which can then be used to unregister
///it an to check whether it is still connected which means that the function is still
///registered (Connection objects can be copied so it may have been unregistered by a copied 
///Connection) and the Callback object is still alive. Any object that can be represented
///by a std::function can be registered at a Callback object, so it is impossible to 
///unregister a function only by its functions (std::function cannot be compared for equality),
///that is why Connections are used to unregister/check the registered functions.
///
///Registered functions that should be called if the Callback is activated must have a signature
///compatible (-> see \c CompatibleFunction for more inforMation on the compatible functions 
///conecpt) to Ret(const ConnectionRef&, Arg...).
///The nytl::ConnectionRef object can optionally be used to unregister the Callback function from
///inside itself when it gets triggered.
///At the moment Callback is not fully threadsafe, if one thread calls e.g. call() while another
///one calls add() it may cause undefined behaviour.
template <class Ret, class ... Args> 
class Callback<Ret(Args...)> : public detail::CallbackBase
{
protected:
    struct CallbackSlot
    {
        std::shared_ptr<detail::ConnectionData> data;
        std::function<Ret(const ConnectionRef&, Args ...)> func;
    };

protected:
    size_t highestID_ {0};
    std::vector<CallbackSlot> slots_;

    virtual void remove(size_t id) override
    {
        if(id == 0)
            return;

        for(auto it = slots_.cbegin(); it != slots_.cend(); ++it)
        {
            if(it->data->id == id)
            {
                it->data->id.store(0);
                slots_.erase(it);
                return;
            }

        }
    };

public:
	///Destroys the Callback object and removes all registered functions.
    ~Callback()
    {
        clear();
    }

    ///Registers a function without returning a Connection object.
    Connection operator+=(CompFunc<Ret(const ConnectionRef&, Args ...)> func)
    {
        return add(func);
    };

	///Resets all registered function and sets the given one as only Callback function.
    Connection operator=(CompFunc<Ret(const ConnectionRef&, Args ...)> func)
    {
        clear();
        return add(func);
    };

	///\brief Registers a Callback function.
	///\details The function must have a compatible signature to the Callbacks one but
	///may additionally have a ConnectionRef parameter as first one which can then
	///be used to unregister the function from within itself.
	///\return A Connection object for the registered function which can be used to 
	///unregister it and check if it is registered, see \c Connection for more inforMation.
    Connection add(CompFunc<Ret(const ConnectionRef&, Args ...)> func)
    {
        slots_.emplace_back();

        auto ptr = std::make_shared<detail::ConnectionData>(++highestID_);
        slots_.back().data = ptr;
        slots_.back().func = func.function();

        return Connection(*this, ptr);
    };

	///Calls all registered functions and returns a Vector with the returned objects.
    std::vector<Ret> call(Args ... a)
    {
        auto Vec = slots_; //if called functions manipulate Callback

        std::vector<Ret> ret;
        ret.reserve(slots_.size());

        for(auto& slot : Vec)
            ret.push_back(slot.func(ConnectionRef(*this, slot.data), a ...));

        return ret;
    };

	///Clears all registered functions.
    void clear()
    {
        slots_.clear();
    }

	///Operator version of call. Calls all registered functions and return their returned objects.
    std::vector<Ret> operator() (Args... a)
    {
        return call(a ...);
    }
};


//The Callback specialization for a void return type.
//\details There has to be a specialization since call cannot return a std::vector of void. 
template <class ... Args> 
class Callback<void(Args...)> : public detail::CallbackBase
{
protected:
    struct CallbackSlot
    {
        std::shared_ptr<detail::ConnectionData> data;
        std::function<void(const ConnectionRef&, Args ...)> func;
    };

protected:
    size_t highestID {0};
    std::vector<CallbackSlot> slots_;

    virtual void remove(size_t id) override
    {
        if(id == 0)
            return;

        for(auto it = slots_.cbegin(); it != slots_.cend(); ++it)
        {
            if(it->data->id == id)
            {
                it->data->id.store(0);
                slots_.erase(it);
                return;
            }
        }
    };

public:
    ~Callback()
    {
        clear();
    }

    Connection operator+=(CompFunc<void(const ConnectionRef&, Args ...)> func)
    {
        return add(func);
    };

    Connection operator=(CompFunc<void(const ConnectionRef&, Args ...)> func)
    {
        clear();
        return add(func);
    };

    Connection add(CompFunc<void(const ConnectionRef&, Args ...)> func)
    {
        slots_.emplace_back();

        auto ptr = std::make_shared<detail::ConnectionData>(++highestID);
        slots_.back().data = ptr;
        slots_.back().func = func.function();

        return Connection(*this, ptr);
    };

    void call(Args ... a)
    {
        auto Vec = slots_; 

        for(auto& slot : Vec)
            slot.func(ConnectionRef(*this, slot.data), a ...);
    };

    void clear()
    {
        slots_.clear();
    }

    void operator() (Args... a)
    {
        call(a ...);
    }
};

}

