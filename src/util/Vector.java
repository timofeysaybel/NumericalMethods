package util;

public class Vector
{
    public static double[] add(double[] a, double[] b)
    {
        double[] c = new double[a.length];
        for (int i = 0; i < a.length; i++)
            c[i] = a[i] + b[i];
        return c;
    }

    public static double[] sub(double[] a, double[] b)
    {
        double[] c = new double[a.length];
        for (int i = 0; i < a.length; i++)
            c[i] = a[i] - b[i];
        return c;
    }

    public static double mul(double[] a, double[] b)
    {
        double c = 0.0;
        if (a.length != b.length)
            System.err.println("Проблема");
        for (int i = 0; i < a.length; i++)
            c += a[i] * b[i];
        return c;
    }

    public static double[] mul(double[] a, double num)
    {
        double[] c = new double[a.length];
        for (int i = 0; i < a.length; i++)
            c[i] = a[i] * num;
        return c;
    }
}
