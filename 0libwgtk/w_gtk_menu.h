/*
 * Public Domain
  
 This header file includes the implementation (.c) 
  
 By default it works with GtkMenu stuff, define one of these macros
 before including this header to change the behavior:
  
 #define USE_GTK_ACTION 1      - use GtkAction
 #define USE_GTK_APPLICATION 1 - use GMenu, GtkApplication, etc
 
 (HOWTO: see some of the apps using this for more details...)
 */

// ==============================================================
// w_gtk_menu.c

#ifndef __W_GTK_MENU__
#define __W_GTK_MENU__

/*#if !defined(USE_GTK_APPLICATION)
# if defined(__clang__)
#   pragma clang diagnostic ignored "-Wdeprecated-declarations"
# elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wdeprecated-declarations"
# endif
#endif*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include "w_gtk.h"

#ifndef gettext
#define gettext(x) (x)
#endif

// ==============================================================
// w_gtk_menu.h

#ifdef USE_GTK_APPLICATION

#define WGtkMenuBar  GMenu
#define WGtkMenu     GMenu
#define WGtkMenuItem GSimpleAction
#define WGTK_MENU_ITEM G_SIMPLE_ACTION
#define w_gtk_menu_bar_new g_menu_new
#define w_gtk_menu_new     g_menu_new

typedef struct _WGtkMenuItemParams
{
    /* GSimpleAction */
    const char * action_name;
    const char * radio_id;
    GtkApplication * gtk_app;
    const char * accel_str;
    void * activate_cb;
    void * cb_data;
    void * cb_data_all;
    //--
    gboolean checkbox;
    gboolean check_state;
    /* GMenuItem */
    GMenu * parent_menu;
    GMenu * submenu;
    const char * label;
    const char * radio_group;
    const char * icon_name;
    const char * icon_alt;
    WGtkActionEntry *actions;
    // unused
    void *accel_group;
    const char *accel_path;
    const char * tooltip;
} WGtkMenuItemParams;

#else /*============ GtkMenu ============*/

#define WGtkMenuBar  GtkWidget
#define WGtkMenu     GtkWidget
#ifdef USE_GTK_ACTION
#define WGtkMenuItem GtkAction
#else
#define WGtkMenuItem GtkMenuItem
#endif
#define WGTK_MENU_ITEM GTK_MENU_ITEM
#define w_gtk_menu_bar_new gtk_menu_bar_new
#define w_gtk_menu_new     gtk_menu_new

typedef struct _WGtkMenuItemParams
{
    GtkWidget * parent_menu;
    GtkWidget * submenu;
    const char * label;           /* GtkMenuItem: empty = separator  */
    const char * icon_name;
    const char * icon_alt;
    void * activate_cb;           /* callback func for the activate signal */
    void * cb_data;               /* callback data      */
    void * cb_data_all;           /* callback for all signals */
    const char * accel_str;       /* i.e: "<Control>n"  */
    GtkAccelGroup * accel_group;  // provided or automatically created
    const char * accel_path;
    gboolean checkbox;
    gboolean check_state;
    const char * radio_group;
    const char * radio_id;
    const char * tooltip;
//#ifdef USE_GTK_ACTION
    GtkActionGroup * gtk_app;
    const char * action_name;
//#endif
    WGtkActionEntry *actions;
} WGtkMenuItemParams;

#endif

// ==============================================================
// Functions to use:

static WGtkMenuItem * w_gtk_menu_item_new (WGtkMenuItemParams * params);
static void w_gtk_menu_item_set_enabled (WGtkMenuItem *menuitem, gboolean enabled);
static void w_gtk_menu_item_set_checked (WGtkMenuItem *menuitem, gboolean state);
static gboolean w_gtk_menu_item_get_checked (WGtkMenuItem *menuitem);
static void w_gtk_menu_item_params_init (WGtkMenuItemParams *params, WGtkActionEntry *actions);

// ==============================================================

void w_gtk_menu_item_set_enabled (WGtkMenuItem *menuitem, gboolean enabled)
{
#if defined(USE_GTK_APPLICATION)
    g_simple_action_set_enabled (G_SIMPLE_ACTION(menuitem), enabled);
#elif defined(USE_GTK_ACTION)
    gtk_action_set_sensitive (GTK_ACTION(menuitem), enabled);
#else
    gtk_widget_set_sensitive (GTK_WIDGET(menuitem), enabled);
#endif
}


