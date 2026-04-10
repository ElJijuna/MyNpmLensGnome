#include <adwaita.h>

#include "window.h"

static void
on_activate (GApplication *app)
{
  MnlWindow *window = mnl_window_new (ADW_APPLICATION (app));
  gtk_window_present (GTK_WINDOW (window));
}

int
main (int argc, char *argv[])
{
  AdwApplication *app = adw_application_new ("io.github.eljijuna.NpmLens",
                                             G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);
  return g_application_run (G_APPLICATION (app), argc, argv);
}
