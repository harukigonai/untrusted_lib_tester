#include <unistd.h>
#include <sys/auxv.h>
#include "lib_wrapper/arg_struct.h"

int main(void) {
        syscall(543);
	syscall(666, getauxval(AT_SYSINFO_EHDR) - sysconf(_SC_PAGESIZE));

	syscall(777);

	struct lib_enter_args args = {0};
	args.ret_entity_index = -1;
	syscall(888, &args);
	syscall(889);

	char *argv[] = { "/root/untrusted_lib_tester/main", 0 };
	char *envp[] = {
		"LD_PRELOAD=/root/extract_ptr_info/hardcoded_wrappers_temp/wrapper_library/libssl_wrapper.so",
		"LD_LIBRARY_PATH=/opt/openssl/lib",
		0
	};
	execve(argv[0], argv, envp);
}
