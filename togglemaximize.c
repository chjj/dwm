void
togglemaximize(const Arg *arg) {
	Arg a = {
		.v = selmon->lt[selmon->sellt] == &layouts[2]
			? &layouts[0]
			: &layouts[2]
	};
	setlayout(&a);
}

void
togglefullscreen(const Arg *arg) {
	Arg a = {0};
	togglemaximize(&a);
	selmon->showbar = selmon->lt[selmon->sellt] == &layouts[0];
	togglebar(&a);
}
