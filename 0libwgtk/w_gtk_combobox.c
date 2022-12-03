/*
 * Public Domain
 */

// w_gtk_combobox is modeled after GtkComboBoxText
// - item index starts at 0
// - invalid item [active] index is -1
// - returned item [active] text must be freed

// Supported widgets:
// - GtkComboBox   (gtk2/3/4) [GtkEntry] [GtkTreeModel / GtkListStore [GtkTreeIter]]
// - GtkCombo      (gtk1/2)   [GtkEntry] [GtkList [GtkListItem]]
// - GtkOptionMenu (gtk1/2)   (no entry) [GtkMenu [GtkMenuItem]]
// - GtkDropDown   (gtk4)     (no entry) [GListModel / GtkStringList]
//     TODO: preliminary support, this has no been tested
//     Limitations: (fixing this requires implementing advanced stuff)
//     * cannot prepend/insert items (only append)
//     * no data field (only text)
//     * the (search) entry contents cannot be retrieved?


// all w_gtk_combo_..() functions take GtkWidget as parameter

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

  - https://docs.gtk.org/gtk4/class.DropDown.html
       https://docs.gtk.org/gtk4/class.StringList.html
          https://docs.gtk.org/gio/iface.ListModel.html
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
#include "w_gtk.h"

#ifdef ENABLE_NLS
#include <locale.h>
#include <libintl.h>
#else
#define gettext(x) (x)
#endif

///#define W_COMBO_DEBUG

#ifdef W_COMBO_DEBUG
# define W_DEBUG_PUTS(text) puts(text) 
#else
# define W_DEBUG_PUTS(text)
#endif


/*--------------------------------------------------------------
/                  w_gtk_combobox_new
/-------------------------------------------------------------*/

/*
   Support combobox with `text` and `data` fields
   text is a char array (char*)
   data is a [hidden] gpointer (void*), a pointer to any object

   - GtkDropDown
     * text is stored in a GtkStringList (not GListStore)
     * data is not supported
   - GtkComboBox
     * text is field 0 of a GtkListStore
     * data is field 1 of a GtkListStore [w_gtk_combo_box_set_w_model()]
   - GtkCombo
     * text is a GtkListItem label
     * data is GtkListItem "itemdata" (object data)
   - GtkOptionMenu
     * text is a GtkMenuItem label [hacky]
     * data is GtkMenuItem "itemdata" (object data)
*/

GtkWidget * w_gtk_combobox_new ()
{
    GtkWidget * combo;

#if GTK_CHECK_VERSION(4,0,0) // 4,10,0 //TODO this has not been tested
    // Some people say GtkComboBox is broken in gtk4
    GtkStringList *list = gtk_string_list_new (NULL);
    combo = gtk_drop_down_new (G_LIST_MODEL(list), NULL);
#endif
#if GTK_CHECK_VERSION(2,24,0)
    combo = gtk_combo_box_text_new (); // deprecated in GTK 4.10
    w_gtk_combo_box_set_w_model (combo, FALSE);
#elif GTK_CHECK_VERSION(2,4,0)
    combo = gtk_combo_box_new_text (); // deprecated in GTK 2.24
    w_gtk_combo_box_set_w_model (combo, FALSE);
#elif !GTK_CHECK_VERSION(2,4,0)
    // GtkOptionMenu is ugly and not suitable for complex operations
    ///combo = gtk_option_menu_new (); // deprecated in GTK 2.4
    ///gtk_option_menu_set_menu (GTK_OPTION_MENU(combo), gtk_menu_new());
    combo = gtk_combo_new (); // deprecated in GTK 2.4
    gtk_editable_set_editable (GTK_EDITABLE(GTK_COMBO(combo)->entry), FALSE);
#endif
    return combo;
}

/*--------------------------------------------------------------
/              w_gtk_combobox_new_with_entry
/-------------------------------------------------------------*/

GtkWidget * w_gtk_combobox_new_with_entry ()
{
    GtkWidget * combo;
#if GTK_CHECK_VERSION(4,0,0) // 4,10,0 //TODO this has not been tested
    // Some people say GtkComboBox is broken in gtk4
    GtkStringList *list = gtk_string_list_new (NULL);
    combo = gtk_drop_down_new (G_LIST_MODEL(list), NULL);
    gtk_drop_down_set_enable_search (GTK_DROP_DOWN(combo), TRUE);
    W_DEBUG_PUTS ("w_gtk_combobox: new GtkDropDown");
#endif
#if GTK_CHECK_VERSION(2,24,0)
    combo = gtk_combo_box_text_new_with_entry ();  // deprecated in GTK 4.10
    w_gtk_combo_box_set_w_model (combo, FALSE);
    W_DEBUG_PUTS ("w_gtk_combobox: new GtkComboBoxText with Entry (w model)");
#elif GTK_CHECK_VERSION(2,4,0)
    combo = gtk_combo_box_entry_new_text ();  // deprecated in GTK 2.24
    w_gtk_combo_box_set_w_model (combo, FALSE);
    W_DEBUG_PUTS ("w_gtk_combobox: new GtkComboBoxEntry with text (w model)");
#elif !GTK_CHECK_VERSION(2,4,0)
    combo = gtk_combo_new ();  // deprecated in GTK 2.4
    W_DEBUG_PUTS ("w_gtk_combobox: new GtkCombo");
#endif
    w_gtk_combobox_set_search_case_insensitive (combo, FALSE);
    return combo;
}


