/*
 * Public Domain
 */

// WGtkCombo is modeled after GtkComboBoxText

#if defined(__clang__)
#   pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include "w_gtk.h"

#ifdef ENABLE_NLS
#include <locale.h>
#include <libintl.h>
#else
#define gettext(x) (x)
#endif

/*
  https://developer-old.gnome.org/gtk2/stable/gtk-migrating-GtkComboBox.html
  https://developer-old.gnome.org/gtk2/stable/migrating-GtkCombo.html

  - GtkComboBoxText            = GtkOptionMenu
  - GtkComboBoxText with entry = GtkCombo

  - https://developer-old.gnome.org/gtk2/stable/GtkComboBox.html
      https://developer-old.gnome.org/gtk2/stable/GtkComboBoxEntry.html
      https://developer-old.gnome.org/gtk2/stable/GtkComboBoxText.html
      - https://developer-old.gnome.org/gtk2/stable/GtkTreeModel.html
          https://developer-old.gnome.org/gtk2/stable/GtkListStore.html

  - https://refspecs.linuxbase.org/gtk/2.6/gtk/GtkOptionMenu.html
  - https://refspecs.linuxbase.org/gtk/2.6/gtk/GtkCombo.html
       https://refspecs.linuxbase.org/gtk/2.6/gtk/GtkList.html
          https://refspecs.linuxbase.org/gtk/2.6/gtk/GtkListItem.html

    http://irtfweb.ifa.hawaii.edu/SoftwareDocs/
*/


//===============================================================
// new W combo, for reference
//===============================================================

GtkWidget * w_gtk_combo_new ()
{
    GtkWidget * combo;

//#if GTK_CHECK_VERSION(4,10,0)
//    combo = gtk_drop_down_new ();
#if GTK_CHECK_VERSION(2,24,0)
    combo = gtk_combo_box_text_new (); // deprecated in GTK 4.10
#elif GTK_CHECK_VERSION(2,4,0)
    combo = gtk_combo_box_new_text (); // deprecated in GTK 2.24
#elif !GTK_CHECK_VERSION(2,4,0)
    // GtkOptionMenu is ugly and not usable for complex operations
    ///combo = gtk_option_menu_new (); // deprecated in GTK 2.4
    ///gtk_option_menu_set_menu (GTK_OPTION_MENU(combo), gtk_menu_new());
    combo = gtk_combo_new (); // deprecated in GTK 2.4
    gtk_editable_set_editable (GTK_EDITABLE(GTK_COMBO(combo)->entry), FALSE);
#endif
    return combo;
}


GtkWidget * w_gtk_combo_new_with_entry ()
{
    GtkWidget * combo;
#if GTK_CHECK_VERSION(2,24,0)
    combo = gtk_combo_box_text_new_with_entry ();  // deprecated in GTK 4.10
#elif GTK_CHECK_VERSION(2,4,0)
    combo = gtk_combo_box_entry_new_text ();  // deprecated in GTK 2.24
#elif !GTK_CHECK_VERSION(2,4,0)
    combo = gtk_combo_new ();  // deprecated in GTK 2.4
#endif
    return combo;
}



//===============================================================
// [BASIC FUNCTIONS] that require handling different widget types
//===============================================================

GtkWidget * w_gtk_combo_get_entry (GtkWidget *combo)
{
    GtkWidget *entry = NULL;
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo))
    {
        return entry;
    }
#endif
#if (GTK_MAJOR_VERSION <= 4 && GTK_CHECK_VERSION(2,4,0))
    if (GTK_IS_COMBO_BOX(combo))
    {
        entry = gtk_bin_get_child (GTK_BIN(combo));
        if (entry && GTK_IS_ENTRY(entry)) {
            return entry;
        }
        return NULL;
    }
#endif
#if GTK_MAJOR_VERSION <= 2 //GTK1 widgets that also supported in GTK2 (but deprecated)
    if (GTK_IS_COMBO(combo)) // GtkOptionMenu doesn't have an entry
    {
        entry = GTK_COMBO(combo)->entry;
    }
    // else: GtkOptionMenu and GtkComboBox without entry
