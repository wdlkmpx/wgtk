/*
 * Public Domain
 */

#if defined(__clang__)
#   pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "w_gtk.h"
#define W_GTK_VERSION GTK_MAJOR_VERSION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// put this in main() before initializing gtk and stuff
void w_gtk_check_version (int gtk_ver)
{
    if (W_GTK_VERSION != gtk_ver) {
        printf ("W_GTK_VERSION  : %d\n", W_GTK_VERSION);
        printf ("App GTK Version: %d\n",   gtk_ver);
        puts ("ERROR!\n");
        exit (-1);
    }
}


void w_gtk_widget_change_tooltip (GtkWidget *widget, const char *new_text)
{ /* changes widget tooltip only if the new_text is different */
    char *tip;
    tip = gtk_widget_get_tooltip_text (widget);
    if (!tip || g_strcmp0(tip,new_text) != 0) {
        gtk_widget_set_tooltip_text (widget, new_text);
    }
    g_free (tip);
}


GtkWidget *w_gtk_widget_set_scrolled_window (GtkWidget *widget,
                                             GtkWidget *parent_box)
{
    GtkWidget *scrollwin;
    scrollwin = g_object_get_data (G_OBJECT(widget), "w_scrolled_window");
    if (scrollwin) {
        return scrollwin;
    }
    scrollwin = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scrollwin),
                                    GTK_POLICY_AUTOMATIC,  // hscrollbar
                                    GTK_POLICY_AUTOMATIC); // vscrollbar
#if (GTK_MAJOR_VERSION == 2 || GTK_MAJOR_VERSION == 3)
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(scrollwin), GTK_SHADOW_IN);
#endif
    if (parent_box) {
        gtk_box_pack_start (GTK_BOX(parent_box), scrollwin, TRUE, TRUE, 0);
    }
    g_object_set_data (G_OBJECT(widget), "w_scrolled_window", scrollwin);

#if GTK_CHECK_VERSION(4,0,0)
    // GTK4 removed GtkContainer
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW(scrollwin), widget);
#elif GTK_CHECK_VERSION(3,8,0)
    // 3.8+: gtk_container_add() automatically adds a GtkViewPort
    // if the child doesn't implement GtkScrollable
    gtk_container_add (GTK_CONTAINER(scrollwin), widget);
#elif GTK_CHECK_VERSION(3,0,0)
    // 3.0+: GtkScrollable is an interface that is implemented by widgets with native scrolling
    if (GTK_IS_SCROLLABLE(widget)) {
        gtk_container_add (GTK_CONTAINER(scrollwin), widget);
    } else {
        gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW(scrollwin), widget);
    }
#else // GTK2 & 1
    GtkWidgetClass *widget_class;
    widget_class = GTK_WIDGET_GET_CLASS(widget);
    if (widget_class->set_scroll_adjustments_signal) {
        gtk_container_add (GTK_CONTAINER(scrollwin), widget);
    } else {
        gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW(scrollwin), widget);
    }
#endif
    return scrollwin;
}

// =====================================================================

GtkWidget * w_gtk_window_new (const char * title,
                              GtkWindow * parent,
                              GtkApplication * app, // NULL gtkcompat.h < 3 = `void * app`
                              gboolean resizable,
                              GtkWidget ** main_vbox  /* out */)
{
    GtkWidget * window;
    if (app) {
#ifdef USE_GTK_APPLICATION
        window = gtk_application_window_new (app);
#else
        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
#endif
    } else {
        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    }
    gtk_window_set_title (GTK_WINDOW (window), title);
    gtk_container_set_border_width (GTK_CONTAINER (window), 4); /* padding */
    if (parent) {
        gtk_window_set_transient_for (GTK_WINDOW (window), GTK_WINDOW (parent));
        gtk_window_set_modal (GTK_WINDOW (window), TRUE);
        gtk_window_set_destroy_with_parent (GTK_WINDOW (window), TRUE);
        gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER_ON_PARENT);
        gtk_window_set_skip_pager_hint (GTK_WINDOW (window), TRUE);
        gtk_window_set_skip_taskbar_hint (GTK_WINDOW (window), TRUE);
    }
    if (!resizable) {
        // no need to call this if TRUE, unexpected behavior in GTK3 IIRC
        gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
    }

    if (main_vbox) {
        *main_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add (GTK_CONTAINER(window), *main_vbox);
        /* padding */
        gtk_container_set_border_width (GTK_CONTAINER (*main_vbox), 5);
    }

    return window;
}


