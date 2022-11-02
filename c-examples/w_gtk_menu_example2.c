#include <stdio.h>
#include <string.h>

// This app requires GtkApplication, hard dependency...
// gtk2 support can be added later...

#define  USE_GTK_APPLICATION
#include "w_gtk_menu.h"

#if GTK_CHECK_VERSION(3,0,0)

// gcc -o zw_gtk_menu2 w_gtk_menu.c zw_gtk_menu_example2.c -g `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -Wall

#define _(x) x
GtkApplication * app;
GtkWindow * main_window;

// =====================================================================================

static void /* Callback function for the "copy" action */
copy_callback (GSimpleAction *simple, GVariant *parameter, gpointer user_data)
{
    g_print ("\"Copy\" activated\n");
    /* Create a window with a title and a default size */
}

static void /* Callback function for the "paste" action */
paste_callback (GSimpleAction *simple, GVariant *parameter, gpointer user_data)
{
    g_print ("\"Paste\" activated\n");
}


static void /* Callback function for the "shape" action */
shape_callback (GSimpleAction *simple, GVariant *parameter, gpointer  user_data)
{
    /* We first gather the value of the GVariant instance with a string type.
     * The overall goal here is to see if shape is set to line, triangle, etc,
     * and put that value within the variable "answer". */
    const char *answer = g_variant_get_string (parameter, NULL);
    /* Note that we set the state of the action */
    g_simple_action_set_state (simple, parameter);
    //g_simple_action_set_enabled (simple, FALSE);
    printf ("Shape is set to %s.\n", answer);
}


static void /* Callback function for the about action (see aboutdialog.c example) */
about_callback (GSimpleAction *simple, GVariant *parameter, gpointer user_data)
{
    GtkWidget *about_dialog = gtk_about_dialog_new ();
    const char *authors[] =     { "zzz", NULL };
    const char *documenters[] = { "i_i", NULL };
    /* Fill in the about_dialog with the desired information */
    gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG (about_dialog), "AboutDialog Example");
    gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (about_dialog), "Copyright \xc2\xa9 2012 XXXXX");
    gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (about_dialog), authors);
    gtk_about_dialog_set_documenters (GTK_ABOUT_DIALOG (about_dialog), documenters);
    gtk_about_dialog_set_website_label (GTK_ABOUT_DIALOG (about_dialog), "GNOME Developer Website");
    //gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (about_dialog), "");
    gtk_window_set_transient_for (GTK_WINDOW (about_dialog), main_window);
    g_signal_connect_swapped (GTK_DIALOG (about_dialog), "response",
                              G_CALLBACK (gtk_widget_destroy), about_dialog);
    /* Show the about dialog */
    gtk_widget_show (about_dialog);
}

static void activate2 (GtkApplication *app, gpointer user_data)
{
    GtkWidget * window;
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "MenuBar Example");
    gtk_window_set_default_size (GTK_WINDOW (window), 400, 400);
    gtk_window_set_transient_for (GTK_WINDOW (window), GTK_WINDOW (main_window));
    gtk_window_set_modal (GTK_WINDOW (window), TRUE);

    GMenu * menubar = g_menu_new ();
    GMenu * menu_file = g_menu_new ();

    WGtkMenuItemParams menuitem;
    memset (&menuitem, 0, sizeof(menuitem));

    menuitem.parent_menu = menubar;
    menuitem.submenu     = menu_file;
    menuitem.label       = _("_File");
    menuitem.gtk_app     = app;
    menuitem.action_name = "MenuFile";
    w_gtk_menu_item_new (&menuitem);

    menuitem.parent_menu = menu_file;
    menuitem.label       = _("_Close");
    menuitem.action_name = "FileClose";
    w_gtk_menu_item_new (&menuitem);

    gtk_application_set_menubar (app, G_MENU_MODEL (menubar));
    gtk_widget_show_all (window);
}

static void /* Callback function for the "FileNew" action */
new_callback (GSimpleAction *simple, GVariant *parameter, gpointer user_data)
{
    g_print ("You clicked \"New\"\n");
    GtkApplication * app;
    app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate2), NULL);
    g_application_run (G_APPLICATION (app), 0, NULL);
    g_object_unref (app);
}


