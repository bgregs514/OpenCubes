#ifndef GTK_SETUP_H
#define GTK_SETUP_H

#include <gtk/gtk.h>

enum {
        COL_TYPE = 0, // column 0
        COL_NAME,    // column 1
        NUM_COLS     // total columns 2
};

int build_frame(GtkApplication *);

#endif