/*--------------------------------------------------------------
/         w_gtk_combobox_add_changed_handler
/-------------------------------------------------------------*/

/* the "changed" signal
 
GtkComboBox:     changed (combo,data)
GtkOptionMenu:   changed (combo,data) [not in GTK1]
GtkCombo->entry: changed (entry,data) / combo = get_parent(entry)

How to handle the "changed" signal:

static void on_combo_changed_cb (GtkWidget *widget, gpointer user_data)
{
    char *text;
    GtkWidget *combo = w_gtk_combobox_cb_ensure_combo (widget);
    if (!combo) {
        return;
    }
    text = w_gtk_combobox_get_active_text (combo);
    if (text) {
        puts (text);
        g_free (text);
    }
}
*/

#if GTK_CHECK_VERSION(4,0,0)
static void dropdown_to_combobox_changed (GtkWidget *widget, GParamSpec *pspec, gpointer user_data)
{
    void (*func) (void*, void*);
    func = user_data;
    func(widget, NULL); // GtkDropdown: cb_data is not passed 
}
#endif


int w_gtk_combobox_add_changed_handler (GtkWidget *combo, gpointer cb, gpointer cb_data)
{
    int handler = -1;
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo)) //TODO: this has not been tested
    {
        W_DEBUG_PUTS ("w_gtk_combobox: add changed cb for GtkDropDown");
        // void selected_cb (GtkDropDown *widget, GParamSpec *pspec, gpointer user_data)
        //                                        ----(problem)----
        // I think there is also notify::selected-item
        handler = g_signal_connect (G_OBJECT(combo), "notify::selected",
                  G_CALLBACK(dropdown_to_combobox_changed), cb);
        return handler;
    }
#endif
#if (GTK_CHECK_VERSION(2,4,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_COMBO_BOX(combo))
    {
        W_DEBUG_PUTS ("w_gtk_combobox: add changed cb for GtkComboBox");
        // void changed_cb (GtkComboBox *widget, gpointer user_data)
        handler = g_signal_connect (G_OBJECT(combo), "changed", G_CALLBACK(cb), cb_data);
        return handler;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_COMBO(combo))
    {
        W_DEBUG_PUTS ("w_gtk_combobox: add changed cb for GtkCombo entry");
        // void changed_cb (GtkEntry *widget, gpointer user_data)
        handler = g_signal_connect (G_OBJECT(GTK_COMBO(combo)->entry),
                                    "changed", G_CALLBACK(cb), cb_data);
        /// void selection_cb (GtkList *list, gpointer user_data)
        ///g_signal_connect (GTK_COMBO(combo)->list, "selection-changed",
        ///                  G_CALLBACK(cb), wcomboentry2);
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        W_DEBUG_PUTS ("w_gtk_combobox: add changed cb for GtkOptionMenu");
#      if GTK_CHECK_VERSION(2,0,0)
        // void changed_cb (GtkOptionMenu *widget, gpointer user_data)
        handler = g_signal_connect (G_OBJECT(combo), "changed", G_CALLBACK(cb), cb_data);
#      else
        // HACK: GTK1 doesn't implement the changed signal.. GtkMenuItems will call w_changed_cb
        g_object_set_data (G_OBJECT(combo), "w_changed_cb", cb);
        g_object_set_data (G_OBJECT(combo), "w_changed_cb_data", cb_data);
#      endif
    }
#endif
    if (handler == -1) {
        g_warning ("Invalid wcombo widget");
    }
    return handler;
}

/*--------------------------------------------------------------
/          w_gtk_combobox_cb_ensure_combo
/-------------------------------------------------------------*/

GtkWidget *w_gtk_combobox_cb_ensure_combo (GtkWidget *widget)
{
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(widget)) {
        W_DEBUG_PUTS ("wcombo changed signal: it's GtkDropDown");
        return widget;
    }
#endif
#if (GTK_CHECK_VERSION(2,4,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_COMBO_BOX(widget)) {
        W_DEBUG_PUTS ("wcombo changed signal: it's GtkComboBox");
        return widget;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_COMBO(gtk_widget_get_parent(widget))) {
        // GtkCombo doesn't have signals, this is coming from its GtkEntry or GtkList
        W_DEBUG_PUTS ("wcombo changed signal: it's GtkCombo entry");
        return gtk_widget_get_parent(widget);
    } else if (GTK_IS_OPTION_MENU(widget)) {
        // GtkOptionMenu has the changed signal only in GTK2
        W_DEBUG_PUTS ("wcombo changed signal: it's GtkOptionMenu");
        return widget;
    }
#  if GTK_MAJOR_VERSION == 1 // GTK1 Hack to emulate the changed signal in GtkOptionMenu
    else {
        GtkWidget *combo = g_object_get_data (G_OBJECT(widget), "w_gtk_combobox");
        if (GTK_IS_OPTION_MENU(combo)) {
            W_DEBUG_PUTS ("wcombo changed signal: GTK1 GtkOptionMenu hack");
            return combo;
        }
    }
#  endif
#endif
    //g_return_val_if_fail (combo != NULL, NULL);
    return NULL;
}


/*--------------------------------------------------------------
/             w_gtk_combobox_get_entry
/-------------------------------------------------------------*/

