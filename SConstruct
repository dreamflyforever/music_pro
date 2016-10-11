env = Environment(
	CC = 'clang',
	CFLAGS = ['-g', '-Wall', '-I.', '-I./fs', '-I./music_list']
)

obj_base_list = env.Object('./music_list/list.c')
obj_music_list = env.Object('./music_list/music_list.c')

obj_cJSON = env.Object('./fs/cJSON.c')
obj_itoa = env.Object('./fs/itoa.c')
obj_json_op = env.Object('./fs/json_op.c')
obj_fop = env.Object('./fs/fop.c');


test_case = env.Program("test_case", ['main.c', obj_base_list, obj_music_list, obj_cJSON, obj_itoa, obj_json_op, obj_fop], LIBS='m')