GtkWidget * w_gtk_window_add_main_vbox (GtkWidget *window)
{ // returns a GtkBox
    GtkWidget * main_vbox = NULL;
    if (GTK_IS_DIALOG(window)) {
        main_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add (GTK_CONTAINER(window), main_vbox);
        // padding
        gtk_container_set_border_width (GTK_CONTAINER (main_vbox), 5);
    } else if (GTK_IS_DIALOG(window)) {
        main_vbox = gtk_dialog_get_content_area (GTK_DIALOG (window));
        // padding
        gtk_container_set_border_width (GTK_CONTAINER (main_vbox), 4);
    }
    return main_vbox;
}


GtkWidget * w_gtk_dialog_new (const char * title,
                              GtkWindow * parent,
                              gboolean resizable,
                              GtkWidget ** main_vbox) /* out */
{
    GtkWidget * dialog;
    dialog = gtk_dialog_new ();
    gtk_window_set_title (GTK_WINDOW (dialog), title);
    gtk_container_set_border_width (GTK_CONTAINER (dialog), 4); /* padding */
    if (parent) {
        gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (parent));
        gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
        gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);
        gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER_ON_PARENT);
        gtk_window_set_skip_pager_hint (GTK_WINDOW (dialog), TRUE);
        gtk_window_set_skip_taskbar_hint (GTK_WINDOW (dialog), TRUE);
    }
#if GTK_MAJOR_VERSION >= 3 // always need a parent window
    else {
        GtkWidget * parent = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (parent));
        gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);
    }
#endif
    if (!resizable) {
        // no need to call this if TRUE, unexpected behavior in GTK3 IIRC
        gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
    }

    if (main_vbox) {
        *main_vbox = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
        /* padding */
        gtk_container_set_border_width (GTK_CONTAINER (*main_vbox), 4);
    }

    return dialog;
}


GtkWidget * w_gtk_frame_new (char * label, /* supports markup */
                             gboolean center_label,
                             GtkWidget * parent_box,
                             GtkWidget * label_widget)
{  // returns a GtkFrame
    GtkWidget * frame       = NULL;
#if GTK_CHECK_VERSION(2,0,0)
    GtkWidget * frame_label = NULL;
    frame = gtk_frame_new (NULL);
    gtk_box_pack_start (GTK_BOX (parent_box), frame, FALSE, FALSE, 0);
    if (label_widget) {
        // it's possible to add other widgets as a label for GtkFrame
        // i.e: GtkCheckMenuItem
        gtk_frame_set_label_widget (GTK_FRAME (frame), label_widget);
    } else if (label) {
        frame_label = gtk_label_new (label);
        gtk_frame_set_label_widget (GTK_FRAME (frame), frame_label);
        gtk_label_set_use_markup (GTK_LABEL (frame_label), TRUE);
        if (frame_label) gtk_widget_show (frame_label);
    }
#else // GTK1 doesn't support specifying a label widget // TODO: remove markup
	frame = gtk_frame_new (label);
    gtk_box_pack_start (GTK_BOX (parent_box), frame, FALSE, FALSE, 0);
#endif
    if (center_label) {
        gtk_frame_set_label_align (GTK_FRAME(frame), 0.5, 0.5);
    }
    // this doesn't work with gtk3+
    //gtk_frame_set_shadow_type (GTK_FRAME(frame), GTK_SHADOW_IN);
    gtk_widget_show (frame);
    return frame;
}


GtkWidget * w_gtk_frame_vbox_new (char * label, /* supports markup */
                                  int children_spacing,
                                  GtkWidget * parent_box,
                                  GtkWidget * frame_label_widget) /* NULL */
{
    // returns a vbox inside a frame
    GtkWidget * frame;
    GtkWidget * frame_vbox;
    frame      = w_gtk_frame_new (label, FALSE, parent_box, frame_label_widget);
    frame_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, children_spacing);
    gtk_container_add (GTK_CONTAINER (frame), frame_vbox);
    /* padding */
    gtk_container_set_border_width (GTK_CONTAINER (frame_vbox), 5);
    return frame_vbox;
}


GtkWidget * w_gtk_expander_vbox_new (char * label,
                                     int children_spacing,
                                     GtkWidget * parent_box)
{
    // returns a vbox inside a gtkexpander
    GtkWidget* expander;
    GtkWidget* hiddenbox;
#if GTK_CHECK_VERSION(2,4,0)
    expander = gtk_expander_new (label);
#else
	// too old, use a frame instead...
	expander = gtk_frame_new (label);
#endif
    if (parent_box) {
        gtk_box_pack_start (GTK_BOX (parent_box), expander, FALSE, FALSE, 0);
    }

    hiddenbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, children_spacing);
    gtk_container_add (GTK_CONTAINER (expander), hiddenbox);
    /* padding */
    //gtk_container_set_border_width (GTK_CONTAINER(hiddenbox), 5);
    return hiddenbox;
}


