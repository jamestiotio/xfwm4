/*      $Id$

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2, or (at your option)
        any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, write to the Free Software
        Foundation, Inc., Inc., 51 Franklin Street, Fifth Floor, Boston,
        MA 02110-1301, USA.


        oroborus - (c) 2001 Ken Lynch
        xfwm4    - (c) 2002-2008 Olivier Fourdan

 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include <X11/cursorfont.h>
#include <X11/extensions/shape.h>

#include <glib.h>
#include <gtk/gtk.h>
#include <sys/time.h>
#include <time.h>

#include "screen.h"
#include "misc.h"
#include "hints.h"
#include "keyboard.h"
#include "mypixmap.h"
#include "mywindow.h"
#include "settings.h"

#ifndef INC_CLIENT_H
#define INC_CLIENT_H

#define APPLY                           +1
#define REMOVE                          -1

#define PLACEMENT_MOUSE                 0
#define PLACEMENT_ROOT                  1

#define NO_CFG_FLAG                     0
#define CFG_CONSTRAINED                 (1<<0)
#define CFG_REQUEST                     (1<<1)
#define CFG_NOTIFY                      (1<<2)
#define CFG_FORCE_REDRAW                (1<<3)

#define INCLUDE_HIDDEN                  (1<<0)
#define INCLUDE_SHADED                  (1<<1)
#define INCLUDE_ALL_WORKSPACES          (1<<2)
#define INCLUDE_SKIP_FOCUS              (1<<3)
#define INCLUDE_SKIP_PAGER              (1<<4)
#define INCLUDE_SKIP_TASKBAR            (1<<5)

#define NO_UPDATE_FLAG                  0
#define UPDATE_BUTTON_GRABS             (1<<0)
#define UPDATE_FRAME                    (1<<1)
#define UPDATE_GRAVITY                  (1<<2)
#define UPDATE_MAXIMIZE                 (1<<3)
#define UPDATE_CACHE                    (1<<4)
#define UPDATE_ALL                      (UPDATE_BUTTON_GRABS | \
                                         UPDATE_FRAME | \
                                         UPDATE_GRAVITY | \
                                         UPDATE_MAXIMIZE | \
                                         UPDATE_CACHE)

#define CLIENT_FILL_VERT                (1L<<0)
#define CLIENT_FILL_HORIZ               (1L<<1)
#define CLIENT_FILL                     (CLIENT_FILL_VERT | \
                                         CLIENT_FILL_HORIZ)


#define STRUTS_SIZE                     12

#ifndef CLIENT_MIN_VISIBLE
#define CLIENT_MIN_VISIBLE              15
#endif

#ifndef CLIENT_XSYNC_TIMEOUT
#define CLIENT_XSYNC_TIMEOUT            1000 /* ms */
#endif

#ifndef CLIENT_BLINK_TIMEOUT
#define CLIENT_BLINK_TIMEOUT            200  /* ms */
#endif

#ifndef CLIENT_PING_TIMEOUT
#define CLIENT_PING_TIMEOUT             3000 /* ms */
#endif

#ifndef MAX_BLINK_ITERATIONS
#define MAX_BLINK_ITERATIONS            5
#endif

#define XFWM_FLAG_HAS_BORDER            (1L<<0)
#define XFWM_FLAG_HAS_MENU              (1L<<1)
#define XFWM_FLAG_HAS_MAXIMIZE          (1L<<2)
#define XFWM_FLAG_HAS_CLOSE             (1L<<3)
#define XFWM_FLAG_HAS_HIDE              (1L<<4)
#define XFWM_FLAG_HAS_MOVE              (1L<<5)
#define XFWM_FLAG_HAS_RESIZE            (1L<<6)
#define XFWM_FLAG_HAS_STICK             (1L<<7)
#define XFWM_FLAG_FOCUS                 (1L<<8)
#define XFWM_FLAG_IS_RESIZABLE          (1L<<9)
#define XFWM_FLAG_MAP_PENDING           (1L<<10)
#define XFWM_FLAG_VISIBLE               (1L<<11)
#define XFWM_FLAG_MANAGED               (1L<<13)
#define XFWM_FLAG_SESSION_MANAGED       (1L<<14)
#define XFWM_FLAG_WORKSPACE_SET         (1L<<15)
#define XFWM_FLAG_WAS_SHOWN             (1L<<16)
#define XFWM_FLAG_DRAW_ACTIVE           (1L<<17)
#define XFWM_FLAG_SEEN_ACTIVE           (1L<<18)
#define XFWM_FLAG_FIRST_MAP             (1L<<19)
#define XFWM_FLAG_LEGACY_FULLSCREEN     (1L<<20)
#define XFWM_FLAG_MOVING_RESIZING       (1L<<21)
#define XFWM_FLAG_NEEDS_REDRAW          (1L<<22)
#define XFWM_FLAG_OPACITY_LOCKED        (1L<<23)

