#PATH = <where your toolchain install and in case you don't put in the system path> 
#CROSS_COMPILE = $(PATH)/arm-none-linux-gnueabi-
CROSS_COMPILE = arm-none-linux-gnueabi-

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP

CFLAGS := -Wall -O2 -g
CFLAGS += -I $(shell pwd)/include -I/opt/libdecode/include

LDFLAGS :=
#LDFLAGS := -ljpeg -lpng -lz -lpthread -L/opt/libdecode/lib

export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := imageplayer


obj-y += main.o

# 添加顶层目录下的子文件夹（注意目录名后面加一个/）
obj-y += fbapp/
obj-y += core_api/
#obj-y += image_manage/

all: 
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(CC) $(LDFLAGS) -o $(TARGET) built-in.o

cp:
	rm -rf ~/nfsroot/rootfs/root/proj_imageplayer
	cp ../proj_imageplayer/ ~/nfsroot/rootfs/root/ -rf

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)

