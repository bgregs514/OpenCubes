#ifndef GTK_MENU_H
#define GTK_MENU_H

#include <gtk/gtk.h>

#define BUFFER_LABEL 10
#define BUFFER_ENTRY 20
#define BUFFER_BODY  20
#define ENTRY_LEN    50

struct Entries {
	GtkWidget *ent_ip;
	GtkWidget *ent_port;
	GtkWidget *ent_user;
	GtkWidget *ent_pass;
};

int display_settings(gpointer src, gpointer par_win);

#endif
