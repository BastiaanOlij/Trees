# Compiler directives (Mac OS X currently...)
CPP = g++
CFLAGS = -c -arch i386 -arch x86_64 -Iinclude -I3rdparty/include
LDFLAGS = -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -arch i386 -arch x86_64

APPNAME = trees
OBJECTDIR = build/Objects
CONTENTSDIR = build/$(APPNAME).app/Contents

OBJECTS = $(patsubst source/%,$(OBJECTDIR)/%,$(patsubst %.cpp,%.o,$(wildcard source/*.cpp)))
RESOURCES = $(patsubst Resources/%,$(CONTENTSDIR)/Resources/%,$(wildcard Resources/*.*))

all: $(CONTENTSDIR)/MacOS \
	$(CONTENTSDIR)/Info.pList \
	$(CONTENTSDIR)/MacOS/$(APPNAME) \
	$(RESOURCES)
	
$(CONTENTSDIR)/MacOS: 
	mkdir -p $(CONTENTSDIR)/MacOS
	
$(CONTENTSDIR)/Info.pList: Info.plist
	cp -f $^ $@
	@chmod 444 $@

$(CONTENTSDIR)/Resources/%: Resources/%
	@mkdir -p $(@D)
	@chmod 755 $(@D)
	cp -f $^ $@
	@chmod 444 $@
	
$(CONTENTSDIR)/MacOS/$(APPNAME): $(OBJECTS) 3rdparty/GLFW/libglfw3_mac.a 3rdparty/GLEW/libGLEW_mac.a
	$(CPP) $(LDFLAGS) -o $@	$^

$(OBJECTDIR)/%.o: source/%.cpp include/*.h
	@mkdir -p $(@D)
	$(CPP) $(CFLAGS) -o $@ $<

clean:
	rm -R -f build
	