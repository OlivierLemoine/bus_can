# This file is generated by gyp; do not edit.

TOOLSET := target
TARGET := can_lib
DEFS_Debug := \
	'-DNODE_GYP_MODULE_NAME=can_lib' \
	'-DUSING_UV_SHARED=1' \
	'-DUSING_V8_SHARED=1' \
	'-DV8_DEPRECATION_WARNINGS=1' \
	'-D_LARGEFILE_SOURCE' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DBUILDING_NODE_EXTENSION' \
	'-DDEBUG' \
	'-D_DEBUG' \
	'-DV8_ENABLE_CHECKS'

# Flags passed to all source files.
CFLAGS_Debug := \
	-fPIC \
	-pthread \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-m64 \
	-g \
	-O0

# Flags passed to only C files.
CFLAGS_C_Debug :=

# Flags passed to only C++ files.
CFLAGS_CC_Debug := \
	-fno-rtti \
	-fno-exceptions \
	-std=gnu++0x

INCS_Debug := \
	-I/home/e2015/o5lemoin/.node-gyp/9.6.1/include/node \
	-I/home/e2015/o5lemoin/.node-gyp/9.6.1/src \
	-I/home/e2015/o5lemoin/.node-gyp/9.6.1/deps/uv/include \
	-I/home/e2015/o5lemoin/.node-gyp/9.6.1/deps/v8/include

DEFS_Release := \
	'-DNODE_GYP_MODULE_NAME=can_lib' \
	'-DUSING_UV_SHARED=1' \
	'-DUSING_V8_SHARED=1' \
	'-DV8_DEPRECATION_WARNINGS=1' \
	'-D_LARGEFILE_SOURCE' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DBUILDING_NODE_EXTENSION'

# Flags passed to all source files.
CFLAGS_Release := \
	-fPIC \
	-pthread \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-m64 \
	-O3 \
	-fno-omit-frame-pointer

# Flags passed to only C files.
CFLAGS_C_Release :=

# Flags passed to only C++ files.
CFLAGS_CC_Release := \
	-fno-rtti \
	-fno-exceptions \
	-std=gnu++0x

INCS_Release := \
	-I/home/e2015/o5lemoin/.node-gyp/9.6.1/include/node \
	-I/home/e2015/o5lemoin/.node-gyp/9.6.1/src \
	-I/home/e2015/o5lemoin/.node-gyp/9.6.1/deps/uv/include \
	-I/home/e2015/o5lemoin/.node-gyp/9.6.1/deps/v8/include

OBJS := \
	$(obj).target/$(TARGET)/main.o

# Add to the list of files we specially track dependencies for.
all_deps += $(OBJS)

# CFLAGS et al overrides must be target-local.
# See "Target-specific Variable Values" in the GNU Make manual.
$(OBJS): TOOLSET := $(TOOLSET)
$(OBJS): GYP_CFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_C_$(BUILDTYPE))
$(OBJS): GYP_CXXFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_CC_$(BUILDTYPE))

# Suffix rules, putting all outputs into $(obj).

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(srcdir)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# Try building from generated source, too.

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj).$(TOOLSET)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# End of this set of suffix rules
### Rules for final target.
LDFLAGS_Debug := \
	-pthread \
	-rdynamic \
	-m64

LDFLAGS_Release := \
	-pthread \
	-rdynamic \
	-m64

LIBS :=

$(obj).target/can_lib.node: GYP_LDFLAGS := $(LDFLAGS_$(BUILDTYPE))
$(obj).target/can_lib.node: LIBS := $(LIBS)
$(obj).target/can_lib.node: TOOLSET := $(TOOLSET)
$(obj).target/can_lib.node: $(OBJS) FORCE_DO_CMD
	$(call do_cmd,solink_module)

all_deps += $(obj).target/can_lib.node
# Add target alias
.PHONY: can_lib
can_lib: $(builddir)/can_lib.node

# Copy this to the executable output path.
$(builddir)/can_lib.node: TOOLSET := $(TOOLSET)
$(builddir)/can_lib.node: $(obj).target/can_lib.node FORCE_DO_CMD
	$(call do_cmd,copy)

all_deps += $(builddir)/can_lib.node
# Short alias for building this executable.
.PHONY: can_lib.node
can_lib.node: $(obj).target/can_lib.node $(builddir)/can_lib.node

# Add executable to "all" target.
.PHONY: all
all: $(builddir)/can_lib.node

