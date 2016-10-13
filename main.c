#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "baselib.h"

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

int main()
{
	int retvalue = 1;
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
	op_reg_cur_output(o_obj, cur_output_cb);

	/*machine open test*/
	machine_open(o_obj);
	music_list_print(m_obj);

#if 1
	/*machine close test*/
	music_list_init(m_obj);
	//music_reset("config");
	ftruncate(fd, 0);  
    	lseek(fd, 0, SEEK_SET);
	machine_close(o_obj, m_obj);
#endif
	/*memory recycle*/
	op_delete(&o_obj);
	music_list_destroy(&m_obj);
	close(fd);
end:
	return retvalue;
}
