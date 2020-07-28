#include "gtk_err.h"

void display_error(gpointer window, const gchar *message)
{
	GtkWidget *dialog;

        dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_CLOSE,
                                        "%s",
                                        message);
                                        
        gtk_window_set_title(GTK_WINDOW(dialog), "Error");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
}
