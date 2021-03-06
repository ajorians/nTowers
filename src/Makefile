GCC = nspire-g++
LD = nspire-ld
GENZEHN = genzehn

GCCFLAGS = -Os -Wall -W -marm -Wno-unused-parameter -I include
LDFLAGS = -L lib -lndls -lTowers -lArchive
OBJCOPY := "$(shell which arm-elf-objcopy 2>/dev/null)"
ifeq (${OBJCOPY},"")
	OBJCOPY := arm-none-eabi-objcopy
endif
EXE = nTowers-sdl
DISTDIR = ..

OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
vpath %.tns $(DISTDIR)
vpath %.elf $(DISTDIR)

all: prepare $(EXE).prg.tns

#Because JGit does not do symbolic links and TeamCity uses JGit
prepare:
	cd ArchiveLib; make; cd ..
	cd TowersLib; make; cd ..
	mkdir include
	mkdir include/ArchiveLib
	mkdir include/TowersLib
	mkdir lib
	cp -v ArchiveLib/include/ArchiveLib.h include/ArchiveLib
	cp -v ArchiveLib/libArchive.a lib
	cp -v TowersLib/include/TowersLib.h include/TowersLib
	cp -v TowersLib/libTowers.a lib

%.o: %.cpp
	$(GCC) $(GCCFLAGS) -c $<

$(EXE).elf: $(OBJS)
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $(DISTDIR)/$@ $(LDFLAGS)

$(EXE).tns: $(EXE).elf
	$(GENZEHN) --input $(DISTDIR)/$^ --output $(DISTDIR)/$@ $(ZEHNFLAGS)

$(EXE).prg.tns: $(EXE).tns
	make-prg $(DISTDIR)/$^ $(DISTDIR)/$@

clean:
	cd ArchiveLib; make clean; cd ..
	cd TowersLib; make clean; cd ..
	rm -rf include
	rm -rf lib
	rm -f *.o *.elf
	rm -f $(DISTDIR)/$(EXE).tns
	rm -f $(DISTDIR)/$(EXE).elf
	rm -f $(DISTDIR)/$(EXE).prg.tns
