set(CMAKE_CXX_STANDARD 17)  # C++17 standardını kullan
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_minimum_required(VERSION 3.10)
project(DeepFakeServer)

# Boost'un bulunduğu dizini belirtiyoruz
set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/libs/boost_1_82_0")

# Boost kütüphanelerini buluyoruz (filesystem, system eklendi)
find_package(Boost REQUIRED COMPONENTS system thread json filesystem)

# ZeroMQ'yu buluyoruz
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

# Derlenecek kaynak dosyalar
add_executable(server server.cpp)

# Kütüphanelerin dizinlerini belirtiyoruz
target_include_directories(server PRIVATE 
    ${Boost_INCLUDE_DIRS} 
    ${ZMQ_INCLUDE_DIRS}
)

# Boost ve ZeroMQ kütüphanelerini programa bağlama
target_link_libraries(server PRIVATE 
    ${Boost_LIBRARIES} 
    ${ZMQ_LIBRARIES}
)

# Ek olarak Boost'un lib dizinini bağlamak gerekiyorsa bunu belirtelim
link_directories(${Boost_LIBRARY_DIRS})
