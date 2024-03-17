TARGET_EXEC := qwerty-test
BUILD_DIR := build

SOURCES := $(wildcard src/*.c)
OBJECTS := $(addprefix ${BUILD_DIR}/, $(SOURCES:.c=.o))
DEPENDENCIES := $(OBJECTS:.o=.d)

CFLAGS := -g -O0
CPPFLAGS := $(addprefix -I, include) -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

all: $(BUILD_DIR)/$(TARGET_EXEC)

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPENDENCIES)
