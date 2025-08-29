### Signals
- Ignore SIGQUIT and show new prompt after SIGINT in interactive mode
- Set the behaviour of SIGINT and SIGQUIT in child process to default
- Check for memory leaks

### Builtins
- [] export
- [] adapt find_env_var to find not only VAR=VAL, but also VAR or VAR=

### Misc
- [] Norminette
- [] Valgrind (without readline leaks)
- [] ~ expansion
- [x] ./minishell exits immediately
- [x] crash if there's only redirect, e.g. > a, << eof
- [x] crash if no environment
- [] SHLVL
