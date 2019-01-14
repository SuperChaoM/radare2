/* radare - LGPL - Copyright 2007-2018 pancake */

#include <r_flag.h>

/* compare names */
static int ncmp(const void *a, const void *b) {
	RFlagItem *fa = (RFlagItem *)a;
	RFlagItem *fb = (RFlagItem *)b;
	return strcmp (fa->name, fb->name);
}

/* compare offsets */
static int cmp(const void *a, const void *b) {
	RFlagItem *fa = (RFlagItem *)a;
	RFlagItem *fb = (RFlagItem *)b;
	if (fa->offset > fb->offset) {
		return 1;
	}
	if (fa->offset < fb->offset) {
		return -1;
	}
	return 0;
}

R_API bool r_flag_sort(RFlag *f, int namesort) {
	r_return_val_if_fail (f, false);
	bool ret = false;
	bool changes = false;
	RFlagItem *flag, *fi = NULL;
	RListIter *iter, *it_elem;
	RList *tmp = r_list_new ();
	// find bigger ones after this
	if (!tmp) {
		return false;
	}
	do {
		changes = false;
		fi = NULL;
		r_list_foreach (f->flags, iter, flag) {
			if (!fi || ((namesort)? ncmp (fi, flag): cmp (fi, flag)) > 0) {
				fi = flag;
				it_elem = iter;
				changes = true;
			}
		}
		if (fi && changes) {
			ret = true;
			r_list_split_iter (f->flags, it_elem);
			free (it_elem);
			r_list_append (tmp, fi);
		}
	} while (changes);

	free (f->flags);
	f->flags = tmp;
	f->flags->free = free;
	return ret;
}