GtkWidget * w_gtk_combobox_get_entry (GtkWidget *combo)
{
    GtkWidget *entry = NULL;
    // GtkDropDown (currently unsupported)
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo)) // TODO
    {
        ///g_warning ("GtkDropDown: no entry");
        return entry;
    }
#endif
    // GtkComboBox
#if (GTK_CHECK_VERSION(2,4,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_COMBO_BOX(combo))
    {
#   if GTK_MAJOR_VERSION == 4 // GtkBin was removed in gtk4
        entry = gtk_combo_box_get_child (GTK_COMBO_BOX(combo));
#   else // gtk 2 & 3
        entry = gtk_bin_get_child (GTK_BIN(combo));
#   endif
        if (entry && GTK_IS_ENTRY(entry)) {
            return entry;
        }
        return NULL;
    }
#endif
    // GtkCombo (GtkOptionMenu doesn't have an entry)
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_COMBO(combo))
    {
        entry = GTK_COMBO(combo)->entry;
    }
#endif
    return entry;
}



/*--------------------------------------------------------------
/            w_gtk_combobox_set_enable_entry
/-------------------------------------------------------------*/

void w_gtk_combobox_set_enable_entry (GtkWidget *combo, gboolean enabled)
{
    GtkWidget *entry = w_gtk_combobox_get_entry (combo);
    if (entry) {
        gtk_editable_set_editable (GTK_EDITABLE(entry), enabled);
    }
}

/*--------------------------------------------------------------
/             w_gtk_combobox_set_entry_text
/-------------------------------------------------------------*/

void w_gtk_combobox_set_entry_text (GtkWidget *combo, const char *text)
{
    GtkWidget *entry = w_gtk_combobox_get_entry (combo);
    if (entry) {
#if GTK_CHECK_VERSION(4,0,0)
        gtk_editable_set_text (GTK_EDITABLE(entry), text ? text : "");
#else
        gtk_entry_set_text (GTK_ENTRY(entry), text ? text : "");
#endif
    }
}


/*--------------------------------------------------------------
/                 w_gtk_combobox_clear
/-------------------------------------------------------------*/

void w_gtk_combobox_clear (GtkWidget *combo)
{
    w_gtk_combobox_set_entry_text (combo, "");
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo)) //TODO: this has not been tested
    {
        GListModel * model = gtk_drop_down_get_model (GTK_DROP_DOWN(combo));
        guint count = g_list_model_get_n_items (model);
        guint i;
        for (i = count-1; i >= 0; i--) {
            gtk_string_list_remove (GTK_STRING_LIST(model), i);
        }
        ///NOT: g_list_store_remove_all (G_LIST_STORE(model));
        return;
    }
#endif
#if (GTK_CHECK_VERSION(2,4,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_COMBO_BOX(combo))
    {
        GtkTreeModel *model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
        gtk_list_store_clear (GTK_LIST_STORE(model)); 
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget *list  = GTK_COMBO(combo)->list;
        gtk_list_clear_items (GTK_LIST(list), 0, -1);
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        // this is essentially broken..
        GtkWidget *menu, *menuitem; // GtkMenu, GtkMenuItem
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
            if (GTK_IS_WIDGET(igl->data)) { /** this may trigger a segfault **/
                menuitem = GTK_WIDGET(igl->data); // igl->data = GtkMenuItem
                gtk_widget_destroy (menuitem); ///gtk_container_remove
            }
        }
#if GTK_CHECK_VERSION(2,0,0)
        // gtk2.24: the lines above don't seem to work, nothing is removed
        gtk_option_menu_remove_menu (GTK_OPTION_MENU(combo));
        gtk_option_menu_set_menu (GTK_OPTION_MENU(combo), gtk_menu_new());
#endif
    }
#endif
}


/*--------------------------------------------------------------
/                 w_gtk_combobox_get_count
/-------------------------------------------------------------*/

int w_gtk_combobox_get_count (GtkWidget *combo)
{
    int count = 0;
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo)) //TODO: this has not been tested
    {
        GListModel * model = gtk_drop_down_get_model (GTK_DROP_DOWN(combo));
        return (int) g_list_model_get_n_items (model);
    }
#endif
#if (GTK_CHECK_VERSION(2,4,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_COMBO_BOX(combo))
    {
        GtkTreeModel * model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
        ///GtkTreeIter iter;
        ///gboolean valid = gtk_tree_model_get_iter_first (model, &iter);
        ///while (valid) { count++; valid = gtk_tree_model_iter_next (model, &iter); }
        return gtk_tree_model_iter_n_children (model, NULL);
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget  *list = GTK_COMBO(combo)->list;
        GList     *items = GTK_LIST(list)->children;
        count = g_list_length (items);
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        GtkWidget *menu = gtk_option_menu_get_menu (GTK_OPTION_MENU(combo));
        GList    *items = GTK_MENU_SHELL(menu)->children;
        count = g_list_length (items);
    }
#endif
    return count;
}


/*--------------------------------------------------------------
/                  w_gtk_combobox_insert
/-------------------------------------------------------------*/

