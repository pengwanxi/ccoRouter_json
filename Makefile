# app macros
APP ?= FrpcCtl
RELEASE = 1.0
PLATFORM = linux/arm/v7

# tool macros
CC = arm-linux-gnueabihf-g++
CXX = arm-linux-gnueabihf-gcc

#CC = g++
#CXX = gcc

CFLAGS := -std=c++11
CXXFLAGS := # FILL: compile flags
DBGFLAGS := -g -O0 -Wall 
COBJFLAGS := $(CFLAGS) -c

INCLUDE_DIR  := common

LDFLAGS = -L./lib -lzlog -lmosquitto -lssl -lcrypto  -pthread

#LDFLAGS = -L./lib -lmosquitto -lz -lssl -lcrypto -lcares -lpthread -lstdc++ -lzlog 
INCLUDES     := -Icommon -Icco -Ilist -Igw1376.2
# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := common
SRC_PATH1 := gw1376.2
SRC_PATH3 := cco
SRC_PATH4 :=.
SRC_PATH5 := list
DBG_PATH := debug

# compile macros
TARGET_NAME := ccoRouter
ifeq ($(OS),Windows_NT)
	TARGET_NAME := $(addsuffix .exe,$(TARGET_NAME))
endif
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)

# src files & obj files
#SRC := ./main.cpp 
SRC += $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
SRC += $(foreach x, $(SRC_PATH1), $(wildcard $(addprefix $(x)/*,.c*)))
SRC += $(foreach x, $(SRC_PATH2), $(wildcard $(addprefix $(x)/*,.c*)))
SRC += $(foreach x, $(SRC_PATH3), $(wildcard $(addprefix $(x)/*,.c*)))
SRC += $(foreach x, $(SRC_PATH4), $(wildcard $(addprefix $(x)/*,.c*)))
SRC += $(foreach x, $(SRC_PATH5), $(wildcard $(addprefix $(x)/*,.c*)))


OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
DISTCLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG)
CLEAN_LIST := $(TARGET) \
			  $(TARGET_DEBUG) \
			  $(DISTCLEAN_LIST)

# default rule
default: makedir all

# non-phony targets
$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LDFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(COBJFLAGS) $(INCLUDES) -o $@ $<

$(OBJ_PATH)/%.o: $(SRC_PATH1)/%.c*
	$(CC) $(COBJFLAGS) $(INCLUDES) -o $@ $<

$(OBJ_PATH)/%.o: $(SRC_PATH2)/%.c*
	$(CC) $(COBJFLAGS) $(INCLUDES) -o $@ $<

$(OBJ_PATH)/%.o: $(SRC_PATH3)/%.c*
	$(CC) $(COBJFLAGS) $(INCLUDES) -o $@ $<

$(OBJ_PATH)/%.o: $(SRC_PATH4)/%.c*
	$(CC) $(COBJFLAGS) $(INCLUDES) -o $@ $<		

$(OBJ_PATH)/%.o: $(SRC_PATH5)/%.c*
	$(CC) $(COBJFLAGS) $(INCLUDES) -o $@ $<		

$(DBG_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(COBJFLAGS) $(DBGFLAGS) $(INCLUDES) -o $@ $<

$(DBG_PATH)/%.o: $(SRC_PATH1)/%.c*
	$(CC) $(COBJFLAGS) $(DBGFLAGS) $(INCLUDES) -o $@ $<

$(DBG_PATH)/%.o: $(SRC_PATH2)/%.c*
	$(CC) $(COBJFLAGS) $(DBGFLAGS) $(INCLUDES) -o $@ $<

$(DBG_PATH)/%.o: $(SRC_PATH3)/%.c*
	$(CC) $(COBJFLAGS) $(DBGFLAGS) $(INCLUDES) -o $@ $<

$(DBG_PATH)/%.o: $(SRC_PATH4)/%.c*
	$(CC) $(COBJFLAGS) $(DBGFLAGS) $(INCLUDES) -o $@ $<

$(DBG_PATH)/%.o: $(SRC_PATH5)/%.c*
	$(CC) $(COBJFLAGS) $(DBGFLAGS) $(INCLUDES) -o $@ $<

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBGFLAGS) $(OBJ_DEBUG) -o $@

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY:echo
echo:
	@echo src $(SRC)
	@echo obj $(OBJ)  
	@echo TARGET $(TARGET)  
	@echo OS $(OS)

.PHONY:cp
cp:
	expect ./cpbin.sh

.PHONY:container
container: all
	docker image rm $(APP):$(RELEASE) || true
#	docker buildx build --platform $(PLATFORM) -t $(APP):$(RELEASE) --load .
	docker build -t $(APP):$(RELEASE) .

.PHONY:push
push: container
	docker save -o $(APP)$(RELEASE).tar $(APP):$(RELEASE) 
	

