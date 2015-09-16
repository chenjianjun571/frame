# cube
系统魔方

# 安装boost
# 下载boost_1_58_0.tar 
# 解压 tar xf boost_1_58_0.tar
./bootstrap.sh --prefix=/usr/local/
#./b2 -sTOOLS=gcc install --without-python
# 不加python
./b2 --without-python


# 安装ace
# 下载ACE ACE-6.3.0.tar.gz
# 解压 tar -zxvf ACE-6.3.0.tar.gz
1.export ACE_ROOT=/home/bfjs/bafei/project/ACE_wrappers
2.创建$ACE_ROOT/ace/config.h加入#include "ace/config-linux.h"
3.创建$ACE_ROOT/include/makeinclude/platform_macros.GNU 加入include $(ACE_ROOT)/include/makeinclude/platform_linux.GNU
4.make


# gcc升级到 4.8以上，支持c++11
gcc升级
wget http://ftp.gnu.org/gnu/gcc/gcc-5.2.0/gcc-5.2.0.tar.gz
tar zxvf gcc-5.2.0.tar.gz
cd gcc-5.2.0
./contrib/download_prerequisites
cd .. && mkdir -p gcc-build
cd gcc-build && ../gcc-5.2.0/configure --enable-checking=release --enable-languages=c,c++ --disable-multilib
make & make install

gdb升级
wget http://ftp.gnu.org/gnu/gdb/gdb-7.10.tar.gz
tar zxvf gdb-7.10.tar.gz
cd gdb-7.10
make & make install
./configure —prefix=/usr

# mysql++
wget http://www.tangentsoft.net/mysql++/releases/mysql++-3.2.2.tar.gz
tar -zxvf mysql++-3.2.2.tar.gz
cd mysql++-3.2.2
./configure
make
make install
头文件：
/usr/local/include/mysql++
库文件：
/usr/local/lib/libmysqlpp.so*

# protobuf安装 mac下面安装
下载protobuf
https://protobuf.googlecode.com/svn/rc/protobuf-2.6.0.tar.gz
解压protobuf
tar -xf protobuf-2.6.0.tar.gz
cd protobuf-2.6.0
替换掉gtest，不然会编译出错
rm -rf gtest
下载gtest(在protobuf-2.6.0目录下)
http://googletest.googlecode.com/files/gtest-1.7.0.zip
unzip gtest-1.7.0.zip
mv gtest-1.7.0 gtest
执行
autoreconf -f -i -Wall,no-obsolete
./configure
make
make check
make install

# protobuf安装 centos下面安装
下载protobuf
https://protobuf.googlecode.com/svn/rc/protobuf-2.6.0.tar.gz
解压protobuf
tar -xf protobuf-2.6.0.tar.gz
cd protobuf-2.6.0
./configure
make
make check
make install

# 协议生成方式
# 生成描述文件,主要用于node
./protoc --descriptor_set_out=******.desc --include_imports *****.proto
# 生成代码，用户java c++
./protoc --proto_path=./ ./*.proto --cpp_out=./ --java_out=./