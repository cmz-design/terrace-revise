TARGETS=graph_bm_weighted graph_bm graph_bm_no_update test_graphcontainer test_btree test_pma test_adj test_powlaw test_test

ifdef D
	DEBUG=-g -DDEBUG_MODE
	OPT=
else
	DEBUG=
	OPT=-Ofast -DNDEBUG
endif

ifdef NH
	ARCH=
else
	#ARCH=-msse4.2 -D__SSE4_2_
	ARCH=-march=native
endif

ifdef P
	PROFILE=-pg -no-pie # for bug in gprof.
endif

CXX = /home/caimz/OpenCilk-10.0.1-Linux/bin/clang++ -std=c++17
CC = /home/caimz/OpenCilk-10.0.1-Linux/bin/clang -std=gnu11
LD= /home/caimz/OpenCilk-10.0.1-Linux/bin/clang++ -std=c++17


SIM_INCLUDE=$(SNIPER)/include
SIM_LIB=$(SNIPER)/lib
LOC_INCLUDE=include
LOC_LIB=lib
LOC_SRC=src
OBJDIR=obj
SER=ser
CILK=1

CXXFLAGS += -Wall $(DEBUG) -g $(PROFILE) $(OPT) $(ARCH) -DOPENMP=$(OPENMP) -DCILK=$(CILK) -m64 -I. -I$(LOC_INCLUDE) -I$(SIM_INCLUDE)

CFLAGS += -Wall $(DEBUG) $(PROFILE) $(OPT) $(ARCH) -DOPENMP=$(OPENMP) -DCILK=$(CILK) -m64 -I. -I$(LOC_INCLUDE) -I$(SIM_INCLUDE)

LDFLAGS += $(DEBUG) $(PROFILE) $(OPT) -L$(LOC_LIB) -L$(SIM_LIB) -lm -lpthread -lssl -lcrypto 

OPENMP?=0
ifeq ($(OPENMP),1)
  CILK=0
else
  CILK?=0
endif

ifeq ($(CILK),1)
  CFLAGS += -fcilkplus
  CXXFLAGS += -fcilkplus
  LDFLAGS += -lcilkrts
endif

ifeq ($(OPENMP),1)
  CFLAGS += -fopenmp
  CXXFLAGS += -fopenmp
  LDFLAGS += -lomp
endif

LDFLAGS +="-Wl,-rpath,lib/"
#
# declaration of dependencies
#

all: graph_bm
tests:	test_graphcontainer test_btree test_pma test_adj test_powlaw test_test

# dependencies between programs and .o files
test_graphcontainer:		$(OBJDIR)/test_graphcontainer.o \
												$(OBJDIR)/util.o \
												$(OBJDIR)/PMA.o $(OBJDIR)/PMA_Lock.o \
												$(OBJDIR)/partitioned_counter.o

test_btree:							$(OBJDIR)/test_btree.o \
												$(OBJDIR)/util.o

test_pma:								$(OBJDIR)/test_pma.o \
												$(OBJDIR)/PMA.o $(OBJDIR)/PMA_Lock.o \
												$(OBJDIR)/util.o
test_adj:                                  $(OBJDIR)/test_adj.o \
	$(OBJDIR)/util.o $(OBJDIR)/adj.o

test_powlaw:                                  $(OBJDIR)/test_powlaw.o \
	$(OBJDIR)/util.o $(OBJDIR)/adj.o

test_test:                                  $(OBJDIR)/test_test.o \
	$(OBJDIR)/util.o $(OBJDIR)/adj.o $(OBJDIR)/tesseract.o

graph_bm:								$(OBJDIR)/graph_bm.o \
												$(OBJDIR)/util.o \
												$(OBJDIR)/PMA.o $(OBJDIR)/PMA_Lock.o \
												$(OBJDIR)/partitioned_counter.o

graph_bm_no_update:			$(OBJDIR)/graph_bm_no_update.o \
												$(OBJDIR)/util.o \
												$(OBJDIR)/PMA.o $(OBJDIR)/PMA_Lock.o \
												$(OBJDIR)/partitioned_counter.o


graph_bm_weighted:			$(OBJDIR)/graph_bm_weighted.o \
												$(OBJDIR)/util.o \
												$(OBJDIR)/PMA.o $(OBJDIR)/PMA_Lock.o \
												$(OBJDIR)/partitioned_counter.o


