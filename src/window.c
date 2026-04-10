#include "window.h"

#include <webkit/webkit.h>

#define APP_URL "https://eljijuna.github.io/MyNpmLens/"

struct _MnlWindow
{
  AdwApplicationWindow parent;
  WebKitWebView       *webview;
};

G_DEFINE_FINAL_TYPE (MnlWindow, mnl_window, ADW_TYPE_APPLICATION_WINDOW)

static void
on_add_clicked (GtkButton *button G_GNUC_UNUSED, gpointer user_data)
{
  WebKitWebView *webview = WEBKIT_WEB_VIEW (user_data);
  webkit_web_view_evaluate_javascript (
    webview,
    "window.dispatchEvent(new CustomEvent('gnome:open-dialog-addpackage'));",
    -1, NULL, NULL, NULL, NULL, NULL);
}

static void
mnl_window_class_init (MnlWindowClass *klass G_GNUC_UNUSED)
{
}

static void
mnl_window_init (MnlWindow *self)
{
  gtk_window_set_default_size (GTK_WINDOW (self), 1000, 700);
  gtk_widget_set_size_request (GTK_WIDGET (self), 600, 400);
  gtk_window_set_title (GTK_WINDOW (self), "NPM Lens");

  /* Header bar */
  GtkWidget *header = adw_header_bar_new ();

  /* "+ Add" button: icon + label */
  GtkWidget *add_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 4);
  gtk_box_append (GTK_BOX (add_box),
                  gtk_image_new_from_icon_name ("list-add-symbolic"));
  gtk_box_append (GTK_BOX (add_box), gtk_label_new ("Add"));

  GtkWidget *add_button = gtk_button_new ();
  gtk_button_set_child (GTK_BUTTON (add_button), add_box);
  gtk_widget_add_css_class (add_button, "suggested-action");
  adw_header_bar_pack_start (ADW_HEADER_BAR (header), add_button);

  /* WebView with UserContentManager for JS ↔ C bridge */
  WebKitUserContentManager *manager = webkit_user_content_manager_new ();
  webkit_user_content_manager_register_script_message_handler (manager, "app", NULL);

  WebKitSettings *settings = webkit_settings_new_with_settings (
    "enable-javascript",              TRUE,
    "hardware-acceleration-policy",   WEBKIT_HARDWARE_ACCELERATION_POLICY_ALWAYS,
    NULL);

  self->webview = WEBKIT_WEB_VIEW (
    g_object_new (WEBKIT_TYPE_WEB_VIEW,
                  "user-content-manager", manager,
                  "settings",            settings,
                  NULL));
  g_object_unref (manager);
  g_object_unref (settings);

  g_signal_connect (add_button, "clicked", G_CALLBACK (on_add_clicked), self->webview);
  gtk_widget_set_vexpand (GTK_WIDGET (self->webview), TRUE);
  gtk_widget_set_hexpand (GTK_WIDGET (self->webview), TRUE);

  /* Toolbar view: header + webview */
  GtkWidget *toolbar_view = adw_toolbar_view_new ();
  adw_toolbar_view_add_top_bar (ADW_TOOLBAR_VIEW (toolbar_view), header);
  adw_toolbar_view_set_content (ADW_TOOLBAR_VIEW (toolbar_view),
                                GTK_WIDGET (self->webview));

  adw_application_window_set_content (ADW_APPLICATION_WINDOW (self),
                                      toolbar_view);

  webkit_web_view_load_uri (self->webview, APP_URL);
}

MnlWindow *
mnl_window_new (AdwApplication *app)
{
  return g_object_new (MNL_TYPE_WINDOW, "application", app, NULL);
}
