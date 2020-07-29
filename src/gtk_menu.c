#include "gtk_menu.h"
#include "actions.h"

/* layout of settings window:
   --vbox_main--
   --------------
   |label entry	|hbox_body
   | |	    |	|   |
   | |      |	|   |
   |\./	   \./	|  \./
   |------------|---------
   |<-hbox_btn->|
   --------------
   --vbox_main--

   vbox_main = fills entire window
   	hbox_body = first addition to vbox_main; holds labels and entry boxes; pad by BUFFER_BODY
		vbox_label = holds labels; static size; pad by BUFFER_LABEL
		vbox_entry = holds entry boxes; fill remaining space; pad by BUFFER_ENTRY
	hbox_btn = second addition to vbox_main; holds buttons
*/

/* NOTE: Need to put the label and entry creation blocks into some form of loop;
   this function should dynamically import all configs */

/***********************
* public definitions 
************************/
int display_settings(gpointer src, gpointer par_win)
{
	GtkWidget *window;
	GtkWidget *vbox_main, *vbox_label, *vbox_entry, *hbox_body, *hbox_btn;
	GtkWidget *lbl_ip, *lbl_port, *lbl_user, *lbl_pass;
	GtkWidget *btn_save, *btn_close;
	struct Entries *entries = malloc(sizeof(struct Entries));
	struct Settings *conf;
	
	/* window */
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(GTK_WIDGET(window), 500, 300);
	gtk_window_set_title(GTK_WINDOW(window), "Settings");

	/* vbox main */
	vbox_main = gtk_box_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox_main);

	/* hbox general */
	hbox_body = gtk_box_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox_main), hbox_body, TRUE, TRUE, BUFFER_BODY);

	/* vbox */
	vbox_label = gtk_box_new(TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox_body), vbox_label, FALSE, FALSE, BUFFER_LABEL);

	vbox_entry = gtk_box_new(TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox_body), vbox_entry, TRUE, TRUE, BUFFER_ENTRY);

	/* label */
	lbl_ip = gtk_label_new("IP:");
	gtk_box_pack_start(GTK_BOX(vbox_label), lbl_ip, TRUE, TRUE, 0);

	lbl_port = gtk_label_new("Port:");
	gtk_box_pack_start(GTK_BOX(vbox_label), lbl_port, TRUE, TRUE, 0);

	lbl_user = gtk_label_new("User:");
	gtk_box_pack_start(GTK_BOX(vbox_label), lbl_user, TRUE, TRUE, 0);

	lbl_pass = gtk_label_new("Password:");
	gtk_box_pack_start(GTK_BOX(vbox_label), lbl_pass, TRUE, TRUE, 0);

	/* entry */
	conf = load_settings(NULL, window);

	entries->ent_ip = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entries->ent_ip), ENTRY_LEN);
	gtk_entry_set_text(GTK_ENTRY(entries->ent_ip), conf->ip);
	gtk_editable_set_editable(GTK_EDITABLE(entries->ent_ip), TRUE);
	gtk_box_pack_start(GTK_BOX(vbox_entry), entries->ent_ip, TRUE, TRUE, 0);
	
	entries->ent_port = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entries->ent_port), ENTRY_LEN);
	gtk_entry_set_text(GTK_ENTRY(entries->ent_port), conf->port);
	gtk_editable_set_editable(GTK_EDITABLE(entries->ent_port), TRUE);
	gtk_box_pack_start(GTK_BOX(vbox_entry), entries->ent_port, TRUE, TRUE, 0);

	entries->ent_user = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entries->ent_user), ENTRY_LEN);
	gtk_entry_set_text(GTK_ENTRY(entries->ent_user), conf->user);
	gtk_editable_set_editable(GTK_EDITABLE(entries->ent_user), TRUE);
	gtk_box_pack_start(GTK_BOX(vbox_entry), entries->ent_user, TRUE, TRUE, 0);

	entries->ent_pass = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entries->ent_pass), ENTRY_LEN);
	gtk_entry_set_text(GTK_ENTRY(entries->ent_pass), conf->pass);
	gtk_editable_set_editable(GTK_EDITABLE(entries->ent_pass), TRUE);
	gtk_box_pack_start(GTK_BOX(vbox_entry), entries->ent_pass, TRUE, TRUE, 0);

	/* hbox btn */
	hbox_btn = gtk_box_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox_main), hbox_btn, TRUE, TRUE, 0);

	/* btns */
	btn_save = gtk_button_new_with_label("Save");
	g_signal_connect_swapped(btn_save, "clicked", G_CALLBACK(save_settings), entries);
	gtk_box_pack_start(GTK_BOX(hbox_btn), btn_save, TRUE, TRUE, 0);

	btn_close = gtk_button_new_with_label("Close");
	g_signal_connect_swapped(btn_close, "clicked", G_CALLBACK(gtk_widget_destroy), window);
	gtk_box_pack_start(GTK_BOX(hbox_btn), btn_close, TRUE, TRUE, 0);

	gtk_widget_show_all(window);

	g_slice_free(struct Settings, conf);

	return 0;
}

int display_about (gpointer src, gpointer par_window)
{
	GtkWidget *about_dialog;
	const gchar *authors[] = {"Ben Gregory", NULL};
	const gchar *documenters[] = {"Ben Gregory", NULL};

	about_dialog = gtk_about_dialog_new();

	/* fill information */
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(about_dialog), "Open Cubes");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(about_dialog), "Copyright \xc2\xa9 Ben Gregory");
	gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(about_dialog), authors);
	gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(about_dialog), documenters);

	gtk_window_set_title(GTK_WINDOW(about_dialog), "About");

	gtk_widget_show(about_dialog);
	
	return 0;
}
