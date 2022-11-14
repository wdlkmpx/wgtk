/*
 * Public Domain
 */

// Supported widgets:
// - GAction
// - GtkAction   / GtkToggleAction  / GtkRadioAction
// - GtkMenuItem / GtkCheckMenuItem / GtkRadioMenuItem
//
// - When it comes to radio items, GAction uses the same action
//    and the only thing that changes is the state, in GtkAction/GtkMenuItem
//    every radio item is a widget/object that is passed in the callback func
// - GTK3, the callback function that receives the toggled/changed_state signal
//   has to manually set the action state, otherwise nothing changes (visually)
//

/*
custom signal handlers:
- checked_cb
- radio_cb
*/

#if defined(__clang__)
#   pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
//#include "w_gtk.h"

#define W_GTK_MENU_C 1
#include "w_gtk_menu.h"

#ifdef ENABLE_NLS
#include <locale.h>
#include <libintl.h>
#else
#define gettext(x) (x)
#endif

///#define W_GTKMENU_DEBUG

#ifdef W_GTKMENU_DEBUG
# define W_DEBUG_PUTS(text) puts(text) 
#else
# define W_DEBUG_PUTS(text)
#endif


/*--------------------------------------------------------------
/                the "checked" signal
/-------------------------------------------------------------*/

/*
GtkCheckMenuItem : toggled   
GtkToggleAction  : toggled
GSimpleAction    :

How to handle the "checked" signal:

static void on_menuitem_checked_cb (gpointer *item, gboolean is_checked)
{
    gpointer menuitem = w_gtk_menuitem_cb_ensure_item (item);
    if (!menuitem) {
        return;
    }
    if (is_checked) {
        // the menuitem was checked
    } else {
        // the menuitem was unchecked
    }
}
*/

#if GTK_CHECK_VERSION(3,4,0)
static void gaction_checked_cb (GSimpleAction *simple, GVariant *parameter, gpointer  user_data)
{
    void (*func) (gpointer, gboolean);
    func = user_data;
    GVariant * action_state = g_action_get_state (G_ACTION (simple));
    gboolean active         = g_variant_get_boolean (action_state);
    GVariant * new_state    = g_variant_new_boolean (!active);
    // set the state of the action (this is not done automatically)
    g_simple_action_set_state (simple, new_state);
    // call real cb..
    func (simple, !active);
}
#endif

#if GTK_MAJOR_VERSION <= 3
static void gtkmenuitem_checked_cb (gpointer menuitem, gpointer user_data)
{
    gboolean checked;
    void (*func) (gpointer, gboolean);
    checked = w_gtk_menuitem_get_checked (menuitem);
    func = user_data;
    // call real cb..
    func (menuitem, checked);
}
#endif


