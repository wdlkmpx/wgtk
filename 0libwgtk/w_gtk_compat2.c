/*
 * Public Domain

 Extended compatibility for GTK 2.24 (make apps compile with versions older than GTK2.24)

 */

#if defined(__clang__)
#   pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include <gtk/gtk.h>
#include "w_gtk.h"


#if GTK_MAJOR_VERSION <= 2


//==================================================
//                   GTK < 2.24                     
//==================================================

#if !GTK_CHECK_VERSION(2, 24, 0)
gboolean gtk_combo_box_get_has_entry (GtkComboBox *combo_box)
{
    GtkWidget *entry = w_gtk_combobox_get_entry (GTK_WIDGET(combo_box));
    if (entry) {
        return TRUE;
    } else {
        return FALSE;
    }
}
#endif


//==================================================
//                   GTK < 2.22
//==================================================

#if !GTK_CHECK_VERSION(2, 22, 0)
gboolean gtk_widget_send_focus_change (GtkWidget *widget, GdkEvent *event)
{
#if GTK_CHECK_VERSION(2,0,0)
    g_object_ref (widget);
    GTK_OBJECT_FLAGS (widget) |= GTK_HAS_FOCUS;
    gtk_widget_event (widget, event);
    g_object_notify (G_OBJECT (widget), "has-focus");
    g_object_unref (widget);
#endif
	return FALSE;
}

#if GTK_CHECK_VERSION(2,8,0) // Cairo stuff was added in gtk 2.8
cairo_surface_t * gdk_window_create_similar_surface (GdkWindow *window,
                                                     cairo_content_t content,
                                                     int width, int height)
{
   cairo_t *cr = gdk_cairo_create (window);
   cairo_surface_t *window_surface, *out_surface;
   window_surface = cairo_get_target (cr);
   out_surface = cairo_surface_create_similar (window_surface, content, width, height);
   cairo_destroy (cr);
   out_surface;
}
#endif
#endif


//==================================================
//                   GTK < 2.16
//==================================================

#if !GTK_CHECK_VERSION(2, 16, 0)
GtkOrientation gtk_orientable_get_orientation (GtkOrientable  *widget)
{
    GtkOrientation ret = (GtkOrientation) 0;
    if (GTK_IS_HBOX(widget)
        || GTK_IS_HBUTTON_BOX(widget)
        || GTK_IS_HPANED(widget)
        || GTK_IS_HRULER(widget)
        || GTK_IS_HSCALE(widget)
        || GTK_IS_HSCROLLBAR(widget)
        || GTK_IS_HSEPARATOR(widget))
    {
        ret = GTK_ORIENTATION_HORIZONTAL;
    }
    else if (GTK_IS_VBOX(widget)
        || GTK_IS_VBUTTON_BOX(widget)
        || GTK_IS_VPANED(widget)
        || GTK_IS_VRULER(widget)
        || GTK_IS_VSCALE(widget)
        || GTK_IS_VSCROLLBAR(widget)
        || GTK_IS_VSEPARATOR(widget))
    {
        ret = GTK_ORIENTATION_VERTICAL;
    }
    else if (GTK_IS_TOOLBAR(widget)) {
        ret = gtk_toolbar_get_orientation (GTK_TOOLBAR(widget));
#if GTK_CHECK_VERSION(2, 14, 0)
    } else if (GTK_IS_SCALE_BUTTON(widget)) {
        ret = gtk_scale_button_get_orientation (GTK_SCALE_BUTTON(widget));
#endif
    } else {
        g_warning ("WGTKCOMPAT2: gtk_orientable_get_orientation: supplied widget is not supported...");
    }
    return ret;
}
#endif


//==================================================
//                   GTK < 2.12
//==================================================

#if !GTK_CHECK_VERSION(2, 12, 0)

void gtk_widget_set_tooltip_text (GtkWidget *widget, const char *text)
{
    GtkTooltips *tooltip; // GtkTooltips was deprecated in gtk 2.12
    if (!text) return;
    tooltip = g_object_get_data(G_OBJECT(widget), "wgtktooltips");
    if (!tooltip) {
        tooltip = gtk_tooltips_new ();
        g_object_set_data (G_OBJECT(widget), "wgtktooltips", tooltip);
    }
    gtk_tooltips_set_tip (tooltip, widget, text, NULL);
}


char *gtk_widget_get_tooltip_text (GtkWidget *widget) // returned string must be freed
{
    char *text = NULL;
    GtkTooltipsData *tdata = gtk_tooltips_data_get (widget);
    if (tdata && tdata->tip_text) {
        text = g_strdup (tdata->tip_text);
    }
    return text;
}


void gtk_widget_set_has_tooltip (GtkWidget *widget, gboolean has_tooltip)
{ // works with gtk1.2, doesn't work with with gtk2.24
    GtkTooltips *tooltip = g_object_get_data(G_OBJECT(widget), "wgtktooltips");
    if (tooltip) {
        if (has_tooltip) {
            gtk_tooltips_enable (tooltip);
        } else {
            gtk_tooltips_disable (tooltip);
        }
    }
}


gboolean gtk_widget_get_has_tooltip (GtkWidget *widget)
{
    GtkTooltips *tooltip = g_object_get_data(G_OBJECT(widget), "wgtktooltips");
    if (tooltip) {
        return tooltip->enabled;
    } else {
        return FALSE;
    }
}
#endif


//==================================================
//                   GTK < 2.4
//==================================================

#if !GTK_CHECK_VERSION(2, 4, 0)

void gtk_combo_box_insert_text (GtkComboBox *combo_box, gint position, const gchar *text)
{
    w_gtk_combobox_insert_text (combo_box, position, text);
}