#define CLIENT_FLAG_HAS_STRUT           (1L<<0)
#define CLIENT_FLAG_HAS_STRUT_PARTIAL   (1L<<1)
#define CLIENT_FLAG_HAS_USER_TIME       (1L<<2)
#define CLIENT_FLAG_HAS_STARTUP_TIME    (1L<<3)
#define CLIENT_FLAG_ABOVE               (1L<<4)
#define CLIENT_FLAG_BELOW               (1L<<5)
#define CLIENT_FLAG_FULLSCREEN          (1L<<6)
#define CLIENT_FLAG_ICONIFIED           (1L<<7)
#define CLIENT_FLAG_MAXIMIZED_VERT      (1L<<8)
#define CLIENT_FLAG_MAXIMIZED_HORIZ     (1L<<9)
#define CLIENT_FLAG_MAXIMIZED           (CLIENT_FLAG_MAXIMIZED_VERT | \
                                         CLIENT_FLAG_MAXIMIZED_HORIZ)
#define CLIENT_FLAG_SHADED              (1L<<10)
#define CLIENT_FLAG_SKIP_PAGER          (1L<<11)
#define CLIENT_FLAG_SKIP_TASKBAR        (1L<<12)
#define CLIENT_FLAG_STATE_MODAL         (1L<<13)
#define CLIENT_FLAG_STICKY              (1L<<15)
#define CLIENT_FLAG_NAME_CHANGED        (1L<<16)
#define CLIENT_FLAG_DEMANDS_ATTENTION   (1L<<17)
#define CLIENT_FLAG_HAS_SHAPE           (1L<<18)

#define WM_FLAG_DELETE                  (1L<<0)
#define WM_FLAG_INPUT                   (1L<<1)
#define WM_FLAG_TAKEFOCUS               (1L<<2)
#define WM_FLAG_CONTEXT_HELP            (1L<<3)
#define WM_FLAG_URGENT                  (1L<<4)
#define WM_FLAG_PING                    (1L<<5)

#define XFWM_FLAG_INITIAL_VALUES        XFWM_FLAG_HAS_BORDER | \
                                        XFWM_FLAG_HAS_MENU | \
                                        XFWM_FLAG_HAS_MAXIMIZE | \
                                        XFWM_FLAG_HAS_STICK | \
                                        XFWM_FLAG_HAS_HIDE | \
                                        XFWM_FLAG_HAS_CLOSE | \
                                        XFWM_FLAG_HAS_MOVE | \
                                        XFWM_FLAG_HAS_RESIZE | \
                                        XFWM_FLAG_FIRST_MAP | \
                                        XFWM_FLAG_NEEDS_REDRAW

#define ALL_WORKSPACES                  (int) 0xFFFFFFFF

#define CONSTRAINED_WINDOW(c)           ((c->win_layer > WIN_LAYER_DESKTOP) && \
                                        !(c->type & (WINDOW_DESKTOP | WINDOW_DOCK)) && \
                                        !FLAG_TEST(c->xfwm_flags, XFWM_FLAG_LEGACY_FULLSCREEN))

#define WINDOW_TYPE_DIALOG              (WINDOW_DIALOG | \
                                         WINDOW_MODAL_DIALOG)
#define WINDOW_TYPE_DONT_PLACE          (WINDOW_DESKTOP | \
                                         WINDOW_DOCK | \
                                         WINDOW_UTILITY | \
                                         WINDOW_SPLASHSCREEN)
#define WINDOW_REGULAR_FOCUSABLE        (WINDOW_NORMAL | \
                                         WINDOW_DIALOG | \
                                         WINDOW_MODAL_DIALOG)
#define WINDOW_TYPE_DONT_FOCUS          (WINDOW_SPLASHSCREEN | \
                                         WINDOW_DOCK)

/* Which bits of opacity are applied */
#define OPACITY_MOVE                    (1<<0)
#define OPACITY_RESIZE                  (1<<1)
#define OPACITY_INACTIVE                (1<<2)

/* Convenient macros */
#define FLAG_TEST(flag,bits)                   (flag & (bits))
#define FLAG_TEST_ALL(flag,bits)               ((flag & (bits)) == (bits))
#define FLAG_TEST_AND_NOT(flag,bits1,bits2)    ((flag & (bits1 | bits2)) == (bits1))
#define FLAG_SET(flag,bits)                    (flag |= (bits))
#define FLAG_UNSET(flag,bits)                  (flag &= ~(bits))
#define FLAG_TOGGLE(flag,bits)                 (flag ^= (bits))

