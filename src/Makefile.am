sysconf_DATA = sudonters
bin_PROGRAMS = sudont
sudont_SOURCES = \
    policies/all.c \
    policies/by_user.c \
    policies/interactive.c \
    config.c \
    main.c \
    policy.c \
    proc.c \
    sudo.c

install-exec-hook:
	chown root $(bindir)/sudont
	chmod u+s $(bindir)/sudont