GtkWidget * w_gtk_image_new_from_icon_name (const char *icon_name, GtkIconSize size)
{
#if GTK_CHECK_VERSION(2,4,0) // GtkIconTheme was introduced in gtk 2.4
    GtkWidget *img = NULL;
    GtkIconTheme *icon_theme = gtk_icon_theme_get_default ();
    if (gtk_icon_theme_has_icon (icon_theme, icon_name)) {
        img = gtk_image_new_from_icon_name (icon_name, size);
    } else if (strncmp (icon_name, "gtk-", 4) == 0) {
        img = gtk_image_new_from_stock (icon_name, size);
    } else {
        // get blank/invalid image
        fprintf (stderr, "(GtkImage-new) %s was not found in icon theme\n", icon_name);
        img = gtk_image_new_from_icon_name (icon_name, size);
    }
    return img;
#else
    return NULL;
#endif

}


void w_gtk_image_set_from_icon_name (GtkImage *img, const char *icon_name, GtkIconSize size)
{
#if GTK_CHECK_VERSION(2,4,0) // GtkIconTheme was introduced in gtk 2.4
    if (!img) {
        return;
    }
    GtkIconTheme *icon_theme = gtk_icon_theme_get_default ();
    if (gtk_icon_theme_has_icon (icon_theme, icon_name)) {
        gtk_image_set_from_icon_name (img, icon_name, size);
    } else if (strncmp (icon_name, "gtk-", 4) == 0) {
        gtk_image_set_from_stock (img, icon_name, size);
    } else {
        // set blank/invalid image
        fprintf (stderr, "(GtkImage-set) %s was not found in icon theme\n", icon_name);
        gtk_image_set_from_icon_name (img, icon_name, size);
    }
#endif
}


void w_gtk_button_set_icon_name (GtkButton *button, const char *icon_name)
{
#if GTK_CHECK_VERSION(2,0,0) // GTK1 doesn't support pixbufs
    GtkWidget *img;
    img = w_gtk_image_new_from_icon_name (icon_name, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image (button, img);
#endif
}


GtkWidget * w_gtk_label_new (const char *text)
{
    GtkWidget * label;
    if (strchr(text, '_')) {
        label = gtk_label_new_with_mnemonic (text);
    } else {
        label = gtk_label_new (text);
    }
    if (strchr(text,'>') && strstr(text,"</")) {
        // example: "<b>_Close</b>"
        gtk_label_set_use_markup (GTK_LABEL(label), TRUE);
    }
    return label;
}


GtkWidget * w_gtk_button_new (const char * label, /* supports markup if not using icons */
                              const char * icon_name,
                              gpointer clicked_cb,
                              gpointer cdata)
{
    GtkWidget * button = NULL;
    GtkWidget * lbl = NULL;
    gboolean markup = FALSE;
    button = gtk_button_new ();
    if (label) {
        button = gtk_button_new_with_mnemonic (label);
        if (strchr(label,'>') && strstr(label,"</")) {
            lbl = gtk_button_get_child (GTK_BUTTON(button));
            gtk_label_set_use_markup (GTK_LABEL(lbl), TRUE);
            markup = TRUE;
        }
    }
    // Icons disrupt markup text, so ignore icons if using markup text
    if (icon_name && !markup) {
        w_gtk_button_set_icon_name (GTK_BUTTON(button), icon_name);
        gtk_button_set_always_show_image (GTK_BUTTON(button), TRUE); //GTK3.6+
    }
    if (clicked_cb) { // void button_cb (GtkButton *button, gpointer user_data)
        g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (clicked_cb), cdata);
    }
    gtk_widget_set_can_default (button, TRUE);
    return button;
}


void w_gtk_button_flat (GtkWidget * button, gboolean reduce_size)
{
    gtk_button_set_relief (GTK_BUTTON(button), GTK_RELIEF_NONE);
    gtk_button_set_focus_on_click (GTK_BUTTON(button), FALSE);
    gtk_widget_set_can_focus (GTK_WIDGET(button), FALSE);
    gtk_widget_set_can_default (button, FALSE);
    if (reduce_size) {
#if GTK_MAJOR_VERSION == 2 // this only works with gtk2, no other version
        /* Make the button as small as possible. */
        GtkRcStyle * rcstyle = gtk_rc_style_new();
        rcstyle->xthickness = rcstyle->ythickness = 0;
        gtk_widget_modify_style (button, rcstyle);
        g_object_ref (rcstyle);
#endif
    }
}


GtkWidget * w_gtk_toolbar_new  (GtkOrientation ori, GtkWidget *parent_box)
{
    GtkWidget *toolbar;
    toolbar = gtk_box_new (ori, 5);
    g_object_set_data (G_OBJECT(toolbar), "wgtktoolbar", toolbar);
    if (parent_box) {
        gtk_box_pack_start (GTK_BOX (parent_box), toolbar, FALSE, FALSE, 0);
    }
    return toolbar;
}