#define CLIENT_CAN_HIDE_WINDOW(c)       (!(c->transient_for) && \
                                         FLAG_TEST(c->xfwm_flags, XFWM_FLAG_HAS_HIDE) && \
                                         !FLAG_TEST(c->flags, CLIENT_FLAG_SKIP_TASKBAR))
#define CLIENT_CAN_MAXIMIZE_WINDOW(c)   (FLAG_TEST_AND_NOT(c->xfwm_flags, XFWM_FLAG_HAS_MAXIMIZE | \
                                                                          XFWM_FLAG_HAS_RESIZE | \
                                                                          XFWM_FLAG_IS_RESIZABLE, \
                                                                          XFWM_FLAG_LEGACY_FULLSCREEN) && \
                                         !FLAG_TEST (c->flags, CLIENT_FLAG_FULLSCREEN))
#define CLIENT_CAN_FILL_WINDOW(c)       (FLAG_TEST_AND_NOT(c->xfwm_flags, XFWM_FLAG_HAS_RESIZE | \
                                                                          XFWM_FLAG_IS_RESIZABLE, \
                                                                          XFWM_FLAG_LEGACY_FULLSCREEN) && \
                                         !FLAG_TEST (c->flags, CLIENT_FLAG_FULLSCREEN | CLIENT_FLAG_MAXIMIZED))

#define CLIENT_CAN_STICK_WINDOW(c)      (!(c->transient_for) && \
                                         FLAG_TEST(c->xfwm_flags, XFWM_FLAG_HAS_STICK))

typedef enum
{
    UNSET = 0,
    WINDOW_NORMAL       = (1 << 0),
    WINDOW_DESKTOP      = (1 << 1),
    WINDOW_DOCK         = (1 << 2),
    WINDOW_DIALOG       = (1 << 3),
    WINDOW_MODAL_DIALOG = (1 << 4),
    WINDOW_TOOLBAR      = (1 << 5),
    WINDOW_MENU         = (1 << 6),
    WINDOW_UTILITY      = (1 << 7),
    WINDOW_SPLASHSCREEN = (1 << 8)
}
netWindowType;

struct _Client
{
    /* Reference to our screen structure */
    ScreenInfo *screen_info;

    Window window;
    Window frame;
    Window transient_for;
    Window user_time_win;
    Window *cmap_windows;
    xfwmWindow title;
    xfwmWindow sides[SIDE_COUNT];
    xfwmWindow corners[CORNER_COUNT];
    xfwmWindow buttons[BUTTON_COUNT];
    Window client_leader;
    Window group_leader;
    xfwmPixmap appmenu[STATE_TOGGLED];
    Colormap cmap;
    unsigned long win_hints;
    unsigned long win_state;
    unsigned long win_layer;
    unsigned long serial;
    Atom type_atom;
    Visual *visual;
    XSizeHints *size;
    XWMHints *wmhints;
    XClassHint class;
    Client *next;
    Client *prev;
    netWindowType type;
    int x;
    int y;
    int width;
    int height;
    int depth;
    int border_width;
    int gravity;
    int win_workspace;
    unsigned int ignore_unmap;
    int old_x;
    int old_y;
    int old_width;
    int old_height;
    int fullscreen_old_x;
    int fullscreen_old_y;
    int fullscreen_old_width;
    int fullscreen_old_height;
    int fullscreen_old_layer;
    int previous_width;
    int previous_height;
    int initial_layer;
    int ncmap;
    int blink_iterations;
    int button_status[BUTTON_COUNT];
    int struts[STRUTS_SIZE];
    gchar *hostname;
    gchar *name;
    Time user_time;
    GPid pid;
    Time ping_time;
    unsigned long flags;
    unsigned long wm_flags;
    unsigned long xfwm_flags;

    /* Termination dialog */
    int dialog_pid;
    int dialog_fd;
    /* Timout for asynchronous icon update */
    guint icon_timeout_id;
    /* Timout for asynchronous frame update */
    guint frame_timeout_id;
    /* Timout to manage blinking decorations for urgent windows */
    guint blink_timeout_id;
    /* Timout for asynchronous icon update */
    guint ping_timeout_id;
    /* Opacity for the compositor */
    guint opacity;
    guint opacity_applied;
    guint opacity_flags;

#ifdef HAVE_LIBSTARTUP_NOTIFICATION
    /* Startup notification */
    gchar *startup_id;
#endif /* HAVE_LIBSTARTUP_NOTIFICATION */

#ifdef HAVE_XSYNC
    XSyncAlarm  xsync_alarm;
    XSyncCounter xsync_counter;
    XSyncValue xsync_value;
    guint xsync_timeout_id;
    gboolean xsync_waiting;
    gboolean xsync_enabled;
#endif /* HAVE_XSYNC */
};