# dependencies between .o files and .cc (or .c) files
$(OBJDIR)/test_graphcontainer.o: 	$(LOC_SRC)/test_graphcontainer.cc \
																	$(LOC_INCLUDE)/graph.h \
																	$(LOC_INCLUDE)/btree.h \
																	$(LOC_INCLUDE)/util.h \
																	$(LOC_INCLUDE)/PMA.hpp \
																	$(LOC_INCLUDE)/PMA_Lock.hpp \
																	$(LOC_INCLUDE)/partitioned_counter.h

$(OBJDIR)/test_btree.o: 					$(LOC_SRC)/test_btree.cc \
																	$(LOC_INCLUDE)/btree.h \
																	$(LOC_INCLUDE)/util.h

$(OBJDIR)/test_pma.o: 					$(LOC_SRC)/test_pma.cc \
																$(LOC_INCLUDE)/PMA.hpp

$(OBJDIR)/test_adj.o: 					$(LOC_SRC)/test_adj.cc \
																$(LOC_INCLUDE)/adj.h \
																	$(LOC_INCLUDE)/util.h

$(OBJDIR)/test_powlaw.o: 					$(LOC_SRC)/test_powlaw.cc \
																$(LOC_INCLUDE)/adj.h \
																	$(LOC_INCLUDE)/util.h

$(OBJDIR)/test_test.o: 					$(LOC_SRC)/test_test.cc \
																$(LOC_INCLUDE)/adj.h \
																	$(LOC_INCLUDE)/util.h

$(OBJDIR)/graph_bm.o: 						$(LOC_SRC)/graph_bm.cc \
																	$(LOC_INCLUDE)/graph.h \
																	$(LOC_INCLUDE)/util.h \
																	$(LOC_INCLUDE)/PMA.hpp \
																	$(LOC_INCLUDE)/partitioned_counter.h

$(OBJDIR)/graph_bm_no_update.o: 	$(LOC_SRC)/graph_bm_no_update.cc \
																	$(LOC_INCLUDE)/graph.h \
																	$(LOC_INCLUDE)/util.h \
																	$(LOC_INCLUDE)/PMA.hpp \
																	$(LOC_INCLUDE)/partitioned_counter.h


$(OBJDIR)/graph_bm_weighted.o: 		$(LOC_SRC)/graph_bm_weighted.cc \
																	$(LOC_INCLUDE)/graph.h \
																	$(LOC_INCLUDE)/util.h \
																	$(LOC_INCLUDE)/PMA.hpp \
																	$(LOC_INCLUDE)/partitioned_counter.h

$(OBJDIR)/gqf.o: 					$(LOC_SRC)/gqf/gqf.c $(LOC_INCLUDE)/gqf/gqf.h
$(OBJDIR)/gqf_file.o: 		$(LOC_SRC)/gqf/gqf_file.c $(LOC_INCLUDE)/gqf/gqf_file.h
$(OBJDIR)/hashutil.o: 		$(LOC_SRC)/gqf/hashutil.c $(LOC_INCLUDE)/gqf/hashutil.h
$(OBJDIR)/rank_select.o: 	$(LOC_SRC)/gqf/rank_select.c $(LOC_INCLUDE)/gqf/rank_select.h
$(OBJDIR)/PMA.o: 					$(LOC_SRC)/PMA.cc $(LOC_INCLUDE)/PMA.hpp
$(OBJDIR)/adj.o: 					$(LOC_SRC)/adj.cc $(LOC_INCLUDE)/adj.h
$(OBJDIR)/tesseract.o: 					$(LOC_SRC)/tesseract.cc $(LOC_INCLUDE)/tesseract.h
$(OBJDIR)/PMA_Lock.o: 		$(LOC_SRC)/PMA_Lock.cc $(LOC_INCLUDE)/PMA_Lock.hpp

#
# generic build rules
#

$(TARGETS):
	$(LD) $^ $(LDFLAGS) -o $@ 

$(OBJDIR)/%.o: $(LOC_SRC)/%.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

$(OBJDIR)/%.o: $(LOC_SRC)/%.c | $(OBJDIR)
	$(CXX) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(OBJDIR)/%.o: $(LOC_SRC)/gqf/%.c | $(OBJDIR)
	$(CXX) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(SER):
	@mkdir -p $(SER)

clean:
	rm -rf $(OBJDIR) core $(TARGETS)