GtkWidget * w_gtk_toolbar_separator_new  (GtkWidget *box_toolbar)
{
    GtkWidget *sep;
    GtkOrientation orientation = GTK_ORIENTATION_VERTICAL;
    if (box_toolbar) {
        orientation = gtk_orientable_get_orientation (GTK_ORIENTABLE(box_toolbar));
    }
    sep = gtk_separator_new (orientation);
    if (box_toolbar) {
        gtk_box_pack_start (GTK_BOX(box_toolbar), sep, FALSE, FALSE, 0);
    }
    return sep;
}


GtkWidget * w_gtk_toolbar_button_new (GtkWidget *box_toolbar,
                                      const char *label, /* DOES NOT support markup */
                                      const char *tooltip,
                                      const char *icon_name,
                                      GtkWidget *wicon,
                                      gpointer clicked_cb, gpointer cdata)
{
    GtkWidget * button;
    GtkWidget * button_icon = wicon;
    // ignore label if there is an icon, it looks bad with both...
    if (!icon_name && !wicon) {
        button = gtk_button_new_with_mnemonic (label);
    } else {
        button = gtk_button_new ();
    }
    gtk_button_set_relief (GTK_BUTTON(button), GTK_RELIEF_NONE);
    if (icon_name) {
        button_icon = w_gtk_image_new_from_icon_name (icon_name, GTK_ICON_SIZE_LARGE_TOOLBAR);
    }
    if (button_icon) {
        gtk_button_set_image (GTK_BUTTON(button), button_icon);
        gtk_button_set_image_position (GTK_BUTTON(button), GTK_POS_TOP);
        gtk_button_set_always_show_image (GTK_BUTTON(button), TRUE); //GTK3.6+
    }
    if (tooltip) {
        gtk_widget_set_tooltip_text (button, tooltip);
    }
    if (clicked_cb) { // void button_cb (GtkButton *button, gpointer user_data)
        g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (clicked_cb), cdata);
    }
    if (box_toolbar) {
        gtk_box_pack_start (GTK_BOX(box_toolbar), button, FALSE, FALSE, 0);
    }
    return button;
}


GtkWidget * w_gtk_button_new_from_actions (WGtkActionEntry *actions,
                                           const char *action_name,
                                           GtkWidget *toolbar)
{  // check if action_name is in the action group
    GtkWidget *button = NULL;
    ///GtkWidget *toolbar = NULL;
    int found = 0;
    int i;
    for (i = 0; actions[i].name; i++)
    {
        if (strcmp(actions[i].name,action_name) == 0) {
            found = 1;
            break;
        }
    }
    if (!found) {
        printf ("not found: %s\n", action_name);
        return NULL; // action_name not found
    }
    ///if (box_parent) {
    ///    toolbar = g_object_get_data (box_parent, "wgtktoolbar");
    ///}
    if (toolbar) {
        button = w_gtk_toolbar_button_new (toolbar,
                                           actions[i].label,
                                           actions[i].tooltip,
                                           actions[i].icon_name,
                                           NULL,
                                           actions[i].callback, NULL);
    } else {
        button = w_gtk_button_new (actions[i].label,
                                   actions[i].icon_name,
                                   actions[i].callback, NULL);
    }

    return button;
}


GtkWidget * w_gtk_notebook_add_tab_box (GtkWidget * notebook, char * label_str)
{ // returns a GtkBox
    GtkWidget *vbox;
    GtkWidget *label;
    vbox  = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
    if (label_str && strchr(label_str,'_')) {
        label = gtk_label_new_with_mnemonic (label_str);
    } else {
        label = gtk_label_new (label_str);
    }
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 5); /* padding */
    gtk_notebook_append_page (GTK_NOTEBOOK(notebook), vbox, label);
    return vbox;
}


GtkWidget * w_gtk_notebook_add_tab_grid (GtkWidget * notebook, char * label_str)
{ // returns GtkGrid / GtkTable
    GtkWidget *vbox;
    GtkWidget *table;
    vbox = w_gtk_notebook_add_tab_box (notebook, label_str);
    table = w_gtk_grid_new (vbox, 0);
    return table;
}


/* ================================================== */
/*               GtkTable / GtkGrid                   */
/* ================================================== */

GtkWidget * w_gtk_grid_new (GtkWidget *parent_box, int row_spacing)
{
    // new GtkGrid or a GtkTable that must be resized
    GtkWidget * table;
    //table = gtkcompat_grid_new (1, 1);
#if GTK_MAJOR_VERSION >= 3
    table = gtk_grid_new ();
    gtk_grid_set_column_spacing (GTK_GRID(table), 5);
    gtk_grid_set_row_spacing (GTK_GRID(table), row_spacing);
#else
    table = gtk_table_new (1, 1, FALSE);
    gtk_table_set_col_spacings (GTK_TABLE(table), 5);
    gtk_table_set_row_spacings (GTK_TABLE(table), row_spacing);
#endif
    if (parent_box) {
        //gtk_box_set_homogeneous (GTK_BOX(box), TRUE);
        /* padding */
        gtk_container_set_border_width (GTK_CONTAINER(parent_box), 5);
        // FALSE, FALSE is required for GtkTable, otherwise
        // the widgets height may be incorrect
        gtk_box_pack_start (GTK_BOX(parent_box), table, FALSE, FALSE, 0);
    }
    return table;
}