extern Client *clients;
extern unsigned int client_count;

Display                 *clientGetXDisplay                      (Client *);
void                     clientClearLastOpTime                  (Client *);
void                     clientUpdateWinState                   (Client *,
                                                                 XClientMessageEvent *);
void                     clientAddUserTimeWin                   (Client *);
void                     clientRemoveUserTimeWin                (Client *);
void                     clientUpdateUrgency                    (Client *);
void                     clientCoordGravitate                   (Client *,
                                                                 int,
                                                                 int *,
                                                                 int *);
void                     clientAdjustCoordGravity               (Client *,
                                                                 unsigned long *,
                                                                 XWindowChanges *);
void                     clientConfigure                        (Client *,
                                                                 XWindowChanges *,
                                                                 unsigned long,
                                                                 unsigned short);
void                     clientGetMWMHints                      (Client *,
                                                                 gboolean);
void                     clientGetWMNormalHints                 (Client *,
                                                                 gboolean);
void                     clientGetWMProtocols                   (Client *);
void                     clientUpdateIcon                       (Client * c);
Client                  *clientFrame                            (DisplayInfo *,
                                                                 Window,
                                                                 gboolean);
void                     clientUnframe                          (Client *,
                                                                 gboolean);
void                     clientFrameAll                         (ScreenInfo *);
void                     clientUnframeAll                       (ScreenInfo *);
void                     clientInstallColormaps                 (Client *);
void                     clientUpdateColormaps                  (Client *);
void                     clientUpdateName                       (Client *);
void                     clientUpdateAllFrames                  (ScreenInfo *,
                                                                 gboolean);
void                     clientGrabButtons                      (Client *);
void                     clientUngrabButtons                    (Client *);
Client                  *clientGetFromWindow                    (Client *,
                                                                 Window,
                                                                 unsigned short);
void                     clientShow                             (Client *,
                                                                 gboolean);
void                     clientHide                             (Client *,
                                                                 int,
                                                                 gboolean);
void                     clientHideAll                          (Client *,
                                                                 int);
void                     clientClearAllShowDesktop              (ScreenInfo *);
void                     clientToggleShowDesktop                (ScreenInfo *);
void                     clientActivate                         (Client *,
                                                                 Time);
void                     clientClose                            (Client *);
void                     clientKill                             (Client *);
void                     clientTerminate                        (Client *);
void                     clientEnterContextMenuState            (Client *);
void                     clientSetLayer                         (Client *,
                                                                 int);
void                     clientSetWorkspace                     (Client *,
                                                                 int,
                                                                 gboolean);
void                     clientShade                            (Client *);
void                     clientUnshade                          (Client *);
void                     clientToggleShaded                     (Client *);
void                     clientStick                            (Client *,
                                                                 gboolean);
void                     clientUnstick                          (Client *,
                                                                 gboolean);
void                     clientToggleSticky                     (Client *,
                                                                 gboolean);
void                     clientToggleFullscreen                 (Client *);
void                     clientToggleLayerAbove                 (Client *);
void                     clientToggleLayerBelow                 (Client *);
void                     clientSetLayerNormal                   (Client *);
void                     clientRemoveMaximizeFlag               (Client *);
void                     clientToggleMaximized                  (Client *,
                                                                 int,
                                                                 gboolean);
void                     clientFill                             (Client *,
                                                                 int);
void                     clientUpdateOpacity                    (Client *);
void                     clientUpdateAllOpacity                 (ScreenInfo *);
void                     clientSetOpacity                       (Client *,
                                                                 guint,
                                                                 guint, guint);
void                     clientIncOpacity                       (Client *);
void                     clientDecOpacity                       (Client *);
void                     clientScreenResize                     (ScreenInfo *);
void                     clientMove                             (Client *,
                                                                 XEvent *);
void                     clientResize                           (Client *,
                                                                 int,
                                                                 XEvent *);
void                     clientCycle                            (Client *,
                                                                 XKeyEvent *);
void                     clientButtonPress                      (Client *,
                                                                 Window,
                                                                 XButtonEvent *);
xfwmPixmap *             clientGetButtonPixmap                  (Client *,
                                                                 int,
                                                                 int);
int                      clientGetButtonState                   (Client *,
                                                                 int,
                                                                 int);
Client                  *clientGetLeader                        (Client *);
#ifdef HAVE_LIBSTARTUP_NOTIFICATION
char                    *clientGetStartupId                     (Client *);
#endif /* HAVE_LIBSTARTUP_NOTIFICATION */
#ifdef HAVE_XSYNC
void                     clientXSyncRequest                     (Client *);
#endif /* HAVE_XSYNC */

#endif /* INC_CLIENT_H */
