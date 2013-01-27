void
center(const Arg *arg) {
	Client *c = selmon->sel;

	if (!c || !c->isfloating) {
		return;
	}

	Monitor *m = c->mon;

	c->x = m->mx + (m->mw / 2 - WIDTH(c) / 2);
	c->y = m->my + (m->mh / 2 - HEIGHT(c) / 2);

	arrange(selmon);
}