#endif
    return entry;
}


void w_gtk_combo_clear (GtkWidget *combo)
{
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo)) // this has not been tested
    {
        GListModel * model;
        model = gtk_drop_down_get_model (GTK_DROP_DOWN(combo));
        g_list_store_remove_all (G_LIST_STORE(model));
        return;
    }
#endif
#if (GTK_MAJOR_VERSION <= 4 && GTK_CHECK_VERSION(2,4,0))
    if (GTK_IS_COMBO_BOX(combo))
    {
        GtkTreeModel *model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
        GtkWidget *entry = gtk_bin_get_child (GTK_BIN(combo));
        g_warn_if_fail (GTK_IS_LIST_STORE(model));
        gtk_list_store_clear (GTK_LIST_STORE(model)); // it shouldn't be a GtkTreeStore
        if (entry && GTK_IS_ENTRY(entry)) {
            gtk_entry_set_text (GTK_ENTRY(entry), "");
        }
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2 //GTK1 widgets that also supported in GTK2 (but deprecated)
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget *list  = GTK_COMBO(combo)->list;
        GtkWidget *entry = GTK_COMBO(combo)->entry;
        gtk_list_clear_items (GTK_LIST(list), 0, -1);
        gtk_entry_set_text (GTK_ENTRY(entry), "");
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        // this is essentially broken..
        GtkWidget *menu, *menuitem;
        GList *items, *igl;
        menu  = gtk_option_menu_get_menu (GTK_OPTION_MENU(combo));
        if (!menu) {
            return;
        }
        items = GTK_MENU_SHELL(menu)->children; // gtk_container_get_children
        if (!items) {
            return;
        }
        for (igl = g_list_last(items); igl != NULL; igl = igl->prev) {
            menuitem = GTK_WIDGET(igl->data);
            gtk_widget_destroy (menuitem);
        }
        // the lines above don't seem to work, nothing is removed
        gtk_option_menu_remove_menu (GTK_OPTION_MENU(combo));
        gtk_option_menu_set_menu (GTK_OPTION_MENU(combo), gtk_menu_new());
    }
#endif
}


int w_gtk_combo_get_count (GtkWidget *combo)
{
    int count = 0;
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo))
    {
        return count;
    }
#endif
#if (GTK_MAJOR_VERSION <= 4 && GTK_CHECK_VERSION(2,4,0))
    if (GTK_IS_COMBO_BOX(combo))
    {
        GtkTreeModel * model;
        GtkTreeIter iter;
        gboolean valid;
        model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
        ///count = gtk_tree_model_iter_n_children (model, NULL);
        valid = gtk_tree_model_get_iter_first (model, &iter);
        while (valid) {
            count++;
            valid = gtk_tree_model_iter_next (model, &iter);
        }
        return count;
    }
#endif
#if GTK_MAJOR_VERSION <= 2 //GTK1 widgets that also supported in GTK2 (but deprecated)
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget  *list = GTK_COMBO(combo)->list;
        GList     *items = GTK_LIST(list)->children;
        count = g_list_length (items);
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        GtkWidget *menu = gtk_option_menu_get_menu (GTK_OPTION_MENU(combo));
        GList *items = GTK_MENU_SHELL(menu)->children;
        count = g_list_length (items);
    }
#endif
    return count;
}


void w_gtk_combo_insert_text (GtkWidget *combo, int position, const char *text)
{
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo))
    {
        return;
    }
