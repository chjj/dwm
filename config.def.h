/* See LICENSE file for copyright and license details. */

// TODO:
// mod4+shift+n - minimize all windows
// mod4+i - clear clipboard
// mod4+shift+h/j - move windows
// mod4+r - run, same as XK_p
// mod4+c - center window
// mod4+n - minimize window
// mod4+m - toggle maximize - equivalent of XK_m+XK_t
// mod4+f - toggle fullscreen
// mod4+ctrl+r - reload
// mod4+wheelup - compton-trans -c +10
// mod4+wheeldown - compton-trans -c -10
// mod4+w - some kind of menu
// autorun, feh, set bg - http://dwm.suckless.org/patches/autostart
// have new windows become slaves instead of masters - http://dwm.suckless.org/patches/attachaside
// keyboard moving/resizing - http://dwm.suckless.org/patches/moveresize
//   another: http://dwm.suckless.org/patches/maximize
//   better: http://dwm.suckless.org/patches/exresize
// focusonclick only - http://dwm.suckless.org/patches/focusonclick
// selfrestart - http://dwm.suckless.org/patches/selfrestart
// urgent border - http://dwm.suckless.org/patches/urgentborder
// attach aside - http://dwm.suckless.org/patches/attachaside
// attach above current client - http://dwm.suckless.org/patches/attachabove

// http://dwm.suckless.org/patches/movestack
// http://dwm.suckless.org/patches/movestack-5.8.2.diff
#include "movestack.c"

// http://dwm.suckless.org/patches/selfrestart
// http://dwm.suckless.org/patches/dwm-r1615-selfrestart.diff
#include "selfrestart.c"

#include "focusurgent.c"

// http://dwm.suckless.org/patches/uselessgap
// http://dwm.suckless.org/patches/dwm-5.9-uselessgap.diff
// #ifndef USELESS_GAP
// #define USELESS_GAP 1
// #endif

// http://dwm.suckless.org/patches/focusonclick
// http://dwm.suckless.org/patches/dwm-6.0-focusonclick.diff
// #ifndef FOCUS_ON_CLICK
// #define FOCUS_ON_CLICK 1
// #endif

/* appearance */
static const char font[]            = "-*-terminus-medium-r-*-*-12-*-*-*-*-*-*-*";

static const char normbordercolor[] = "#000000";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#aaaaaa";
static const char selbordercolor[]  = "#535d6c";
static const char selbgcolor[]      = "#535d6c";
static const char selfgcolor[]      = "#ffffff";

static const unsigned int borderpx  = 1;        /* border pixel of windows */
#if USELESS_GAP
static const unsigned int gappx     = 6;        /* gap pixel between windows */
#endif
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

#if FOCUS_ON_CLICK
/* False means using the scroll wheel on a window will not change focus */
static const Bool focusonwheelscroll = False;
#endif

/* tagging */
static const char *tags[] = { "root", "www", "media", "opt", "etc" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "qemu-system-x86_64", NULL, NULL,   0,            True,        -1 },
	{ "MPlayer",  NULL,       NULL,       0,            True,        -1 },
	{ "Skype",    NULL,       NULL,       0,            False,       -1 },
	{ "xclock",   NULL,       NULL,       0,            True,        -1 },
};

/* layout(s) */
//static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const float mfact      = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "uxterm", NULL };

// Custom:
static const char *voldowncmd[]   = { "amixer", "set", "Master", "5%-", NULL };
static const char *volupcmd[]     = { "amixer", "set", "Master", "5%+", NULL };
static const char *voltogglecmd[] = { "amixer", "set", "Master", "toggle", NULL };
static const char *captogglecmd[] = { "amixer", "set", "Capture", "toggle", NULL };
static const char *pastecmd[]     = { "xdotool", "click", "2", NULL };
static const char *scrotcmd[]     = { "scrot", "-e", "mv $f ~/screenshots/ 2> /dev/null", NULL };
static const char *amenucmd[] = { "amenu", "d", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	//{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	//{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	//{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

	// Custom:
	// NOTE: See /usr/include/X11/keysymdef.h
	{ MODKEY,                       XK_comma,  spawn,          {.v = voldowncmd } },
	{ MODKEY,                       XK_period, spawn,          {.v = volupcmd } },
	{ MODKEY,                       XK_v,      spawn,          {.v = voltogglecmd } },
	{ MODKEY,                       XK_b,      spawn,          {.v = captogglecmd } },
	{ MODKEY,                       XK_Menu,   spawn,          {.v = pastecmd } },
	{ MODKEY,                       XK_Print,  spawn,          {.v = scrotcmd } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_r,      spawn,          {.v = amenucmd } },
	{ MODKEY,                       XK_w,      spawn,          {.v = amenucmd } },
	{ MODKEY|ControlMask,           XK_r,      self_restart,   {0} },
	{ MODKEY,                       XK_u,      focusurgent,    {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
