static int maximized = 0;

void
togglemaximize(const Arg *arg) {
	Arg a = {
		.v = maximized
			? &layouts[0]
			: &layouts[2]
	};
	setlayout(&a);
	maximized ^= 1;
}

void
togglefullscreen(const Arg *arg) {
	Arg a = {0};
	togglemaximize(&a);
	selmon->showbar = maximized;
	togglebar(&a);
}
