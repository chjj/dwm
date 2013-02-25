void
size_up(const Arg *arg) {
	Client *c = selmon->sel;

	if (!c || !c->isfloating) {
		return;
	}

	c->h = MAX(c->h - 100, 100);

	arrange(selmon);
}

void
size_left(const Arg *arg) {
	Client *c = selmon->sel;

	if (!c || !c->isfloating) {
		return;
	}

	c->w = MAX(c->w - 100, 100);

	arrange(selmon);
}

void
size_down(const Arg *arg) {
	Client *c = selmon->sel;

	if (!c || !c->isfloating) {
		return;
	}

	c->h += 100;

	if (c->y + HEIGHT(c) > c->mon->my + c->mon->mh) {
		//c->h -= (c->y + c->h) - (c->mon->my + c->mon->mh);
		c->h -= (c->y + HEIGHT(c)) - (c->mon->my + c->mon->mh);
	}

	arrange(selmon);
}

void
size_right(const Arg *arg) {
	Client *c = selmon->sel;

	if (!c || !c->isfloating) {
		return;
	}

	c->w += 100;

	if (c->x + WIDTH(c) > c->mon->mx + c->mon->mw) {
		//c->w -= (c->x + c->w) - (c->mon->mx + c->mon->mw);
		c->w -= (c->x + WIDTH(c)) - (c->mon->mx + c->mon->mw);
	}

	arrange(selmon);
}

void
move_up(const Arg *arg) {
	Client *c = selmon->sel;

	if (!c || !c->isfloating) {
		return;
	}

	c->y = MAX(c->y - 100, 0);

	arrange(selmon);
}

void
move_left(const Arg *arg) {
	Client *c = selmon->sel;

	if (!c || !c->isfloating) {
		return;
	}

	c->x = MAX(c->x - 100, 0);

	arrange(selmon);
}

void
move_down(const Arg *arg) {
	Client *c = selmon->sel;

	if (!c || !c->isfloating) {
		return;
	}

	c->y += 100;

	if (c->y + HEIGHT(c) > c->mon->my + c->mon->mh) {
		c->y = c->mon->my + c->mon->mh - HEIGHT(c);
		if (c->y < 0) {
			c->h += c->y;
			c->y = 0;
		}
	}

	arrange(selmon);
}

void
move_right(const Arg *arg) {
	Client *c = selmon->sel;

	if (!c || !c->isfloating) {
		return;
	}

	c->x += 100;

	if (c->x + WIDTH(c) > c->mon->mx + c->mon->mw) {
		c->x = c->mon->mx + c->mon->mw - WIDTH(c);
		if (c->x < 0) {
			c->w += c->x;
			c->x = 0;
		}
	}

	arrange(selmon);
}
