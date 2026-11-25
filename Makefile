export NPROC ?= $(shell (nproc))
export NJOB  ?= $(shell expr '(' $(NPROC) + 1 ')')

.PHONY: default
default: release/all

# C/C++ standard
STD_FLAGS :=
# Parsing --stdc_<ver> and --stdcxx_<ver> from command line
# $(1) - language suffix (c, cxx); $(2) - cmake option name; $(3) - env var name (C_STD, CXX_STD)
define check_standard
STD_CMD_$(1) := $$(filter --std$(1)_%,$$(MAKECMDGOALS))
ifneq ($$(STD_CMD_$(1)),)
    $$(eval $$(STD_CMD_$(1)):;@:)
endif

ifdef $(3)
    STD_FLAGS += -D$(2)=$($(3))
else ifneq ($$(STD_CMD_$(1)),)
    STD_VALUE_$(1) := $$(patsubst --std$(1)_%,%,$$(STD_CMD_$(1)))
    STD_FLAGS += -D$(2)=$$(STD_VALUE_$(1))
else
    STD_FLAGS += -D$(2)=default
endif
endef

$(eval $(call check_standard,c,PROJECT_C_STANDARD,C_STD))
$(eval $(call check_standard,cxx,PROJECT_CXX_STANDARD,CXX_STD))

# Sanitizers
SANITIZER_FLAGS :=
# Sanitizer processing template
# $(1) — command line argument; $(2) — cmake option name
define check_sanitizer
ifeq ($$(filter $(1),$$(MAKECMDGOALS)),$(1))
    SANITIZER_FLAGS += -D$(2)=ON
    # Hide flag for make
    $$(eval $(1):;@:)
else
    SANITIZER_FLAGS += -D$(2)=OFF
endif
endef

$(eval $(call check_sanitizer,--sanitizer_addr,USE_ADDR_SANITIZER))
$(eval $(call check_sanitizer,--sanitizer_leak,USE_LEAK_SANITIZER))
$(eval $(call check_sanitizer,--sanitizer_ub,USE_BEHAVIOR_SANITIZER))
$(eval $(call check_sanitizer,--sanitizer_thread,USE_THREAD_SANITIZER))

# Building extra flags for cmake
CMAKE_EXPORT_COMPILE_COMMANDS_FLAG := -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
CMAKE_EXTRA_FLAGS := $(CMAKE_EXPORT_COMPILE_COMMANDS_FLAG) $(STD_FLAGS) $(SANITIZER_FLAGS)

# Base rule declaration
define base_rule

.PHONY: $(1)/configure
$(1)/configure: build_$(1)/Makefile

$(1)/%: $(1)/configure
	@make -j $(NJOB) --output-sync=target --no-print-directory -C build_$(1) $$*

.PHONY: $(1)
$(1): $(1)/all

.PHONY: $(1)/clean
$(1)/clean: check_gitignore
	@rm -rf build_$(1)

.PHONY: $(1)/rebuild
$(1)/rebuild: $(1)/clean $(1)/all