void w_gtk_menu_item_set_checked (WGtkMenuItem *menuitem, gboolean state)
{
#if defined(USE_GTK_APPLICATION)
    GVariant * xstate;
    xstate = g_variant_new_boolean (state);
    g_action_change_state (G_ACTION(menuitem), xstate);
#elif defined(USE_GTK_ACTION)
    gtk_toggle_action_set_active (GTK_TOGGLE_ACTION(menuitem), state);
#else
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(menuitem), state);
#endif
}


gboolean w_gtk_menu_item_get_checked (WGtkMenuItem *menuitem)
{
    gboolean ret;
#if defined(USE_GTK_APPLICATION)
    GVariant * state;
    state = g_action_get_state (G_ACTION(menuitem));
    ret = g_variant_get_boolean (state);
#elif defined(USE_GTK_ACTION)
    ret = gtk_toggle_action_get_active (GTK_TOGGLE_ACTION(menuitem));
#else
    ret = gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(menuitem));
#endif
    return ret;
}


void w_gtk_menu_item_params_init (WGtkMenuItemParams *params, WGtkActionEntry *actions)
{
    memset (params, 0, sizeof(WGtkMenuItemParams));
    if (actions) {
        params->actions = actions;
    }
}

// ==============================================================
//                w_gtk_menu_item_new
// ==============================================================

static void menuitem_params_from_actions (WGtkMenuItemParams *params,
                                          WGtkActionEntry *actions)
{  // check if params->action_name is in the action group
    int found = 0;
    int i;
    for (i = 0; actions[i].name; i++)
    {
        if (strcmp(actions[i].name,params->action_name) == 0) {
            found = 1;
            break;
        }
    }
    if (!found) {
        printf ("not found: %s\n", params->action_name);
        return; // action_name not found
    }
    // only override non NULL fields...
    if (actions[i].icon_name) params->icon_name  = actions[i].icon_name;
    if (actions[i].label)     params->label      = gettext(actions[i].label);
    if (actions[i].accel_str) params->accel_str  = actions[i].accel_str;
    if (actions[i].tooltip)   params->tooltip    = gettext(actions[i].tooltip);
    if (actions[i].callback)  params->activate_cb = actions[i].callback;
}


static const char * get_valid_icon_name (const char *icon1, const char *icon2)
{
    GtkIconTheme *icon_theme = gtk_icon_theme_get_default ();
    if (gtk_icon_theme_has_icon (icon_theme, icon1)) {
        return icon1;
    } else if (strncmp (icon1, "gtk-", 4) == 0) {
        return icon1;
    } else if (icon2 && *icon2) {
        if (gtk_icon_theme_has_icon (icon_theme, icon2)) {
            return icon2;
        } else if (strncmp (icon2, "gtk-", 4) == 0) {
            return icon2;
        }
        fprintf (stderr, "(menu) %s was not found in icon theme\n", icon2);
    } else {
        fprintf (stderr, "(menu) %s was not found in icon theme\n", icon1);
    }
    return NULL;
}


static void menuitem_params_reset (WGtkMenuItemParams *params)
{
    // keep some values that must overridden manually (by the uzer)
    params->action_name = NULL;
    params->radio_id = NULL;
    ///params->gtk_app = NULL;
    params->accel_str = NULL;
    params->activate_cb = NULL;
    params->cb_data = NULL;
    ///params->cb_data_all = NULL;
    params->checkbox = FALSE;
    params->check_state = FALSE;
    ///params->parent_menu = NULL;
    params->submenu = NULL;
    params->label = NULL;
    params->radio_group = NULL;
    params->icon_name = NULL;
    params->icon_alt = NULL;
    ///params->actions = NULL;
    ///params->accel_group = NULL;
    params->accel_path = NULL;
}

// ==============================================================
// GtkApplication
// --------------

#if defined(USE_GTK_APPLICATION)

