include ../make_begin

base_dir = ${cur_dir}/../..

MODULE			= gateserver
MACD            = 

LIBA_FILE 		= 
LIBSO_FILE		= 
EXEC_FILE 		= gateserver
PROJ_RELY		= ${lib_dir}/libserver.a

OTHER_SRC		= $(shell find ../protocol -name "*.cc")

COMP_OBJ		= ${COMM_OBJ}
INLIB_FILE		= -lpthread -lglog -lprotobuf -lmysqlclient -lserver -lcurl -ltcmalloc
INLIB_DIR		= -L/usr/local/lib -L/usr/local/lib64 -L/usr/lib -L/usr/lib64 -L${lib_dir} -L/usr/lib/mysql
INC_PATH 		= -I./ -I../server -I../protocol -I${3rd_dir}/protobuf/protobuf-2.5.0/src -I/usr/include/mysql -I${3rd_dir}/curl/include -I${3rd_dir}/rapidjson/include

include ../make_end
