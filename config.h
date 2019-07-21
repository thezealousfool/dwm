/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const char *fonts[] = {
	"Roboto Bold:size=10"
};
static const char dmenufont[]       = "Roboto:size=10";
static const char normbordercolor[] = "#282828"; // #002b36"; // "#17191b"; // "#444444";
static const char normbgcolor[]     = "#282828"; // #002b36"; // "#17191b"; // "#222222";
static const char normfgcolor[]     = "#fbf1c7"; // #93a1a1"; //"#bbbbbb";
static const char selbordercolor[]  = "#fabd2f"; // #2aa198"; // "#00796b"; // "#519fa5"; // "#005577";
static const char selbgcolor[]      = "#3c3836"; // #005469"; // "#00796b"; // "#519fa5"; // "#005577";
static const char selfgcolor[]      = "#d79921"; // #eeeeee"; // "#17191b"; // "#eeeeee";
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

static const unsigned int gappx = 5; /* gap pixel between windows */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   isterminal   noswallow   monitor */
// 	{ "Gimp",     NULL,       NULL,       0,            1,           0,           0,          -1 },
// 	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           0,           0,          -1 },
	{ "mpv",      NULL,       NULL,       0,            1,           0,           0,          -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[F]",      monocle },
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
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *firefoxcmd[]  = { "firefox", NULL };
static const char *rangercmd[]  = { "st", "-e", "ranger", NULL };
static const char *nvimcmd[]  = { "st", "-e", "nvim", NULL };
static const char *poweroffcmd[]  = { "poweroff", NULL };
static const char *rebootcmd[]  = { "reboot", NULL };
static const char *mpv[] = { "bash", "-c", "xclip -out -selection clipboard | xargs mpv", NULL };
static const char *youtube[] = { "bash", "-c", "xclip -out -selection clipboard | xargs youtube-dl", NULL };

#define UPDATE_STATUS ";"

/* volume controls */
static const char *upvol[] = { "pactl", "set-sink-volume", "0", "+5%", NULL };
static const char *downvol[] = { "pactl", "set-sink-volume", "0", "-5%", NULL };
static const char *mutevol[] = { "pactl", "set-sink-mute", "0", "toggle", NULL };

/* brightness control */
static const char *brightnessup[] = { "xbacklight", "-inc", "5", "-time", "100", NULL };
static const char *brightnessdown[] = { "xbacklight", "-dec", "5", "-time", "100", NULL };

#include "zoomswap.c"
static Key keys[] = {
	/* modifier                     key                       function        argument */
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn,          {.v = upvol } },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn,          {.v = downvol } },
	{ 0,                            XF86XK_AudioMute,         spawn,          {.v = mutevol } },
	{ 0,                            XF86XK_MonBrightnessUp,   spawn,          {.v = brightnessup } },
	{ 0,                            XF86XK_MonBrightnessDown, spawn,          {.v = brightnessdown } },
	{ MODKEY,                       XK_m,                     spawn,          {.v = mpv } },
	{ MODKEY,                       XK_y,                     spawn,          {.v = youtube } },
	{ MODKEY,                       XK_a,                     spawn,          {.v = brightnessup } },
	{ MODKEY,                       XK_r,                     spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,                spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_c,                     spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_q,                     spawn,          {.v = firefoxcmd } },
	{ MODKEY,                       XK_e,                     spawn,          {.v = rangercmd } },
	{ MODKEY,                       XK_n,                     spawn,          {.v = nvimcmd } },
	{ MODKEY,                       XK_b,                     togglebar,      {0} },
	{ MODKEY,                       XK_j,                     focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                     focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_s,                     swapfocus,      {0} },
	{ MODKEY,                       XK_i,                     incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,                     incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                     setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                     setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return,                zoom,           {0} },
	{ MODKEY,                       XK_Tab,                   view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,                     killclient,     {0} },
	{ MODKEY,                       XK_t,                     setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                     togglemonocle,  {0} },
	{ MODKEY,                       XK_space,                 setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,                 togglefloating, {0} },
	{ MODKEY,                       XK_0,                     view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                     tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                 focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                 tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                                     0)
	TAGKEYS(                        XK_2,                                     1)
	TAGKEYS(                        XK_3,                                     2)
	TAGKEYS(                        XK_4,                                     3)
	TAGKEYS(                        XK_5,                                     4)
	TAGKEYS(                        XK_6,                                     5)
	TAGKEYS(                        XK_7,                                     6)
	TAGKEYS(                        XK_8,                                     7)
	TAGKEYS(                        XK_9,                                     8)
	{ MODKEY|ShiftMask,             XK_q,                     quit,           {0} },
	{ MODKEY|ShiftMask,             XK_x,                     spawn,          {.v = poweroffcmd} },
	{ MODKEY|ShiftMask,             XK_r,                     spawn,          {.v = rebootcmd} },
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

