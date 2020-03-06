SRC_C = $(filter %.c, $(CC_SRC))
SRC_CPP = $(filter %.cpp, $(CC_SRC))
CC_OBJ = $(addprefix $(BUILD_PATH)/c/, $(SRC_C:.c=.o)) \
	 $(addprefix $(BUILD_PATH)/cpp/, $(SRC_CPP:.cpp=.o))

all : $(CC_OBJ)
	@echo $(CC_SRC)
	-@echo linking: $(LD_BIN)
	$(LN) -o $(LD_BIN) $(CC_OBJ) $(LD_INC) $(LD_LIB)

$(BUILD_PATH)/c/%.o : %.c
	@mkdir -p $(BUILD_PATH)/c/$(dir $<)
	-@echo "> $<";
	@$(CC) $(FLAG_CC) $(CC_INC) -o $@ -c $<

$(BUILD_PATH)/cpp/%.o : %.cpp
	@mkdir -p $(BUILD_PATH)/cpp/$(dir $<)
	-@echo "> $<";
	@$(CC) $(FLAG_CC) $(CC_INC) -o $@ -c $<

clean:
	-@rm -f -r $(BUILD_PATH)
	-@rm -f -r $(LD_BIN)
