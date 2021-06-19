
#ifndef CALLBACK_BASE_H_
#define CALLBACK_BASE_H_

#include <functional>
#include <memory>
namespace net {
class TcpConnection;
class Buffer;

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

using readCallback=std::function<void()>;
using writeCallback=std::function<void()>;
using closeCallback=std::function<void()>;
using errorCallback=std::function<void()>;
using timerCallback=std::function<void()>;
using signalCallback=std::function<void()>;

using TcpConnectionPtr=std::shared_ptr<TcpConnection>;
using NewConnectionCallback=std::function<void(int)>;
using ConnectionCallback=std::function<void(const TcpConnectionPtr&)>;
using CloseConnectionCallback=std::function<void(const TcpConnectionPtr&)>;
using MessageCallback=std::function<void(const TcpConnectionPtr&,Buffer *)>;
using WriteCompleteCallback=std::function<void(const TcpConnectionPtr&)>;
using HighWaterMarkCallback=std::function<void(const TcpConnectionPtr&)>;
}
#endif 


