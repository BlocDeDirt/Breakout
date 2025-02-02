#https://stackoverflow.com/questions/1950926/create-directories-using-make-file
#https://gl.developpez.com/tutoriel/outil/makefile/
BUILDDIR = build
SDLLIB = `sdl2-config --cflags --libs`
HEADERS = app.h
SOURCES = app.c main.c color.c paddle.c vector.c collider.c texture.c ball.c math_helper.c brick.c level_parser.c cloud.c
OBJECTS = $(patsubst %.c, $(BUILDDIR)/%.o, $(SOURCES))

# Target to build the main executable
$(BUILDDIR)/main: $(OBJECTS)
	@mkdir -p $(@D)
	@gcc -o $@ $^ $(SDLLIB)

# Generic rule to build object files
$(BUILDDIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(@D)
	@gcc -c $< -o $@ $(SDLLIB)

.PHONY: clean

clean:
	rm -rf $(BUILDDIR)

