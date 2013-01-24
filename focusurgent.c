void
focusurgent(const Arg *arg) {
  Client *c = NULL;
  for (c = selmon->clients; c; c = c->next) {
    if (ISVISIBLE(c) && c->isurgent) {
      Arg a = {.ui = c->tags};
      view(&a);
      focus(c);
    }
  }
}
