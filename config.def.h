/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "brave"
#define BROWCLASS "Brave-browser"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=12", "NotoColorEmoji:pixelsize=12:antialias=true:autohint=true" };
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

typedef struct {
    const char *name;
    const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm",      spcmd1},
    {"spcalc",      spcmd2},
};


/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class       instance     title             tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",      NULL,        NULL,             0,         1,          0,           0,        -1 },
	{ BROWCLASS,   NULL,        NULL,             1 << 1,    0,          0,          -1,        -1 },
	{ TERMCLASS,   NULL,        NULL,             0,         0,          1,           0,        -1 },
	{ NULL,        NULL,        "Event Tester",   0,         0,          0,           1,        -1 }, /* xev */
    { TERMCLASS,   "floatterm", NULL,             0,         1,          1,           0,        -1 },
    { TERMCLASS,   "bg",        NULL,             1 << 7,    0,          1,           0,        -1 },
    { TERMCLASS,   "spterm",    NULL,             SPTAG(0),  1,          1,           0,        -1 },
    { TERMCLASS,   "spcalc",    NULL,             SPTAG(1),  1,          1,           0,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_v,     ACTION##stack, {.i = 0 } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell Leftnds in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"
#include "shiftview.c"
#include <X11/XF86keysym.h>

/* Leftnds */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
/* static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL }; */
static const char *termcmd[]  = { TERMINAL, NULL };

static const Key keys[] = {
	/* modifier                   key             function         argument */
	TAGKEYS(                      XK_1,                            0)
	TAGKEYS(                      XK_2,                            1)
	TAGKEYS(                      XK_3,                            2)
	TAGKEYS(                      XK_4,                            3)
	TAGKEYS(                      XK_5,                            4)
	TAGKEYS(                      XK_6,                            5)
	TAGKEYS(                      XK_7,                            6)
	TAGKEYS(                      XK_8,                            7)
	TAGKEYS(                      XK_9,                            8)
	STACKKEYS(MODKEY,                             focus)
	STACKKEYS(MODKEY|ShiftMask,                   push)
    { MODKEY,                     XK_d,           spawn,           {.v = (const char*[]){ "dmenu_run", NULL } } },
    { MODKEY|ShiftMask,           XK_d,           spawn,           {.v = (const char*[]){ "passmenu", NULL } } },
	{ MODKEY,                     XK_Return,      spawn,           {.v = termcmd } },
	{ MODKEY|ShiftMask,           XK_Return,      zoom,            {0} },
	{ MODKEY,                     XK_b,           togglebar,       {0} },
	{ MODKEY,                     XK_i,           incnmaster,      {.i = +1 } },
	{ MODKEY,                     XK_p,           incnmaster,      {.i = -1 } },
	{ MODKEY,                     XK_r,           spawn,           {.v = (const char*[]){ TERMINAL, "-e", "lfub", NULL } } },
	{ MODKEY|ShiftMask,           XK_r,           spawn,           {.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
	{ MODKEY,                     XK_h,           setmfact,        {.f = -0.05} },
	{ MODKEY,                     XK_l,           setmfact,        {.f = +0.05} },
	{ MODKEY|Mod1Mask,            XK_u,           incrgaps,        {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,  XK_u,           incrgaps,        {.i = -1 } },
	{ MODKEY|Mod1Mask,            XK_0,           togglegaps,      {0} },
	{ MODKEY|Mod1Mask|ShiftMask,  XK_0,           defaultgaps,     {0} },

	{ MODKEY,                     XK_Tab,         view,            {0} },
	{ MODKEY,                     XK_t,           setlayout,       {.v = &layouts[0]} },
	{ MODKEY,                     XK_f,           setlayout,       {.v = &layouts[1]} },
	{ MODKEY,                     XK_m,           setlayout,       {.v = &layouts[2]} },
    { MODKEY,                     XK_g,           shiftview,       { .i = -1 } },
    { MODKEY|ShiftMask,           XK_g,           shifttag,        { .i = -1 } },
    { MODKEY,                     XK_semicolon,   shiftview,       { .i = 1 } },
    { MODKEY|ShiftMask,           XK_semicolon,   shifttag,        { .i = 1 } },
    { MODKEY,                     XK_apostrophe,  togglescratch,   {.ui = 1} },
    { MODKEY|ShiftMask,           XK_apostrophe,  togglescratch,   {.ui = 0} },
	{ MODKEY,                     XK_space,       setlayout,       {0} },
	{ MODKEY|ShiftMask,           XK_space,       togglefloating,  {0} },
	{ MODKEY|ShiftMask,           XK_f,           togglefullscr,   {0} },
	{ MODKEY,                     XK_0,           view,            {.ui = ~0 } },
	{ MODKEY|ShiftMask,           XK_0,           tag,             {.ui = ~0 } },
    { MODKEY,                     XK_comma,       spawn,           {.v = (const char*[]){ "mpc", "prev", NULL } } },
    { MODKEY|ShiftMask,           XK_comma,       spawn,           {.v = (const char*[]){ "mpc", "seek", "0%", NULL } } },
    { MODKEY,                     XK_period,      spawn,           {.v = (const char*[]){ "mpc", "next", NULL } } },
    { MODKEY|ShiftMask,           XK_period,      spawn,           {.v = (const char*[]){ "mpc", "repeat", NULL } } },
    
	{ MODKEY,                     XK_Left,        focusmon,        {.i = -1 } },
	{ MODKEY,                     XK_Right,       focusmon,        {.i = +1 } },
	{ MODKEY|ShiftMask,           XK_Left,        tagmon,          {.i = -1 } },
	{ MODKEY|ShiftMask,           XK_Right,       tagmon,          {.i = +1 } },

	{ MODKEY,                     XK_w,           spawn,           {.v = (const char*[]){ BROWSER, NULL } } },
    { MODKEY|ShiftMask,           XK_w,           spawn,           {.v = (const char*[]){ TERMINAL, "-e", "sudo", "nmtui", NULL } } },
	{ MODKEY,                     XK_q,           killclient,      {0} },
	{ MODKEY|ShiftMask,           XK_q,           spawn,           {.v = (const char*[]){ "sysact", NULL } } },

    /* XF86XK */  
    { 0, XF86XK_AudioMute,          spawn,        SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioRaiseVolume,   spawn,        SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%+; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioLowerVolume,   spawn,        SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%-; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioPrev,          spawn,        {.v = (const char*[]){ "mpc", "prev", NULL } } },
    { 0, XF86XK_AudioNext,          spawn,        {.v = (const char*[]){ "mpc",  "next", NULL } } },
    { 0, XF86XK_AudioPause,         spawn,        {.v = (const char*[]){ "mpc", "pause", NULL } } },
    { 0, XF86XK_AudioPlay,          spawn,        {.v = (const char*[]){ "mpc", "play", NULL } } },
    { 0, XF86XK_AudioStop,          spawn,        {.v = (const char*[]){ "mpc", "stop", NULL } } },
    { 0, XF86XK_AudioRewind,        spawn,        {.v = (const char*[]){ "mpc", "seek", "-10", NULL } } },
    { 0, XF86XK_AudioForward,       spawn,        {.v = (const char*[]){ "mpc", "seek", "+10", NULL } } },
    { 0, XF86XK_AudioMedia,         spawn,        {.v = (const char*[]){ TERMINAL, "-e", "ncmpcpp", NULL } } },
    { 0, XF86XK_AudioMicMute,       spawn,        SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
    /* { 0, XF86XK_PowerOff,        spawn,        {.v = (const char*[]){ "sysact", NULL } } }, */
    { 0, XF86XK_Calculator,         spawn,        {.v = (const char*[]){ TERMINAL, "-e", "bc", "-l", NULL } } },
    { 0, XF86XK_Sleep,              spawn,        {.v = (const char*[]){ "sudo", "-A", "zzz", NULL } } },
    { 0, XF86XK_WWW,                spawn,        {.v = (const char*[]){ BROWSER, NULL } } },
    { 0, XF86XK_DOS,                spawn,        {.v = termcmd } },
    { 0, XF86XK_ScreenSaver,        spawn,        SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
    { 0, XF86XK_TaskPane,           spawn,        {.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
    { 0, XF86XK_Mail,               spawn,        SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
    { 0, XF86XK_MyComputer,         spawn,        {.v = (const char*[]){ TERMINAL, "-e",  "lfub",  "/", NULL } } },
    /* { 0, XF86XK_Battery,         spawn,        SHCMD("") }, */
    { 0, XF86XK_Launch1,            spawn,        {.v = (const char*[]){ "xset", "dpms", "force", "off", NULL } } },
    { 0, XF86XK_TouchpadToggle,     spawn,        SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
    { 0, XF86XK_TouchpadOff,        spawn,        {.v = (const char*[]){ "synclient", "TouchpadOff=1", NULL } } },
    { 0, XF86XK_TouchpadOn,         spawn,        {.v = (const char*[]){ "synclient", "TouchpadOff=0", NULL } } },
    { 0, XF86XK_MonBrightnessUp,    spawn,        {.v = (const char*[]){ "xbacklight", "-inc", "15", NULL } } },
    { 0, XF86XK_MonBrightnessDown,  spawn,        {.v = (const char*[]){ "xbacklight", "-dec", "15", NULL } } },


	/* { MODKEY|Mod1Mask,              XK_i,      incrigaps,       {.i = +1 } }, */
	/* { MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,       {.i = -1 } }, */
	/* { MODKEY|Mod1Mask,              XK_o,      incrogaps,       {.i = +1 } }, */
	/* { MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,       {.i = -1 } }, */
	/* { MODKEY|Mod1Mask,              XK_6,      incrihgaps,      {.i = +1 } }, */
	/* { MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,      {.i = -1 } }, */
	/* { MODKEY|Mod1Mask,              XK_7,      incrivgaps,      {.i = +1 } }, */
	/* { MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,      {.i = -1 } }, */
	/* { MODKEY|Mod1Mask,              XK_8,      incrohgaps,      {.i = +1 } }, */
	/* { MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,      {.i = -1 } }, */
	/* { MODKEY|Mod1Mask,              XK_9,      incrovgaps,      {.i = +1 } }, */
	/* { MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,      {.i = -1 } }, */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkTagBar,            0,              Button4,        shiftview,      {.i = -1} },
    { ClkTagBar,            0,              Button5,        shiftview,      {.i = 1} },
};

