/* See LICENSE file for copyright and license details. */

// All patches:
// movestack, uselessgap (improved version), focusonclick (fixed),
// urgentborder, focusurgent (custom), center (custom), restart (custom)
// moveresize, fixmplayer (custom), autostart (custom)

// http://dwm.suckless.org/patches/movestack
// http://dwm.suckless.org/patches/movestack-5.8.2.diff
// alternative: http://dwm.suckless.org/patches/push
#include "movestack.c"

// http://dwm.suckless.org/patches/selfrestart
// http://dwm.suckless.org/patches/dwm-r1615-selfrestart.diff
// #include "selfrestart.c"

// http://dwm.suckless.org/patches/moveresize
#include "moveresize.c"

// Restart function
#include "restart.c"

// Focus urgent function
#include "focusurgent.c"

// Center window function
#include "center.c"

/* appearance */
static const char font[]            = "-*-terminus-medium-r-*-*-14-*-*-*-*-*-*-*";

/* Macbook Pro Resolution */
// static const char font[]            = "-*-terminus-medium-r-*-*-22-*-*-*-*-*-*-*";

static const char normbordercolor[] = "#222222";
static const char normbgcolor[]     = "#441111";
static const char normfgcolor[]     = "#ffffff";
static const char selbordercolor[]  = "#ffffff";
static const char selbgcolor[]      = "#441111";
static const char selfgcolor[]      = "#ee5500";
#if URGENT_BORDER
static const char urgbordercolor[]  = "#ff0000";
#endif
static const char dmenubgcolor[]    = "#110000";

static const unsigned int borderpx  = 2;        /* border pixel of windows */
#if USELESS_GAP
//static const unsigned int gappx     = 15;        /* gap pixel between windows */
static const unsigned int gappx     = 18;        /* gap pixel between windows */
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
	{ "XTerm",    "qemu-term", NULL,      0,            True,        -1 },
	{ "URxvt",    "qemu-term", NULL,      0,            True,        -1 },
	{ "MPlayer",  NULL,       NULL,       0,            True,        -1 },
	{ "mpv",      NULL,       NULL,       0,            True,        -1 },
	{ "Vncviewer",NULL,       NULL,       0,            True,        -1 },
	{ "Skype",    NULL,       NULL,       0,            False,       -1 },
	{ "XClock",   NULL,       NULL,       0,            True,        -1 },
	{ "feh",      NULL,       NULL,       0,            True,        -1 },
};

/* layout(s) */
static const float mfact      = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
#if USELESS_GAP
	/* symbol     gap       arrange function */
	{ "[]=",      True,     tile },    /* first entry is default */
	{ "><>",      False,    NULL },    /* no layout function means floating behavior */
	{ "[M]",      False,    monocle },
#else
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
#endif
};