static void set_walignment (WGtkAlignmentParams *col)
{
    GtkAlign align = col->align;
    if (!col->w) {
        return;
    }
#if GTK_MAJOR_VERSION >= 3
    // GtkGrid: columns don't have a proper width, this doesn't happen with GtkTable
    //   specially noticeable in GtkNotebook tabs
    gtk_widget_set_hexpand (col->w, TRUE);
#endif
    gtk_widget_set_halign (col->w, col->align);
    if (col->margin_start)  gtk_widget_set_margin_start (col->w, col->margin_start);
    if (col->margin_end)    gtk_widget_set_margin_end   (col->w, col->margin_end);
    if (col->margin_top)    gtk_widget_set_margin_top   (col->w, col->margin_top);
    if (col->margin_bottom) gtk_widget_set_margin_bottom(col->w, col->margin_bottom);
    memset (col, 0, sizeof(*col));
    // keep align
    col->align = align;
}

void w_gtk_grid_append_row (WGtkTableParams *t)
{
    int ncols;
    if (!t->c1.w && !t->c2.w && !t->c3.w && !t->c4.w) {
        t->c1.w = gtk_label_new ("");
    }
    if (t->cols == 1)
    {
        gtk_grid_attach (GTK_GRID(t->table), t->c1.w, 0, t->row, 1, 1);
    }
    else if (t->cols == 2)
    {
        if (t->c1.w && t->c2.w) {
            gtk_grid_attach (GTK_GRID(t->table), t->c1.w, 0, t->row, 1, 1);
            gtk_grid_attach (GTK_GRID(t->table), t->c2.w, 1, t->row, 1, 1);
        } else if (t->c1.w && !t->c2.w) {
            ncols = t->c1.width ? t->c1.width : 2;
            gtk_grid_attach (GTK_GRID(t->table), t->c1.w, 0, t->row, ncols, 1);
        } else if (t->c2.w) {
            // only 2nd column
            gtk_grid_attach (GTK_GRID(t->table), t->c2.w, 1, t->row, 1, 1);
        }
    }
    else if (t->cols == 3 || t->cols == 4)
    {
        if (t->c1.w) {
            gtk_grid_attach (GTK_GRID(t->table), t->c1.w, 0, t->row, 1, 1);
        }
        if (t->c2.w) {
            gtk_grid_attach (GTK_GRID(t->table), t->c2.w, 1, t->row, 1, 1);
        }
        if (t->c3.w) {
            gtk_grid_attach (GTK_GRID(t->table), t->c3.w, 2, t->row, 1, 1);
        }
        if (t->cols == 4) {
            if (t->c4.w) {
                gtk_grid_attach (GTK_GRID(t->table), t->c4.w, 3, t->row, 1, 1);
            }
        }
    }
    else
    {
        fprintf (stderr, "w_gtk_grid_append_row(): no more than 4 rows are supported");
    }
    set_walignment (&(t->c1));
    set_walignment (&(t->c2));
    set_walignment (&(t->c3));
    set_walignment (&(t->c4));
    // increase current row
    t->row = t->row + 1;
}

/* ================================================== */
/*                  TREE VIEW                         */
/* ================================================== */

#if GTK_CHECK_VERSION(2,0,0)

int w_gtk_tree_view_get_num_selected (GtkWidget *tv)
{
    GtkTreeView      *tree = GTK_TREE_VIEW (tv);
    GtkTreeSelection *tsel = gtk_tree_view_get_selection (tree);
    int              count = gtk_tree_selection_count_selected_rows (tsel);
    return count;
}


void w_gtk_tree_view_clear (GtkWidget *tv)
{
    GtkTreeView  *tree  = GTK_TREE_VIEW (tv);
    GtkTreeModel *model = gtk_tree_view_get_model (tree);
    if (GTK_IS_LIST_STORE(model)) {
        gtk_list_store_clear (GTK_LIST_STORE(model));
    } else if (GTK_IS_TREE_STORE(model)) {
        gtk_tree_store_clear (GTK_TREE_STORE(model));
    } else {
        fprintf (stderr, "w_gtk_tree_view_clear: error\n");
    }
}


void w_gtk_tree_view_select_all (GtkWidget *tv)
{
    GtkTreeView      *tree = GTK_TREE_VIEW (tv);
    GtkTreeSelection *tsel = gtk_tree_view_get_selection (tree);
    gtk_tree_selection_select_all (tsel);
}


