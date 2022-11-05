/*
 * Public Domain
 */

/** 2022-10-20 **/

/*
 * gtkcompat.h, GTK2+ compatibility layer
 * 
 * see w_gtk_compat2.h for extended gtk2.24 compatibility (for versions older than 2.24)
 * 
 * The older the GTK version, the more compatible functions are "defined"
 * so it's not wise to use the compiled binary in newer distros or something.
 * 
 */

#ifndef __GTKCOMPAT_H
#define __GTKCOMPAT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "glib-compat.h"
#include <gtk/gtk.h>

/* ================================================== */
/*                     GDK KEYS                       */
/* ================================================== */

#include <gdk/gdkkeysyms.h>
#if !defined(GDK_KEY_a) // GTK_MAJOR_VERSION >= 3
#define GDK_KEY(symbol) GDK_##symbol
#else
#define GDK_KEY(symbol) GDK_KEY_##symbol
#endif


/* ================================================== */
/*                       GTK 3                        */
/* ================================================== */

// GTK >= 3.0 -- applies to GTK3, GTK4...
#if GTK_MAJOR_VERSION >= 3
#define GTKCOMPAT_DRAW_SIGNAL "draw"
#define gtkcompat_widget_set_halign_left(w)   gtk_widget_set_halign(GTK_WIDGET(w), GTK_ALIGN_START)
#define gtkcompat_widget_set_halign_center(w) gtk_widget_set_halign(GTK_WIDGET(w), GTK_ALIGN_CENTER)
#define gtkcompat_widget_set_halign_right(w)  gtk_widget_set_halign(GTK_WIDGET(w), GTK_ALIGN_END)
// using GtkTable format, translate to GtkGrid
#define gtkcompat_grid_attach(table,child,left,right,top,bottom) \
    gtk_grid_attach((table),(child), (left), (top), (right)-(left), (bottom)-(top))
#define gtkcompat_grid_new(rows,cols) (gtk_grid_new())
#endif

#if GTK_MAJOR_VERSION < 4
#define gtk_button_get_child(button) (gtk_bin_get_child(GTK_BIN(button)))
#define gtk_button_set_child(button,child) gtk_container_add(GTK_CONTAINER(button),GTK_WIDGET(child))
//GtkWidget* gtk_button_get_child (GtkButton* button)
//void gtk_button_set_child (GtkButton* button,GtkWidget* child)
#endif


#if GTK_MAJOR_VERSION == 3

// GTK < 3.22
#if GTK_MINOR_VERSION < 22
#define gtk_scrolled_window_set_propagate_natural_height(sw,propagate)
#define gtk_scrolled_window_set_propagate_natural_width(sw,propagate)
#define gtk_scrolled_window_get_propagate_natural_height(sw) (TRUE)
#define gtk_scrolled_window_get_propagate_natural_widtht(sw) (TRUE)
#endif

// GTK >= 3.20 (gtk_widget_set_focus_on_click)
#if GTK_MINOR_VERSION >= 20
#define gtk_button_set_focus_on_click(w,b) gtk_widget_set_focus_on_click(GTK_WIDGET(w),b)
#define gtk_button_get_focus_on_click(w)   gtk_widget_get_focus_on_click(GTK_WIDGET(w))
#define gtk_combo_box_set_focus_on_click(w,b) gtk_widget_set_focus_on_click(GTK_WIDGET(w),b)
#define gtk_combo_box_get_focus_on_click(w)   gtk_widget_get_focus_on_click(GTK_WIDGET(w))
#define gtk_file_chooser_button_set_focus_on_click(w,b) gtk_widget_set_focus_on_click(GTK_WIDGET(w),b)
#define gtk_file_chooser_button_get_focus_on_click(w)   gtk_widget_get_focus_on_click(GTK_WIDGET(w))
#endif

// GTK < 3.16
#if GTK_MINOR_VERSION < 16
#define gtk_scrolled_window_set_overlay_scrolling(swindow,overlay)
#define GTK_POLICY_EXTERNAL GTK_POLICY_NEVER
#endif

// GTK < 3.12
#if GTK_MINOR_VERSION < 12 
#define gtk_widget_set_margin_start(widget,margin) gtk_widget_set_margin_left(widget,margin)
#define gtk_widget_set_margin_end(widget,margin)   gtk_widget_set_margin_right(widget,margin)
#if GTK_MINOR_VERSION >= 4
#define gtk_application_set_accels_for_action(app,name,accels)   gtk_application_add_accelerator(app,accels[0],name,NULL)
#endif
#endif

