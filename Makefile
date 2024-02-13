TARGET			= yellow-snow
BUILD_DIR		= .build
SRC_DIR         ?= src

CC				?= gcc
CFLAGS_BASE		= -std=c99 $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf SDL2_mixer)
CFLAGS_RELEASE	= -O2
LDLIBS_BASE		= $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)


ifeq ($(OS),Windows_NT)
	CFLAGS_DEV	= -O0 -ggdb3 -Wall -Wextra -Werror -Wpedantic -Wwrite-strings -Wconversion \
				-Wshadow -Wmissing-prototypes -Wmissing-declarations -Wfloat-equal \
				-Wsign-compare -Wundef -Wcast-align -Wstrict-prototypes -Wswitch-default \
				-Wold-style-definition -Wmissing-include-dirs 
	LDLIBS_DEV	=
	CLEAN		= del /f $(TARGET).exe & if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
else
	CFLAGS_DEV	= -O0 -ggdb3 -Wall -Wextra -Werror -Wpedantic -Wwrite-strings -Wconversion \
				-Wshadow -Wmissing-prototypes -Wmissing-declarations -Wfloat-equal \
				-Wsign-compare -Wundef -Wcast-align -Wstrict-prototypes -Wswitch-default \
				-Wold-style-definition -Wmissing-include-dirs -fsanitize=address \
				-fsanitize-address-use-after-scope
	LDLIBS_DEV	= -fsanitize=address -fsanitize-address-use-after-scope
	CLEAN		= $(RM) -r $(BUILD_DIR) $(TARGET)
endif

CFLAGS			?= $(CFLAGS_BASE) $(CFLAGS_DEV)
LDFLAGS			?=
LDLIBS			?= $(LDLIBS_BASE) $(LDLIBS_DEV)
SRCS			= $(wildcard $(SRC_DIR)/*.c)
OBJS			= $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.c=.o)))

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

.PHONY: all release run clean

all: $(TARGET)

release: CFLAGS	= $(CFLAGS_BASE) $(CFLAGS_RELEASE)
release: LDLIBS	= $(LDLIBS_BASE)
release: all

run: $(TARGET)
	./$<

clean:
	$(CLEAN)

rebuild: clean all