void w_gtk_combobox_insert (GtkWidget *combo, int position, const char *text, gpointer wdata)
{
    // position -1: append
    // position  0: prepend
    // position  0+: insert
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo)) //TODO: this has not been tested
    {
        GListModel *model = gtk_drop_down_get_model (GTK_DROP_DOWN(combo));
        GtkStringList *list = GTK_STRING_LIST(model);
        switch (position) {
           case -1: gtk_string_list_append (list, text); break;
           case  0: g_warning ("GtkDropDown does not support prepending items"); break;
           default:
            g_warning ("GtkDropDown does not support inserting items");
        }
        return;
    }
#endif
#if (GTK_CHECK_VERSION(2,4,0) && GTK_MAJOR_VERSION <= 4)
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
        if (wdata && gtk_tree_model_get_n_columns(GTK_TREE_MODEL(list)) == 2) {
            // the W combo created with w_gtk_combobox_new()
            gtk_list_store_set (list, &iter, 1, wdata, -1);
        }
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget *list; // GtkList
        GtkWidget *item; // GtkListem
        GList     *glist = NULL;
        if (!text) return;
        list  = GTK_COMBO(combo)->list;
        item  = gtk_list_item_new_with_label (text);
        glist = g_list_append (glist, item);
        gtk_widget_show (item);
        /// position -1 = gtk_list_append_items (GTK_LIST(list), glist); // gtk_container_add(GTK_CONTAINER(list), item);
        /// position  0 = gtk_list_prepend_items (GTK_LIST(list), glist);
        gtk_list_insert_items (GTK_LIST(list), glist, position);
        if (wdata) {
            g_object_set_data (G_OBJECT(item), "itemdata", wdata);
        }
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        GtkWidget *menu; // GtkMenu
        GtkWidget *item; // GtkMenuItem
        if (!text) return;
        item = gtk_menu_item_new_with_label (text);
        // -- Getting warnings & segfaults trying to get the GtkMenuItem label in other functions
        //puts (gtk_menu_item_get_label (GTK_MENU_ITEM(item))); // this works here
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
        if (wdata) {
            g_object_set_data (G_OBJECT(item), "itemdata", wdata);
        }
#if GTK_MAJOR_VERSION == 1 //GTK1: hack
        g_object_set_data (G_OBJECT(item), "w_gtk_combobox", (gpointer)combo);
        gpointer cb_func = g_object_get_data (G_OBJECT(combo), "w_changed_cb");
        gpointer cb_data = g_object_get_data (G_OBJECT(combo), "w_changed_cb_data");
        if (cb_func) {
            g_signal_connect (G_OBJECT(item), "activate", G_CALLBACK(cb_func), cb_data);
        }
#endif
    }
#endif
}


/*--------------------------------------------------------------
/                w_gtk_combobox_append
/-------------------------------------------------------------*/

void w_gtk_combobox_append (GtkWidget *combo, const char *text, gpointer wdata)
{
    w_gtk_combobox_insert (combo, -1, text, wdata);
}


/*--------------------------------------------------------------
/                w_gtk_combobox_prepend
/-------------------------------------------------------------*/

void w_gtk_combobox_prepend (GtkWidget *combo, const char *text, gpointer wdata)
{
    w_gtk_combobox_insert (combo, 0, text, wdata);
}


/*--------------------------------------------------------------
/               w_gtk_combobox_insert_text
/-------------------------------------------------------------*/

void w_gtk_combobox_insert_text (GtkWidget *combo, int position, const char *text)
{
    w_gtk_combobox_insert (combo, position, text, NULL);
}


/*--------------------------------------------------------------
/               w_gtk_combobox_append_text
/-------------------------------------------------------------*/

void w_gtk_combobox_append_text (GtkWidget *combo, const char *text)
{
    w_gtk_combobox_insert (combo, -1, text, NULL);
}


/*--------------------------------------------------------------
/              w_gtk_combobox_prepend_text
/-------------------------------------------------------------*/

void w_gtk_combobox_prepend_text (GtkWidget *combo, const char *text)
{
    w_gtk_combobox_insert (combo, 0, text, NULL);
}


/*--------------------------------------------------------------
/                 w_gtk_combobox_remove
/-------------------------------------------------------------*/

void w_gtk_combobox_remove (GtkWidget *combo, int position) // index
{
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo)) //TODO: this has not been tested
    {
        GListModel * model = gtk_drop_down_get_model (GTK_DROP_DOWN(combo));
        gtk_string_list_remove (GTK_STRING_LIST(model), (guint) position);
        ///NOT: g_list_store_remove (G_LIST_STORE(model), (guint) position);
        return removed;
    }
#endif
#if (GTK_CHECK_VERSION(2,4,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_COMBO_BOX(combo))
    {
        GtkTreeModel *model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
        ///GtkTreePath  *path  = gtk_tree_path_new_from_indices (position, -1);
        GtkTreeIter iter;
        if (gtk_tree_model_iter_nth_child(model, &iter, NULL, position)) {
        ///if (gtk_tree_model_get_iter (model, &iter, path)) {
            gtk_list_store_remove (GTK_LIST_STORE(model), &iter);
        }
        ///gtk_tree_path_free (path);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget  *list = GTK_COMBO(combo)->list;
        GList     *items = GTK_LIST(list)->children;
        GtkWidget *witem = GTK_WIDGET (g_list_nth_data (items, position));
        if (witem) { // witem = GtkListItem
            gtk_widget_destroy (witem); ///gtk_container_remove
        }
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        GtkWidget *menu = gtk_option_menu_get_menu (GTK_OPTION_MENU(combo));
        GList *items = GTK_MENU_SHELL(menu)->children;
        GList *nitem = g_list_nth (items, position);
        if (nitem) { // nitem->data = GtkMenuItem
            gtk_widget_destroy (GTK_WIDGET(nitem->data)); ///gtk_container_remove
        }
    }
