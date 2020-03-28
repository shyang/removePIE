CC=clang
CFLAGS=-ggdb -Wall -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk -I../include

PROGS= removePIE

all: $(PROGS)

removePIE: removePIE.o 
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o *.dSYM $(PROGS)