void gtk_combo_box_append_text (GtkComboBox *combo_box, const gchar *text)
{
    w_gtk_combobox_insert_text (combo_box, -1, text);
}

void gtk_combo_box_prepend_text (GtkComboBox *combo_box, const gchar *text)
{
    w_gtk_combobox_insert_text (combo_box, 0, text);
}


void gtk_combo_box_remove_text (GtkComboBox *combo_box, gint position)
{
    w_gtk_combobox_remove (combo_box, position);
}


gchar *gtk_combo_box_get_active_text (GtkComboBox *combo_box)
{
    // return string must be freed
    return w_gtk_combobox_get_active_text (GTK_WIDGET(combo_box));
}


gint gtk_combo_box_get_active (GtkComboBox *combo_box)
{
    return w_gtk_combobox_get_active_index (combo_box);
}


void gtk_combo_box_set_active (GtkComboBox *combo_box, gint index_)
{
    w_gtk_combobox_set_active_index (combo_box, index_);
}
#endif



//==================================================
//             GTK1 (assume GTK 1.2)
//==================================================

#if GTK_MAJOR_VERSION == 1

// GLIB

int g_strcmp0 (const char *s1, const char *s2)
{
    if (!s1 && !s2) {
        return 0;
    } else if (!s1 && s2) {
        return 1;
    } else if (s1 && !s2) {
        return -1;
    } else {
        return strcmp (s1, s2);
    }
}


#define W_DIR_SEPARATOR '/'
char *g_path_get_basename (const char *filename)
{
    char *bname = NULL;
    char *p;
    if (filename) {
        p = strrchr (filename, W_DIR_SEPARATOR);
        if (p) {
            bname = strdup (p+1);
        } else if (!*filename) {
            bname = strdup ("."); // can't be empty, glib2 returns '.'
        } else {
            bname = strdup (filename);
        }
    }
    return bname;
}


char *g_path_get_dirname (const char *filename)
{
    char *dname = NULL;
    char *p;
    if (filename) {
        p = strrchr (filename, W_DIR_SEPARATOR);
        if (p) {
            dname = strdup (filename);
            p = strrchr (dname, W_DIR_SEPARATOR);
            *p = '\0';
        } else { // not a valid path, return '.' (glib2 behavior)
            dname = strdup (".");
        }
    }
    return dname;
}


// GTK


gpointer g_object_ref (gpointer object)
{
    g_return_val_if_fail (GTK_IS_OBJECT(object), object);
    gtk_object_ref (GTK_OBJECT(object));
    return (gpointer) object;
}

void g_object_unref (gpointer object)
{
    g_return_if_fail (GTK_IS_OBJECT(object));
    gtk_object_unref (GTK_OBJECT(object));
}


void gtk_table_get_size (GtkTable *table, guint *rows, guint *columns)
{
    if (rows) {
        *rows = table->nrows;
    }
    if (columns) {
        *columns = table->ncols;
    }
}


gint gtk_option_menu_get_history (GtkOptionMenu *option_menu)
{
    GtkWidget *menu, *menuitem;
    gint active_index = -1;
    menu = gtk_option_menu_get_menu (GTK_OPTION_MENU(option_menu));
    g_return_val_if_fail (menu != NULL, -1);
    menuitem = gtk_menu_get_active (GTK_MENU(menu));
    if (menuitem) {
        active_index = g_list_index (GTK_MENU_SHELL(menu)->children, menuitem);
    }
    return active_index;
}


static void gtk1_set_label_mnemonic (GtkWidget *accel_widget, GtkLabel *labelw, const char *text)
{
    static GtkAccelGroup *gtk1_accelgroup = NULL;
    guint key;
    if (text && strchr(text,'_'))
    {
        if (!gtk1_accelgroup) {
            gtk1_accelgroup = gtk_accel_group_new(); // hack, the mnemonic will not work
        }
        // https://github.com/wdlkmpx/gWaveEdit/commit/ad5603b4cbd7aa996896fbc9ef2fb6c65d67da01
        key = gtk_label_parse_uline (labelw, text);
        gtk_widget_add_accelerator (accel_widget, "clicked",
                                    gtk1_accelgroup,
                                    key, GDK_MOD1_MASK, (GtkAccelFlags) 0);
    }
}


GtkWidget* gtk_button_new_with_mnemonic (const char *label)
{
    GtkWidget *button = gtk_button_new_with_label (label);
    gtk1_set_label_mnemonic (button, GTK_LABEL(GTK_BIN(button)->child), label);
    return button;
}

GtkWidget* gtk_check_button_new_with_mnemonic (const char *label)
{
    GtkWidget *button = gtk_check_button_new_with_label (label);
    gtk1_set_label_mnemonic (button, GTK_LABEL(GTK_BIN(button)->child), label);
    return button;
}

GtkWidget* gtk_toggle_button_new_with_mnemonic (const char *label)
{
    GtkWidget *button = gtk_toggle_button_new_with_label (label);
    gtk1_set_label_mnemonic (button, GTK_LABEL(GTK_BIN(button)->child), label);
    return button;
}

GtkWidget* gtk_radio_button_new_with_mnemonic (GSList *group, const gchar *label)
{
    GtkWidget *button = gtk_radio_button_new_with_label (group, label);
    gtk1_set_label_mnemonic (button, GTK_LABEL(GTK_BIN(button)->child), label);
    return button;
}

GtkWidget* gtk_radio_button_new_with_mnemonic_from_widget (GtkRadioButton *radio_group_member, const gchar *label)
{
    GtkWidget *button = gtk_radio_button_new_with_label_from_widget (radio_group_member, label);
    gtk1_set_label_mnemonic (button, GTK_LABEL(GTK_BIN(button)->child), label);
    return button;
}

#endif

#endif // GTK <= 2
