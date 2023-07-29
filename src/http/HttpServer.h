#ifndef HTTP_HTTPSERVER_H
#define HTTP_HTTPSERVER_H

#include "TcpServer.h"      // 引入 TcpServer 类，用于创建 TCP 服务器
#include "noncopyable.h"    // 引入 noncopyable 类，用于禁止拷贝构造和赋值操作
#include "Logging.h"        // 引入 Logging 类，用于记录日志信息
#include <string>

class HttpRequest;      // 前置声明 HttpRequest 类，用于表示 HTTP 请求
class HttpResponse;     // 前置声明 HttpResponse 类，用于表示 HTTP 响应

class HttpServer : noncopyable
{
public:
    /**
     * 定义 HttpCallback 类型，是一个函数对象，
     * 接受一个 HttpRequest 常引用和一个 HttpResponse 指针作为参数
     */
    using HttpCallback = std::function<void (const HttpRequest&, HttpResponse*)>;

    // HttpServer 的构造函数，接受参数与 TcpServer 相同
    HttpServer(EventLoop *loop,
            const InetAddress& listenAddr,
            const std::string& name,
            TcpServer::Option option = TcpServer::kNoReusePort);
    
    EventLoop* getLoop() const { return server_.getLoop(); }

    void setHttpCallback(const HttpCallback& cb)
    {
        httpCallback_ = cb;
    }
    
    void start();

private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr &conn,
                    Buffer *buf,
                    Timestamp receiveTime);
    void onRequest(const TcpConnectionPtr&, const HttpRequest&);

    TcpServer server_;
    HttpCallback httpCallback_;
};

#endif // HTTP_HTTPSERVER_H