void w_gtk_tree_view_deselect_all (GtkWidget *tv)
{
    GtkTreeView      *tree = GTK_TREE_VIEW (tv);
    GtkTreeSelection *tsel = gtk_tree_view_get_selection (tree);
    gtk_tree_selection_unselect_all (tsel);
}


void w_gtk_tree_view_select_row (GtkWidget *tv, int n)
{
    GtkTreeView      *tree  = GTK_TREE_VIEW (tv);
    GtkTreeSelection *tsel  = gtk_tree_view_get_selection (tree);
    GtkTreePath      *tpath = gtk_tree_path_new_from_indices (n, -1);
    gtk_tree_selection_select_path (tsel, tpath);
    gtk_tree_path_free (tpath);
}

#endif

/* ================================================== */
/*                   menus ....                       */
/* ================================================== */

GtkWidget * w_gtk_recent_menu_new (const char * application,
                                   gpointer activated_cb)
{
#if GTK_CHECK_VERSION(2,10,0) // GktRecentFilter was added in gtk 2.10
    GtkRecentFilter *recent_filter;
    GtkWidget *recent_menu;
    recent_filter = gtk_recent_filter_new ();
    recent_menu   = gtk_recent_chooser_menu_new ();
    gtk_recent_filter_add_application (recent_filter, application);
    gtk_recent_chooser_add_filter (GTK_RECENT_CHOOSER(recent_menu), recent_filter);
    gtk_recent_chooser_set_show_tips (GTK_RECENT_CHOOSER(recent_menu), TRUE);
    gtk_recent_chooser_set_sort_type (GTK_RECENT_CHOOSER(recent_menu), GTK_RECENT_SORT_MRU);
    gtk_recent_chooser_set_local_only (GTK_RECENT_CHOOSER(recent_menu), FALSE);
    if (activated_cb) {
        g_signal_connect(G_OBJECT(recent_menu),
                         "item-activated", G_CALLBACK(activated_cb), NULL);
    }
    return recent_menu;
#else
	return gtk_menu_new ();
#endif
}


#if GTK_CHECK_VERSION(2,4,0) // GtkAction was added in gtk 2.4
void w_gtk_action_group_destroy_action (GtkActionGroup *action_group,
                                        const char *action_name)
{
    GtkAction * action;
    GSList * widget;
    action = gtk_action_group_get_action (action_group, action_name);
    if (action) {
        gtk_action_group_remove_action (action_group, action);
        widget = gtk_action_get_proxies (action);
        while (widget)
        {
            gtk_widget_destroy (GTK_WIDGET(widget->data));
            widget = widget->next;
        }
        g_object_unref (action);
    }
}
#endif


/* ================================================== */
/*                   GTK < 3.0                        */
/* ================================================== */

#if GTK_MAJOR_VERSION < 3

GtkWidget * gtk_grid_new (void)
{
    return gtk_table_new (1, 1, FALSE);
}

GtkWidget * w_gtk_widget_get_walignment (GtkWidget *widget, gboolean create_if_needed)
{
    // this will be handy to emulate some gtk3 functions (gtk_widget_set_halign, etc)
    // use g_object_get_data(widget,"walignment") to get the alignment to apply properties
    GtkWidget *alignment;
    if (!widget) {
        return NULL; /* error */
    }
    // return itself if widget is a GtkAlignment
    if (GTK_IS_ALIGNMENT(widget)) {
        return widget;
    }
    // see if widget already has a GtkAlignment widget
    alignment = g_object_get_data (G_OBJECT(widget), "walignment");
    if (alignment) {
        return alignment;
    }
    if (create_if_needed) {
        // create new GtkAlignment and assign values
        alignment = gtk_alignment_new (0.5, 0.5, 0, 0);
        gtk_container_add (GTK_CONTAINER(alignment), widget);
        g_object_set_data (G_OBJECT(widget), "walignment", alignment);
        g_object_set_data (G_OBJECT(alignment), "waligned", widget);
    }
    return alignment;
}


static void misc_set_alignment (GtkWidget *widget, GtkAlign align, gboolean horizontal)
{
    // only GtkLabel/GtkArrow/GtkImage/GtkPixmap support this feature
    GtkMisc *misc = GTK_MISC (widget);
    gfloat *misc_align = horizontal ? &(misc->xalign) : &(misc->yalign);
    char *align_str    = horizontal ? "xalign" : "yalign";
    //--
    gfloat walign = 0.0;
    if   (align == GTK_ALIGN_CENTER) walign = 0.5;
    else if (align == GTK_ALIGN_END) walign = 1.0;
    if (walign != *misc_align) {
        g_object_freeze_notify (G_OBJECT (misc));
        g_object_notify (G_OBJECT (misc), align_str);
        *misc_align = walign;
        if (gtk_widget_is_drawable (widget)) {
            gtk_widget_queue_draw (widget);
        }
        g_object_thaw_notify (G_OBJECT (misc));
    }
}

