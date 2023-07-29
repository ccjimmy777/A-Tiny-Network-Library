#include "TcpServer.h"
#include "EventLoop.h"
#include "Logging.h"

class TinyServer
{
private:
    TcpServer _server;
    EventLoop * _loop;

     // 连接建立或断开的回调函数
    void onConnection(const TcpConnectionPtr &conn);
    // 可读写事件回调
    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time);

public:
    TinyServer(EventLoop * loop, const InetAddress & addr, const std::string & name);
    ~TinyServer();

    void start()
    {
        _server.start();
        // _loop->loop();
    }

};

TinyServer::TinyServer(EventLoop * loop, const InetAddress & addr, const std::string & name)
    : _server(loop, addr, name)
    , _loop(loop)
{
    // 注册回调函数
    _server.setConnectionCallback(
        std::bind(&TinyServer::onConnection, this, std::placeholders::_1));
        
    _server.setMessageCallback(
        std::bind(&TinyServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // 设置合适的subloop线程数量
    _server.setThreadNum(4);
}

TinyServer::~TinyServer()
{
}

 // 连接建立或断开的回调函数
void TinyServer::onConnection(const TcpConnectionPtr &conn)   
{
    if (conn->connected())
    {
        LOG_INFO << "Connection ON : " << conn->peerAddress().toIpPort()
            << " -> " << conn->localAddress().toIpPort();
    }
    else
    {
        LOG_INFO << "Connection OFF : " << conn->peerAddress().toIpPort()
            << " -> " << conn->localAddress().toIpPort();
    }
}

// 可读写事件回调
void TinyServer::onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
{
    std::string msg = buf->retrieveAllAsString();
    LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, "
                << "data received at " << time.toFormattedString();
    conn->send(msg);
    // conn->shutdown();   // 关闭写端 底层响应 EPOLLHUP => 执行closeCallback_
}


int main() 
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    InetAddress addr(1234, "127.0.0.1");
    TinyServer server(&loop, addr, "TinyServer");

    server.start();
    loop.loop();

    return 0;
}