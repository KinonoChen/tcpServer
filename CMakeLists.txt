cmake_minimum_required(VERSION 3.10)
project(cppweb)

# 设置 Boost 的根目录
set(BOOST_ROOT "D:/work_study/boost_1_85_0")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")

# 查找 Boost 库
find_package(Boost REQUIRED COMPONENTS system)

# 包含 Boost 头文件目录
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# 链接 Boost 库目录
link_directories(${BOOST_LIBRARYDIR})

# 设置源文件
set(HTTP_SERVER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/tcpServer.cpp)

# 创建可执行文件
add_executable(tcpServer ${HTTP_SERVER_SOURCE})

# 链接 Boost 系统库
target_link_libraries(tcpServer Boost::system)
