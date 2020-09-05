# 这句是链接时候的命令,在g++前面加入了@echo linking $@
# 这样在链接时候就会先输出linking xxx,
# 这行直接写g++也是没有任何问题的
LINK  = @echo linking $@ && g++ 

# 编译c++代码时候用的,一样会显示compiling xxx
GCC  = @echo compiling $@ && g++ 

# 编译c代码时候用gcc
GC   = @echo compiling $@ && gcc 

# 生成静态库时候用ar命令
AR   = @echo generating static library $@ && ar crv

# 这是编译时候的参数设置,下面-g表示编译时候加入调试信息,
# -DDEBUG表示编译debug版本
# -W -Wall表示输出所有警告
# -fPIC是生成dll时候用的
FLAGS = -g -DDEBUG -W -Wall -fPIC
GCCFLAGS = -std=c++11
DEFINES = 

# 这里指出头文件的目录为./
HEADER = -I./

# 需要引用的库文件
LIBS  = 
LINKFLAGS = -lpthread

# 更多头文件可以用 += 加进来
#HEADER += -I./

# 如果需要librt.so,就把-lrt加进来
#LIBS  += -lrt

# 如果需要写多线程程序,就需要引用-pthread
#LIBS  += -pthread

# 这里是主要需要修改的地方,每一个.c或.cpp对应于这里的一项,
# 如main.cpp对应于main.o
# 多个.o可以用空格分开,也可以像下面这样用"\"换行,然后写在新一行
OBJECT := server.o \

# 下面举个例子,这里编译表示两个代码文件
# OBJECT := main.o \
#     other.o

BIN_PATH = ./

TARGET = main

# 链接用的,可以看到后面加了$(LIBS),因为只有链接时候才需要引用库文件
$(TARGET) : $(OBJECT) 
	$(LINK) $(FLAGS) $(LINKFLAGS) -o $@ $^ $(LIBS)

# 编译cpp代码用这个目标
.cpp.o:
	$(GCC) -c $(HEADER) $(FLAGS) $(GCCFLAGS) -fpermissive -o $@ $<

# 编译c代码用这个
.c.o:
	$(GC) -c $(HEADER) $(FLAGS) -fpermissive -o $@ $<

# 把生成的$(TARGET)拷贝到$(BIN_PATH)下
install: $(TARGET)
	cp $(TARGET) $(BIN_PATH)

clean:
	rm -rf $(TARGET) *.o *.so *.a
