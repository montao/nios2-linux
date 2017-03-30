CFLAGS += -Wall

LDLIBS += -lm

all: v4l2_video_capture v4l2_video_display

romfs:
	$(ROMFSINST) /bin/v4l2_video_capture
	$(ROMFSINST) /bin/v4l2_video_display

clean:
	rm -f v4l2_video_capture v4l2_video_display *.elf *.gdb *.o

.PHONY: all clean romfs