#endif
}


/*--------------------------------------------------------------
/               w_gtk_combobox_get_item
/-------------------------------------------------------------*/

void w_gtk_combobox_get_item (GtkWidget *combo, int position, WGtkSimpleListItem *out_item)
{
    // if out_item->text != NULL, it must be freed
    g_return_if_fail (out_item != NULL);
    //g_free (out_item->text);
    memset (out_item, 0, sizeof(*out_item));
    out_item->index = position;
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo)) // TODO: this has not been tested
    {
        GListModel * model = gtk_drop_down_get_model (GTK_DROP_DOWN(combo));
        GtkStringList *sl = GTK_STRING_LIST(model);
        out_item->text  = g_strdup (gtk_string_list_get_string(sl, sel));
        return;
    }
#endif
#if (GTK_CHECK_VERSION(2,4,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_COMBO_BOX(combo))
    {
        GtkTreeModel *model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
        GtkTreePath  *path  = gtk_tree_path_new_from_indices (position, -1);
        GtkTreeIter iter;
        if (gtk_tree_model_get_n_columns(model) == 2) {
            // the W combo created with w_gtk_combobox_new()
            if (gtk_tree_model_get_iter (model, &iter, path)) {
                gtk_tree_model_get (model, &iter,
                                    0, &(out_item->text),
                                    1, &(out_item->data), -1);
            }
        } else { // normal ComboBoxText
            if (gtk_tree_model_get_iter (model, &iter, path)) {
                gtk_tree_model_get (model, &iter,
                                    0, &(out_item->text), -1);
            }
        }
        gtk_tree_path_free (path);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget *label_w, *list_item;
        GtkWidget *list = GTK_COMBO(combo)->list;  // GtkList
        GList *items = GTK_LIST(list)->children;
        GList *nitem = g_list_nth (items, position);
        if (nitem) { // nitem->data = GtkListItem
            list_item = GTK_WIDGET(nitem->data); 
            label_w   = gtk_bin_get_child (GTK_BIN(list_item));
            out_item->text = g_strdup (gtk_label_get_text (GTK_LABEL(label_w)));
            out_item->data = g_object_get_data (G_OBJECT(list_item), "itemdata");
        }
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        GtkWidget *menu = gtk_option_menu_get_menu (GTK_OPTION_MENU(combo));
        GList *items = GTK_MENU_SHELL(menu)->children;
        GList *nitem = g_list_nth (items, position);
        if (nitem) // nitem->data = GtkMenuItem
        { // gtk_menu_item_get_label() fails here, broken in gtk1 & gtk2
            out_item->text = g_strdup (g_object_get_data (G_OBJECT(nitem->data), "wlabel"));
            out_item->data = g_object_get_data (G_OBJECT(nitem->data), "itemdata"); 
        }
    }
#endif
}

/*--------------------------------------------------------------
/              w_gtk_combobox_get_item_text
/-------------------------------------------------------------*/

char *w_gtk_combobox_get_item_text (GtkWidget *combo, int position)
{
    WGtkSimpleListItem combo_item;
    w_gtk_combobox_get_item (combo, position, &combo_item);
    return combo_item.text; //must be freed
}

/*--------------------------------------------------------------
/              w_gtk_combobox_get_item_data
/-------------------------------------------------------------*/

char *w_gtk_combobox_get_item_data (GtkWidget *combo, int position)
{
    WGtkSimpleListItem combo_item;
    w_gtk_combobox_get_item (combo, position, &combo_item);
    return combo_item.data;
}


/*--------------------------------------------------------------
/              w_gtk_combobox_get_selected
/-------------------------------------------------------------*/

int w_gtk_combobox_get_selected (GtkWidget *combo, WGtkSimpleListItem *out_item)
{
    // Returns an integer which is the index of the currently active item
    //   or -1 if there's no active item.
    // if out_item->text != NULL, it must be freed
    int active = -1;
    if (out_item) {
        memset (out_item, 0, sizeof(*out_item));
        out_item->index = -1;
    }
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo)) //TODO: this has not been tested
    {
        unsigned int sel;
        GListModel * model = gtk_drop_down_get_model (GTK_DROP_DOWN(combo));
        GtkStringList *sl = GTK_STRING_LIST(model);
        sel = gtk_drop_down_get_selected (GTK_DROP_DOWN(combo));
        if (sel == GTK_INVALID_LIST_POSITION) {
            return -1;
        } else {
            active = (int) sel;
        }
        if (out_item && active > -1) {
            out_item->index = active;
            out_item->text  = g_strdup (gtk_string_list_get_string(sl, sel));
        }
        return active;
    }
