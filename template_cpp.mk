NAME ?= $(shell basename $$(pwd))

SRCS_DIR ?= .

OBJS_DIR ?= obj

INCLUDE_DIR ?= .

SRCS ?= $(wildcard *.cpp)

OBJS_DIR_TREE := $(sort $(addprefix ${OBJS_DIR}/, $(dir ${SRCS})))

LIB_ARG = $(foreach path, $(LIBS), -L $(dir $(path)) -l $(notdir $(path)))

INCLUDE_ARG = $(addprefix -I ,${INCLUDE_DIR}) $(addprefix -I, $(dir ${LIBS}))

OBJS := $(SRCS:%.cpp=${OBJS_DIR}/%.o)

IDEPS := $(SRCS:%.cpp=${OBJS_DIR}/%.d)

CXXFLAGS ?= -Wall -Werror -Wextra

all: $(NAME)

$(NAME): ${OBJS_DIR} ${OBJS_DIR_TREE} $(OBJS) $(MAKE_DEP)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIB_ARG) $(LDFLAGS)

${OBJS_DIR}:
	mkdir -p $@
	[ -f .gitignore ] && (grep '^'"$@"'$$' .gitignore || echo "$@" >> .gitignore) || true

${OBJS_DIR_TREE}:
	mkdir -p $@

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.cpp
	$(CXX) $(CXXFLAGS) ${INCLUDE_ARG} -MMD -c $< -o $@

$(MAKE_DEP):
	make -C $(dir $@) $(notdir $@)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean all fclean re

.PRECIOUS: ${OBJS_DIR}/%.d

-include ${IDEPS}
