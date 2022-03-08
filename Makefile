

#Compiler and Linker
CC          = gcc

#The Target Binary Program
TARGET      := cgp_mig

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
LIBDIR      := lib
BUILDDIR    := obj
TARGETDIR   := bin

#Flags, Libraries and Includes
CFLAGS      := -std=c11 -pedantic -Wno-pedantic-ms-format -Wno-unused-function -Wall -O3 -fopenmp -D NO_DLL

CGP_LIB 	:= $(LIBDIR)/cgp_lib
CGP_DATA    := cgp_data_creation
COMB_CIRC   := combination_circuits

##########################################################################

all: clean linking subdirs

linking: $(TARGETDIR)/$(TARGET).exe

run: linking
	@./$(TARGETDIR)/$(TARGET).exe

dataset: $(TARGETDIR)/$(CGP_DATA).exe
	@./$<

$(TARGETDIR)/$(TARGET).exe: $(BUILDDIR)/$(TARGET).o $(BUILDDIR)/cgp_lib.o
	$(CC) $(CFLAGS) $^ -o $@


$(TARGETDIR)/$(CGP_DATA).exe: $(BUILDDIR)/$(CGP_DATA).o $(BUILDDIR)/cgp_lib.o $(BUILDDIR)/$(COMB_CIRC).o
	$(CC) $(CFLAGS) $^ -o $@

##########################################################################
# Object files

#CGP_MIG
$(BUILDDIR)/$(TARGET).o: $(SRCDIR)/$(TARGET).c $(SRCDIR)/$(TARGET).h 
	$(CC) $(CFLAGS) -c $< -o $@

#CGP_LIB
$(BUILDDIR)/cgp_lib.o: $(CGP_LIB)/cgp.c $(CGP_LIB)/cgp.h
	$(CC) $(CFLAGS) -c $< -o $@

#CGP_DATA
$(BUILDDIR)/$(CGP_DATA).o: $(SRCDIR)/$(CGP_DATA).c $(SRCDIR)/$(CGP_DATA).h 
	$(CC) $(CFLAGS) -c $< -o $@

#COMB_CIRC
$(BUILDDIR)/$(COMB_CIRC).o: $(SRCDIR)/$(COMB_CIRC).c $(SRCDIR)/$(COMB_CIRC).h 
	$(CC) $(CFLAGS) -c $< -o $@

##########################################################################


.PHONY: subdirs clean

subdirs:
	-@mkdir $(TARGETDIR) $(BUILDDIR) || : 
	
clean:
	-@del /q /s $(TARGETDIR)
	-@del /q /s $(BUILDDIR)