#endif
#if (GTK_MAJOR_VERSION <= 4 && GTK_CHECK_VERSION(2,4,0))
    if (GTK_IS_COMBO_BOX(combo))
    {
        GtkListStore *list;
        GtkTreeIter iter;
        list = GTK_LIST_STORE (gtk_combo_box_get_model (GTK_COMBO_BOX(combo)));
        switch (position) {
           case -1: gtk_list_store_append (list, &iter); break;
           case  0: gtk_list_store_prepend (list, &iter); break;
           default: gtk_list_store_insert (list, &iter, position);
        }
        gtk_list_store_set (list, &iter, 0, text, -1);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2 //GTK1 widgets that also supported in GTK2 (but deprecated)
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget *list;
        GtkWidget *item;
        GList     *glist = NULL;
        if (!text) return;
        list  = GTK_COMBO(combo)->list;
        item  = gtk_list_item_new_with_label (text);
        glist = g_list_append (glist, item);
        gtk_widget_show (item);
        /// position -1 = gtk_list_append_items (GTK_LIST(list), glist); // gtk_container_add(GTK_CONTAINER(list), item);
        /// position  0 = gtk_list_prepend_items (GTK_LIST(list), glist);
        gtk_list_insert_items (GTK_LIST(list), glist, position);
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        GtkWidget *menu;
        GtkWidget *item;
        if (!text) return;
        item = gtk_menu_item_new_with_label (text);
        // Getting warnings & segfaults trying to get the label in other functions
        // Something is broken, this happens when closing a test app: 
        //    - Gtk-WARNING **: Attempting to add a widget with type GtkAccelLabel to a GtkMenuItem,
        //    but as a GtkBin subclass a GtkMenuItem can only contain one widget at a time;
        //    it already contains a widget of type GtkAccelLabel
        //    - GLib-GObject-CRITICAL **: g_object_notify: assertion 'G_IS_OBJECT (object)' failed
        ///puts (gtk_menu_item_get_label (GTK_MENU_ITEM(item))); // this works here
        // -- Try an alternate method that works
        g_object_set_data (G_OBJECT(item), "wlabel", (gpointer)gtk_menu_item_get_label (GTK_MENU_ITEM(item)));
        // --
        menu = gtk_option_menu_get_menu (GTK_OPTION_MENU(combo));
        if (!menu) {
            menu = gtk_menu_new ();
            gtk_option_menu_set_menu (GTK_OPTION_MENU(combo), menu);
        }
        gtk_widget_show (item);
        gtk_menu_shell_insert (GTK_MENU_SHELL(menu), item, position);
        /// position -1 = gtk_menu_shell_append (GTK_MENU_SHELL(menu), item);
        /// position  0 = gtk_menu_shell_prepend (GTK_MENU_SHELL(menu), item);
    }
#endif
}


void w_gtk_combo_append_text (GtkWidget *combo, const char *text)
{
    w_gtk_combo_insert_text (combo, -1, text);
}


void w_gtk_combo_prepend_text (GtkWidget *combo, const char *text)
{
    w_gtk_combo_insert_text (combo, 0, text);
}


void w_gtk_combo_remove_index (GtkWidget *combo, int position)
{
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo))
    {
        return;
    }
#endif
#if (GTK_MAJOR_VERSION <= 4 && GTK_CHECK_VERSION(2,4,0))
    if (GTK_IS_COMBO_BOX(combo))
    {
        GtkTreeModel *model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
        GtkTreePath  *path  = gtk_tree_path_new_from_indices (position, -1);
        GtkTreeIter iter;
        if (gtk_tree_model_get_iter (model, &iter, path)) {
            gtk_list_store_remove (GTK_LIST_STORE(model), &iter);
        }
        gtk_tree_path_free (path);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2 //GTK1 widgets that also supported in GTK2 (but deprecated)
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget  *list = GTK_COMBO(combo)->list;
        GList     *items = GTK_LIST(list)->children;
        GtkWidget *nitem = GTK_WIDGET (g_list_nth_data (items, position));
        if (nitem) {
            ///gtk_container_remove (GTK_CONTAINER(list), nitem);
            gtk_widget_destroy (nitem);
        }
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        GtkWidget *menu, *menuitem;
        GList * items, * nitem;
        menu = gtk_option_menu_get_menu (GTK_OPTION_MENU(combo));
        items = GTK_MENU_SHELL(menu)->children;
        nitem = g_list_nth (items, position);
        if (nitem) {
            menuitem = GTK_WIDGET(nitem->data);
            ///gtk_container_remove (GTK_CONTAINER(menu), menuitem);
            gtk_widget_destroy (menuitem);
        }
    }
#endif
}


