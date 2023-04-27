#pragma once

#include <memory>
#include <vector>

namespace util
{

template <typename T>
class RingBuffer
{
  public:
    RingBuffer( int size );
    bool canRead();
    bool canWrite();
    void push(T);
    bool pop(T &out);
    T operator[]( int i ) const;
    int size();

  private:
    std::vector<T> mBuffer;
    int mIRead;
    int mIWrite;
    int mNumReadable;
};

template <typename T>
RingBuffer<T>::RingBuffer( int size ) : mBuffer( size )
                                      , mIRead( 0 )
                                      , mIWrite( 0 )
                                      , mNumReadable( 0 )
{
}

template <typename T> int RingBuffer<T>::size()
{
    return mBuffer.size();
}

template <typename T>
T RingBuffer<T>::operator[]( int i ) const
{
    return mBuffer [ i ];
}

template <typename T>
bool RingBuffer<T>::canRead()
{
    return mNumReadable > 0;
}

template <typename T>
bool RingBuffer<T>::canWrite()
{
    return mNumReadable < mBuffer.size();
}

template <typename T>
void RingBuffer<T>::push(T val)
{
    if (!canWrite()) return;
    mBuffer[mIWrite] = val;
    mIWrite = (mIWrite + 1) % mBuffer.size();
    mNumReadable++;
}

template <typename T>
bool RingBuffer<T>::pop(T &out)
{
    if (!canRead()) return false;
    out = mBuffer[mIRead];
    mIRead = (mIRead + 1) % mBuffer.size();
    mNumReadable--;
    return true;
}


} // namespace util
