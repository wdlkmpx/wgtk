#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "w_gtk_menu.h"

// https://developer.gnome.org/gnome-devel-demos/stable/menubar.c.html.en
// https://developer.gnome.org/gnome-devel-demos/stable/gmenu.c.html.en
// https://developer.gnome.org/gnome-devel-demos/stable/c.html.en
// https://stackoverflow.com/questions/33911544/gtk3-gmenuitem-with-icon-alignment
// https://wiki.gnome.org/HowDoI/GAction

// gcc -o zw_gtk_menu w_gtk.c w_gtk_menu.c zw_gtk_menu_example.c -g `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0` -Wall
// gcc -o zw_gtk_menu w_gtk.c w_gtk_menu.c zw_gtk_menu_example.c -g `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -Wall

#define _(x) x

GtkWidget * dialog;
GtkWidget * menu;
GtkApplication * app;

static void
on_gtk_button_clicked_stop (GtkButton *widget, gpointer user_data)
{
    GtkWidget *win = GTK_WIDGET (user_data);
    gtk_widget_destroy (win);
}

static gboolean on_gtk_window_ButtonReleaseEvent (GtkWidget * widget,
                              GdkEventButton * event, gpointer data)
{
    if (event->button == 3)
    { // right click
        gtk_menu_popup (GTK_MENU (menu), NULL, NULL, NULL, NULL, event->button, event->time);
    }
    return (FALSE);
}

static void on_gtk_window_destroy (GtkWidget * object, gpointer udata)
{
    g_application_quit (G_APPLICATION (app));
} 


// ===============================================================================

#if GTK_MAJOR_VERSION >= 3

static void on_menuitem_newfolder (GSimpleAction *act, GVariant *param, gpointer data) {
    fprintf (stderr, "new folder!\n");
}

static void on_menuitem_newitem (GSimpleAction *act, GVariant *param, gpointer data) {
    fprintf (stderr, "new item!\n");
}

static void on_menuitem_delete (GSimpleAction *act, GVariant *param, gpointer data) {
    fprintf (stderr, "delete !\n");
}

static void on_menuitem_properties (GSimpleAction *act, GVariant *param, gpointer data) {
    fprintf (stderr, "properties!\n");
}

static void on_menuitem_close (GSimpleAction *act, GVariant *param, gpointer data) {
    fprintf (stderr, "close!\n");
}


static void create_menu_bar (GtkWindow *window)
{
    static const GActionEntry actions[] =
    {
        { "newfolder", on_menuitem_newfolder  },
        { "newitem",   on_menuitem_newitem    },
        { "delete",    on_menuitem_delete     },
        { "properties",on_menuitem_properties },
        { "close",     on_menuitem_close      },
    };
    static const char * accel_new_item[] = { "<Control>n", NULL };
    static const char * accel_close[]    = { "<Control>w", NULL };
    gtk_application_set_accels_for_action (app, "app.newitem", accel_new_item);
    gtk_application_set_accels_for_action (app, "app.close",   accel_close);
    g_action_map_add_action_entries (G_ACTION_MAP (app), actions,
                                     G_N_ELEMENTS (actions), app);

    GMenu * menubar;
    GMenu * menu_file;

    menubar  = g_menu_new ();
    menu_file = g_menu_new ();
    g_menu_append_submenu (menubar, "_File", G_MENU_MODEL (menu_file));
    g_menu_append (menu_file, "New _Folder", "app.newfolder");
    g_menu_append (menu_file, "New _Item",   "app.newitem");
    g_menu_append (menu_file, "_Delete",     "app.delete");
    g_menu_append (menu_file, "_Properties", "app.properties");
    g_menu_append (menu_file, "_Close",      "app.close");

    gtk_application_set_menubar (app, G_MENU_MODEL (menubar));

    g_object_unref (menubar);
    g_object_unref (menu_file);
}


#else // ============= GTK2 ==============

static void on_menuitem_newfolder (GtkMenuItem *menuitem, gpointer data) {
    fprintf (stderr, "new folder!\n");
}
static void on_menuitem_newitem (GtkMenuItem *menuitem, gpointer data) {
    fprintf (stderr, "new item!\n");
}
static void on_menuitem_delete (GtkMenuItem *menuitem, gpointer data) {
    fprintf (stderr, "delete !\n");
}
static void on_menuitem_properties (GtkMenuItem *menuitem, gpointer data) {
    fprintf (stderr, "properties!\n");
}
static void on_menuitem_close (GtkMenuItem *menuitem, gpointer data) {
    fprintf (stderr, "close!\n");
}

