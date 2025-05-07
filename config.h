/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int refreshrate    = 60;       /* refresh rate for smoother window movements */
static const unsigned int borderpx       = 1;        /* border pixel of windows */
static const unsigned int snap           = 15;       /* snap pixel */
static const unsigned int systraypinning = 0;        /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft  = 0;        /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 5;        /* systray spacing */
static const int systraypinningfailfirst = 1;        /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;        /* 0 means no systray */
static const int swallowfloating         = 1;        /* 1 means swallow floating windows by default */
static const int swterminheritfs         = 1;        /* 1 terminal inherits fullscreen on unswallow, 0 otherwise */
static const int showbar                 = 1;        /* 0 means no bar */
static const int topbar                  = 1;        /* 0 means bottom bar */
static const char *fonts[]               = { "MesloLGS Nerd Font Mono:size=16", "NotoColorEmoji:pixelsize=16:antialias=true:autohint=true" };
static const char normfg[]               = "#d8dee9";
static const char normbg[]               = "#2e3440";
static const char normborder[]           = "#3b4252";
static const char selfg[]                = "#eceff4";
static const char selbg[]                = "#434c5e";
static const char selborder[]            = "#434c5e";

static const char *colors[][3]           = {
	/*               fg         bg         border   */
	[SchemeNorm] = { normfg,    normbg,    normborder },
	[SchemeSel]  = { selfg,     selbg,     selborder  },
};

static const char *const autostart[] = {
	"sh", "-c", "xset s off && xset s noblank && xset -dpms", NULL,
	"dbus-update-activation-environment", "--systemd", "--all", NULL,
	"/usr/lib/mate-polkit/polkit-mate-authentication-agent-1", NULL,
	"sh", "-c", "feh --randomize --bg-fill ~/Images/Wallpapers/*", NULL,
	"picom", "-b", NULL,
	"flameshot", NULL,
	"dunst", NULL,
	"slstatus", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "", "", "󰊖", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class             instance    title            tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Alacritty",       NULL,       NULL,            0,         0,          1,           0,        -1 },
	{ "Thunar",          NULL,       NULL,            0,         1,          0,           0,        -1 },
    { "Lutris",          NULL,       NULL,            0,         1,          0,           0,        -1 },
    { "steam",           NULL,       NULL,            0,         1,          0,           0,        -1 },
	{ "zenity",          NULL,       NULL,            0,         1,          0,           0,        -1 },
	{ "Virt-manager",    NULL,       NULL,            0,         1,          0,           0,        -1 },
	{ "dolphin-emu",     NULL,       NULL,            0,         1,          0,           0,        -1 },
	{ NULL,              NULL,       "Event Tester",  0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.6;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",         tile },   /* first entry is default */
	{ "",         NULL },   /* no layout function means floating behavior */
	{ "",         monocle },
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
static const char *launcher[] = { "rofi", "-show", "drun", NULL };
static const char *terminal[] = { "alacritty", NULL };

#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key            function        argument */
	{ MODKEY,                       XK_BackSpace,  spawn,            {.v = launcher } },
	{ MODKEY,                       XK_Return,     spawn,            {.v = terminal } },
	{ MODKEY,                       XK_p,          spawn,            SHCMD ("flameshot full -p ~/Images/Screenshots") },
	{ MODKEY|ShiftMask,             XK_p,          spawn,            SHCMD ("flameshot gui -p ~/Images/Screenshots") },
	{ MODKEY|ControlMask,           XK_p,          spawn,            SHCMD ("flameshot gui --clipboard") },
	{ MODKEY|ShiftMask,             XK_w,          spawn,            SHCMD ("feh --randomize --bg-fill ~/Images/Wallpapers/*") },
	{ MODKEY,                       XK_equal,      spawn,            SHCMD ("amixer sset Master 5%+ unmute") },
	{ MODKEY,                       XK_minus,      spawn,            SHCMD ("amixer sset Master 5%- unmute") },
	{ MODKEY,                       XK_m,          spawn,            SHCMD ("amixer sset Master $(amixer get Master | grep -q '\\[on\\]' && echo 'mute' || echo 'unmute')") },
	{ MODKEY|ControlMask,           XK_i,          spawn,            SHCMD ("pgrep -x 'picom' > /dev/null && killall picom || picom -b") },
	{ MODKEY|ControlMask,           XK_g,          spawn,            SHCMD ("xdg-open https://github.com/gabrieldallagnoli") },
	{ MODKEY|ShiftMask,             XK_b,          togglebar,        {0} },
	{ MODKEY,                       XK_k,          focusstack,       {.i = +1 } },
	{ MODKEY,                       XK_j,          focusstack,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_k,          movestack,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_j,          movestack,        {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_i,          incnmaster,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_d,          incnmaster,       {.i = -1 } },
	{ MODKEY,                       XK_h,          setmfact,         {.f = -0.05} },
	{ MODKEY,                       XK_l,          setmfact,         {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_l,          setcfact,         {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_h,          setcfact,         {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,          setcfact,         {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_Return,     zoom,             {0} },
	{ MODKEY,                       XK_Tab,        view,             {0} },
	{ MODKEY,                       XK_c,          killclient,       {0} },
	{ MODKEY,                       XK_d,          setlayout,        {.v = &layouts[0]} },
	{ MODKEY,                       XK_e,          setlayout,        {.v = &layouts[1]} },
	{ MODKEY,                       XK_q,          setlayout,        {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_e,          togglefloating,   {0} },
	{ MODKEY,                       XK_0,          view,             {.ui = ~0 } },
	{ MODKEY,                       XK_comma,      focusmon,         {.i = -1 } },
	{ MODKEY,                       XK_period,     focusmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,      tagmon,           {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,     tagmon,           {.i = +1 } },
	TAGKEYS(                        XK_1,                            0)
	TAGKEYS(                        XK_2,                            1)
	TAGKEYS(                        XK_3,                            2)
	TAGKEYS(                        XK_4,                            3)
	TAGKEYS(                        XK_5,                            4)
	{ MODKEY|ShiftMask,             XK_BackSpace,  quit,             {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_s,          spawn,            SHCMD ("systemctl poweroff") },
	{ MODKEY|ControlMask|ShiftMask, XK_r,          spawn,            SHCMD ("systemctl reboot") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkClientWin,         MODKEY,         Button1,        moveorplace,    {.i = 2} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
