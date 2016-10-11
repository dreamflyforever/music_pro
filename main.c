#include <stdlib.h>
#include <stdio.h>
#include "fop.h"
#include "json_op.h"


/*user callback*/
int low_output_cb(int arg, char *s, int size)
{
	int retvalue = 1;
	if (s == NULL) {
		retvalue = -1;
		goto end;
	}
	//print("%s [%d]\n", s, size);
	file_write(arg, s, size);
end:
	return retvalue;
}

/*user callback*/
int high_output_cb(char *a, char *b, char *c)
{
	int retvalue = 1;
	if ((a == NULL) || (b == NULL) || (c == NULL)) {
		retvalue = -1;
		goto end;
	}
	print("[%s] [%s] [%s]\n", a, b, c);
end:
	return retvalue;
}

/*user callback*/
int low_input_cb(int arg, char *s, int size)
{
	int retvalue = 1;

	size = file_read(arg, s, size);
	if (size == -1) {
		retvalue = -1;
		goto end;
	}
end:
	return retvalue;
}

int machine_close(struct op *o)
{

	return 0;
}

int machine_open()
{
	return 0;
}

int main()
{
	int retvalue = -1;
	int fd;

	fd = file_create("./config");
	if (fd == -1) {
		retvalue = -1;
		goto end;
	}

	struct op *o_obj;
	op_init(&o_obj, fd);
	op_reg_low_output(o_obj, low_output_cb);
	op_reg_high_output(o_obj, high_output_cb);
	op_reg_low_input(o_obj, low_input_cb);

	op_delete(&o_obj);
end:
	return retvalue;
}
