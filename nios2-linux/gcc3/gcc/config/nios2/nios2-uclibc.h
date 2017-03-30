#ifdef USE_UCLIBC

#undef TARGET_SWITCHES
#define TARGET_SWITCHES					\
{							\
    { "hw-div", HAS_DIV_FLAG,				\
      N_("Enable DIV, DIVU") },				\
    { "no-hw-div", -HAS_DIV_FLAG,			\
      N_("Disable DIV, DIVU (default)") },		\
    { "hw-mul", HAS_MUL_FLAG,				\
      N_("Enable MUL instructions (default)") },				\
    { "hw-mulx", HAS_MULX_FLAG,				\
      N_("Enable MULX instructions, assume fast shifter") },				\
    { "no-hw-mul", -HAS_MUL_FLAG,			\
      N_("Disable MUL instructions") },		\
    { "no-hw-mulx", -HAS_MULX_FLAG,			\
      N_("Disable MULX instructions, assume slow shifter (default and implied by -mno-hw-mul)") },		\
    { "fast-sw-div", FAST_SW_DIV_FLAG,				\
      N_("Use table based fast divide (default at -O3)") },				\
    { "no-fast-sw-div", -FAST_SW_DIV_FLAG,			\
      N_("Don't use table based fast divide ever") },		\
    { "inline-memcpy", INLINE_MEMCPY_FLAG,				\
      N_("Inline small memcpy (default when optimizing)") },				\
    { "no-inline-memcpy", -INLINE_MEMCPY_FLAG,			\
      N_("Don't Inline small memcpy") },		\
    { "cache-volatile", CACHE_VOLATILE_FLAG,				\
      N_("Volatile accesses use non-io variants of instructions (default)") },				\
    { "no-cache-volatile", -CACHE_VOLATILE_FLAG,			\
      N_("Volatile accesses use io variants of instructions") },		\
    { "bypass-cache", BYPASS_CACHE_FLAG,				\
      N_("All ld/st instructins use io variants") },				\
    { "no-bypass-cache", -BYPASS_CACHE_FLAG,			\
      N_("All ld/st instructins do not use io variants (default)") },		\
    { "smallc", 0,			\
      N_("Link with a limited version of the C library") },			\
    { "stack-check", STACK_CHECK_FLAG,				\
      N_("Enable stack limit checking.") },				\
    { "no-stack-check", -STACK_CHECK_FLAG,				\
      N_("Disable stack limit checking (default).") },				\
    { "reverse-bitfields", REVERSE_BITFIELDS_FLAG,          \
      N_("Reverse the order of bitfields in a struct.") },      \
    { "no-reverse-bitfields", -REVERSE_BITFIELDS_FLAG,          \
      N_("Use the normal order of bitfields in a struct (default).") }, \
    { "eb", BIG_ENDIAN_FLAG,                                            \
      N_("Use big-endian byte order") },                                \
    { "el", -BIG_ENDIAN_FLAG,                                           \
      N_("Use little-endian byte order") },                             \
    { "", TARGET_DEFAULT, 0 }				\
}

/* The GNU C++ standard library requires that these macros be defined.  */
#undef CPLUSPLUS_CPP_SPEC
#define CPLUSPLUS_CPP_SPEC "-D_GNU_SOURCE %(cpp)"

#undef LIB_SPEC
#define LIB_SPEC \
"--start-group %{msmallc: -lsmallc} %{!msmallc: -lc} -lgcc \
 %{msys-lib=*: -l%*} \
 %{!msys-lib=*: -lc} \
 --end-group \
 %{msys-lib=: %eYou need a library name for -msys-lib=} \
"

#undef STARTFILE_SPEC 
#define STARTFILE_SPEC  \
"%{msys-crt0=*: %*} %{!msys-crt0=*: crt1%O%s} \
 %{msys-crt0=: %eYou need a C startup file for -msys-crt0=} \
 crti%O%s crtbegin%O%s \
"

#undef ENDFILE_SPEC 
#define ENDFILE_SPEC \
 " crtend%O%s crtn%O%s"

#endif /* USE_UCLIBC */
