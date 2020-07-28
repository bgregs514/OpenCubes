#ifndef GTK_ERR_H
#define GTK_ERR_H

#include <gtk/gtk.h>

#define ERR_IP "IP Error: Check REST API configuration"
#define ERR_CONFIG_CREATE "Config Error: Configuration file created with dummy values"

void display_error(gpointer window, const gchar *message);

#endif