static void on_menuitem_edit (void *menuitem, gpointer data)
{
    // - activate/toggled/changed trigger this cb twice with different menuitems
    // - the menuitem being deactivated and the menuitem being activated
    // - we only want to know activated menuitems
    int active = FALSE;
    int index;
    char * id;
#if defined(USE_GTK_ACTION) && GTK_CHECK_VERSION(2,4,0)
    const char * name;
    active = gtk_toggle_action_get_active (GTK_TOGGLE_ACTION(menuitem));
    if (active) {
        name = gtk_action_get_name (GTK_ACTION(menuitem));
        puts (name);
        index = gtk_radio_action_get_current_value (GTK_RADIO_ACTION(menuitem));
        printf ("%d\n", index);
    }
#else
    active = gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(menuitem));
    if (active) {
        index = GPOINTER_TO_INT (g_object_get_data (G_OBJECT(menuitem), "index"));
        printf ("%d\n", index);
    }
#endif
    if (active) {
        id = g_object_get_data (G_OBJECT(menuitem), "id");
        if (id) {
            puts (id);
        }
    }
}

static void on_menuitem_show (void *menuitem, gpointer data)
{
    gboolean active;
#if defined(USE_GTK_ACTION) && GTK_CHECK_VERSION(2,4,0)
    active = gtk_toggle_action_get_active (GTK_TOGGLE_ACTION (menuitem));
    //gtk_action_set_visible   (GTK_ACTION (menuitem), FALSE);
    //gtk_action_set_sensitive (GTK_ACTION (menuitem), FALSE);
#else
    active = gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM (menuitem));
#endif
    if (active)
        puts ("Item has been ACTIVATED");
    else
        puts ("Item has been de-ACTIVATED");
}

// ===============================================================================

static GtkWidget * create_menu_bar (GtkWindow *window)
{
    GtkWidget * menubar   = gtk_menu_bar_new ();
    GtkWidget * menu_file = gtk_menu_new ();
    GtkWidget * menu_edit = gtk_menu_new ();
    GtkWidget * menu_help = gtk_menu_new ();
    menu = menu_file;

    GtkAccelGroup *accel_group = gtk_accel_group_new ();
    gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);

    GtkActionGroup * amap = NULL;
#if defined(USE_GTK_ACTION) && GTK_CHECK_VERSION(2,4,0)
    amap = gtk_action_group_new ("menu");
#endif

    WGtkMenuItemParams menuitem;
    memset (&menuitem, 0, sizeof(menuitem));

    // Menu File
    menuitem.parent_menu = menubar;
    menuitem.submenu     = menu_file;
    menuitem.label       = _("_File");
    menuitem.accel_group = accel_group; /* 1 */
    menuitem.action_name = "MenuFile";
    menuitem.gtk_app     = amap; /* 1 */
    menuitem.tooltip     = "aaaaaaaa bbbb cccc ";
    w_gtk_menu_item_new (&menuitem);

    // Menu Help
    menuitem.parent_menu = menubar;
    menuitem.submenu     = menu_help;
    menuitem.label       = _("_Help");
    menuitem.action_name = "MenuHelp";
    w_gtk_menu_item_new (&menuitem);

    // Menu Edit
    menuitem.parent_menu = menubar;
    menuitem.submenu     = menu_edit;
    menuitem.label       = _("_Edit");
    menuitem.action_name = "MenuEdit";
    w_gtk_menu_item_new (&menuitem);
    
    // ---------------------------------
    
    // File->New folder
    menuitem.parent_menu = menu_file;
    menuitem.label       = _("New _Folder...");
    menuitem.activate_cb = on_menuitem_newfolder;
    menuitem.action_name = "FileNewFolder";
    w_gtk_menu_item_new (&menuitem);

    // File->New Item
    menuitem.parent_menu = menu_file;
    menuitem.label       = _("New _Item...");
    menuitem.icon_name   = "gtk-new";
    menuitem.activate_cb = on_menuitem_newitem;
    menuitem.accel_str   = "<Control>n";
    menuitem.action_name = "FileNewItem";
    w_gtk_menu_item_new (&menuitem);

    // File->Delete
    menuitem.parent_menu = menu_file;
    menuitem.label       = _("_Delete");
    menuitem.icon_name   = "gtk-delete";
    menuitem.activate_cb = on_menuitem_delete;
    menuitem.action_name = "FileDelete";
    w_gtk_menu_item_new (&menuitem);

    // File->Properties
    menuitem.parent_menu = menu_file;
    menuitem.label       = _("_Properties");
    menuitem.icon_name   = "gtk-properties";
    menuitem.activate_cb = on_menuitem_properties;
    menuitem.action_name = "FileProperties";
    w_gtk_menu_item_new (&menuitem);

    // File-> ----
    menuitem.parent_menu = menu_file;
    w_gtk_menu_item_new (&menuitem);

    // File->Close
    menuitem.parent_menu = menu_file;
    menuitem.label       = _("_Close");
    menuitem.icon_name   = "gtk-close";
    menuitem.activate_cb = on_menuitem_close;
    menuitem.accel_str   = "<Control>w";
    menuitem.action_name = "FileClose";
    w_gtk_menu_item_new (&menuitem);

    // File-> ----
    menuitem.parent_menu = menu_file;
    w_gtk_menu_item_new (&menuitem);
    
    // File->Show
    menuitem.parent_menu = menu_file;
    menuitem.label       = _("_Show");
    menuitem.checkbox    = TRUE;
    menuitem.check_state = TRUE;
    menuitem.activate_cb = on_menuitem_show;
    menuitem.accel_str   = "<Control>s";
    menuitem.action_name = "FileShow";
    w_gtk_menu_item_new (&menuitem);

    // Edit->One
    menuitem.parent_menu = menu_edit;
    menuitem.label       = "One";
    menuitem.action_name = "Edit/Number";
    menuitem.radio_group = "edit";
    menuitem.radio_id    = "edit1";
    menuitem.activate_cb = on_menuitem_edit;
    w_gtk_menu_item_new (&menuitem);

    // Edit->Two
    menuitem.parent_menu = menu_edit;
    menuitem.label       = "Two";
    menuitem.action_name = "Edit/Number";
    menuitem.radio_group = "edit";
    menuitem.radio_id    = "edit2";
    menuitem.activate_cb = on_menuitem_edit;
    w_gtk_menu_item_new (&menuitem);

    // Edit->Three
    menuitem.parent_menu = menu_edit;
    menuitem.label       = "Three";
    menuitem.action_name = "Edit/Number";
    menuitem.radio_group = "edit";
    menuitem.radio_id    = "edit3";
    menuitem.check_state = TRUE;
    menuitem.activate_cb = on_menuitem_edit;
    w_gtk_menu_item_new (&menuitem);

    // Edit->Four
    menuitem.parent_menu = menu_edit;
    menuitem.label       = "Four";
    menuitem.action_name = "Edit/Number";
    menuitem.radio_group = "edit";
    menuitem.radio_id    = "edit4";
    menuitem.activate_cb = on_menuitem_edit;
    w_gtk_menu_item_new (&menuitem);

    gtk_widget_show_all (menubar);

    //w_gtk_action_group_destroy_action (amap, "FileShow");

    return (menubar);
}

