# This Makefile will be used to build your compiler.

CC = g++
CCFLAGS = -lfl -std=c++11
all:    vsopc

.PHONY: all archive clean install-tools

# Your compiler
vsopc:clean
	flex -o vsop.yy.cpp vsop.lex 
	bison  -d vsop.y -b vsop
	$(CC)  *.c* nodes/*.cpp semantic/*.cpp vsopl/*.cpp -o vsopc  $(CCFLAGS)



# Should clean built files, and keep only source code. Adapt to your language
# and folder structure. If using git, you might want to use `git clean`.
clean:
	rm -f vsopc *~ *.bak *.o *.class *.pyc *.yy.cpp

# First rule called when in a new VM. Use this target to install any software
# tools or libraries needed to build your compiler. e.g. assuming a C++ project
# using flex and bison, you might do something along the lines of:
#    sudo DEBIAN_FRONTEND=noninteractive apt-get -qq install clang flex bison
# Remember to use `sudo` if you need administrator privileges.
# Obviously, `make` is already installed. It is also the case of `llvm-dev`.
install-tools:
	sudo DEBIAN_FRONTEND=noninteractive apt-get -qq install flex bison

#Makes the documentation of the code.
#You need doxygen installed on your machine
#This script also launches the main html in Mozilla Firefox
doc:
	$ doxygen doxygen/Doxyfile
	$ firefox doxygen/html/index.html

# This target will build a new archive of this cleaned folder in its parent
# directory. You can use it to generate an archive to be submitted for
# evaluation.
archive:    clean
	cd ..; tar cvJf vsopcompiler.tar.xz --exclude=.git --exclude=.svn --exclude=.svn-base vsopcompiler
make_archive: clean
	mkdir vsopcompiler
	mkdir vsopcompiler/nodes
	mkdir vsopcompiler/semantic	
	mkdir vsopcompiler/vsopl
	mkdir vsopcompiler/report
	cp report/report.pdf vsopcompiler/report/
	cp *.cpp  vsopcompiler/
	cp *.hpp  vsopcompiler/
	cp Makefile vsopcompiler/
	cp *.lex vsopcompiler/
	cp *.y vsopcompiler/
	cp nodes/* vsopcompiler/nodes/
	cp semantic/* vsopcompiler/semantic/
	cp vsopl/* vsopcompiler/vsopl/
	tar -cJf vsopcompiler.tar.xz vsopcompiler
	rm -r vsopcompiler
