#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "w_gtk.h"

#define _(x) x

GtkWidget * dialog;
GtkWidget * menu;
GtkApplication * app;

GtkWidget *wcombo1;      // GtkComboBoxText
GtkWidget *wcomboentry1; // GtkComboBoxText with entry
#if !GTK_CHECK_VERSION(3,0,0)
GtkWidget *wcombo2;      // GtkOptionMenu
GtkWidget *wcomboentry2; // GtkCombo
#endif

static void on_gtk_button_clicked_stop (GtkButton *widget, gpointer user_data)
{
    GtkWidget *win = GTK_WIDGET (user_data);
    gtk_widget_destroy (win);
}


static const char *items_strv[] =
{
    "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", NULL,
};


static void on_gtk_button_clicked_fill (GtkButton *widget, gpointer user_data)
{
    w_gtk_combo_populate_from_strv (wcombo1, items_strv, 2, FALSE);
    w_gtk_combo_populate_from_strv (wcomboentry1, items_strv, 2, FALSE);

    w_gtk_combo_prepend_text (wcombo1, "prepended");
    w_gtk_combo_prepend_text (wcomboentry1, "prepended");

    w_gtk_combo_insert_text (wcombo1, 6, "inserted");
    w_gtk_combo_insert_text (wcomboentry1, 6, "inserted");

    w_gtk_combo_remove_index (wcombo1, 8);
    w_gtk_combo_remove_index (wcomboentry1, 8);
#if !GTK_CHECK_VERSION(3,0,0)
    w_gtk_combo_populate_from_strv (wcombo2, items_strv, 2, FALSE);
    w_gtk_combo_populate_from_strv (wcomboentry2, items_strv, 2, FALSE);

    w_gtk_combo_prepend_text (wcombo2, "prepended");
    w_gtk_combo_prepend_text (wcomboentry2, "prepended");

    w_gtk_combo_insert_text (wcombo2, 6, "inserted");
    w_gtk_combo_insert_text (wcomboentry2, 6, "inserted");

    w_gtk_combo_remove_index (wcombo2, 8);
    w_gtk_combo_remove_index (wcomboentry2, 8);
#endif
}


static void on_gtk_button_clicked_print_active_text (GtkButton *widget, gpointer user_data)
{
    char *text;
    puts ("** ACTIVE TEXT");
    text = w_gtk_combo_get_active_text (wcombo1);
    printf ("combo1: %s\n", text);
    g_free (text);
    text = w_gtk_combo_get_active_text (wcomboentry1);
    printf ("comboentry1: %s\n", text);
    g_free (text);
#if !GTK_CHECK_VERSION(3,0,0)
    text = w_gtk_combo_get_active_text (wcombo2); // failing: GtkOptionMenu
    printf ("combo2: %s\n", text);
    g_free (text);
    text = w_gtk_combo_get_active_text (wcomboentry2);
    printf ("comboentry2: %s\n", text);
    g_free (text);
#endif
}


static void on_gtk_button_clicked_print_active_index (GtkButton *widget, gpointer user_data)
{
    int x;
    puts ("** COMBO SELECTED INDEX");
    x = w_gtk_combo_get_active_index (wcombo1);
    printf ("combo1: %d\n", x);
    x = w_gtk_combo_get_active_index (wcomboentry1);
    printf ("comboentry1: %d\n", x);
#if !GTK_CHECK_VERSION(3,0,0)
    x = w_gtk_combo_get_active_index (wcombo2);
    printf ("combo2: %d\n", x);
    x = w_gtk_combo_get_active_index (wcomboentry2);
    printf ("comboentry2: %d\n", x);
#endif
}


static void on_gtk_button_clicked_print_count (GtkButton *widget, gpointer user_data)
{
    int x;
    puts ("** COMBO COUNT");
    x = w_gtk_combo_get_count (wcombo1);
    printf ("combo1: %d\n", x);
    x = w_gtk_combo_get_count (wcomboentry1);
    printf ("comboentry1: %d\n", x);
#if !GTK_CHECK_VERSION(3,0,0)
    x = w_gtk_combo_get_count (wcombo2);
    printf ("combo2: %d\n", x);
    x = w_gtk_combo_get_count (wcomboentry2);
    printf ("comboentry2: %d\n", x);
#endif
}


static void on_gtk_button_clicked_find_8 (GtkButton *widget, gpointer user_data)
{
    w_gtk_combo_find_str (wcombo1, "eight", TRUE);
    w_gtk_combo_find_str (wcomboentry1, "eight", TRUE);
#if !GTK_CHECK_VERSION(3,0,0)
    w_gtk_combo_find_str (wcombo2, "eight", TRUE);  //broken: GtkOptionMenu
    w_gtk_combo_find_str (wcomboentry2, "eight", TRUE);
#endif
}


