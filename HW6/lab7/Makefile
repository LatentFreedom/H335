CFLAGS=  $(shell  sdl-config --cflags) -g -I/opt/local/include -I. 
LDFLAGS= -g $(shell sdl-config --libs) -lSDL_gfx -lSDLmain -lm 
CC=gcc

OBJS = c335sim.o main.o screen.o glcdfont.o lcd_app_helpers.o
 
all: lab7 

lab7: $(OBJS) 
	$(CC) $(OBJS) -o lab7 $(LDFLAGS)
clean:
	rm -f $(OBJS) lab7 


