/*
 * Public Domain

 Extended compatibility for GTK < 2.24

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


#if GTK_MAJOR_VERSION == 4

// some compat functions
void gtk_paned_pack1 (GtkPaned* paned, GtkWidget* child, gboolean resize, gboolean shrink)
{
    gtk_paned_set_start_child (paned, child);
    gtk_paned_set_resize_start_child (paned, resize);
    gtk_paned_set_shrink_start_child (paned, shrink);
}

void gtk_paned_pack2 (GtkPaned* paned, GtkWidget* child, gboolean resize, gboolean shrink)
{
    gtk_paned_set_end_child (paned, child);
    gtk_paned_set_resize_end_child (paned, resize);
    gtk_paned_set_shrink_end_child (paned, shrink);
}

void gtk_paned_add1 (GtkPaned* paned, GtkWidget* child)
{
    gtk_paned_set_start_child (paned, child);
    gtk_paned_set_resize_start_child (paned, FALSE);
    gtk_paned_set_shrink_start_child (paned, TRUE);
}

void gtk_paned_add2 (GtkPaned* paned, GtkWidget* child)
{
    gtk_paned_set_end_child (paned, child);
    gtk_paned_set_resize_end_child (paned, TRUE);
    gtk_paned_set_shrink_end_child (paned, TRUE);
}


//=========================
#elif GTK_MAJOR_VERSION < 4
//=========================

void gtk_scrolled_window_set_child (GtkScrolledWindow* scrolled_window,  GtkWidget* child)
{
# if GTK_CHECK_VERSION(3,8,0)
    // 3.8+: gtk_container_add() automatically adds a GtkViewPort
    // if the child doesn't implement GtkScrollable
    gtk_container_add (GTK_CONTAINER(scrolled_window), child);
# elif GTK_CHECK_VERSION(3,0,0)
    // 3.0+: GtkScrollable is an interface that is implemented by widgets with native scrolling
    if (GTK_IS_SCROLLABLE(child)) {
        gtk_container_add (GTK_CONTAINER(scrolled_window), child);
    } else {
        gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW(scrolled_window), child);
    }
# else // GTK2 & 1
    GtkWidgetClass *widget_class;
    widget_class = GTK_WIDGET_GET_CLASS(child);
    if (widget_class->set_scroll_adjustments_signal) {
        gtk_container_add (GTK_CONTAINER(scrolled_window), child);
    } else {
        gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW(scrolled_window), child);
    }
# endif
}


GtkWidget* gtk_paned_get_start_child (GtkPaned* paned)
{
#if GTK_CHECK_VERSION(3,0,0)
    return gtk_paned_get_child1 (paned);
#else
    return paned->child1;
#endif
}

GtkWidget* gtk_paned_get_end_child (GtkPaned* paned)
{
#if GTK_CHECK_VERSION(3,0,0)
    return gtk_paned_get_child2 (paned);
#else
    return paned->child2;
#endif
}


void gtk_paned_set_start_child (GtkPaned* paned, GtkWidget* child)
{
    gtk_paned_pack1 (paned, child, TRUE, TRUE);
}

void gtk_paned_set_end_child (GtkPaned* paned, GtkWidget* child)
{
    gtk_paned_pack2 (paned, child, TRUE, TRUE);
}


void gtk_button_set_icon_name (GtkButton* button, const char* icon_name)
{
#if GTK_CHECK_VERSION(2,0,0) // GTK1 doesn't support pixbufs
    GtkWidget *img = NULL;
    img = w_gtk_image_new_from_icon_name (icon_name, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image (button, img);
#endif
}

const char* gtk_button_get_icon_name (GtkButton* button)
{
    const char *icon_name = NULL;
#if GTK_CHECK_VERSION(2,6,0)
    GtkWidget *img = gtk_button_get_image (button);
    if (img) {
        gtk_image_get_icon_name (GTK_IMAGE(img), &icon_name, NULL);
    }
#endif
    return icon_name;
}

#endif // GTK == 4