static void on_gtk_button_clicked_clear (GtkButton *widget, gpointer user_data)
{
    w_gtk_combo_clear (wcombo1);
    w_gtk_combo_clear (wcomboentry1);
#if !GTK_CHECK_VERSION(3,0,0)
    w_gtk_combo_clear (wcombo2);
    w_gtk_combo_clear (wcomboentry2);
#endif
}

//===========================================================

static void on_gtk_window_destroy (GtkWidget * object, gpointer udata)
{
    g_application_quit (G_APPLICATION (app));
}

// ==========================================================

void create_window (void)
{
    GtkWidget * textwid;
    GtkWidget * main_vbox, * vbox;
    GtkWidget * combo;
    GtkWidget *w;

    dialog = gtk_application_window_new (app);
    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER_ALWAYS);
    //gtk_widget_set_size_request (GTK_WIDGET (dialog), 200, 100);

    gtk_window_set_title (GTK_WINDOW (dialog), "Getting directory listings");
    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);

    main_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add (GTK_CONTAINER (dialog), main_vbox);

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 7);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
    gtk_container_add (GTK_CONTAINER (main_vbox), vbox);

    textwid = gtk_label_new (NULL);
    gtk_box_pack_start (GTK_BOX (vbox), textwid, TRUE, TRUE, 0);

    textwid = gtk_label_new ("xxxx");
   
    // test W model
    GtkTreeModel *model;
    //combo = gtk_combo_box_new ();          // works ok
    
#if GTK_MAJOR_VERSION == 2
   // combo = gtk_combo_box_new (); // entry broken with this
    combo = gtk_combo_box_entry_new (); // entry broken with this
    //combo = gtk_combo_box_new_with_entry (); // works ok
#else
    combo = gtk_combo_box_new_with_entry (); // works ok
#endif
    w_gtk_combo_set_w_model (combo, FALSE);
    model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
    printf ("model cols: %d\n", gtk_tree_model_get_n_columns (model));
    printf ("has-entry : %d\n", gtk_combo_box_get_has_entry (GTK_COMBO_BOX(combo)));
    w_gtk_combo_append_text (combo, "mmm");
    w_gtk_combo_append_text (combo, "zzz");
    w_gtk_combo_append_text (combo, "bbb");
    w_gtk_combo_append_text (combo, "abby road");
    w_gtk_combo_append_text (combo, "might");
    w_gtk_combo_append_text (combo, "question");
    gtk_combo_box_set_active (GTK_COMBO_BOX(combo), 0);
    w_gtk_combo_find_str (combo, "question", TRUE);
    gtk_box_pack_start (GTK_BOX(vbox), combo, TRUE, TRUE, 0);

    wcombo1 = gtk_combo_box_text_new ();
    wcomboentry1 = gtk_combo_box_text_new_with_entry ();

    gtk_box_pack_start (GTK_BOX(vbox), wcombo1, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(vbox), wcomboentry1, TRUE, TRUE, 0);

#if !GTK_CHECK_VERSION(3,0,0)
    wcombo2 = gtk_option_menu_new ();
    gtk_option_menu_set_menu (GTK_OPTION_MENU(wcombo2), gtk_menu_new());

    wcomboentry2 = gtk_combo_new ();
    gtk_editable_set_editable (GTK_EDITABLE(GTK_COMBO(wcomboentry2)->entry), FALSE);

    gtk_box_pack_start (GTK_BOX(vbox), wcombo2, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(vbox), wcomboentry2, TRUE, TRUE, 0);
#endif

    //===

    w = gtk_button_new_with_label ("FILL combos");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_fill), NULL);

    w = gtk_button_new_with_label ("Print ITEM count");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_print_count), NULL);

    w = gtk_button_new_with_label ("Print SELECTED index");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_print_active_index), NULL);

    w = gtk_button_new_with_label ("Print ACTIVE text");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_print_active_text), NULL);

    w = gtk_button_new_with_label ("Find 'eight'");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_find_8), NULL);

    w = gtk_button_new_with_label ("CLEAR combos");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_clear), NULL);

    w = gtk_button_new_with_label ("  QUIT  ");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_stop), dialog);

    gtk_widget_add_events (dialog, GDK_BUTTON_RELEASE_MASK);
    g_signal_connect (dialog, "destroy", G_CALLBACK(on_gtk_window_destroy), NULL);

    gtk_widget_show_all (dialog);
}


// ==========================================================

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