.PHONY: $(1)/help
$(1)/help: $(1)/configure
	@echo "Usage: make <command> [profile/<target>] [VAR=value ...] [-- <options>]"
	@echo ""
	@echo "Available profiles:"
	@echo "  release           Build with optimizations (Default)"
	@echo "  debug             Build with debug symbols"
	@echo "  coverage          Build with code coverage analysis"
	@echo ""
	@echo "Commands:"
	@echo "  all               Build all targets for profile (the default if no target is provided)"
	@echo "  configure         Generate cmake files for profile"
	@echo "  clean             Remove build directory for profile"
	@echo "  rebuild           Clean and rebuild everything for profile"
	@echo "  test              Build and run tests for profile"
	@echo "  coverage          Build and run coverage analysis and generate report"
	@echo "  help              Display this help message"
	@echo ""
	@echo "Variables:"
	@echo "  C_STD=<ver>       Set C standard version (e.g., 99, 11, 17, 23, default)"
	@echo "  CXX_STD=<ver>     Set C++ standard version (e.g., 14, 17, 20, 23, default)"
	@echo ""
	@echo "Options (passed after '--'):"
	@echo "  --stdc_<ver>        Set C standard version (e.g., 99, 11, 17, 23, default)"
	@echo "  --stdcxx_<ver>      Set C++ standard version (e.g., 14, 17, 20, 23, default)"
	@echo "  --sanitizer_addr    Enable AddressSanitizer (ASan) for memory errors"
	@echo "  --sanitizer_leak    Enable LeakSanitizer (LSan) for memory leaks"
	@echo "  --sanitizer_ub      Enable UndefinedBehaviorSanitizer (UBSan)"
	@echo "  --sanitizer_thread  Enable ThreadSanitizer (TSan) for data races"
	@echo ""
	@echo "List of all cmake targets:"
	@make --no-print-directory -C build_$(1) help

.PHONY: $(1)/test
$(1)/test: $(1)/all
	@make -j $(NJOB) -C build_$(1) test

# Dynamic import of cmake targets for auto completion.
$(if $(wildcard build_$(1)/Makefile),
    $(eval _RAW_TARGETS := $(shell $(MAKE) --no-print-directory -C build_$(1) help 2>/dev/null | awk '/^\.\.\./ {print $$2}'))
    $(foreach t,$(_RAW_TARGETS),
        $(if $(filter-out configure clean rebuild help test,$(t)),
            $(eval .PHONY: $(1)/$(t))
            $(eval $(1)/$(t): build_$(1)/Makefile ; @\$$(MAKE) -j \$$(NJOB) --output-sync=target --no-print-directory -C build_$(1) $(t))
        )
    )
)

endef

# Common rule declaration
define common_rule

build_$(1)/Makefile: check_gitignore
	@mkdir -p $$(@D) && \
	touch $$(@D)/.kdev_ignore && \
	cd $$(@D) && \
	cmake -DCMAKE_BUILD_TYPE="$(1)" $$(CMAKE_EXTRA_FLAGS) ../

$$(eval $$(call base_rule,$(1)))

endef

# Coverage rule declaration
define coverage_rule

build_$(1)/Makefile: check_gitignore
	@mkdir -p $$(@D) && \
	touch $$(@D)/.kdev_ignore && \
	cd $$(@D) && \
	cmake -DCMAKE_BUILD_TYPE="debug" -DCOVERAGE_BUILD=ON -DBUILD_TESTS=ON $$(CMAKE_EXTRA_FLAGS) ../

$$(eval $$(call base_rule,$(1)))

endef

# Debug and release build types definition
$(eval $(call common_rule,debug))
$(eval $(call common_rule,release))
$(eval $(call coverage_rule,coverage))

.PHONY: all
all: release/all

.PHONY: clean
clean: release/clean

.PHONY: configure
configure: release/configure

.PHONY: rebuild
rebuild: release/clean release/all

.PHONY: help
help: release/help

.PHONY: coverage
coverage: coverage/all coverage/coverage

#.PHONY: coverage
#coverage: coverage/coverage

.PHONY: check_gitignore
check_gitignore:
	@if [ -f .gitignore ]; then \
		if ! grep -Fxq "build*/" .gitignore; then \
			echo "Updating .gitignore with build and IDE paths..."; \
			(echo "# Build generated"; \
			 echo "build*/"; \
			 echo ".kdev4/"; \
			 echo "*.kdev4"; \
			 echo ""; \
			 cat .gitignore) > .gitignore.tmp && mv .gitignore.tmp .gitignore; \
		fi; \
		if grep -Fxq "Makefile" .gitignore; then \
			echo "Removing Makefile from .gitignore..."; \
			sed -i.tmp '/^Makefile$$/d' .gitignore && rm -f .gitignore.tmp; \
		fi \
	fi

.PHONY: test
test: release/test