#endif
#if (GTK_CHECK_VERSION(2,4,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_COMBO_BOX(combo))
    {
        active = gtk_combo_box_get_active (GTK_COMBO_BOX(combo));
        if (out_item && active > -1)
        {
            out_item->index = active;
            GtkTreeIter iter;
            GtkTreeModel *model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
            if (gtk_tree_model_get_n_columns(model) == 2) {
                // the W combo created with w_gtk_combobox_new()
                if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo), &iter)) {
                    gtk_tree_model_get (model, &iter,
                                        0, &(out_item->text),
                                        1, &(out_item->data), -1);
                }
            } else { // normal ComboBoxText
                if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo), &iter)) {
                    gtk_tree_model_get (model, &iter,
                                        0, &(out_item->text), -1);
                }
            }
        }
        return active;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget *list  = GTK_COMBO(combo)->list;
        GList *selection = GTK_LIST(list)->selection;
        if (selection) { // selection->data = GtkListItem
            active = g_list_index (GTK_LIST(list)->children, selection->data);
            ///active = gtk_list_child_position (GTK_LIST(list), selection->data);
            if (out_item && active > -1) {
                out_item->index = active;
                GtkWidget *list_item = GTK_WIDGET(selection->data);
                GtkWidget *label_w   = gtk_bin_get_child (GTK_BIN(list_item));
                out_item->text = g_strdup (gtk_label_get_text (GTK_LABEL(label_w)));
                out_item->data = g_object_get_data (G_OBJECT(list_item), "itemdata"); 
            }
        }
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        ///active = gtk_option_menu_get_history (GTK_OPTION_MENU(combo));
        GtkWidget *menu = gtk_option_menu_get_menu (GTK_OPTION_MENU(combo));
        GtkWidget *menu_item = gtk_menu_get_active (GTK_MENU(menu));
        if (menu_item) { // GtkMenuItem
            active = g_list_index (GTK_MENU_SHELL(menu)->children, menu_item);
            if (out_item && active > -1) {
                out_item->index = active;
                out_item->data = g_object_get_data (G_OBJECT(menu_item), "itemdata"); 
                // gtk_menu_item_get_label() fails here, broken in gtk1 & gtk2
                out_item->text = g_strdup (g_object_get_data (G_OBJECT(menu_item), "wlabel")); 
            }
        }
    }
#endif
    return active;
}


/*--------------------------------------------------------------
/              w_gtk_combobox_get_active_text
/-------------------------------------------------------------*/

char *w_gtk_combobox_get_active_text (GtkWidget *combo)
{
    // if returned string != NULL, is must be freed
    char *itext = NULL;
    const char *entry_text = w_gtk_combobox_get_entry_text (combo);
    WGtkSimpleListItem combo_item;
    if (entry_text)
    { // for GtkCombo and GtkComboBox with entry, this ends here
        itext = g_strdup (entry_text);
        return itext;
    }
    w_gtk_combobox_get_selected (combo, &combo_item);
    itext = combo_item.text;
    return itext;
}


/*--------------------------------------------------------------
/             w_gtk_combobox_get_active_index
/-------------------------------------------------------------*/

int w_gtk_combobox_get_active_index (GtkWidget *combo)
{
    return w_gtk_combobox_get_selected (combo, NULL);
}


/*--------------------------------------------------------------
/            w_gtk_combobox_set_active_index
/-------------------------------------------------------------*/

void w_gtk_combobox_set_active_index (GtkWidget *combo, int index)
{
    // index -1: no active item
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo)) //TODO: this has not been tested
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
#if (GTK_CHECK_VERSION(2,4,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_COMBO_BOX(combo))
    {
        gtk_combo_box_set_active (GTK_COMBO_BOX(combo), index);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
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


/*--------------------------------------------------------------
/               w_gtk_combobox_search_text
/-------------------------------------------------------------*/

int w_gtk_combobox_search_text (GtkWidget *combo, const char *str,
                                gboolean select, void **out_data)
{
    // returns item index or -1 (error)
    intptr_t case_insensitive;
    int (*strcmp_func) (const char*, const char*);
    char *value = NULL;
    int index = -1;
    if (out_data) {
        *out_data = NULL;
    }
    if (!str) {
        return -1;
    }
    case_insensitive = GPOINTER_TO_INT (g_object_get_data (G_OBJECT(combo), "w_search_insensitive"));
    if (case_insensitive) {
        strcmp_func = strcasecmp;
    } else {
        strcmp_func = strcmp;
    }
    ///printf ("case_insensitive: %d\n", case_insensitive);
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo)) //TODO: this has not been tested
    {
        GListModel * model  = gtk_drop_down_get_model (GTK_DROP_DOWN(combo));
        GtkStringList *list = GTK_STRING_LIST(model);
        guint count = g_list_model_get_n_items (model);
        guint i;
        for (i = 0; i < count; i++)
        {
            value = (char*) gtk_string_list_get_string (list, i);
            g_warn_if_fail (value != NULL);
            if (strcmp_func (value, str) == 0)
            { // found
                if (select) {
                    gtk_drop_down_set_selected (GTK_DROP_DOWN(combo), i);
                }
                index = (int) i;
                return index;
            }
        }
        return -1;
    }