char *w_gtk_combo_get_active_text (GtkWidget *combo)
{
    char *itext = NULL;
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo))
    {
        return itext; //must be freed
    }
#endif

#if (GTK_MAJOR_VERSION <= 4 && GTK_CHECK_VERSION(2,4,0))
    if (GTK_IS_COMBO_BOX(combo)) {
        GtkTreeIter iter;
        GtkTreeModel *model;
        GtkWidget *entry = gtk_bin_get_child (GTK_BIN(combo));
        if (entry && GTK_IS_ENTRY(entry)) {
            itext = g_strdup (gtk_entry_get_text (GTK_ENTRY(entry)));
        } else {
            if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo), &iter)) {
                model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
                gtk_tree_model_get (model, &iter, 0, &itext, -1);
            }
        }
        return itext; //must be freed
    }
#endif
#if GTK_MAJOR_VERSION <= 2 //GTK1 widgets that also supported in GTK2 (but deprecated)
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget *entry;
        entry = GTK_COMBO(combo)->entry;
        itext = g_strdup (gtk_entry_get_text (GTK_ENTRY(entry)));
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        GtkWidget *menu, *menuitem;
        char *lbl;
        menu  = gtk_option_menu_get_menu (GTK_OPTION_MENU(combo));
        menuitem = gtk_menu_get_active (GTK_MENU(menu));
        if (menuitem) {
            lbl = (char*)g_object_get_data (G_OBJECT(menuitem), "wlabel");
            if (lbl) {
                itext = g_strdup (lbl);
            } else {
                //. this is broken, but it shouldn't be
                ///label_widget = gtk_bin_get_child (GTK_BIN(menuitem));
                ///itext = g_strdup (gtk_label_get_text (GTK_LABEL(label_widget)));
            }
        }
    }
#endif
    return itext; //must be freed
}


int w_gtk_combo_get_active_index (GtkWidget *combo)
{
    // Returns an integer which is the index of the currently active item
    //   or -1 if there's no active item.
    int active = -1;
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo))
    {
        unsigned int sel;
        sel = gtk_drop_down_get_selected (GTK_DROP_DOWN(combo));
        if (sel == GTK_INVALID_LIST_POSITION) {
            return -1;
        }
        active = (int) sel;
        return active;
    }
#endif
#if (GTK_MAJOR_VERSION <= 4 && GTK_CHECK_VERSION(2,4,0))
    if (GTK_IS_COMBO_BOX(combo))
    {
        active = gtk_combo_box_get_active (GTK_COMBO_BOX(combo));
        return active;
    }
#endif
#if GTK_MAJOR_VERSION <= 2 //GTK1 widgets that also supported in GTK2 (but deprecated)
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget *list = GTK_COMBO(combo)->list;
        GList *selection = GTK_LIST(list)->selection;
        GtkWidget *selitem = NULL;
        if (selection) {
            selitem = GTK_WIDGET (selection->data);
            active = gtk_list_child_position (GTK_LIST(list), selitem);
        }
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        active = gtk_option_menu_get_history (GTK_OPTION_MENU(combo));
    }
#endif
    return active;
}


void w_gtk_combo_set_active_index (GtkWidget *combo, int index)
{
    // index: index in the model passed during construction, or -1 to have no active item
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo))
    {
        guint sel; 
        if (index == -1) {
            sel = GTK_INVALID_LIST_POSITION;
        } else {
            sel = (guint) index;
        }
        gtk_drop_down_set_selected (GTK_DROP_DOWN(combo), sel);
        return;
    }
#endif
#if (GTK_MAJOR_VERSION <= 4 && GTK_CHECK_VERSION(2,4,0))
    if (GTK_IS_COMBO_BOX(combo))
    {
        gtk_combo_box_set_active (GTK_COMBO_BOX(combo), index);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2 //GTK1 widgets that also supported in GTK2 (but deprecated)
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget *list;
        list = GTK_COMBO(combo)->list;
        if (index == -1) {
            gtk_list_unselect_all (GTK_LIST(list));
        } else {
            gtk_list_select_item (GTK_LIST(list), index);
        }
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        gtk_option_menu_set_history (GTK_OPTION_MENU(combo), index);
    }
