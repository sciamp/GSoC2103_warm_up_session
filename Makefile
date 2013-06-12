all:
	gcc main.c -o ex `pkg-config --cflags --libs gtk+-3.0` `pkg-config --libs --cflags evince-document-3.0` `pkg-config evince-view-3.0 --cflags --libs` `pkg-config poppler-cairo --libs`
