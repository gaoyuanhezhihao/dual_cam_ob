CC :=g++
SRCDIR := src
BUILDDIR := build
TARGET :=bin/runner

SRCEXT:=cpp
SOURCES:=$(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS:=$(patsubst $(SRCDIR)/%, $(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS:= -std=c++11
CFLAGS+=`pkg-config --cflags /home/hzh/study/opencv3/so_build/lib/pkgconfig/opencv.pc`
LDFLAGS= `pkg-config --libs  /home/hzh/study/opencv3/so_build/lib/pkgconfig/opencv.pc`

LDFLAGS+= -L/home/hzh/study/opencv3/so_build/share/OpenCV/3rdparty/lib -lippicv
LDFLAGS+= -L/home/hzh/Downloads/leptonica-1.73/build/src -llept -ltiff
INC:= -I include

$(TARGET):$(OBJECTS)
	@echo "Linking..."
	@echo "$(CC) $^ -o $(TARGET) $(LIB)"
	$(CC) -o $(TARGET)  $^ $(LDFLAGS) 
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "$(CC) $(CFLAGS) $(INC) -c -o $@ $<"
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
clean:
	@echo "cleaning...";
	@echo "$(RM) -r $(BUILDDIR) $(TARGET)";
	$(RM) -r $(BUILDDIR) $(TARGET)
.PHONY:clean