#endif
}


gboolean w_gtk_combo_find_str (GtkWidget *combo, const char *str, gboolean select)
{
    gboolean case_sensitive = TRUE;
    gboolean found = FALSE;
    int cmpret = 0;
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo))
    {
        return found;
    }
#endif
#if (GTK_MAJOR_VERSION <= 4 && GTK_CHECK_VERSION(2,4,0))
    if (GTK_IS_COMBO_BOX(combo))
    {
        GtkTreeModel *model;
        GtkTreeIter iter;
        gboolean valid;
        char *value = NULL;
        if (!str) {
            return found;
        }
        model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
        valid = gtk_tree_model_get_iter_first (model, &iter);
        while (valid)
        {
            gtk_tree_model_get (model, &iter, 0, &value, -1);
            if (!value) {
                continue;
            }
            if (case_sensitive) {
                cmpret = strcmp (value, str);
            } else {
                cmpret = strcasecmp (value, str);
            }
            if (cmpret == 0) {
                found = TRUE;
                g_free (value);
                break;
            }
            g_free (value); // string values have to be freed
            valid = gtk_tree_model_iter_next (model, &iter);
        }
        if (found && select) {
            gtk_combo_box_set_active_iter (GTK_COMBO_BOX(combo), &iter);
        }
        return found;
    }
#endif
#if GTK_MAJOR_VERSION <= 2 //GTK1 widgets that also supported in GTK2 (but deprecated)
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget *list, *list_item; //GtkList, GtkListItem
        GtkWidget *label_widget;
        GList *items, *igl;
        const char *value = NULL;
        list  = GTK_COMBO(combo)->list;
        items = GTK_LIST(list)->children;
        for (igl = items; igl != NULL; igl = igl->next)
        {
            list_item    = GTK_WIDGET(igl->data);
            label_widget = gtk_bin_get_child (GTK_BIN(list_item));
            value        = gtk_label_get_text (GTK_LABEL(label_widget));
            if (case_sensitive) {
                cmpret = strcmp (value, str);
            } else {
                cmpret = strcasecmp (value, str);
            }
            if (cmpret == 0) {
                found = TRUE;
                break;
            }
       }
       if (found && select) {
           ///gtk_list_item_select (GTK_LIST_ITEM(list_item));
           gtk_list_select_child (GTK_LIST(list), list_item);
       }
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        GtkWidget *menu, *menu_item;
        ///GtkWidget *label_widget;
        GList *items, *igl;
        int index = 0;
        const char *value = NULL;
        menu  = gtk_option_menu_get_menu (GTK_OPTION_MENU(combo));
        items = GTK_MENU_SHELL(menu)->children;
        for (igl = items; igl != NULL; igl = igl->next)
        {
            menu_item = GTK_WIDGET(igl->data);
            value     = g_object_get_data (G_OBJECT(menu_item), "wlabel");
            if (!value) {
                //. this is broken, but it shouldn't be
                ///label_widget = GTK_BIN(menu_item)->child;
                ///value        = gtk_label_get_text (GTK_LABEL(label_widget));
                continue;
            }
            if (case_sensitive) {
                cmpret = strcmp (value, str);
            } else {
                cmpret = strcasecmp (value, str);
            }
            if (cmpret == 0) {
                found = TRUE;
                break;
            }
            index++;
        }
        if (found && select) {
            ///gtk_menu_shell_select_item (GTK_MENU_SHELL(menu), menu_item); // this doesn't work
            gtk_option_menu_set_history (GTK_OPTION_MENU(combo), index);
        }
    }
#endif
    return found;
}



//===============================================================
//  [ADVANCED FUNCTIONS] that only use basic functions...
//===============================================================

void w_gtk_combo_set_enable_entry (GtkWidget *combo, gboolean enabled)
{
    GtkWidget *entry = w_gtk_combo_get_entry (combo);
    if (entry) {
        gtk_editable_set_editable (GTK_EDITABLE(entry), enabled);
    }
}


