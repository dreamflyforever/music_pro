env = Environment(
	CC = 'clang',
	CFLAGS = ['-g', '-Wall', '-I.', '-I./fop', '-I./music_list']
)

obj_base_list = env.Object('./music_list/list.c')
obj_music_list = env.Object('./music_list/music_list.c')

obj_cJSON = env.Object('./fop/cJSON.c')
obj_itoa = env.Object('./fop/itoa.c')
obj_json_op = env.Object('./fop/json_op.c')
obj_fop = env.Object('./fop/fop.c');


test_case = env.Program("test_case", ['main.c', obj_base_list, obj_music_list, obj_cJSON, obj_itoa, obj_json_op, obj_fop], LIBS='m')