void w_gtk_menuitem_add_checked_handler (gpointer menuitem, gpointer cb, gpointer cb_data)
{
#if GTK_CHECK_VERSION(3,4,0)
    if (G_IS_SIMPLE_ACTION(menuitem)) {
        g_signal_connect (menuitem, "activate", G_CALLBACK(gaction_checked_cb), cb);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 3
    if (GTK_IS_MENU_ITEM(menuitem)
#if GTK_CHECK_VERSION(2,4,0)
        || GTK_IS_ACTION(menuitem)
#endif
       )
    {
        g_signal_connect (menuitem, "activate", G_CALLBACK(gtkmenuitem_checked_cb), cb);
        return;
    }
#endif
    g_warning ("Invalid widget type for WGtkMenuItem");
}


/*--------------------------------------------------------------
/               the "radio_changed" signal
/-------------------------------------------------------------*/

/* 
GtkCheckMenuItem : 
GtkToggleAction  :  
GSimpleAction    :

How to handle the "radio_changed" signal:

static void on_menuitem_radio_changed_cb (gpointer *item, const char *radio_id)
{
    
}
*/

#if GTK_CHECK_VERSION(3,4,0)
static void gaction_radio_changed_cb (GSimpleAction *simple, GVariant *parameter, gpointer  user_data)
{
    const char *radio_id;
    void (*func) (gpointer, const char*);
    radio_id = g_variant_get_string (parameter, NULL);
    func = user_data;
    // set the state of the action (this is not done automatically)
    g_simple_action_set_state (simple, parameter);
    printf ("radio_id is %s.\n", radio_id);
    // call real cb..
    func (simple, radio_id);
}
#endif

#if GTK_MAJOR_VERSION <= 3
static void gtkmenuitem_radio_changed_cb (gpointer menuitem, gpointer user_data)
{
    // - activate/toggled/changed trigger this cb twice with different menuitems
    // - the menuitem being deactivated and the menuitem being activated
    // - we only want to know about activated menuitems
    const char *radio_id = NULL;
    void (*func) (gpointer, const char*);
    gboolean active = w_gtk_menuitem_get_checked (menuitem);
    if (active) {
        radio_id = g_object_get_data (G_OBJECT(menuitem), "id");
        g_return_if_fail (radio_id != NULL);
        func = user_data;
        printf ("radio_id is %s.\n", radio_id);
        // call real cb..
        func (menuitem, radio_id);
    }
}
#endif


void w_gtk_menuitem_add_radio_changed_handler (gpointer menuitem, gpointer cb, gpointer cb_data)
{
#if GTK_CHECK_VERSION(3,4,0)
    if (G_IS_SIMPLE_ACTION(menuitem)) {
        g_signal_connect (menuitem, "activate", G_CALLBACK(gaction_radio_changed_cb), cb);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 3
    if (GTK_IS_MENU_ITEM(menuitem)
#if GTK_CHECK_VERSION(2,4,0)
        || GTK_IS_ACTION(menuitem)
#endif
       )
    {
        g_signal_connect (menuitem, "activate", G_CALLBACK(gtkmenuitem_radio_changed_cb), cb);
        return;
    }
#endif
    g_warning ("Invalid widget type for WGtkMenuItem");
}


/*--------------------------------------------------------------
/             w_gtk_menuitem_set_enabled
/-------------------------------------------------------------*/

void w_gtk_menuitem_set_enabled (gpointer menuitem, gboolean enabled)
{
#if GTK_CHECK_VERSION(3,4,0)
    if (G_IS_SIMPLE_ACTION(menuitem)) {
        g_simple_action_set_enabled (G_SIMPLE_ACTION(menuitem), enabled);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 3
    if (GTK_IS_MENU_ITEM(menuitem)) {
        gtk_widget_set_sensitive (GTK_WIDGET(menuitem), enabled);
        return;
    }
# if GTK_CHECK_VERSION(2,4,0)
    if (GTK_IS_ACTION(menuitem)) {
        gtk_action_set_sensitive (GTK_ACTION(menuitem), enabled);
        return;
    }
# endif
#endif
    g_warning ("Invalid widget type for WGtkMenuItem");
}


/*--------------------------------------------------------------
/             w_gtk_menuitem_set_checked
/-------------------------------------------------------------*/

void w_gtk_menuitem_set_checked (gpointer menuitem, gboolean state)
{
#if GTK_CHECK_VERSION(3,4,0)
    if (G_IS_SIMPLE_ACTION(menuitem)) {
        GVariant * xstate;
        xstate = g_variant_new_boolean (state);
        g_action_change_state (G_ACTION(menuitem), xstate);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 3
    if (GTK_IS_MENU_ITEM(menuitem)) {
        gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(menuitem), state);
        return;
    }
# if GTK_CHECK_VERSION(2,4,0)
    if (GTK_IS_ACTION(menuitem)) {
        gtk_toggle_action_set_active (GTK_TOGGLE_ACTION(menuitem), state);
        return;
    }
# endif
#endif
    g_warning ("Invalid widget type for WGtkMenuItem");
}


/*--------------------------------------------------------------
/           w_gtk_menuitem_get_checked     
/-------------------------------------------------------------*/

gboolean w_gtk_menuitem_get_checked (gpointer menuitem)
{
    gboolean ret;
#if GTK_CHECK_VERSION(3,4,0)
    if (G_IS_SIMPLE_ACTION(menuitem)) {
        GVariant * state;
        state = g_action_get_state (G_ACTION(menuitem));
        ret = g_variant_get_boolean (state);
        return ret;
    }
#endif
#if GTK_MAJOR_VERSION <= 3
    if (GTK_IS_MENU_ITEM(menuitem)) {
        ret = gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(menuitem));
        return ret;
    }
# if GTK_CHECK_VERSION(2,4,0)
    if (GTK_IS_ACTION(menuitem)) {
        ret = gtk_toggle_action_get_active (GTK_TOGGLE_ACTION(menuitem));
        return ret;
    }
# endif
#endif
    g_warning ("Invalid widget type for WGtkMenuItem");
    return FALSE;
}


/*--------------------------------------------------------------
/             w_gtk_menuitem_params_init
/-------------------------------------------------------------*/

void w_gtk_menuitem_params_init (WGtkMenuItemParams *params, WGtkActionEntry *actions)
{
    memset (params, 0, sizeof(WGtkMenuItemParams));
    if (actions) {
        params->actions = actions;
    }
}


// ==============================================================
//                w_gtk_menuitem_new_*
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


#if GTK_CHECK_VERSION(2,4,0) // GtkIconTheme was introduced in gtk 2.4
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
#endif


static void menuitem_params_reset (WGtkMenuItemParams *params)
{
    // keep some values that must overridden manually (by the uzer)
    params->action_name = NULL;
    params->radio_id = NULL;
    ///params->gtk_app = NULL;
    params->accel_str = NULL;
    params->activate_cb = NULL;
    params->checked_cb = NULL;
    params->radio_cb = NULL;
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
    params->action_group = NULL;
    params->accel_path = NULL;
}



//===================================================================================
//                GMenu, GAction, GtkApplication, etc...
//===================================================================================

#if GTK_CHECK_VERSION(3,4,0)

//GtkApplication * gtk_app,  /* required */
//const char * action_name,  /* required */
static GSimpleAction * new_gaction (WGtkMenuItemParams *params)
{
    // only GtkApplication (app.xxx) actions are supported
    // GTK only supports a menubar per GtkApplication ... lame
    GSimpleAction * action = NULL;
    GVariantType * type_string = NULL;

    // check if action_name exists, if so then return the existing action
    action = (GSimpleAction *) g_action_map_lookup_action (G_ACTION_MAP (params->gtk_app),
                                                           params->action_name);
    if (action)
    {
        if (params->radio_id && params->check_state == TRUE) {
            // hack, radio item is selected
            g_simple_action_set_state (action, g_variant_new_string (params->radio_id));
        }
        return action;
    }

    // 1=normal  2=check  3=radio target
    int type=1;
    if (params->checkbox) type=2;
    if (params->radio_id) type=3;

    switch (type)
    {
        case 1: /* normal */
            action = g_simple_action_new (params->action_name, NULL);
            break;
        case 2: /* check */
            action = g_simple_action_new_stateful (params->action_name, NULL,
                                                   g_variant_new_boolean (params->check_state));
            break;
        case 3: /* radio target */
            type_string = g_variant_type_new ("s");
            action = g_simple_action_new_stateful (params->action_name, type_string,
                                                   g_variant_new_string (params->radio_id));
            g_variant_type_free (type_string);
            break;
    }
    if (params->checked_cb) {
        w_gtk_menuitem_add_checked_handler (action, params->checked_cb, params->cb_data);
    }
    else if (params->radio_cb) {
        w_gtk_menuitem_add_radio_changed_handler (action, params->radio_cb, params->cb_data);
    }
    else if (params->activate_cb) {
        g_signal_connect (action, "activate", G_CALLBACK(params->activate_cb), params->cb_data);
    }
    g_action_map_add_action (G_ACTION_MAP (params->gtk_app),
                             G_ACTION (action));
    if (params->accel_str) {
        char * detailed_action = g_strconcat ("app.", params->action_name, NULL);  // e.g: app.new
#if GTK_CHECK_VERSION (3, 12, 0)
        const char * accels[] = { params->accel_str, NULL };
        gtk_application_set_accels_for_action (GTK_APPLICATION(params->gtk_app),
                                               detailed_action, accels);
#else // 3.4+
        gtk_application_add_accelerator (GTK_APPLICATION(gtk_app),
                                         params->accel_str, detailed_action, NULL);
#endif
        g_free (detailed_action);
    }
    return action;
}


GSimpleAction * w_gtk_menuitem_new_gaction (WGtkMenuItemParams * params)
{
    GSimpleAction * action = NULL;

    if (params->actions && params->action_name) {
        menuitem_params_from_actions (params, params->actions);
    }

    g_warn_if_fail(params->action_name != NULL);
    g_warn_if_fail(params->gtk_app != NULL);

    // - the action is added to GtkApplication * app (GActionMap)
    // - for radio items, new_gaction may return an existing action rather than creating a new one
    // - if action is a menu, then this returns the GMenu:
    //                      g_object_get_data (action, "menu");
    action = new_gaction (params);

    // == new GMenuItem ==
    GMenuItem * menuitem;
    char * detailed_action = NULL;
    const char * menuitem_target = params->radio_id;
    if (params->action_name) {
        detailed_action = g_strconcat ("app.", params->action_name, NULL);
    }
    menuitem = g_menu_item_new (params->label, detailed_action);
    if (menuitem_target) {
        g_menu_item_set_attribute (menuitem, "target", "s", menuitem_target);
    }
    if (params->icon_name && *params->icon_name) {
#if GLIB_CHECK_VERSION(2,38,0)
        GIcon * icon;
        char *icon_names[3] = { (char*) params->icon_name, (char*) params->icon_alt, NULL };
        icon = g_themed_icon_new_from_names (icon_names, -1);
        ///icon = g_themed_icon_new (params->icon_name);
        g_menu_item_set_icon (menuitem, icon);
        g_object_unref (icon);
#else
        g_menu_item_set_attribute (menuitem, "icon", "s", params->icon_name);
#endif
    }
    if (params->submenu) { // always add submenu before appending to parent menu
        g_menu_item_set_submenu (menuitem, G_MENU_MODEL(params->submenu));
    }
    if (params->parent_menu != NULL) {
        g_menu_append_item (G_MENU(params->parent_menu), menuitem);
    }
    if (detailed_action) {
        g_free (detailed_action);
    }
    g_object_unref (menuitem);
    // == end of new GMenuItem ==

    // if action is a menu, retrieve the GMenu like this:
    //           g_object_get_data (action, "menu");
    if (action && params->action_name && params->submenu) {
        g_object_set_data (G_OBJECT (action), "menu", (gpointer) params->submenu);
    }
    // reset params structure so that it can be used for the next menu item
    menuitem_params_reset (params);

    return action;
}

#endif

//===================================================================================
//                   GtkAction, GtkActionGroup, etc
//===================================================================================

#if GTK_CHECK_VERSION(2,4,0) && GTK_MAJOR_VERSION <= 3

GtkAction * w_gtk_menuitem_new_gtkaction (WGtkMenuItemParams * params)
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
                                                       params->tooltip, NULL,
                                                       g_slist_length(list)));
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
            gtk_menu_set_accel_group (GTK_MENU (params->submenu),
                                      GTK_ACCEL_GROUP (params->accel_group));
        }
    }
    if (params->parent_menu) {
        gtk_menu_shell_append (GTK_MENU_SHELL (params->parent_menu), item);
    }

    if (params->checked_cb) {
        w_gtk_menuitem_add_checked_handler (action, params->checked_cb, params->cb_data);
    }
    else if (params->radio_cb) {
        w_gtk_menuitem_add_radio_changed_handler (action, params->radio_cb, params->cb_data);
    }
    else if (params->activate_cb) {
        g_signal_connect (action, params->checkbox ? "toggled" : "activate",
                          G_CALLBACK (params->activate_cb),
                          params->cb_data ? params->cb_data : params->cb_data_all);
    }

    if (params->accel_str)
    {
        if (!params->accel_group) {
            params->accel_group = (gpointer) gtk_accel_group_new ();
        }
        guint accel_key = 0;            // GDK_KEY_n
        GdkModifierType accel_mods = 0; // GDK_CONTROL_MASK
        gtk_accelerator_parse (params->accel_str, &accel_key, &accel_mods);
        gtk_widget_add_accelerator (GTK_WIDGET (item), "activate",
                                    GTK_ACCEL_GROUP (params->accel_group),
                                    accel_key, accel_mods, GTK_ACCEL_VISIBLE);
    }
    if (params->action_group && action) {
        if (!gtk_action_group_get_action (GTK_ACTION_GROUP(params->action_group), params->action_name)) {
            gtk_action_group_add_action (GTK_ACTION_GROUP(params->action_group), action);
        }
    }
    if (params->accel_group && action) {
        gtk_action_set_accel_group (action, GTK_ACCEL_GROUP(params->accel_group));
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

#endif


//===================================================================================
//                   GtkMenu, Gtk*MenuItem, GtkMenuBar
//===================================================================================

#if GTK_MAJOR_VERSION <= 3

GtkMenuItem * w_gtk_menuitem_new_gtkmenu (WGtkMenuItemParams * params)
{
    GtkWidget * item = NULL;
#if GTK_CHECK_VERSION(2,0,0)
    GtkWidget *img;
#endif
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
#if GTK_CHECK_VERSION(2,0,0)
            item = gtk_image_menu_item_new_with_mnemonic (params->label);
            tmp = (void*) get_valid_icon_name (params->icon_name, params->icon_alt);
            if (tmp) {
                img = w_gtk_image_new_from_icon_name ((char*)tmp, GTK_ICON_SIZE_MENU);
            } else {
                img = w_gtk_image_new_from_icon_name (params->icon_name, GTK_ICON_SIZE_MENU);
            }
            gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (item), img);
#else // gtk1 can't handle images other than pixmaps (xpm)
            item = gtk_menu_item_new_with_label (params->label);
#endif
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
            gtk_menu_set_accel_group (GTK_MENU (params->submenu),
                                      GTK_ACCEL_GROUP (params->accel_group));
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

    if (params->checked_cb) {
        w_gtk_menuitem_add_checked_handler (item, params->checked_cb, params->cb_data);
    }
    else if (params->radio_cb) {
        w_gtk_menuitem_add_radio_changed_handler (item, params->radio_cb, params->cb_data);
    }
    else if (params->activate_cb) {
        g_signal_connect (item, params->checkbox ? "toggled" : "activate",
                          G_CALLBACK (params->activate_cb),
                          params->cb_data ? params->cb_data : params->cb_data_all);
    }

#if GTK_MAJOR_VERSION == 1
    // gtk_*_menu_item_with_mnemonics don't exist in gtk1, they are wgtkcompat2 macros to *_menu_item_new_with_label
    // weird way to mnemonics to gtk menu items...
    // https://www.geany.org/manual/gtk/gtk-faq/x781.html / gtkitemfactory.c
    if (params->label && strchr(params->label,'_'))
    {
        guint akey = gtk_label_parse_uline (GTK_LABEL(GTK_BIN(item)->child), params->label);
        if (akey != GDK_VoidSymbol) {
            if (GTK_IS_MENU_BAR (params->parent_menu)) {
                if (!params->accel_group) {
                    params->accel_group = (gpointer) gtk_accel_group_new ();
                }
                gtk_widget_add_accelerator (item, "activate_item",
                                            GTK_ACCEL_GROUP (params->accel_group),
                                            akey, GDK_MOD1_MASK,   GTK_ACCEL_LOCKED);
            }
            if (GTK_IS_MENU (params->parent_menu)) {
                gtk_widget_add_accelerator (item, "activate_item",
                                            gtk_menu_ensure_uline_accel_group (GTK_MENU(params->parent_menu)),
                                            akey, 0,   GTK_ACCEL_LOCKED);
            }
        }
    }
#endif
    if (params->accel_str)
    {
        if (!params->accel_group) {
            params->accel_group = (gpointer) gtk_accel_group_new ();
        }
        guint accel_key = 0;            // GDK_KEY_n
        GdkModifierType accel_mods = 0; // GDK_CONTROL_MASK
        gtk_accelerator_parse (params->accel_str, &accel_key, &accel_mods);
        gtk_widget_add_accelerator (GTK_WIDGET (item), "activate",
                                    GTK_ACCEL_GROUP (params->accel_group),
                                    accel_key, accel_mods, GTK_ACCEL_VISIBLE);
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

