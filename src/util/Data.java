package util;

import static java.lang.Math.*;

public class Data
{
    public static final byte TEST = 0,
                             VISUAL = 1;

    public static final byte IMPLICIT = 0,
                             EXPLICIT = 1,
                             CN = 2;

    public static final int X_START = 0, X_END = 1;
    public static final int T_START = 0, T_END = 10;

    public static final int T_DIV = 100;

    public static double u(double t, double x)
    {
        return exp(-PI * PI * t) * sin(PI * x);
    }

    public static double ux(double t, double x) { return PI * exp(-PI * PI * t) * cos(PI * x);}

    public static double ft(double t,double x) {return -PI * PI * exp(-PI * PI * t) * sin(PI * x);}

    public static final byte FIRST = 0, SECOND = 1;
    public static final byte[] BOUNDARY_CONDITION = {FIRST,SECOND};

    public static final double A2 = 1;

    public static double f(double t, double x)
    {
        return 0;
    }

    public static double u0(double x)
    {
        return sin(PI * x);
    }

    public static double phi1(double t)
    {
        return 0;
    }

    public static double phi2(double t)
    {
        return 0;
    }

    public static double psi1(double t)
    {
        return 0;
    }

    public static double psi2(double t)
    {
        return 0;
    }

    public static final String filename = "plt/result.dat";
}
