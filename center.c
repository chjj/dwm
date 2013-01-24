void center(const Arg *arg) {
  Client *c = selmon->sel;
  Monitor *m = c->mon;

  if (!c || !c->isfloating) return;
  //if (!ISVISIBLE(c)) return;

  //c->x = (c->mon->ww / 2) - (c->w / 2);
  //c->y = (c->mon->wh / 2) - (c->h / 2);
  //arrange(selmon);

  c->x = m->mx + (m->mw / 2 - WIDTH(c) / 2);
  c->y = m->my + (m->mh / 2 - HEIGHT(c) / 2);
}
