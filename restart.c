#ifndef DWM_PATH
#define DWM_PATH "/usr/bin/dwm"
#endif

void
self_restart(const Arg *arg) {
	char *const argv[] = {DWM_PATH, NULL};
	execv(argv[0], argv);
}
