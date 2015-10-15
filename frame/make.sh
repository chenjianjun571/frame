#! /bin/sh

#定义代码根目录
export PROJECT_SRC_ROOT=src/

#定义头文件目录
PROJECT_INC_DIR=" -I/home/bfjs/jsbn/HtProjet/third/include/ "
PROJECT_INC_DIR+=" -I/home/bfjs/jsbn/HtProjet/third/include/google/ "
PROJECT_INC_DIR+=" -I/home/bfjs/jsbn/HtProjet/third/include/event/ "
PROJECT_INC_DIR+=" -I/home/bfjs/jsbn/HtProjet/third/include/thrift/ "
export PROJECT_INC_DIR

#生成可执行文件的目录
export TARGETDIR=./
#生成的程序名
export NAME=frame

#记住当前目录
SRC_DIR=`pwd`
INC_DIR_LIST=""
INC_SRC_LIST=""
INC_SRC_LIST_C=""
function get_list()
{
        cd $1

        #当前目录
        DIR=`pwd`
        DIR+="/"

        #代码list
        SRC_LIST=`ls *.cpp 2>/dev/null`
        SRC_LIST_C=`ls *.c 2>/dev/null`
        for src_file in $SRC_LIST
        do
                INC_SRC_LIST+=" "
                INC_SRC_LIST+=${DIR}
                INC_SRC_LIST+=$src_file
        done

        for src_file in $SRC_LIST_C
        do
                INC_SRC_LIST_C+=" "
                INC_SRC_LIST_C+=${DIR}
                INC_SRC_LIST_C+=$src_file
        done

        #添加目录list
        INC_DIR_LIST+=" -I"
        INC_DIR_LIST+=${DIR}

        #当前目录的目录集
        DIR_LIST=`ls -l | grep ^d | awk '{print $9}' 2>/dev/null`
        for dir_name in $DIR_LIST
        do
                get_list ${dir_name}
        done

        cd ..
}
get_list ${PROJECT_SRC_ROOT}

#回到当前目录
cd $SRC_DIR

export INC_SRC_LIST
export INC_SRC_LIST_C
export INC_DIR_LIST

case "$1" in
        r|release)
                  echo "comple frame of release..."
                  #export LD_LIBRARY_PATH=$ITEL_LIB_LOCAL_ROOT_RELEASE:$LD_LIBRARY_PATH
                  make -f Makefile.mak COMPILE=release
                  exit $?
                  ;;
        d|debug)
                  echo "comple frame of debug..."
                  #export LD_LIBRARY_PATH=$ITEL_LIB_LOCAL_ROOT_DEBUG:$LD_LIBRARY_PATH
                  make -f Makefile.mak COMPILE=debug
                  exit $?
                  ;;
        cr|clear_release)
                  echo "clean and comple frame of release..."
                  #export LD_LIBRARY_PATH=$ITEL_LIB_LOCAL_ROOT_RELEASE:$LD_LIBRARY_PATH
                  make -f Makefile.mak COMPILE=release clean;make -f Makefile.mak COMPILE=release
                  exit $?
                  ;;
        cd|clear_debug)
                  echo "clean and comple frame of debug..."
                  #export LD_LIBRARY_PATH=$ITEL_LIB_LOCAL_ROOT_DEBUG:$LD_LIBRARY_PATH
                  make -f Makefile.mak COMPILE=debug clean;make -f Makefile.mak COMPILE=debug
                  exit $?
                  ;;
        clean_release)
                  echo "clean and comple frame of release..."
                  #export LD_LIBRARY_PATH=$ITEL_LIB_LOCAL_ROOT_RELEASE:$LD_LIBRARY_PATH
                  make -f Makefile.mak COMPILE=release clean;
                  exit $?
                  ;;
        clean_debug)
                  echo "clean and comple frame of debug..."
                  #export LD_LIBRARY_PATH=$ITEL_LIB_LOCAL_ROOT_DEBUG:$LD_LIBRARY_PATH
                  make -f Makefile.mak COMPILE=debug clean;
                  exit $?
                  ;;
        *)
                  echo "------------------------------------------"
                  echo "./make.sh release        :comple frame of release."
                  echo "./make.sh debug         :comple frame of debug."
                  echo "./make.sh clear_release  :clean and comple frame of release."
                  echo "./make.sh clear_debug   :clean and comple frame of debug."
                  echo "./make.sh clean_release  :clean frame of release."
                  echo "./make.sh clean_debug   :clean frame of debug."
                  echo "------------------------------------------"
                  exit 1
                  ;;
esac