static void misc_set_padding (GtkWidget *widget, gint margin, gboolean horizontal)
{
    // only GtkLabel/GtkArrow/GtkImage/GtkPixmap support this feature
    GtkMisc *misc = GTK_MISC (widget);
    guint16 *misc_pad = horizontal ? &(misc->xpad) : &(misc->ypad);
    char *pad_str = horizontal ? "xpad" : "ypad";
    if (margin < 0) margin = 0;
    if (margin != *misc_pad) {
        g_object_freeze_notify (G_OBJECT (misc));
        g_object_notify (G_OBJECT (misc), pad_str);
        *misc_pad = margin;
        if (gtk_widget_is_drawable (widget)) {
            gtk_widget_queue_resize (GTK_WIDGET (misc));
        }
        g_object_thaw_notify (G_OBJECT (misc));
    }
}


static int alignment_set_align (GtkWidget *widget, GtkAlign align, char *property)
{
    char *p2;
    GtkWidget *alignment = w_gtk_widget_get_walignment (widget, FALSE);
    if (!alignment) {
        return -1;
    }
    if (*property == 'x') {
        p2 = "xscale";
    } else {
        p2 = "yscale";
    }
    gfloat walign = 0.0;
    if   (align == GTK_ALIGN_CENTER) walign = 0.5;
    else if (align == GTK_ALIGN_END) walign = 1.0;
    if (align == GTK_ALIGN_FILL) {
        /* fill GtkAlignment widget */
        g_object_set (G_OBJECT(alignment), p2, 1.0, NULL);
    } else {
        g_object_set (G_OBJECT(alignment), property, walign, NULL);
    }
    return 0;
}

void gtk_widget_set_halign (GtkWidget *widget, GtkAlign align)
{
    if (alignment_set_align (widget, align, "xalign") != -1) {
        return;
    }
    if (GTK_IS_MISC(widget)) {
        misc_set_alignment (widget, align, TRUE);
        return;
    }
}

void gtk_widget_set_hexpand (GtkWidget *widget, gboolean expand)
{
    if (expand == TRUE) {
        alignment_set_align (widget, GTK_ALIGN_FILL, "xalign");
    }
}

void gtk_widget_set_vexpand (GtkWidget *widget, gboolean expand)
{
    if (expand == TRUE) {
        alignment_set_align (widget, GTK_ALIGN_FILL, "yalign");
    }
}

void gtk_widget_set_valign (GtkWidget *widget, GtkAlign align)
{
    if (alignment_set_align (widget, align, "yalign") != -1) {
        return;
    }
    if (GTK_IS_MISC(widget)) {
        misc_set_alignment (widget, align, FALSE);
        return;
    }
}

void gtk_widget_set_margin_start  (GtkWidget *widget, gint margin)
{
    GtkWidget *alignment = w_gtk_widget_get_walignment (widget, FALSE);;
    if (alignment) {
        g_object_set (G_OBJECT(alignment), "left-padding", margin, NULL);
        return;
    }
    if (GTK_IS_MISC(widget)) {
        misc_set_padding (widget, margin, TRUE);
        return;
    }
}

void gtk_widget_set_margin_end (GtkWidget *widget, gint margin)
{
    GtkWidget *alignment = w_gtk_widget_get_walignment (widget, FALSE);;
    if (alignment) {
        g_object_set (G_OBJECT(alignment), "right-padding", margin, NULL);
    }
}

void gtk_widget_set_margin_bottom (GtkWidget *widget, gint margin)
{
    GtkWidget *alignment = w_gtk_widget_get_walignment (widget, FALSE);;
    if (alignment) {
        g_object_set (G_OBJECT(alignment), "bottom-padding", margin, NULL);
    }
}

void gtk_widget_set_margin_top (GtkWidget *widget, gint margin)
{
    GtkWidget *alignment = w_gtk_widget_get_walignment (widget, FALSE);;
    if (alignment) {
        g_object_set (G_OBJECT(alignment), "top-padding", margin, NULL);
    }
    if (GTK_IS_MISC(widget)) {
        misc_set_padding (widget, margin, FALSE);
        return;
    }
}


