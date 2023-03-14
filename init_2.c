#include <unistd.h>
#include <sys/auxv.h>

int main(void) {
        syscall(543);
        syscall(666, getauxval(AT_SYSINFO_EHDR) - sysconf(_SC_PAGESIZE));
        syscall(777);

	char *argv[] = { "/root/simple_lib/main", 0 };
	char *envp[] = { "LD_PRELOAD=/root/simple_lib/wrap_sin/libsin.so", 0 };
        execve(argv[0], argv, envp);
}
