AC_DEFUN([FI_SUET_CONFIGURE],[
       suet_h_happy=0
       AS_IF([test x"$enable_suet" != x"no"], [suet_h_happy=1])
       AS_IF([test $suet_h_happy -eq 1], [$1], [$2])
])