// Toggle maximize function
// #include "togglemaximize.c"

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
#if !USE_GRADIENT
static const char *dmenucmd[] = { "dmenu_run", "-b", "-p", ">", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
#else
static const char *dmenucmd[] = { "dmenu_run", "-b", "-p", ">", "-fn", font, "-nb", dmenubgcolor, "-nf", normfgcolor, "-sb", dmenubgcolor, "-sf", selfgcolor, NULL };
#endif
static const char *termcmd[]  = { "urxvt", "-name", "tmux", "-T", "tmux", "-e", "tmux", NULL };

// Custom:
static const char *voldowncmd[]   = { "amixer", "set", "Master", "5%-", NULL };
static const char *volupcmd[]     = { "amixer", "set", "Master", "5%+", NULL };
static const char *voltogglecmd[] = { "amixer", "set", "Master", "toggle", NULL };
static const char *captogglecmd[] = { "amixer", "set", "Capture", "toggle", NULL };
static const char *pastecmd[]     = { "sh", "-c", "sleep 0.5 && xdotool type --delay 0ms \"$(xsel -o -p)\"", NULL };
static const char *scrotcmd[]     = { "scrot", "-e", "mv $f ~/screenshots", NULL };
#if !USE_GRADIENT
static const char *startcmd[]     = { "dwm-start", "menu", "-b", "-p", ">",
                                      "-fn", font, "-nb", normbgcolor,
                                      "-nf", normfgcolor, "-sb", selbgcolor,
                                      "-sf", selfgcolor, NULL };
#else
static const char *startcmd[]     = { "dwm-start", "menu", "-b", "-p", ">",
                                      "-fn", font, "-nb", dmenubgcolor,
                                      "-nf", normfgcolor, "-sb", dmenubgcolor,
                                      "-sf", selfgcolor, NULL };
#endif
static const char *clipcmd[]      = { "sh", "-c", "xsel -c -p && xsel -c -s && xsel -c -b", NULL };
static const char *transupcmd[]   = { "compton-trans", "-c", "-o", "+10", NULL };
static const char *transdowncmd[] = { "compton-trans", "-c", "-o", "-10", NULL };
static const char *transdelcmd[]  = { "compton-trans", "-c", "-d", NULL };

static const char *touchcmd[]     = { "sh", "-c", "synclient "
	" TouchpadOff=$(synclient -l | grep -c 'TouchpadOff.*=.*0')"
	" && xdotool mousemove 32767 32767",
	NULL };
// Toggle with xinput:
// Example: xinput --set-prop device "Device Enabled" 0
// static const char *touchcmd[]     = { "sh", "-c", "xinput --set-prop "
// 	" $(xinput --list | grep -i 'Mouse' | sed 's/^.*id=\\([0-9]\\+\\).*$/\\1/')"
// 	" 'Device Enabled' $(xinput --list-props $(xinput --list | grep -i 'Mouse'"
// 	" | sed 's/^.*id=\\([0-9]\\+\\).*$/\\1/') | grep -c 'Device Enabled.*0')"
// 	" && xdotool mousemove 32767 32767",
// 	NULL };
static const char *touchscreencmd[]     = { "sh", "-c", "xinput --set-prop "
	" $(xinput --list | grep -i 'maXTouch Digitizer' | sed 's/^.*id=\\([0-9]\\+\\).*$/\\1/')"
	" 'Device Enabled' $(xinput --list-props $(xinput --list | grep -i 'maXTouch Digitizer'"
	" | sed 's/^.*id=\\([0-9]\\+\\).*$/\\1/') | grep -c 'Device Enabled.*0')"
	" && xdotool mousemove 32767 32767",
	NULL };

static const char *slockcmd[]  = { "slock", NULL };

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
	//{ 0,                            XK_Menu,   spawn,          {.v = pastecmd } },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = pastecmd } },
	{ 0,                            XK_Print,  spawn,          {.v = scrotcmd } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_r,      spawn,          {.v = startcmd } },
	{ MODKEY,                       XK_w,      spawn,          {.v = startcmd } },
	{ MODKEY|ControlMask,           XK_r,      self_restart,   {0} },
	{ MODKEY,                       XK_u,      focusurgent,    {0} },
	{ MODKEY,                       XK_c,      center,         {0} },
	{ MODKEY,                       XK_g,      spawn,          {.v = clipcmd } },
	{ MODKEY,                       XK_a,      spawn,          {.v = touchcmd } },
	{ MODKEY|ShiftMask,             XK_a,      spawn,          {.v = touchscreencmd } },
	{ MODKEY|ShiftMask,             XK_l,      spawn,          {.v = slockcmd } },

	// If togglemaximize is enabled:
	//{ MODKEY,                       XK_m,      togglemaximize, {0} },

	// Resizing / Moving
	{ MODKEY,                       XK_Down,   moveresize,     {.v = (int []){ 0, 50, 0, 0 }}},
	{ MODKEY,                       XK_Up,     moveresize,     {.v = (int []){ 0, -50, 0, 0 }}},
	{ MODKEY,                       XK_Right,  moveresize,     {.v = (int []){ 50, 0, 0, 0 }}},
	{ MODKEY,                       XK_Left,   moveresize,     {.v = (int []){ -50, 0, 0, 0 }}},
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = (int []){ 0, 0, 0, 50 }}},
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = (int []){ 0, 0, 0, -50 }}},
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = (int []){ 0, 0, 50, 0 }}},
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = (int []){ 0, 0, -50, 0 }}},
	{ MODKEY|ControlMask,           XK_Up,     moveresize,     {.v = (int []){ 0, 0, 50, 50 }}},
	{ MODKEY|ControlMask,           XK_Down,   moveresize,     {.v = (int []){ 0, 0, -50, -50 }}},
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

	// Custom:
	{ ClkClientWin,         MODKEY,         Button4,        spawn,          {.v = transupcmd } },
	{ ClkClientWin,         MODKEY,         Button5,        spawn,          {.v = transdowncmd } },
	{ ClkClientWin,         MODKEY,         Button3,        spawn,          {.v = transdelcmd } },
};
