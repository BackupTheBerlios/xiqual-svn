	if(!xi_init(XI_WIDTH, 800, XI_HEIGHT, 600, XI_DEPTH, 16,
		XI_CURSORFILE, "gfx/cursor.png", XI_CURSOR_HOTX, 2, XI_CURSOR_HOTY, 1,
		XI_MAINHOOK, realmain, TAG_END)) {
		fprintf(stderr, "Error (graphics/audio subsystem): %s\n", xi_lasterror);
		return 2;
	}
	atexit(xi_quit);
