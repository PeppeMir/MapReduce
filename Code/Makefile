CFLAGS=-std=c++11 -O3 -Wall -pedantic
LIBS=-pthread
OBJ=MR
PRINT_MACRO=-DVERBOSE
REPORT_MACRO=-DREPORT
CHECK_MACRO=-DCHECK
XEON_MACRO=-DXEON
PHI_MACRO=-DPHI
FILEDIR=./generated/

STD_RULE=g++ $(CFLAGS) $(LIBS)
XEON_RULE=icc $(CFLAGS) $(LIBS) $(XEON_MACRO)
PHI_RULE=icc $(CFLAGS) $(LIBS) $(PHI_MACRO) -mmic

FILE_SUFFIX=*.cpp -o $(OBJ)

generatefile:
ifndef SIZE
	$(error SIZE is not set)
endif
	clear
	cd FileGen && make -f ../Makefile dowork

dowork:
	if [ ! -d $(FILEDIR) ]; then mkdir $(FILEDIR); fi
	g++ $(CFLAGS) $(LIBS) *.cpp -o RandomFileGenerator
	./RandomFileGenerator $(FILEDIR) $(SIZE)

compilemic:
	$(PHI_RULE) $(FILE_SUFFIX) 

compilemicverbose:
	$(PHI_RULE) $(PRINT_MACRO) $(FILE_SUFFIX)

compilemicreport:
	$(PHI_RULE) $(REPORT_MACRO) $(FILE_SUFFIX)

compilemiccheck:
	$(PHI_RULE) $(PRINT_MACRO) $(CHECK_MACRO) $(FILE_SUFFIX)

compilexeon:
	$(XEON_RULE) $(FILE_SUFFIX)
	
compilexeonverbose:
	$(XEON_RULE) $(PRINT_MACRO) $(FILE_SUFFIX)

compilexeonreport:
	$(XEON_RULE) $(REPORT_MACRO) $(FILE_SUFFIX)

compilexeoncheck:
	$(XEON_RULE) $(PRINT_MACRO) $(CHECK_MACRO) $(FILE_SUFFIX)
	
compilestandard:
	$(STD_RULE) $(FILE_SUFFIX)

compilestandardverbose:
	$(STD_RULE) $(PRINT_MACRO) $(FILE_SUFFIX)

compilestandardreport:
	$(STD_RULE) $(REPORT_MACRO) $(FILE_SUFFIX)

compilestandardcheck:
	$(STD_RULE) $(PRINT_MACRO) $(CHECK_MACRO) $(FILE_SUFFIX)

copymic0:
	ssh mic0 'mkdir Code; cd Code; mkdir FileGen; cd FileGen; mkdir generated'
	scp -r ../script mic0:
	scp $(OBJ) mic0:Code/
	scp repo_gen.sh mic0:Code/
	@echo "Files copied on mic0."

copymic1:
	ssh mic1 'mkdir Code; cd Code; mkdir FileGen; cd FileGen; mkdir generated'
	scp -r ../script mic1:
	scp $(OBJ) mic1:Code/
	scp repo_gen.sh mic1:Code/
	@echo "Files copied on mic1."

downloadreport:
	sshpass -p "pipposega90" scp spm1410@131.114.137.225:repo_file.csv ~/Desktop/
	
clean:
	clear
	rm -rf *.o $(OBJ)
	
cleanmic0:
	ssh mic0 'rm -rf *'
	
cleanmic1:
	ssh mic1 'rm -rf *'
