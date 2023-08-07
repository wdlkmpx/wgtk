/*
 * Public Domain

 Don't include this header, include w_gtk.h instead

 */

#ifndef __W_GTK_COMPAT4_H__
#define __W_GTK_COMPAT4_H__

#ifdef __cplusplus
extern "C"
{
#endif


#if GTK_MAJOR_VERSION == 4

// some compat functions
void gtk_paned_pack1 (GtkPaned* paned, GtkWidget* child, gboolean resize, gboolean shrink);
void gtk_paned_pack2 (GtkPaned* paned, GtkWidget* child, gboolean resize, gboolean shrink);
void gtk_paned_add1 (GtkPaned* paned, GtkWidget* child);
void gtk_paned_add2 (GtkPaned* paned, GtkWidget* child);


#elif GTK_MAJOR_VERSION < 4

void gtk_scrolled_window_set_child (GtkScrolledWindow* scrolled_window,  GtkWidget* child);
GtkWidget* gtk_paned_get_start_child (GtkPaned* paned);
GtkWidget* gtk_paned_get_end_child (GtkPaned* paned);
void gtk_paned_set_start_child (GtkPaned* paned, GtkWidget* child);
void gtk_paned_set_end_child (GtkPaned* paned, GtkWidget* child);

void gtk_button_set_icon_name (GtkButton* button, const char* icon_name);
const char* gtk_button_get_icon_name (GtkButton* button);

#endif

#ifdef __cplusplus
}
#endif

#endif /* __W_GTK_COMPAT4_H__ */
	