// GTK < 3.8
#if GTK_MINOR_VERSION < 8
#define gtk_widget_set_opacity(w,o) gtk_window_set_opacity(GTK_WINDOW(w),o)
#define gtk_widget_get_opacity(w)  (gtk_window_get_opacity(GTK_WINDOW(w))
#endif

// GTK < 3.6
#if GTK_MINOR_VERSION < 6
#define gtk_button_set_always_show_image(button,show)
#endif

// GTK < 3.4
#if GTK_MINOR_VERSION < 4
#define gtk_application_window_new(app) gtk_window_new(GTK_WINDOW_TOPLEVEL)
#define gtk_entry_set_placeholder_text(entry,text)
#endif

#endif /* ------- GTK3 ------- */



/* ================================================== */
/*         GTK3 compat with GTK 2/1                   */
/* ================================================== */

#if GTK_MAJOR_VERSION <= 2

// GTK < 3.22
#define gtk_scrolled_window_set_propagate_natural_height(sw,propagate)
#define gtk_scrolled_window_set_propagate_natural_width(sw,propagate)
#define gtk_scrolled_window_get_propagate_natural_height(sw) (TRUE)
#define gtk_scrolled_window_get_propagate_natural_widtht(sw) (TRUE)

// GTK < 3.16
#define gtk_scrolled_window_set_overlay_scrolling(swindow,overlay)
#define GTK_POLICY_EXTERNAL GTK_POLICY_NEVER

// define some GTK3.14+ functions
// GTK < 3.10
// gdk_window_create_similar_image_surface() was removed in gtk4
//                  only use gdk_window_create_similar_surface()
// GTK < 3.8
#define gtk_widget_set_opacity(w,o) gtk_window_set_opacity(GTK_WINDOW(w),o)
#define gtk_widget_get_opacity(w)  (gtk_window_get_opacity(GTK_WINDOW(w))
// GTK < 3.6
#define gtk_button_set_always_show_image(button,show)
// GTK < 3.4
#define gtk_application_window_new(app) gtk_window_new(GTK_WINDOW_TOPLEVEL)
#define gtk_entry_set_placeholder_text(entry,text)

/*** GTK 3.0 ***/
#define gtk_widget_get_preferred_size(w,min,natural) gtk_widget_size_request(w,min)
#define GTKCOMPAT_DRAW_SIGNAL "expose_event"
#define gtk_box_new(ori,spacing) \
  ((ori == GTK_ORIENTATION_HORIZONTAL) ? gtk_hbox_new(FALSE,spacing) \
                                       : gtk_vbox_new(FALSE,spacing))
#define gtk_button_box_new(ori) \
  ((ori == GTK_ORIENTATION_HORIZONTAL) ? gtk_hbutton_box_new() \
                                       : gtk_vbutton_box_new())
#define gtk_scale_new(ori,adjustment) \
  ((ori == GTK_ORIENTATION_HORIZONTAL) ? gtk_hscale_new(adjustment) \
                                       : gtk_vscale_new(adjustment))
#define gtk_scale_new_with_range(ori,min,max,step) \
  ((ori == GTK_ORIENTATION_HORIZONTAL) ? gtk_hscale_new_with_range(min,max,step) \
                                       : gtk_vscale_new_with_range(min,max,step))
#define gtk_separator_new(ori) \
  ((ori == GTK_ORIENTATION_HORIZONTAL) ? gtk_hseparator_new() \
                                       : gtk_vseparator_new())
#define gtk_scrollbar_new(ori,adjustment) \
  ((ori == GTK_ORIENTATION_HORIZONTAL) ? gtk_hscrollbar_new(adjustment) \
                                       : gtk_vscrollbar_new(adjustment))
#define gtk_paned_new(ori) \
  ((ori == GTK_ORIENTATION_HORIZONTAL) ? gtk_hpaned_new() \
                                       : gtk_vpaned_new())
