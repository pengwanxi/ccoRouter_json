set(APP ccoRouter)  # 项目名称
set(EXECUTABLE_NAME ccoRouter)  # 可执行文件名称

# 项目信息
project(${APP})

#set(FrpcCtl_PATH "${ROOT_DIR}/src/FrpcCtl/")
set(FrpcCtl_PATH "${ROOT_DIR}")

file(GLOB_RECURSE FRPC_SRC "${FrpcCtl_PATH}/*.c" "${FrpcCtl_PATH}/*.cpp")
file(GLOB_RECURSE COMMON_SRC "${FrpcCtl_PATH}/common/*.c" "${FrpcCtl_PATH}/common/*.cpp")

set(FRPC_INC_DIR ${FrpcCtl_PATH}/)
set(COMMON_INC_DIR ${FrpcCtl_PATH}/common/)

message(STATUS "DIR:${COMMON_INC_DIR} ${ROOT_DIR}/zlog/")

# 增加头文件
include_directories(
  ${ROOT_DIR}/inc/zlog/
  ${ROOT_DIR}/inc/mosquitto/
  ${ROOT_DIR}/inc/cJSON/
  ${FRPC_INC_DIR}
  ${COMMON_INC_DIR}
  ./
)

# 增加当前文件夹
aux_source_directory(. DIR_SRCS)
add_executable(${EXECUTABLE_NAME} ${DIR_SRCS})

# 第三方源文件夹
target_sources(
  ${EXECUTABLE_NAME}
  PRIVATE
  ${FRPC_SRC}
  ${COMMON_SRC}
)

# 线程
find_package(Threads)
target_link_libraries(${EXECUTABLE_NAME} ${CMAKE_THREAD_LIBS_INIT})

# 依赖库
SET(APP_LINK_LIBRARIES
  ${LIBRARY_DIR}/libzlog.so.1.2
  ${LIBRARY_DIR}/libzlog.so
  ${LIBRARY_DIR}/libcJSON.so
  ${LIBRARY_DIR}/libmosquitto.so.1
  ${LIBRARY_DIR}/libssl.so.3
  ${LIBRARY_DIR}/libz.so
  ${LIBRARY_DIR}/libcrypto.so.3
)

target_link_libraries(${EXECUTABLE_NAME} ${APP_LINK_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} dl)
message(STATUS "LINK_LIBRARY:" ${APP_LINK_LIBRARIES})

# 形成库
#install(FILES ${ROOT_DIR}/data/FrpcCtl DESTINATION ${PACKAGES_DIR}/)
install(TARGETS ${EXECUTABLE_NAME} DESTINATION ${BINARY_DIR}/FrpcCtl)
install(TARGETS ${EXECUTABLE_NAME} DESTINATION ${PACKAGES_DIR}/FrpcCtl/bin/)
install(FILES ${APP_LINK_LIBRARIES} DESTINATION ${PACKAGES_DIR}/FrpcCtl/lib/)



