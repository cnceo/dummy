include ../make_begin

base_dir = ${cur_dir}/../..

MODULE			= gameserver
MACD            = 

LIBA_FILE 		= 
LIBSO_FILE		= 
EXEC_FILE 		= gameserver
PROJ_RELY		= ${lib_dir}/libserver.a

OTHER_SRC		= $(shell find ../protocol -name "*.cc")

COMP_OBJ		= ${COMM_OBJ}
INLIB_FILE		= -lpthread -lrt -lglog -lprotobuf -lmysqlclient -lserver -lcurl
INLIB_DIR		= -L/usr/local/lib -L/usr/local/lib64 -L/usr/lib -L/usr/lib64 -L${lib_dir} -L/usr/lib/mysql
INC_PATH 		= -I./ -I../server -I../protocol -I${3rd_dir}/protobuf/protobuf-2.5.0/src -I/usr/include/mysql -I${3rd_dir}/curl/include

include ../make_end
