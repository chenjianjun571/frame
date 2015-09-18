#目录，标识makefile.pub所在的目录
ROOT_PATH=./

LINUX26=1

#编译标识，打开为编译动态库，关闭为编译可执行程序
#SHARED=1

#打开为release版本，关闭是debug版本
ifeq ($(COMPILE),release)
    NDEBUG=1
    DEBUG_DEFINE=-D_DEBUG_STACK -rdynamic -D_LANG_CH_US -DENDECRYPT_DLL -DMYSQLPP_MYSQL_HEADERS_BURIED -std=c++11
else
    #NDEBUG=1
    DEBUG_DEFINE=-D_DEBUG_STACK -rdynamic -D_LANG_CH_US -DENDECRYPT_DLL -DMYSQLPP_MYSQL_HEADERS_BURIED -std=c++11
endif

#C++代码集
SRC=$(shell ls *.cpp)
SRC+=${INC_SRC_LIST}

#CC代码集
SRC_CC=$(shell ls *.cc)
SRC_CC+=${INC_SRC_LIST_CC}

#C代码集
SRC_C=$(shell ls *.c)
SRC_C+=${INC_SRC_LIST_C}

#头文件依赖路径
INC=${PROJECT_INC_DIR} ${INC_DIR_LIST}
#库文件依赖路径
LIB_DIR=${PROJECT_LIB_DIR}

ifdef NDEBUG
 # release库依赖添加
 LIB=-lframe -lglog -lACE -levent -levent_pthreads -lmysqlclient -lmysqlpp -lprotobuf-lite
 CXXFLAGS=-fPIC -fno-inline -D_LINUX -DNDEBUG -Wno-deprecated $(DEBUG_DEFINE)
else
 # debug库依赖添加
 LIB=-lframe -lglog -lACE -levent -levent_pthreads -lmysqlclient -lmysqlpp -lprotobuf-lite
 CXXFLAGS=-fPIC -fno-inline -D_LINUX -Wno-deprecated $(DEBUG_DEFINE)
endif

include $(ROOT_PATH)/makefile.pub