#endif

// ===============================================================================

void create_window (void)
{
   GtkWidget * textwid, * stopwid;
   GtkWidget * main_vbox, * vbox;
   GtkWidget * menubar;

   dialog = gtk_application_window_new (app);
   gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER_ALWAYS);
   gtk_widget_set_size_request (GTK_WIDGET (dialog), 200, 100);

   gtk_window_set_title (GTK_WINDOW (dialog), "WGTKMENU TEST");
   gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);

   main_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
   gtk_container_add (GTK_CONTAINER (dialog), main_vbox);

#if GTK_MAJOR_VERSION < 3
   menubar = create_menu_bar (GTK_WINDOW (dialog));
   gtk_box_pack_start (GTK_BOX (main_vbox), menubar, FALSE, FALSE, 2);
#else
   create_menu_bar (GTK_WINDOW (dialog));
#endif

   vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
   gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
   gtk_container_add (GTK_CONTAINER (main_vbox), vbox);

   textwid = gtk_label_new (NULL);
   gtk_box_pack_start (GTK_BOX (vbox), textwid, TRUE, TRUE, 0);

   textwid = gtk_label_new ("xxxx");
  
   stopwid = gtk_button_new_with_label ("  Stop  ");
   gtk_box_pack_start (GTK_BOX (vbox), stopwid, TRUE, TRUE, 0);
   g_signal_connect (stopwid, "clicked",
                     G_CALLBACK (on_gtk_button_clicked_stop), dialog);

   g_signal_connect (dialog, "destroy",
                     G_CALLBACK(on_gtk_window_destroy), NULL);

   g_signal_connect (dialog, "button_release_event",
                     G_CALLBACK (on_gtk_window_ButtonReleaseEvent), NULL);

   gtk_widget_add_events (dialog, GDK_BUTTON_RELEASE_MASK);

   gtk_widget_show_all (dialog);
}

int main (int argc, char *argv[])
{
#if GTK_MAJOR_VERSION >= 3
   app = gtk_application_new ("org.gtk.app", G_APPLICATION_FLAGS_NONE);
   g_signal_connect (app, "activate", G_CALLBACK(create_window), NULL);
   g_application_run (G_APPLICATION (app), argc, argv);
   return 0;
#else  
   gtk_init (&argc, &argv);

   create_window ();

   gtk_main ();
#endif
	return 0;
}