#endif
#if (GTK_CHECK_VERSION(2,4,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_COMBO_BOX(combo))
    {
        GtkTreeModel *model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
        GtkTreeIter iter;
        gboolean valid = gtk_tree_model_get_iter_first (model, &iter);
        while (valid)
        {
            index++;
            gtk_tree_model_get (model, &iter, 0, &value, -1);
            g_warn_if_fail (value != NULL);
            if (strcmp_func (value, str) == 0)
            { // found
                g_free (value); // string values must be freed
                if (out_data) {
                    g_warn_if_fail (gtk_tree_model_get_n_columns(model) == 2);
                    gtk_tree_model_get (model, &iter, 1, out_data, -1);
                }
                if (select) {
                    gtk_combo_box_set_active_iter (GTK_COMBO_BOX(combo), &iter);
                }
                return index;
                break;
            }
            g_free (value);
            valid = gtk_tree_model_iter_next (model, &iter);
        }
        return -1;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget *list = GTK_COMBO(combo)->list;
        GList    *items = GTK_LIST(list)->children;
        GtkWidget *label_w, *list_item;
        GList *igl;
        for (igl = items; igl != NULL; igl = igl->next)
        {
            index++;
            list_item = GTK_WIDGET(igl->data); // igl->data = GtkListItem
            label_w   = gtk_bin_get_child (GTK_BIN(list_item));
            value     = (char*) gtk_label_get_text (GTK_LABEL(label_w));
            if (strcmp_func (value, str) == 0)
            { // found
                if (out_data) {
                    *out_data = g_object_get_data (G_OBJECT(list_item), "itemdata");
                }
                if (select) {
                    gtk_list_select_child (GTK_LIST(list), list_item);
                }
                return index;
            }
        }
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        GtkWidget *menu, *menu_item; //GtkMenu, GtkMenuItem
        GList *items, *igl;
        menu  = gtk_option_menu_get_menu (GTK_OPTION_MENU(combo));
        items = GTK_MENU_SHELL(menu)->children;
        for (igl = items; igl != NULL; igl = igl->next)
        {
            index++;
            menu_item = GTK_WIDGET(igl->data); // igl->data = GtkMenuItem
            // gtk_menu_item_get_label() fails here, broken in gtk1 & gtk2
            value     = g_object_get_data (G_OBJECT(menu_item), "wlabel");
            if (value && strcmp_func(value, str) == 0)
            { // found
                if (out_data) {
                    *out_data = g_object_get_data (G_OBJECT(menu_item), "itemdata");
                }
                if (select) {
                    gtk_option_menu_set_history (GTK_OPTION_MENU(combo), index);
                }
                return index;
            }
        }
    }
#endif
    return -1;
}


/*--------------------------------------------------------------
/              w_gtk_combobox_search_data
/-------------------------------------------------------------*/

int w_gtk_combobox_search_data (GtkWidget *combo, gpointer wdata, gboolean select)
{
    // returns item index or -1 (error)
    int index = -1;
    gpointer value = NULL;
    if (!wdata) {
        return -1;
    }
#if GTK_CHECK_VERSION(4,0,0)
    if (GTK_IS_DROP_DOWN(combo)) //TODO
    {
        g_warning ("GtkDropDown does not support searching by data");
        return index;
    }
#endif
#if (GTK_CHECK_VERSION(2,4,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_COMBO_BOX(combo))
    {
        GtkTreeModel *model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
        g_warn_if_fail (gtk_tree_model_get_n_columns(model) == 2);
        GtkTreeIter iter;
        gboolean valid = gtk_tree_model_get_iter_first (model, &iter);
        while (valid)
        {
            index++;
            gtk_tree_model_get (model, &iter, 1, &value, -1);
            if (value == wdata) { // found
                if (select) {
                    gtk_combo_box_set_active_iter (GTK_COMBO_BOX(combo), &iter);
                }
                return index;
            }
            valid = gtk_tree_model_iter_next (model, &iter);
        }
        return -1;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_COMBO(combo))
    {
        GtkWidget *list = GTK_COMBO(combo)->list;
        GList    *items = GTK_LIST(list)->children;
        GList *igl;
        for (igl = items; igl != NULL; igl = igl->next)
        {
            index++; // igl->data = GtkListItem
            value = g_object_get_data (G_OBJECT(igl->data), "itemdata");
            if (value == wdata) { // found
                if (select) {
                    gtk_list_item_select (GTK_LIST_ITEM(G_OBJECT(igl->data)));
                    ///gtk_list_select_child (GTK_LIST(list), list_item);
                }
                return index;
            }
        }
    }
    else if (GTK_IS_OPTION_MENU(combo))
    {
        GtkWidget *menu = gtk_option_menu_get_menu (GTK_OPTION_MENU(combo));
        GList    *items = GTK_MENU_SHELL(menu)->children;
        GList *igl;
        for (igl = items; igl != NULL; igl = igl->next)
        {
            index++; // igl->data = GtkMenuItem
            value = g_object_get_data (G_OBJECT(igl->data), "itemdata");
            if (value == wdata) { // found
                if (select) {
                    gtk_option_menu_set_history (GTK_OPTION_MENU(combo), index);
                    //these don't work: gtk_menu_item_select / gtk_menu_shell_select_item
                }
                return index;
            }
        }
    }
#endif
    return -1;
}


/*--------------------------------------------------------------
/         w_gtk_combobox_set_search_case_insensitive
/-------------------------------------------------------------*/

void w_gtk_combobox_set_search_case_insensitive (GtkWidget *combo, gboolean insensitive)
{
    if (insensitive) {
        g_object_set_data (G_OBJECT(combo), "w_search_insensitive", GINT_TO_POINTER(1));
    } else {
        g_object_set_data (G_OBJECT(combo), "w_search_insensitive", GINT_TO_POINTER(0));
    }
}