void w_gtk_combo_set_entry_text (GtkWidget *combo, const char *text)
{
    GtkWidget *entry = w_gtk_combo_get_entry (combo);
    if (entry) {
        gtk_entry_set_text (GTK_ENTRY(entry), text);
    }
}


void w_gtk_combo_populate_from_glist (GtkWidget *combo, GList *strings, int default_index)
{ //GtkCombo: gtk_combo_set_popdown_strings (GTK_COMBO(combo), strings);
    GList * list;
    char * text;
    int len = 0;

    w_gtk_combo_clear (combo);
    if (!strings) {
        return; // nothing to add
    }
    for (list = strings;  list;  list = list->next)
    {
        text = (char *) list->data;
        w_gtk_combo_append_text (combo, text);
        len++;
    }
    if (default_index >= len) {
        default_index = 0;
    }
    if (default_index >= 0) {
        w_gtk_combo_set_active_index (combo, default_index);
    }
}


void w_gtk_combo_populate_from_strv (GtkWidget *combo,
                                         const char **strv,
                                         int default_index,
                                         gboolean gettext)
{
    int i;
    char *str;

    w_gtk_combo_clear (combo);
    if (!strv || !*strv) {
        return; /* nothing to add */
    }
    for (i = 0; strv[i]; i++) {
#ifdef ENABLE_NLS
        str = gettext ? gettext(strv[i]) : (char*) strv[i];
#else
        str = (char*) strv[i];
#endif
        w_gtk_combo_append_text (combo, str);
    }
    if (default_index >= i) {
        default_index = 0;
    }
    if (default_index >= 0) {
        w_gtk_combo_set_active_index (combo, default_index);
    }
}


void w_gtk_combo_select_or_prepend (GtkWidget *combo, const char *str)
{
    if (!str || !*str) {
        return;
    }
    if (!w_gtk_combo_find_str (combo, str, TRUE)) {
        w_gtk_combo_prepend_text (combo, str);
        w_gtk_combo_set_active_index (combo, 0);
    }
}


// ================================================================
// GtkComboBox-specific

void w_gtk_combo_set_w_model (GtkWidget *combo, gboolean sort)
{
#if (GTK_MAJOR_VERSION <= 4 && GTK_CHECK_VERSION(2,4,0))
    // column 0: text
    // column 1: pointer (hidden)
    GtkCellRenderer * cell;
    GtkListStore * store;
    GtkTreeSortable * sortable;
    GtkWidget *entry = w_gtk_combo_get_entry (combo);

    if (entry) { // combobox with entry looks weird without this
        gtk_combo_box_set_model (GTK_COMBO_BOX(combo), NULL);
        gtk_cell_layout_clear (GTK_CELL_LAYOUT(combo));
    }

    store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);
    gtk_combo_box_set_model (GTK_COMBO_BOX(combo), GTK_TREE_MODEL(store));
    g_object_unref (store);

    cell = gtk_cell_renderer_text_new ();
    gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (combo), cell, TRUE);
    gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (combo), cell,
                                    "text", 0, NULL);
    if (sort) {
        sortable = GTK_TREE_SORTABLE(store);
        gtk_tree_sortable_set_sort_column_id (sortable, 0, GTK_SORT_ASCENDING);
    }
    // got warnings, have to set text_column
    // but 1 warning remains...
    //    gtk_cell_layout_set_attributes: assertion 'GTK_IS_CELL_RENDERER (cell)' failed
#if GTK_MAJOR_VERSION == 2
    if (GTK_IS_COMBO_BOX_ENTRY(combo)) {
        // GtkComboBoxEntry was removed in gtk3, apply suitable command
        gtk_combo_box_entry_set_text_column (GTK_COMBO_BOX_ENTRY(combo), 0);
        return;
    }
#endif
#if GTK_CHECK_VERSION(2,24,0)
    if (gtk_combo_box_get_has_entry (GTK_COMBO_BOX(combo))) {
        gtk_combo_box_set_entry_text_column (GTK_COMBO_BOX(combo), 0);
    }
#endif
#endif
}
