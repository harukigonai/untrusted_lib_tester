#include <unistd.h>
#include <sys/auxv.h>

int main(void) {
        syscall(543);
        syscall(666, getauxval(AT_SYSINFO_EHDR) - sysconf(_SC_PAGESIZE));
        syscall(777);

	char *argv[] = { "/root/untrusted_lib_tester/main", 0 };
	char *envp[] = { 0 };
        execve(argv[0], argv, envp);
}
