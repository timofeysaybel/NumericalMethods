iter = iter + 1
plot "../result.dat" index iter u 1:2 w l
pause 0.25
if (iter <5) reread