/*--------------------------------------------------------------
/         w_gtk_combobox_get_search_case_insensitive
/-------------------------------------------------------------*/

gboolean w_gtk_combobox_get_search_case_insensitive (GtkWidget *combo)
{
    intptr_t ret;
    ret = GPOINTER_TO_INT (g_object_get_data (G_OBJECT(combo), "w_search_insensitive"));
    return ret;
}


/* --------------------------------------------------------------
/            w_gtk_combobox_get_entry_text
/-------------------------------------------------------------*/

const char *w_gtk_combobox_get_entry_text (GtkWidget *combo)
{
    GtkWidget *entry = w_gtk_combobox_get_entry (combo);
    const char *text = NULL;
    if (entry) {
#if GTK_CHECK_VERSION(4,0,0)
        text = gtk_editable_get_text (GTK_EDITABLE(entry));
#else
        text = gtk_entry_get_text (GTK_ENTRY(entry));
#endif
    }
    return text;
}


/*--------------------------------------------------------------
/            w_gtk_combobox_populate_from_glist
/-------------------------------------------------------------*/

void w_gtk_combobox_populate_from_glist (GtkWidget *combo, GList *strings, int default_index)
{
    //GtkCombo: gtk_combo_set_popdown_strings (GTK_COMBO(combo), strings);
    GList * list;
    char * text;
    int len = 0;

    w_gtk_combobox_clear (combo);
    if (!strings) {
        return; // nothing to add
    }
    for (list = strings;  list;  list = list->next)
    {
        text = (char *) list->data;
        w_gtk_combobox_append_text (combo, text);
        len++;
    }
    if (default_index >= len) {
        default_index = 0;
    }
    if (default_index >= 0) {
        w_gtk_combobox_set_active_index (combo, default_index);
    }
}


/*--------------------------------------------------------------
/             w_gtk_combobox_populate_from_strv
/-------------------------------------------------------------*/

void w_gtk_combobox_populate_from_strv (GtkWidget *combo,
                                         const char **strv,
                                         int default_index,
                                         gboolean gettext)
{
    int i;
    char *str;

    w_gtk_combobox_clear (combo);
    if (!strv || !*strv) {
        return; /* nothing to add */
    }
    for (i = 0; strv[i]; i++) {
#ifdef ENABLE_NLS
        str = gettext ? gettext(strv[i]) : (char*) strv[i];
#else
        str = (char*) strv[i];
#endif
        w_gtk_combobox_append_text (combo, str);
    }
    if (default_index >= i) {
        default_index = 0;
    }
    if (default_index >= 0) {
        w_gtk_combobox_set_active_index (combo, default_index);
    }
}


/*--------------------------------------------------------------
/            w_gtk_combobox_select_or_prepend
/-------------------------------------------------------------*/

void w_gtk_combobox_select_or_prepend (GtkWidget *combo, const char *str)
{
    if (!str || !*str) {
        return;
    }
    if (!w_gtk_combobox_search_text (combo, str, TRUE, NULL)) {
        w_gtk_combobox_prepend_text (combo, str);
        w_gtk_combobox_set_active_index (combo, 0);
    }
}


// ================================================================
// GtkComboBox-specific

void w_gtk_combo_box_set_w_model (GtkWidget *combo, gboolean sort)
{
#if (GTK_CHECK_VERSION(2,4,0) && GTK_MAJOR_VERSION <= 4)
    // this works with GtkComboBox, GtkComboBoxEntry, GtkComboBoxText
    // and other comboboxes, assuming the text column is 0
    // column 0: text
    // column 1: pointer (hidden)
    GtkListStore * store;
    GtkTreeSortable * sortable;
    GtkWidget *entry = w_gtk_combobox_get_entry (combo);

    store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);
    gtk_combo_box_set_model (GTK_COMBO_BOX(combo), GTK_TREE_MODEL(store));
    g_object_unref (store);

    if (!entry // GtkComboBoxEntry and GtkComboBoxText have already created a cell
#if GTK_CHECK_VERSION(2,24,0)
        && !GTK_IS_COMBO_BOX_TEXT(combo)
#endif
       )
    {
        GtkCellRenderer * cell;
        cell = gtk_cell_renderer_text_new ();
        gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (combo), cell, TRUE);
        gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (combo), cell,
                                        "text", 0, NULL);
    }
    if (sort) {
        sortable = GTK_TREE_SORTABLE(store);
        gtk_tree_sortable_set_sort_column_id (sortable, 0, GTK_SORT_ASCENDING);
    }

    // slackware 14.0 - gtk2.24 / gtk3.4 (this may not happen in newer distros)
    // * GLib-GObject-WARNING: unable to set property `text' of type `gchararray' from value of type `guchar'
#if GTK_MAJOR_VERSION == 2
    if (GTK_IS_COMBO_BOX_ENTRY(combo)) {
        gtk_combo_box_entry_set_text_column (GTK_COMBO_BOX_ENTRY(combo), 0);
        return;
    }
#endif
#if GTK_CHECK_VERSION(2,24,0)
    if (entry) {
        gtk_combo_box_set_entry_text_column (GTK_COMBO_BOX(combo), 0);
    }
#endif
#endif
}
