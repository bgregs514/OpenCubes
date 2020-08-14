#ifndef GTK_SETUP_H
#define GTK_SETUP_H

#include <gtk/gtk.h>
#include <tm1_py.h>

#define CUBE_TYPE "Cube"
#define CUBE_PAR  "All Cubes"
#define PROC_TYPE "Process"
#define PROC_PAR  "All Processes"

enum {
        COL_TYPE = 0, // column 0
        COL_NAME,    // column 1
        NUM_COLS     // total columns 2
};

struct store_data {
	GtkTreeIter 	  iter_parent;
	GtkTreeIter 	  iter_child;
	char 		  *col_type;
	char		  *col_name;
	struct cube_names *names;
};

int build_frame(GtkApplication *);

#endif
