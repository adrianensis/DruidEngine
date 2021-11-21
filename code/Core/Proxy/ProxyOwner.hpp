#pragma once

#include "Core/BasicTypes.hpp"

class ProxyOwner
{
    PUB using ProxyCallback = std::function<void()>;
    PRI ProxyCallback mCallback = nullptr;

PUB
    void setProxyCallback(ProxyCallback callback)
    {
        mCallback = callback;
    }
    
    void notifyProxy()
    {
        if(mCallback)
        {
            mCallback();
        }
    }
};