static void /* Callback function for the "FileQuit" action */
quit_callback (GSimpleAction *simple, GVariant *parameter, gpointer user_data)
{
    GApplication *application = G_APPLICATION (app);
    g_print ("You clicked \"Quit\"\n");
    g_application_quit (application);
}

static void /* Callback function for the "state" action */
state_callback (GSimpleAction *simple, GVariant *parameter, gpointer user_data)
{
    /* The two possibilities in this case for the "answer": "on" or "off" */
    const char *answer = g_variant_get_string (parameter, NULL);
    /* Note that we set the state of the action */
    g_simple_action_set_state (simple, parameter);
    printf ("State is set to %s.\n", answer);
}

static void /* Callback function for the "awesome" action */
awesome_callback (GSimpleAction *simple, GVariant *parameter, gpointer user_data)
{
    GVariant * action_state = g_action_get_state (G_ACTION (simple));
    gboolean active         = g_variant_get_boolean (action_state);
    GVariant * new_state    = g_variant_new_boolean (!active);
    /* Set the new state for the action.
     * (Keeps track of whether it was last checked or unchecked).
     */
    g_simple_action_set_state (simple, new_state);

    if (active)
        g_print ("You unchecked \"Awesome\"\n");
    else
        g_print ("You checked \"Awesome\"\n");
}

// =====================================================================

