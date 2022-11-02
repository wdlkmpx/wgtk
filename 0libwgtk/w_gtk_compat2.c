/*
 * Public Domain

 Extended compatibility for GTK 2.24 (make apps that require older versions compile)

 */

#if defined(__clang__)
#   pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include "w_gtk_compat2.h"


#if GTK_MAJOR_VERSION <= 2

//==================================================
//                   GTK < 2.24                     
//==================================================

#if !GTK_CHECK_VERSION(2, 24, 0)
gboolean gtk_combo_box_get_has_entry (GtkComboBox *combo_box)
{
    GtkWidget *entry = w_gtk_combo_get_entry (GTK_WIDGET(combo_box))
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
    g_object_ref (widget);
    GTK_OBJECT_FLAGS (widget) |= GTK_HAS_FOCUS;
    gtk_widget_event (widget, event);
    g_object_notify (G_OBJECT (widget), "has-focus");
    g_object_unref (widget);
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


char *gtk_widget_get_tooltip_text (GtkWidget *widget)
{
    char *text = NULL;
    GtkTooltipsData *tdata = gtk_tooltips_data_get (widget);
    if (tdata && tdata->tip_text) {
        text = g_strdup (tdata->tip_text);
    }
    return text;
}


void gtk_widget_set_has_tooltip (GtkWidget *widget, gboolean has_tooltip)
{
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

GtkWidget *gtk_combo_box_new_text (void)
{
    // GtkOptionMenu is ugly and not usable for complex operations
    ///GtkWidget *optionmenu = gtk_option_menu_new ();
    ///GtkWidget *menu       = gtk_menu_new ();
    ///gtk_option_menu_set_menu (GTK_OPTION_MENU(optionmenu), menu);
    ///return optionmenu;
    GtkWidget *combo = gtk_combo_new ();
    GtkWidget *entry = GTK_COMBO(combo)->entry;
    gtk_editable_set_editable (GTK_EDITABLE(entry), FALSE);
    return combo;
}


GtkWidget *gtk_combo_box_entry_new_text (void)
{
    GtkWidget *combo = gtk_combo_new ();
    return combo;
}


void gtk_combo_box_insert_text (GtkComboBox *combo_box, gint position, const gchar *text)
{
    w_gtk_combo_insert_text (combo, position, text);
}

void gtk_combo_box_append_text (GtkComboBox *combo_box, const gchar *text)
{
    w_gtk_combo_insert_text (combo, -1, text);
}

void gtk_combo_box_prepend_text (GtkComboBox *combo_box, const gchar *text)
{
    w_gtk_combo_insert_text (combo, 0, text);
}


void gtk_combo_box_remove_text (GtkComboBox *combo_box, gint position)
{
    w_gtk_combo_remove_index (combo, position);
}


gchar *gtk_combo_box_get_active_text (GtkComboBox *combo_box)
{
    // return string must be freed
    return w_gtk_combo_get_active_text (GTK_WIDGET(combo_box));
}


gint gtk_combo_box_get_active (GtkComboBox *combo_box)
{
    return w_gtk_combo_get_active_index (combo_box);
}


void gtk_combo_box_set_active (GtkComboBox *combo_box, gint index_)
{
    w_gtk_combo_set_active_index (combo_box, index_);
}
#endif


//==================================================
//             GTK1 (assume GTK 1.2)
//==================================================

#if GTK_MAJOR_VERSION == 1
#define G_OBJECT GTK_OBJECT
#endif

#endif // GTK <= 2
