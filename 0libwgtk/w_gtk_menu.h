/*
 * Public Domain

 w_gtk_menuitem_new (WGtkMenuItemParams*):

   By default it works with GtkMenu stuff, define one of these macros
   before including this header to change the behavior:
  
   #define USE_GTK_ACTION 1      - (GTK2.4+) use GtkAction
   #define USE_GTK_APPLICATION 1 - (GTK3.0+) use GMenu, GtkApplication, etc
 
   GtkMenu works with GTK 1/2/3
 
 (HOWTO: see some of the apps using this for more details...)
*/

// ==============================================================
// w_gtk_menu.c

#ifndef __W_GTK_MENU__
#define __W_GTK_MENU__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include "w_gtk.h"

#ifndef gettext
#define gettext(x) (x)
#endif

#ifdef USE_GTK_APPLICATION
#if !GTK_CHECK_VERSION(3,4,0)
// GtkApplicationWindow supports the GtkApplication menubar
// All the windows share the same GtkApplication menubar, sad
#error "w_gtk_menu: GtkApplicationWindow requires GTK 3.4+"
#endif
#endif

#ifdef USE_GTK_ACTION
#if !GTK_CHECK_VERSION(2,4,0)
#error "w_gtk_menu: GtkAction requires gtk 2.4+"
#endif
#endif

#if (!defined(USE_GTK_APPLICATION) && GTK_MAJOR_VERSION >= 4)
#define USE_GTK_APPLICATION 1
#endif


// ==============================================================

#if defined(USE_GTK_APPLICATION) && GTK_CHECK_VERSION(3,4,0)

#define WGtkMenuBar  GMenu
#define WGtkMenu     GMenu
#define w_gtk_menu_bar_new g_menu_new
#define w_gtk_menu_new     g_menu_new

#define WGtkMenuItem GSimpleAction
#define WGTK_MENU_ITEM G_SIMPLE_ACTION

#else /* GtkMenu */

#define WGtkMenuBar  GtkWidget
#define WGtkMenu     GtkWidget
#define w_gtk_menu_bar_new gtk_menu_bar_new
#define w_gtk_menu_new     gtk_menu_new

#if defined(USE_GTK_ACTION) && GTK_CHECK_VERSION(2,4,0)
#define WGtkMenuItem GtkAction
#define WGTK_MENU_ITEM GTK_ACTION
#else
#define WGtkMenuItem GtkMenuItem
#define WGTK_MENU_ITEM GTK_MENU_ITEM
#endif

#endif

// ==============================================================

typedef struct _WGtkMenuItemParams
{
    // Common vars
    gpointer parent_menu;     // GtkWidget* (GtkMenu*) or GMenu
    gpointer submenu;         // GtkWidget* (GtkMenu*) or GMenu
    const char * label;       // GtkMenuItem: empty = separator
                              // GMenu: empty = ignore  
    const char * icon_name;
    const char * icon_alt;    // alternative icon name

    void * activate_cb;       /* callback func for the activate signal */
                              //    on_menuitem_cb (gpointer *item)
    void * checked_cb;        // custom callback func for check menuitems (does not support cb_data)
                              //    on_menuitem_toggled_cb (gpointer *item, gboolean is_checked)
    void * radio_cb;          // custom callback func for radio menuitems (does not support cb_data)
                              //    void on_menuitem_radio_changed_cb (gpointer *item, const char *radio_id)
    void * cb_data;           /* callback data
                                 this is ignored when using custom callback functions */
    void * cb_data_all;       /* callback for all signals
                                 this is ignored when using custom callback functions */
    const char * accel_str;   // i.e: "<Control>n"
    gboolean checkbox;
    gboolean check_state;
    const char * radio_id;
    const char * tooltip;
    WGtkActionEntry *actions;
    const char * action_name; // for GAction/GtkAction, should always be specified...

    // GMenu
    gpointer gtk_app;         // GtkApplication

    // GtkMenu
    const char * radio_group; // GtkMenu/GtkAction. GAction = action_name
    const char * accel_path;
    gpointer accel_group;     // GtkAccelGroup*: provided or automatically created

    // GtkMenu with GtkAction, this is rarely used
    gpointer action_group;    // GtkActionGroup
} WGtkMenuItemParams;


// ==============================================================
// Functions to use:

void w_gtk_menuitem_set_enabled (gpointer menuitem, gboolean enabled);
void w_gtk_menuitem_set_checked (gpointer menuitem, gboolean state);
gboolean w_gtk_menuitem_get_checked (gpointer menuitem);
void w_gtk_menuitem_params_init (WGtkMenuItemParams *params, WGtkActionEntry *actions);
void w_gtk_menuitem_add_radio_changed_handler (gpointer menuitem, gpointer cb, gpointer cb_data);
void w_gtk_menuitem_add_checked_handler (gpointer menuitem, gpointer cb, gpointer cb_data);


#if GTK_CHECK_VERSION(3,4,0)
GSimpleAction * w_gtk_menuitem_new_gaction (WGtkMenuItemParams * params);
#endif
#if GTK_MAJOR_VERSION <= 3
GtkMenuItem * w_gtk_menuitem_new_gtkmenu (WGtkMenuItemParams * params);
# if GTK_CHECK_VERSION(2,4,0)
GtkAction * w_gtk_menuitem_new_gtkaction (WGtkMenuItemParams * params);
# endif
#endif


#if !defined(W_GTK_MENU_C)

// ==============================================================
//                w_gtk_menuitem_new
// ==============================================================

///static WGtkMenuItem * w_gtk_menuitem_new (WGtkMenuItemParams * params);

#if defined(USE_GTK_APPLICATION) && GTK_CHECK_VERSION(3,4,0)
// GtkApplication, GMenu, etc
#define w_gtk_menuitem_new w_gtk_menuitem_new_gaction

#elif defined(USE_GTK_ACTION) && GTK_CHECK_VERSION(2,4,0)
// GtkAction, GtkActionGroup, etc
#define w_gtk_menuitem_new w_gtk_menuitem_new_gtkaction

#else
// GtkMenu, Gtk*MenuItem, GtkMenuBar
#define w_gtk_menuitem_new w_gtk_menuitem_new_gtkmenu
#endif

#endif

// ==============================================================

#endif /* __W_GTK_MENU__ */