static GSimpleAction * new_action (GtkApplication * gtk_app,  /* required */
                                   const char * action_name,  /* required */
                                   const char * radio_id,
                                   const char * accel_str,
                                   gboolean checkbox,
                                   gboolean check_state,
                                   gpointer action_callback,
                                   gpointer cb_data)
{
    // only GtkApplication (app.xxx) actions are supported
    // GTK only supports a menubar per GtkApplication ... lame
    GSimpleAction * action = NULL;
    GVariantType * type_string = NULL;
    GActionMap * amap = G_ACTION_MAP (gtk_app); /* GtkApplication implements GActionMap */

    // check if action_name exists, if so then return the existing action
    action = (GSimpleAction *) g_action_map_lookup_action (amap, action_name);
    if (action)
    {
        if (radio_id && check_state == TRUE) {
            // hack, radio item is selected
            g_simple_action_set_state (action, g_variant_new_string (radio_id));
        }
        return action;
    }

    // 1=normal  2=check  3=radio target
    int type=1;
    if (checkbox) type=2;
    if (radio_id) type=3;

    switch (type)
    {
        case 1: /* normal */
            action = g_simple_action_new (action_name, NULL);
            break;
        case 2: /* check */
            action = g_simple_action_new_stateful (action_name, NULL,
                                                   g_variant_new_boolean (check_state));
            break;
        case 3: /* radio target */
            type_string = g_variant_type_new ("s");
            action = g_simple_action_new_stateful (action_name, type_string,
                                                   g_variant_new_string (radio_id));
            g_variant_type_free (type_string);
            break;
    }

    if (action_callback) {
        g_signal_connect (action, "activate", G_CALLBACK (action_callback), cb_data);
    }
    g_action_map_add_action (amap, G_ACTION (action));

    if (accel_str) {
        char * detailed_action = g_strconcat ("app.", action_name, NULL);  // e.g: app.new
#if GTK_CHECK_VERSION (3, 12, 0)
        const char * accels[] = { accel_str, NULL };
        gtk_application_set_accels_for_action (gtk_app, detailed_action, accels);
        g_free (detailed_action);
#else // 3.4-
        gtk_application_add_accelerator (gtk_app, accel_str, detailed_action, NULL);
#endif
    }
    return action;
}


static GMenuItem * new_menuitem (GMenu * menuitem_parent,
                                 GMenu * menuitem_submenu,
                                 const char * menuitem_label,
                                 const char * action_name,
                                 const char * menuitem_target,
                                 const char * menuitem_icon)
{
    GMenuItem * mitem;
    char * detailed_action = NULL;
    if (action_name) {
        detailed_action = g_strconcat ("app.", action_name, NULL);
    }

    mitem = g_menu_item_new (menuitem_label, detailed_action);
    if (menuitem_target) {
        g_menu_item_set_attribute (mitem, "target", "s", menuitem_target);
    }
    if (menuitem_icon && *menuitem_icon) {
        ///g_menu_item_set_attribute (mitem, "icon", "s", menuitem_icon);
        GIcon * icon = g_themed_icon_new (menuitem_icon);
        g_menu_item_set_icon (mitem, icon);
        g_object_unref (icon);
    }

    if (menuitem_submenu) { // always add submenu before appending to parent menu
        g_menu_item_set_submenu (mitem, G_MENU_MODEL (menuitem_submenu));
    }
    if (menuitem_parent) {
        g_menu_append_item (menuitem_parent, mitem);
    }

    if (detailed_action) {
        g_free (detailed_action);
    }
    g_object_unref (mitem);
    return mitem;
}

//--

GSimpleAction * w_gtk_menu_item_new (WGtkMenuItemParams * params)
{
    GSimpleAction * action = NULL;

    if (params->actions && params->action_name) {
        menuitem_params_from_actions (params, params->actions);
    }

    // the action is added to GtkApplication * app (GActionMap)
    // if action is a menu, then this returns the GMenu:
    //                      g_object_get_data (action, "menu");
    if (params->action_name)
    {   // no action is created if action_name = NULL
        action = new_action (params->gtk_app,
                             params->action_name,
                             params->radio_id,
                             params->accel_str,
                             params->checkbox,
                             params->check_state,
                             params->activate_cb,
                             params->cb_data ? params->cb_data : params->cb_data_all);
    }
    new_menuitem (params->parent_menu,
                  params->submenu,
                  params->label,
                  params->action_name,
                  params->radio_id,
                  params->icon_name);

    // if action is a menu, retrieve the GMenu like this:
    //           g_object_get_data (action, "menu");
    if (action && params->action_name && params->submenu) {
        g_object_set_data (G_OBJECT (action), "menu", (gpointer) params->submenu);
    }

    // reset params structure so that it can be used for the next menu item
    menuitem_params_reset (params);

    return action;
}


#elif defined(USE_GTK_ACTION)
// ====================================================================
// GtkAction
// ---------

