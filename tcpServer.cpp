#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <threadpool.h>
//using namespace boost::asio;
void handle_connection(boost::asio::ip::tcp::socket socket) {
    try {
        // 读取请求行
        char data[1024];
        boost::system::error_code error;
        size_t length = socket.read_some(boost::asio::buffer(data), error);

        if (error == boost::asio::error::eof) {
            return; // 连接关闭
        } else if (error) {
            throw boost::system::system_error(error); // 处理错误
        }

        std::string request_line(data, length);
        std::cout << "Request: " << request_line << std::endl;

        // 简单的请求行解析
        std::string status_line;
        std::string filename;
        std::string contents; // 声明 contents 变量

        if (request_line.find("GET / HTTP/1.1") != std::string::npos) {
            status_line = "HTTP/1.1 200 OK";
            filename = "hello.html";
        } else if (request_line.find("GET /sleep HTTP/1.1") != std::string::npos) {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            status_line = "HTTP/1.1 200 OK";
            filename = "hello.html";
        } else if (request_line.find("GET /sl HTTP/1.1") != std::string::npos) {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            status_line = "HTTP/1.1 200 OK";
            filename = "hello.html";
        } else {
            status_line = "HTTP/1.1 404 NOT FOUND";
            filename = "404.html";
        }

        // 读取文件内容
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "File " << filename << " not found\n";
            status_line = "HTTP/1.1 404 NOT FOUND";
            contents = "404 Not Found";
        } else {
            std::stringstream buffer;
            buffer << file.rdbuf();
            contents = buffer.str();
        }

        // 发送响应
        std::string response = status_line + "\r\nContent-Length: " + std::to_string(contents.size()) + "\r\n\r\n" + contents;
        boost::asio::write(socket, boost::asio::buffer(response), error);

        if (error) {
            throw boost::system::system_error(error); // 处理错误
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in connection handler: " << e.what() << "\n";
    }
}
int main() {
    try {
        // 创建 IO 服务
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 7867));
        ThreadPool pool(4);
        for (;;) {
            boost::asio::ip::tcp::socket socket(io_context);
            acceptor.accept(socket);
            //handle_connection(std::move(socket));
            pool.execute([socket = std::move(socket)]() mutable {
            handle_connection(std::move(socket));
            });
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}