all: build_exec

#####       Build cbuild/default_tasks      ######
build_exec: 
	@cbuild/call_task.sh build_exec
build_exec_dbg: 
	@cbuild/call_task.sh build_exec_dbg

#####      Launch cbuild/default_tasks      ######
exec: build_exec
	@cbuild/call_task.sh exec

valgrind: build_exec_dbg
	@cbuild/call_task.sh valgrind

#####	    	Recompile kerep.a			######
rebuild_kerep: 
	@cbuild/call_task.sh rebuild_kerep
