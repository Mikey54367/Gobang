#include<iostream>
#include<string>
#include<functional>
#include<websocketpp/config/asio_no_tls.hpp>
#include<websocketpp/server.hpp>
typedef websocketpp::server<websocketpp::config::asio> wsServer_t;
void close_call_back(websocketpp::connection_hdl hdl)
{
    std::cout<<"websocket连接断开"<<std::endl;
}
void open_call_back(websocketpp::connection_hdl hdl)
{
    std::cout<<"websocket握手成功"<<std::endl;
}
void http_call_back(wsServer_t *srv,websocketpp::connection_hdl hdl)
{
    wsServer_t::connection_ptr conn=srv->get_con_from_hdl(hdl);
    std::cout<<"body: "<<conn->get_request_body()<<std::endl;
    websocketpp::http::parser::request req = conn->get_request();
    std::cout<<"method: "<<req.get_method()<<std::endl;
    std::cout<<"uri: "<<req.get_uri()<<std::endl;

    std::string body="<html><body><h1>Hello World</h1></body></html>";
    conn->set_body(body);
    conn->append_header("Content-Type", "text/html");
}
void message_call_back(wsServer_t *srv,websocketpp::connection_hdl hdl,wsServer_t::message_ptr mptr)
{
    wsServer_t::connection_ptr conn = srv->get_con_from_hdl(hdl);
    std::cout<<"wsmsg: "<<mptr->get_payload()<<std::endl;
    std::string rsp ="client say: "+mptr->get_payload();
    conn->send(rsp);
}
int main()
{
    //1.使用websocket创建服务器
    wsServer_t wsServer;
    //2.设置日志等级
    wsServer.set_access_channels(websocketpp::log::alevel::none);
    //3.初始化asio
    wsServer.init_asio();
    //4.设置回调函数
    wsServer.set_close_handler(close_call_back);
    wsServer.set_open_handler(open_call_back);
    wsServer.set_http_handler(std::bind(http_call_back,&wsServer,std::placeholders::_1));
    wsServer.set_message_handler(std::bind(message_call_back,&wsServer,std::placeholders::_1,std::placeholders::_2));
    //5.listen
    wsServer.listen(8080);
    //6.accept
    wsServer.start_accept();
    //7.Run
    wsServer.run();
    return 0;
}