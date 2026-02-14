SOURCE_DIR  = src
BUILD_DIR   = build
C_SOURCES   = $(wildcard $(SOURCE_DIR)/*.c) $(wildcard $(SOURCE_DIR)/states/*.c)
OBJECTS   	= $(patsubst %.c, $(BUILD_DIR)/%.o, $(C_SOURCES))

CFLAGS  = -pipe -fno-math-errno -Werror -Wno-error=missing-braces -Wno-error=strict-aliasing -I$(SOURCE_DIR)
LDFLAGS = -g
TARGET   = $(BUILD_DIR)/inscryption.exe

CC = gcc

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) -mconsole -lgdi32
	mkdir -p $(BUILD_DIR)/resources
	cp resources/*.pak $(BUILD_DIR)/resources
	@echo "-----------------------------"
	@echo "Binary size:"
	@wc -c < $@
	@echo "Total with resources:"
	@echo $$(( $$(wc -c < $@) + $$(wc -c resources/*.pak | tail -1 | awk '{print $$1}') ))
	@echo "-----------------------------"

$(BUILD_DIR)/%.o : %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS)