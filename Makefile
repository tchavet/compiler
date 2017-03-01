# This Makefile will be used to build your compiler.

CC = g++
CCFLAGS = -lfl
all:    vsopc

.PHONY: all archive clean install-tools

# Your compiler
vsopc:clean Token.hpp
	flex -o vsop.yy.cpp vsop.lex
	$(CC)  *.cpp -o vsopc $(CCFLAGS)


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
	sudo DEBIAN_FRONTEND=noninteractive apt-get -qq install flex

# This target will build a new archive of this cleaned folder in its parent
# directory. You can use it to generate an archive to be submitted for
# evaluation.
archive:    clean
	cd ..; tar cvJf vsopcompiler.tar.xz --exclude=.git --exclude=.svn --exclude=.svn-base vsopcompiler
make_archive: clean
	mkdir vsopcompiler
	cp *.cpp  vsopcompiler/
	cp Makefile vsopcompiler/
	cp *.hpp vsopcompiler/
	cp *.lex vsopcompiler/
	tar -cJf vsopcompiler.tar.xz vsopcompiler
	rm -r vsopcompiler
