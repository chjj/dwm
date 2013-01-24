void
focusurgent(const Arg *arg) {
  Client *c = NULL;
  for (c = selmon->clients; c; c = c->next) {
    //if (ISVISIBLE(c) && c->isurgent) {
    if (c->isurgent) {
      Arg a = {.ui = c->tags};
      view(&a);
      focus(c);
    }
  }
}

#if 0
void
focusurgent(const Arg *arg) {
  Monitor *m = NULL;
  Client *c = NULL;
	for (m = mons; m; m = m->next) {
    for (c = m->clients; c; c = c->next) {
      //if (ISVISIBLE(c) && c->isurgent) {
      if (c->isurgent) {
        selmon = m;
        Arg a = {.ui = c->tags};
        view(&a);
        focus(c);
      }
    }
  }
}
#endif
