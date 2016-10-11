#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "fop.h"
#include "json_op.h"
#include "music_list.h"

int music_list_init(music_obj *m)
{
	int retvalue = 1;
	if (m == NULL) {
		retvalue = -1;
		print("error\n");
		goto end;
	}
	music_info *tmp;
	int i = 100;
	while (i--) {
		music_info_alloc(&tmp, "a", "b", "c");
		music_list_insert(m, tmp);

		music_info_alloc(&tmp, "d", "e", "f");
		music_list_insert(m, tmp);

		music_info_alloc(&tmp, "1", "2", "3");
		music_list_insert(m, tmp);

		music_info_alloc(&tmp, "4", "5", "6");
		music_list_insert(m, tmp);

		music_info_alloc(&tmp, "4", "5", "6");
		music_list_insert(m, tmp);
	}
end:
	return retvalue;
}

/*user callback*/
int low_output_cb(int arg, char *s, int size)
{
	int retvalue = 1;
	if (arg <= 0) {
		print("error\n");
		retvalue = -1;
		goto end;
	}

	if (s == NULL) {
		retvalue = -1;
		goto end;
	}
	print("%s [%d]\n", s, size);
	file_write(arg, s, size);
end:
	return retvalue;
}

/*user callback*/
int high_output_cb(void *context, char *a, char *b, char *c)
{
	music_obj *m = (music_obj *)context;
	int retvalue = 1;

	if ((m == NULL) || (a == NULL) || (b == NULL) || (c == NULL)) {
		print("error\n");
		retvalue = -1;
		goto end;
	}

	print("[%s] [%s] [%s]\n", a, b, c);
	music_info *tmp;
	music_info_alloc(&tmp, a, b, c);
	music_list_insert(m, tmp);
end:
	return retvalue;
}

/*user callback*/
int low_input_cb(int arg, char *s, int size)
{

	int retvalue = 1;

	if (arg <= 0) {
		print("error\n");
		retvalue = -1;
		goto end;
	}

	size = file_read(arg, s, size);
	if (size == -1) {
		retvalue = -1;
		goto end;
	}
end:
	return retvalue;
}

int node_get(struct op *o, music_obj *m)
{
	int retvalue = 1;
	int i = 0;
	music_info *tmp;
	if ((o == NULL) || (m == NULL)) {
		retvalue = -1;
		print("error\n");
		goto end;
	}
	/*for music list current point move to beginning*/
	while (1) {
		tmp = music_prev_get(m);
		if (tmp == NULL)
			break;
	}

	print("insert node:\n");
	/*insert first music list node*/
	tmp = music_cur_get(m);
	if (tmp != NULL) {
		print("[title:artist:url] [%s : %s : %s]\n",
			tmp->title, tmp->artist, tmp->url);

		/*wrap cjson format*/
		op_high_input(i, o, tmp->title, tmp->artist, tmp->url);
		i++;
	} else {
		print("no node\n");
		goto end;
	}
	/*loop get next music list node*/
	while (1) {
		tmp = music_next_get(m);
		if (tmp == NULL) {
			break;
		} else {
			print("[title:artist:url] [%s : %s : %s]\n",
				tmp->title, tmp->artist, tmp->url);

			/*wrap cjson format*/
			op_high_input(i, o, tmp->title, tmp->artist, tmp->url);
			i++;
		}
	}
end:
	return retvalue;
}

int machine_close(struct op *o, music_obj *m)
{
	if ((o == NULL) || (m == NULL)) {
		print("error\n");
		return 0;
	}
	int retvalue = 1;
	retvalue = node_get(o, m);
	if (retvalue == -1) {
		goto end;
	}
	op_low_output(o);
end:
	return retvalue;
}

int machine_open(struct op *o)
{
	int retvalue = -1;

	if (o == NULL) {
		print("error\n");
	}

	retvalue = op_low_input(o);
	if (retvalue == -1) {
		goto end;
	}

	int i;
	for (i = 0; i < 20; i++) {
		op_high_output(o, i);
	}
end:
	return retvalue;
}

int music_list_print(music_obj *m)
{
	if (m == NULL)
		goto end;

	/*for music list current point move to beginning*/
	music_info *tmp;
	while (1) {
		tmp = music_prev_get(m);
		if (tmp == NULL)
			break;
	}

	tmp = music_cur_get(m);
	if (tmp != NULL) {
		print("[title:artist:url] [%s : %s : %s]\n",
			tmp->title, tmp->artist, tmp->url);
	} else {
		print("no node\n");
		goto end;
	}
	/*loop get next music list node*/
	while (1) {
		tmp = music_next_get(m);
		if (tmp == NULL) {
			break;
		} else {
			print("[title:artist:url] [%s : %s : %s]\n",
				tmp->title, tmp->artist, tmp->url);

		}
	}
end:
	return 0;
}

int main()
{
	int retvalue = -1;
	int fd;
	struct op *o_obj;
	music_obj *m_obj;

	/*music list init*/
	music_list_alloc(&m_obj, 20);

	/*config file init*/
	fd = file_create("./config");
	if (fd == -1) {
		retvalue = -1;
		goto end;
	}

	/*file operate init*/
	op_init(&o_obj, fd, m_obj);
	op_reg_low_output(o_obj, low_output_cb);
	op_reg_high_output(o_obj, high_output_cb);
	op_reg_low_input(o_obj, low_input_cb);

	/*machine open test*/
	machine_open(o_obj);
	music_list_print(m_obj);
#if 1
	/*machine close test*/
	music_list_init(m_obj);
	ftruncate(fd, 0);  
    	lseek(fd, 0, SEEK_SET);
	machine_close(o_obj, m_obj);
#endif
	/*memory recycle*/
	op_delete(&o_obj);
	music_list_destroy(&m_obj);
end:
	return retvalue;
}