GtkAction * w_gtk_menu_item_new (WGtkMenuItemParams * params)
{
    GtkWidget * item = NULL;
    GtkAction * action = NULL;
    void *tmp;
    GSList * list = NULL;
    if (!params->radio_group && params->radio_id) {
        params->radio_group = params->action_name;
    }

    if (params->actions && params->action_name) {
        menuitem_params_from_actions (params, params->actions);
    }

    /* 0=normal 1=image 2=check 3=separator 4=radio*/
    int type = 0;
    if (params->icon_name
        && *params->icon_name) type = 1;
    if (params->checkbox)      type = 2;
    if (params->label == NULL) type = 3;
    if (params->radio_group)   type = 4;

    switch (type)
    {
        case 0: /* normal */
            action = gtk_action_new (params->action_name, params->label, params->tooltip, NULL);
            item = gtk_action_create_menu_item (action);
            break;
        case 1: /* image */
            action = gtk_action_new (params->action_name, params->label, params->tooltip, NULL);
            tmp = (void*) get_valid_icon_name (params->icon_name, params->icon_alt);
            if (tmp) {
                gtk_action_set_icon_name (action, (char*)tmp);
            } else {
                gtk_action_set_icon_name (action, params->icon_name);
            }
            item = gtk_action_create_menu_item (action);
            break;
        case 2: /* check */
            action = GTK_ACTION (gtk_toggle_action_new (params->action_name, params->label, params->tooltip, NULL));
            item = gtk_action_create_menu_item (action);
            break;
        case 3: /* separator */
            item = gtk_separator_menu_item_new ();
            break;
        case 4: /* radio */
            tmp = g_object_get_data (G_OBJECT(params->parent_menu), params->radio_group);
            if (tmp) {
                list = gtk_radio_action_get_group (GTK_RADIO_ACTION(tmp));
            }
            // GtkRadioAction is a special GtkToggleAction for widget groups
            action = GTK_ACTION (gtk_radio_action_new (params->action_name, params->label,
                                                       params->tooltip, NULL, g_slist_length(list)));
            gtk_radio_action_set_group (GTK_RADIO_ACTION(action), list);
            // store last GtkRadioAction in parent_menu (key: params->radio_group)
            g_object_set_data (G_OBJECT(params->parent_menu), params->radio_group, action);
            if (params->radio_id) {
                g_object_set_data (G_OBJECT(action), "id", (void*)params->radio_id);
            }
            item = gtk_action_create_menu_item (action);
            break;
    }
    // by default the checkbox/radio is not ticked
    if (params->check_state == TRUE) {
        gtk_toggle_action_set_active (GTK_TOGGLE_ACTION (action), TRUE);
    }

    if (params->submenu) {
        gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), params->submenu);
        // gtk_action_set_accel_path() requires this, otherwise this isn't needed
        if (params->accel_group) {
            gtk_menu_set_accel_group (GTK_MENU (params->submenu), params->accel_group);
        }
    }
    if (params->parent_menu) {
        gtk_menu_shell_append (GTK_MENU_SHELL (params->parent_menu), item);
    }

    if (params->activate_cb) {
        g_signal_connect (action, params->checkbox ? "toggled" : "activate",
                          G_CALLBACK (params->activate_cb),
                          params->cb_data ? params->cb_data : params->cb_data_all);
    }

    if (params->accel_str)
    {
        if (!params->accel_group) {
            params->accel_group = gtk_accel_group_new ();
        }
        guint accel_key = 0;            // GDK_KEY_n
        GdkModifierType accel_mods = 0; // GDK_CONTROL_MASK
        gtk_accelerator_parse (params->accel_str, &accel_key, &accel_mods);
        gtk_widget_add_accelerator (GTK_WIDGET (item), "activate",
                                    params->accel_group, accel_key, accel_mods,
                                    GTK_ACCEL_VISIBLE);
    }
    if (params->gtk_app && action) {
        if (!gtk_action_group_get_action (params->gtk_app, params->action_name)) {
            gtk_action_group_add_action (params->gtk_app, action);
        }
    }
    if (params->accel_group && action) {
        gtk_action_set_accel_group (action, params->accel_group);
    }
    if (params->accel_path) {
        gtk_action_set_accel_path (action, params->accel_path);
    }

    // reset params structure so that it can be used for the next menu item
    menuitem_params_reset (params);

    // popup menus require this
    gtk_widget_show (item);

    return action;
}


#else
// ====================================================================
// GtkMenu
// -------