#define gtk_widget_get_allocated_height(widget) (GTK_WIDGET(widget)->allocation.height )
#define gtk_widget_get_allocated_width(widget)  (GTK_WIDGET(widget)->allocation.width  )
#define gtk_combo_box_text_remove_all(cmb) { \
   gtk_list_store_clear (GTK_LIST_STORE (gtk_combo_box_get_model (GTK_COMBO_BOX (cmb)))); \
}
#define gtk_tree_model_iter_previous(model,iter) ({ \
   GtkTreePath * xpathx = gtk_tree_model_get_path (model, iter); \
   gboolean xvalidx = gtk_tree_path_prev (xpathx); \
   if (xvalidx) gtk_tree_model_get_iter (model, iter, xpathx); \
   gtk_tree_path_free (xpathx); \
   xvalidx; \
})
#define gtk_progress_bar_set_show_text(pb,show)
#define gtk_widget_override_font(w,f) gtk_widget_modify_font(w,f)
#define gtkcompat_widget_set_halign_left(w)   gtk_misc_set_alignment(GTK_MISC(w), 0.0, 0.5)
#define gtkcompat_widget_set_halign_center(w) gtk_misc_set_alignment(GTK_MISC(w), 0.5, 0.5)
#define gtkcompat_widget_set_halign_right(w)  gtk_misc_set_alignment(GTK_MISC(w), 1.0, 0.5)
//-
#define GTK_GRID GTK_TABLE
#define GtkGrid  GtkTable
#ifndef W_GTK
#define gtk_grid_attach(grid,child,left,top,width,height) gtk_table_attach_defaults(grid,child,left,left+width,top,top+height)
#endif
#define gtkcompat_grid_new(rows,cols) (gtk_table_new((rows),(cols),FALSE))
#define gtkcompat_grid_attach       gtk_table_attach_defaults
#define gtk_grid_set_column_spacing gtk_table_set_col_spacings
#define gtk_grid_set_row_spacing    gtk_table_set_row_spacings
#define gtk_grid_get_column_spacing gtk_table_get_default_col_spacing
#define gtk_grid_get_row_spacing    gtk_table_get_default_row_spacing
#define gtk_grid_set_row_homogeneous    gtk_table_set_homogeneous
#define gtk_grid_set_column_homogeneous gtk_table_set_homogeneous
#define gtk_grid_get_row_homogeneous    gtk_table_get_homogeneous
#define gtk_grid_get_column_homogeneous gtk_table_gset_homogeneous
//-
#define GDK_SCROLL_SMOOTH 3
//-
typedef enum /* GtkAlign */
{
  GTK_ALIGN_FILL,
  GTK_ALIGN_START,
  GTK_ALIGN_END,
  GTK_ALIGN_CENTER
} GtkAlign;
/* GtkApplication */
#define GtkApplication void
#define g_application_quit(app) gtk_main_quit()
#undef G_APPLICATION
#define G_APPLICATION(app) ((void *) (app))
//-
#define gdk_error_trap_pop_ignored gdk_error_trap_pop
#endif /* ------- GTK2 ------- */



// ===================================================

// CAIRO < 1.10
#ifdef CAIRO_VERSION
#if CAIRO_VERSION < CAIRO_VERSION_ENCODE(1, 10, 0)
#define cairo_region_t         GdkRegion
#define cairo_rectangle_int_t  GdkRectangle
#define cairo_region_create    gdk_region_new
#define cairo_region_copy      gdk_region_copy
#define cairo_region_destroy   gdk_region_destroy
#define cairo_region_create_rectangle gdk_region_rectangle
#define cairo_region_get_extents gdk_region_get_clipbox
#define cairo_region_is_empty  gdk_region_empty
#define cairo_region_equal     gdk_region_empty
#define cairo_region_contains_point gdk_region_point_in
#define cairo_region_contains_rectangle gdk_region_rect_in
#define cairo_region_translate gdk_region_rect_in
#define cairo_region_union_rectangle gdk_region_union_with_rect
#define cairo_region_intersect gdk_region_intersect
#define cairo_region_union     gdk_region_union
#define cairo_region_subtract  gdk_region_subtract
#define cairo_region_xor       gdk_region_xor
//#define cairo_region_num_rectangles / cairo_region_get_rectangle  gdk_region_get_rectangles
#endif
#endif


// PANGO
#ifdef PANGO_VERSION
#ifndef PANGO_WEIGHT_MEDIUM
#define PANGO_WEIGHT_MEDIUM 500
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif /* __GTKCOMPAT_H */
