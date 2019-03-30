SOURCES := api.h cpu_info_getter_linux.cpp
OBJECTS := cpu_info_getter_linux.o
CXX := g++
LINK := g++
LFLAGS        = -Wl,-rpath,/home/arsdever/qt5/qtbase/lib -shared -Wl,-soname,libcollector.so.1
DEFINES := -DAPI_LIB
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
FLAGS := -std=c++11 -Wall
DEL_FILE       = rm -f
MOVE          = mv -f
TARGET_DIR := ../../build/lib/
TARGET        = libinfo_getter_api.so.1.0.0
TARGET0       = libinfo_getter_api.so
TARGET1       = libinfo_getter_api.so.1
TARGET2       = libinfo_getter_api.so.1.0

.PHONY : first

first : $(OBJECTS)
	@test -d $(TARGET_DIR) || mkdir -p $(TARGET_DIR)
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS)
	-ln -s $(TARGET) $(TARGET0)
	-ln -s $(TARGET) $(TARGET1)
	-ln -s $(TARGET) $(TARGET2)
	-$(DEL_FILE) $(TARGET_DIR)$(TARGET)
	-$(MOVE) $(TARGET) $(TARGET_DIR)$(TARGET)
	-$(DEL_FILE) $(TARGET_DIR)$(TARGET0)
	-$(DEL_FILE) $(TARGET_DIR)$(TARGET1)
	-$(DEL_FILE) $(TARGET_DIR)$(TARGET2)
	-$(MOVE) $(TARGET0) $(TARGET_DIR)$(TARGET0)
	-$(MOVE) $(TARGET1) $(TARGET_DIR)$(TARGET1)
	-$(MOVE) $(TARGET2) $(TARGET_DIR)$(TARGET2)

cpu_info_getter_linux.o : api.h cpu_info_getter_linux.cpp
	$(CXX) -c $(CXXFLAGS) -o cpu_info_getter_linux.o cpu_info_getter_linux.cpp