GtkMenuItem * w_gtk_menu_item_new (WGtkMenuItemParams * params)
{
    GtkWidget * item = NULL;
    GtkWidget *img;
    void *tmp;
    GSList * list = NULL;
    if (!params->radio_group && params->radio_id) {
        params->radio_group = params->action_name;
    }

    if (params->actions && params->action_name) {
        menuitem_params_from_actions (params, params->actions);
    }

    /* 0=normal 1=image 2=check 3=separator 4=radio*/
    int type = 0;
    if (params->icon_name
        && *params->icon_name) type = 1;
    if (params->checkbox)      type = 2;
    if (params->label == NULL) type = 3;
    if (params->radio_group)   type = 4;

    switch (type)
    {
        case 0: /* normal */
            item = gtk_menu_item_new_with_mnemonic (params->label);
            break;
        case 1: /* image */
            item = gtk_image_menu_item_new_with_mnemonic (params->label);
            tmp = (void*) get_valid_icon_name (params->icon_name, params->icon_alt);
            if (tmp) {
                img = w_gtk_image_new_from_icon_name ((char*)tmp, GTK_ICON_SIZE_MENU);
            } else {
                img = w_gtk_image_new_from_icon_name (params->icon_name, GTK_ICON_SIZE_MENU);
            }
            gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (item), img);
            break;
        case 2: /* check */
            item = gtk_check_menu_item_new_with_mnemonic (params->label);
            break;
        case 3: /* separator */
            item = gtk_separator_menu_item_new ();
            break;
        case 4: /* radio */
            // GtkRadioMenuItem is a special GtkCheckMenuItem for widget groups
            tmp = g_object_get_data (G_OBJECT(params->parent_menu), params->radio_group);
            if (tmp) {
                list = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (tmp));
            }
            item = gtk_radio_menu_item_new_with_mnemonic (list, params->label);
            // store last GtkRadioMenuItem in parent_menu (key: params->radio_group)
            g_object_set_data (G_OBJECT(params->parent_menu), params->radio_group, item);
            // store index in menu item (key: index)
            g_object_set_data (G_OBJECT(item), "index", GINT_TO_POINTER(g_slist_length(list)));
            if (params->radio_id) {
                g_object_set_data (G_OBJECT(item), "id", (void*)params->radio_id);
            }
            break;
    }
    // by default the checkbox/radio is not ticked
    if (params->check_state == TRUE) {
        gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (item), params->check_state);
    }

    if (params->submenu) {
        gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), params->submenu);
        // gtk_menu_item_set_accel_path() requires this, otherwise this isn't needed
        if (params->accel_group) {
            gtk_menu_set_accel_group (GTK_MENU (params->submenu), params->accel_group);
        }
    }
    if (params->parent_menu) {
        gtk_menu_shell_append (GTK_MENU_SHELL (params->parent_menu), item);
        // store pointer to menuitem in parent menu (key: params->action_name)
        if (params->action_name) {
            g_object_set_data (G_OBJECT(params->parent_menu),
                               params->action_name,
                               (void*)item);
        }
    }

    if (params->activate_cb) {
        g_signal_connect (item, params->checkbox ? "toggled" : "activate",
                          G_CALLBACK (params->activate_cb),
                          params->cb_data ? params->cb_data : params->cb_data_all);
    }

    if (params->accel_str)
    {
        if (!params->accel_group) {
            params->accel_group = gtk_accel_group_new ();
        }
        guint accel_key = 0;            // GDK_KEY_n
        GdkModifierType accel_mods = 0; // GDK_CONTROL_MASK
        gtk_accelerator_parse (params->accel_str, &accel_key, &accel_mods);
        gtk_widget_add_accelerator (GTK_WIDGET (item), "activate",
                                    params->accel_group, accel_key, accel_mods,
                                    GTK_ACCEL_VISIBLE);
    }
    if (params->accel_path) {
        gtk_menu_item_set_accel_path (GTK_MENU_ITEM (item), params->accel_path);
    }

    if (params->tooltip) {
        gtk_widget_set_tooltip_text (GTK_WIDGET(item), params->tooltip);
    }

    // reset params structure so that it can be used for the next menu item
    menuitem_params_reset (params);

    // popup menus require this
    gtk_widget_show (item);

    return GTK_MENU_ITEM(item);
}
#endif

// ========================================================================

#endif /* __W_GTK_MENU__ */
