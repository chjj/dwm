void
focusurgent(const Arg *arg) {
  Monitor *m = NULL;
  Client *c = NULL;
	for (m = mons; m; m = m->next) {
    for (c = m->clients; c; c = c->next) {
      if (c->isurgent) {
        selmon = m;
        Arg a = {.ui = c->tags};
        view(&a);
        focus(c);
      }
    }
  }
}