void gtk_grid_attach (GtkGrid *grid, GtkWidget *child, gint left, gint top, gint width, gint height)
{
    // gtk_widget_set_*align / gtk_widget_set_*expand / gtk_widget_set_margin_*
    //   will only work after gtk_grid_attach, calling them before has no effect...
    int col = left+1;
    int row = top+1;
    guint rows;
    guint cols;
    gtk_table_get_size (GTK_TABLE(grid), &rows, &cols);
    //printf ("%d-%d    %u-%u\n", col, row, cols, rows);
    if (col > cols || row > rows) {
        // the table needs to be bigger, resize (it should be created with 1_row 1_col)
        //printf ("--resize  %u-%u\n", (col > cols) ? col : cols, (row > rows) ? row : rows);
        gtk_table_resize (GTK_TABLE(grid),
                          (row > rows) ? row : rows,
                          (col > cols) ? col : cols);
    }
    // attach widget with a GtkAlignment and enable emulation of
    //   gtk_widget_set_halign, gtk_widget_set_valign, etc
    GtkWidget *alignment;
    alignment = w_gtk_widget_get_walignment (child, TRUE);
    //if (GTK_IS_ALIGNMENT(child)) {
        gtk_table_attach (grid, alignment, left, left+width, top, top+height,
                          GTK_EXPAND|GTK_FILL, GTK_EXPAND|GTK_FILL, 0, 0);
    //} else {
    //    gtk_table_attach_defaults (grid, child, left, left+width, top, top+height);
    //}
}



char *gtk_font_chooser_get_font (GtkFontChooser* fontchooser)
{ // gtk3.2+
    char *ret = NULL;
    if (GTK_IS_FONT_SELECTION(fontchooser)) {
        ret = gtk_font_selection_get_font_name (GTK_FONT_SELECTION(fontchooser));
    } else if (GTK_IS_FONT_SELECTION_DIALOG(fontchooser)) {
        ret = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG(fontchooser));
#if GTK_CHECK_VERSION(2,4,0) // GtkFontButton appears in 2.4
    } else if (GTK_IS_FONT_BUTTON(fontchooser)) {
        // returns an internal copy of the font name, strdup
        ret = g_strdup (gtk_font_button_get_font_name (GTK_FONT_BUTTON(fontchooser)));
#endif
    }
    return ret;
}


void gtk_font_chooser_set_font (GtkFontChooser *fontchooser, const char *fontname)
{ // gtk3.2+
    if (GTK_IS_FONT_SELECTION(fontchooser)) {
        gtk_font_selection_set_font_name (GTK_FONT_SELECTION(fontchooser), fontname);
    } else if (GTK_IS_FONT_SELECTION_DIALOG(fontchooser)) {
        gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG(fontchooser), fontname);
#if GTK_CHECK_VERSION(2,4,0) // GtkFontButton appears in 2.4
    } else if (GTK_IS_FONT_BUTTON(fontchooser)) {
        // GtkFontButton is not deprecated in GTK3, it implements GtkFontChooser
        //  in GTK2, GtkFontButton does not implement GtkFontSelection (publicly)
        gtk_font_button_set_font_name (GTK_FONT_BUTTON(fontchooser), fontname);
#endif
    }
}

void gtk_font_chooser_set_preview_text (GtkFontChooser *fontchooser, const char *text)
{
    if (GTK_IS_FONT_SELECTION(fontchooser)) {
        gtk_font_selection_set_preview_text (GTK_FONT_SELECTION(fontchooser), text);
    } else if (GTK_IS_FONT_SELECTION_DIALOG(fontchooser)) {
        gtk_font_selection_dialog_set_preview_text (GTK_FONT_SELECTION_DIALOG(fontchooser), text);
    }
}

char * gtk_font_chooser_get_preview_text (GtkFontChooser *fontchooser)
{
    const char * ptext = NULL;
    if (GTK_IS_FONT_SELECTION(fontchooser)) {
        ptext = gtk_font_selection_get_preview_text (GTK_FONT_SELECTION(fontchooser));
    } else if (GTK_IS_FONT_SELECTION_DIALOG(fontchooser)) {
        ptext = gtk_font_selection_dialog_get_preview_text (GTK_FONT_SELECTION_DIALOG(fontchooser));
    }
    if (ptext) {
        return g_strdup(ptext); // must be freed
    } else {
        return NULL;
    }
}

//https://developer-old.gnome.org/gtk2/stable/GtkFontSelection.html
// 2.14: GtkFontSelection adopts Pango
#if GTK_CHECK_VERSION(2,14,0)
PangoFontFace * gtk_font_chooser_get_font_face (GtkFontChooser *fontchooser)
{
    return gtk_font_selection_get_face (GTK_FONT_SELECTION(fontchooser));
}

PangoFontFamily * gtk_font_chooser_get_font_family (GtkFontChooser *fontchooser)
{
    return gtk_font_selection_get_family (GTK_FONT_SELECTION(fontchooser));
}

int gtk_font_chooser_get_font_size (GtkFontChooser *fontchooser)
{
    return gtk_font_selection_get_size (GTK_FONT_SELECTION(fontchooser));
}

GtkWidget* gtk_font_chooser_widget_new (void)
{
    return gtk_font_selection_new ();
}
#endif // Pango

#endif /*   GTK < 3.0  */

