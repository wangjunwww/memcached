#IDIR =$()../include
#CC=gcc
#CFLAGS=-I$(IDIR)
#
#ODIR=obj
#LDIR =../lib
#
#_DEPS = hellomake.h
#DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
#
#_OBJ = hellomake.o hellofunc.o 
#OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
#
#
#$(ODIR)/%.o: %.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)
#
#hellomake: $(OBJ)
#	gcc -o $@ $^ $(CFLAGS)
#
#clean:
#	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
INCS=\
	-I$(AB_ROOT)/common/include

CFLAGS=\
	-g  \
	-lmemcached \
	$(INCS) \

TEST_SRCS=\
	test.c 
SIMPLE_SRCS=\
	simple_test.c

LIB_COMMON=$(AB_ROOT)/common/lib/

LIB_COMMON_OBJS=\
	$(BUILD)/timer.o

TEST_OBJS=\
	$(patsubst %, $(BUILD)/%, $(patsubst %.c, %.o, $(TEST_SRCS))) $(LIB_COMMON_OBJS)
SIMPLE_TEST_OBJS=\
	$(patsubst %, $(BUILD)/%, $(patsubst %.c, %.o, $(SIMPLE_SRCS))) $(LIB_COMMON_OBJS)

####varibles to fit in the make.in template

TARGETS=\
	test simple_test

DEP_SRCS=\
	$(TEST_SRCS) \
	$(SIMPLE_SRCS)

MODULES=\
	$(LIB_COMMON) \

MODULE_OBJS=\
	$(LIB_COMMON_OBJS) \


################## include
include $(AB_ROOT)/common/make.in



######################

#defined in the included template
#all:$(BUILD_TARGETS) $(BUILD)


$(BUILD)/test:$(TEST_OBJS)
	$(CC) $^ -o $@

$(BUILD)/simple_test:$(SIMPLE_TEST_OBJS) 
	$(CC) $^ -o $@

test:$(BUILD) $(BUILD)/test
	
simple_test:$(BUILD) $(BUILD)/simple_test

