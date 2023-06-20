include(pico_sdk_import.cmake)
# initialize the Raspberry Pi Pico SDK
pico_sdk_init()


# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)
#target_link_libraries(PCF8575 pico_stdlib)
# 生成链接库

add_library (PCF8575 ${DIR_LIB_SRCS})