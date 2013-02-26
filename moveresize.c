static void moveresize(const Arg *arg) {
	XEvent ev;
	Monitor *m = selmon;

#ifdef MOVERESIZE_AUTOFLOAT
	if(!(m->sel && arg && arg->v))
		return;
	if(m->lt[m->sellt]->arrange && !m->sel->isfloating)
		togglefloating(NULL);
#else
	if(!(m->sel && arg && arg->v && m->sel->isfloating))
		return;
#endif

	resize(m->sel, m->sel->x + ((int *)arg->v)[0],
		m->sel->y + ((int *)arg->v)[1],
		m->sel->w + ((int *)arg->v)[2],
		m->sel->h + ((int *)arg->v)[3],
		True);

	while(XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}