static void activate (GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    //GSimpleAction * action;

    WGtkMenuItemParams menuitem;
    memset (&menuitem, 0, sizeof(menuitem));

    /* Create a window with a title and a default size */
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "MenuBar Example");
    gtk_window_set_default_size (GTK_WINDOW (window), 400, 400);
    main_window = GTK_WINDOW (window);

    // if (g_action_map_lookup_action (app, "FileNew")) {
    // }

    /* create menus, everything is a menu item, GMenu references a menu without label/header */
    GMenu * menubar   = g_menu_new ();
    GMenu * menu_file = g_menu_new ();
    GMenu * menu_edit = g_menu_new ();
    GMenu * menu_choices[4] = { g_menu_new(), g_menu_new(), g_menu_new(), g_menu_new() };
    GMenu * menu_choices_shapes = g_menu_new ();
    GMenu * menu_help = g_menu_new ();

    // Menubar (File)
    menuitem.parent_menu = menubar;
    menuitem.submenu     = menu_file;
    menuitem.label       = _("_File");
    menuitem.gtk_app     = app; /* 1 */
    menuitem.action_name = "MenuFile";
    w_gtk_menu_item_new (&menuitem);

    // Menubar (Edit)
    menuitem.parent_menu = menubar;
    menuitem.submenu     = menu_edit;
    menuitem.label       = _("_Edit");
    menuitem.action_name = "MenuEdit";
    w_gtk_menu_item_new (&menuitem);

    // Menubar (Choices)
    menuitem.parent_menu = menubar;
    menuitem.submenu     = menu_choices[0];
    menuitem.label       = _("_Choices");
    menuitem.action_name = "MenuChoices";
    w_gtk_menu_item_new (&menuitem);

    // Menubar (Help)
    menuitem.parent_menu = menubar;
    menuitem.submenu     = menu_help;
    menuitem.label       = _("_Help");
    menuitem.action_name = "MenuHelp";
    w_gtk_menu_item_new (&menuitem);

    // -------------------------------
    // File->New
    menuitem.parent_menu = menu_file;
    menuitem.label       = _("_New");
    menuitem.icon_name   = "gtk-new";
    menuitem.accel_str   = "<Control>n";
    menuitem.activate_cb = new_callback;
    menuitem.action_name = "FileNew";
    w_gtk_menu_item_new (&menuitem);

    // File->Quit
    menuitem.parent_menu = menu_file;
    menuitem.label       = _("_Quit");
    menuitem.icon_name   = "gtk-quit";
    menuitem.accel_str   = "<Control>q";
    menuitem.activate_cb = quit_callback;
    menuitem.action_name = "FileQuit";
    w_gtk_menu_item_new (&menuitem);

    // -------------------------------
    // Edit->Copy
    menuitem.parent_menu = menu_edit;
    menuitem.label       = _("_Copy");
    menuitem.accel_str   = "<Control>c";
    menuitem.activate_cb = copy_callback;
    menuitem.action_name = "copy";
    w_gtk_menu_item_new (&menuitem);

    // Edit->Paste
    menuitem.parent_menu = menu_edit;
    menuitem.label       = _("_Paste");
    menuitem.accel_str   = "<Control>v";
    menuitem.activate_cb = paste_callback;
    menuitem.action_name = "paste";
    w_gtk_menu_item_new (&menuitem);

    // -------------------------------
    // Menu Choices - sections - separators
    g_menu_append_section (menu_choices[0], NULL, G_MENU_MODEL (menu_choices[1]));
    g_menu_append_section (menu_choices[0], NULL, G_MENU_MODEL (menu_choices[2]));
    g_menu_append_section (menu_choices[0], NULL, G_MENU_MODEL (menu_choices[3]));

    //               Submenu Shapes
    menuitem.parent_menu = menu_choices[1];
    menuitem.submenu     = menu_choices_shapes;
    menuitem.label       = _("shapes");
    menuitem.action_name = "MenuChoicesShapes";
    w_gtk_menu_item_new (&menuitem);

    // Choices->Shapes->Line
    menuitem.parent_menu = menu_choices_shapes;
    menuitem.label       = _("Line");
    menuitem.action_name = "Choices/Shape";
    menuitem.radio_group = "shape";
    menuitem.radio_id    = "line";
    menuitem.activate_cb = shape_callback;
    w_gtk_menu_item_new (&menuitem);
    // Choices->Shapes->Triangle
    menuitem.label       = _("Triangle");
    menuitem.action_name = "Choices/Shape";
    menuitem.radio_group = "shape";
    menuitem.radio_id    = "triangle";
    menuitem.activate_cb = shape_callback;
    w_gtk_menu_item_new (&menuitem);
    // Choices->Shapes->Square
    menuitem.label       = _("Square");
    menuitem.action_name = "Choices/Shape";
    menuitem.radio_group = "shape";
    menuitem.radio_id    = "square";
    menuitem.activate_cb = shape_callback;
    w_gtk_menu_item_new (&menuitem);
    // Choices->Shapes->Circle
    menuitem.label       = _("Circle");
    menuitem.action_name = "Choices/Shape";
    menuitem.radio_group = "shape";
    menuitem.radio_id    = "circle";
    menuitem.check_state = TRUE;
    menuitem.activate_cb = shape_callback;
    w_gtk_menu_item_new (&menuitem);

    // Choices->On
    menuitem.parent_menu = menu_choices[2];
    menuitem.label       = _("On");
    menuitem.action_name = "Choices/State";
    menuitem.radio_group = "state";
    menuitem.radio_id    = "on";
    menuitem.activate_cb = state_callback;
    w_gtk_menu_item_new (&menuitem);
    // Choices->Off
    menuitem.parent_menu = menu_choices[2];
    menuitem.label       = _("Off");
    menuitem.action_name = "Choices/State";
    menuitem.radio_group = "state";
    menuitem.radio_id    = "off";
    menuitem.check_state = TRUE;
    menuitem.activate_cb = state_callback;
    w_gtk_menu_item_new (&menuitem);

    // Choices->Awesome
    menuitem.parent_menu = menu_choices[3];
    menuitem.label       = _("Awesome");
    menuitem.action_name = "awesome";
    menuitem.accel_str   = "<Alt>a";
    menuitem.checkbox    = TRUE;
    menuitem.check_state = TRUE;
    menuitem.activate_cb = awesome_callback;
    w_gtk_menu_item_new (&menuitem);

    // -------------------------------
    // Help->About
    menuitem.parent_menu = menu_help;
    menuitem.label       = _("_About");
    menuitem.accel_str   = "<Control>h";
    menuitem.activate_cb = about_callback;
    menuitem.action_name = "about";
    w_gtk_menu_item_new (&menuitem);

    gtk_application_set_menubar (GTK_APPLICATION (app), G_MENU_MODEL (menubar));
    gtk_widget_show_all (window);
}


/* Startup function for the menu we are creating in this sample */
static void startup (GtkApplication *app, gpointer  user_data)
{
}

// =====================================================================

/* Startup function for the application */
int main (int argc, char **argv)
{
    //  GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    g_signal_connect (app, "startup", G_CALLBACK (startup), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}

#else
int main (int argc, char **argv)
{
    return 0;
}
#endif