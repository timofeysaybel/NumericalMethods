package HeatEquation;

import util.Arguments;
import util.Data;

import java.io.FileWriter;
import java.io.IOException;

public class HeatEquation
{
    private final double[][] uGrid;
    private final double[] alphaGrid;
    private final double[] betaGrid;
    private double A;
    private double B;
    private double C;
    private final Arguments args;
    private double uMin = 0.0, uMax = 0.0;

    public HeatEquation(Arguments args)
    {
        uGrid = new double[2][args.M + 1];
        alphaGrid = new double[args.M + 1];
        betaGrid = new double[args.M + 1];

        A = B = Data.A2 * args.t / (args.h * args.h);
        C = 1 + 2 * A;
        this.args = args;
    }

    public void solve() throws IOException
    {
        if (args.scheme == Data.EXPLICIT)
            solveExplicit();
        if (args.scheme == Data.IMPLICIT)
            solveImplicit();
        if (args.scheme == Data.CN)
            solveCN();

        if (args.mode == Data.TEST)
            printErrors();
    }

    private void solveExplicit() throws IOException
    {
        FileWriter file = new FileWriter(Data.filename);

        if (args.t >= args.h * args.h / (2 * Data.A2))
            System.err.println("Не удовлетворяет условию τ < h2/2a2");

        for (int i = 0; i < args.N + 1; i++)
        {
            System.arraycopy(uGrid[1], 0, uGrid[0], 0, uGrid[0].length);

            for (int j = 1; j < args.M; j++)
                uGrid[1][j] = getExplicitU(i,j);
            uGrid[1][0] = getExplicitU(i,0);
            uGrid[1][args.M] = getExplicitU(i, args.M);

            if (args.mode == Data.VISUAL && i % Data.T_DIV == 0)
                writeToFile(file);
        }
        file.close();
        if (args.mode == Data.VISUAL)
        {
            int tmp = ((args.N + 1) / Data.T_DIV);
            System.out.println("N0 = " + tmp);
            System.out.println("UMin = " + uMin + "\nUMax = " + uMax);
            makePlt("plt/main.plt", ((args.N + 1) / Data.T_DIV));
        }
    }

    private void solveImplicit() throws IOException
    {
        FileWriter file = new FileWriter(Data.filename);

        for (int i = 1; i < args.M + 1; i++)
            alphaGrid[i] = getAlpha(i);

        for (int i = 1; i < args.N + 1; i++)
        {
            System.arraycopy(uGrid[1], 0, uGrid[0], 0, uGrid[0].length);

            for (int j = 1; j < args.M + 1; j++)
                betaGrid[j] = getBeta(i,j);

            for (int j = args.M; j >= 0; j--)
                uGrid[1][j] = getImplicitU(i,j);

            if (args.mode == Data.VISUAL && i % Data.T_DIV == 0)
                writeToFile(file);
        }
        file.close();
        if (args.mode == Data.VISUAL)
        {
            int tmp = ((args.N + 1) / Data.T_DIV);
            System.out.println("N0 = " + tmp);
            System.out.println("UMin = " + uMin + "\nUMax = " + uMax);
            makePlt("plt/main.plt", ((args.N + 1) / Data.T_DIV));
        }
    }

    private void solveCN() throws IOException
    {
        A = B = args.t * Data.A2 / (2 * args.h * args.h);
        C = 1 + 2 * A;
        solveImplicit();
    }

    private double getHi1()
    {
        return args.conditions[0] == Data.FIRST ? 0 : 1;
    }

    private double getHi2()
    {
        return args.conditions[1] == Data.FIRST ? 0 : 1;
    }

    private double getMu1(int n)
    {
        if (args.mode == Data.VISUAL)
            return args.conditions[0] == Data.FIRST ? Data.phi1(n * args.t) : -args.h * Data.psi1(n * args.t);
        else
            return args.conditions[0] == Data.FIRST ? Data.u(n * args.t,Data.X_START) : -args.h * Data.ux(n * args.t,Data.X_START);
    }

    private double getMu2(int n)
    {
        if (args.mode == Data.VISUAL)
            return args.conditions[1] == Data.FIRST ? Data.phi2(n * args.t) : args.h * Data.psi2(n * args.t);
        else
            return args.conditions[1] == Data.FIRST ? Data.u(n * args.t,Data.X_END) : args.h * Data.ux(n * args.t,Data.X_END);
    }

    private double getAlpha(int m)
    {
        return m == 1 ? getHi1() : B / (C - alphaGrid[m-1] * A);
    }

    private double getF(int n,int m)
    {
        if (args.scheme == Data.IMPLICIT)
        {
            if (args.mode == Data.VISUAL)
                return n == 1 ? Data.u0(m * args.h) + args.t * Data.f(args.t, m * args.h) : uGrid[0][m] + args.t * Data.f(n * args.t, m * args.h);
            else
                return n == 1 ? Data.u(Data.T_START, m * args.h) + args.t * Data.ft(args.t, m * args.h) : uGrid[0][m] + args.t * Data.ft(n * args.t, m * args.h);
        }
        else
        {
            if (args.mode == Data.VISUAL)
                return
                        n == 1 ? Data.u0(m * args.h) + args.t * Data.f(args.t, m * args.h) + args.t * Data.A2 / (2 * args.h * args.h) * (Data.u0((m+1) * args.h) - 2 * Data.u0(m * args.h) + Data.u0((m - 1) * args.h))
                                : uGrid[0][m] + args.t * Data.f((n - 0.5) * args.t, m * args.h) + args.t * Data.A2 / (2 * args.h * args.h) * (uGrid[0][m+1] - 2 * uGrid[0][m] + uGrid[0][m-1]);
            else
                return n == 1 ? Data.u(Data.T_START, m * args.h) + args.t * Data.ft(args.t, m * args.h) + args.t * Data.A2 / (2 * args.h * args.h) * (Data.u(Data.T_START, (m + 1) * args.h) - 2 * Data.u(Data.T_START, m * args.h) + Data.u(Data.T_START, (m - 1) * args.h))
                        : uGrid[0][m] + args.t * Data.ft((n - 0.5) * args.t, m * args.h) + args.t * Data.A2 / (2 * args.h * args.h) * (uGrid[0][m+1] - 2 * uGrid[0][m] + uGrid[0][m-1]);
        }
    }

    private double getBeta(int n, int m)
    {
        return m == 1 ? getMu1(n) : (A * betaGrid[m - 1] + getF(n,m - 1)) / (C - alphaGrid[m - 1] * A);
    }

    private double getExplicitU(int n, int m)
    {
        if (args.mode == Data.VISUAL)
        {
            if (n == 0)
                return Data.u0(m * args.h);

            if (m == 0)
                return args.conditions[0] == Data.FIRST ? Data.phi1(n * args.t) : (4 * uGrid[1][1] - uGrid[1][2] - 2 * args.h * Data.psi1(n * args.t)) / 3;

            if (m == args.M)
                return args.conditions[1] == Data.FIRST ? Data.phi2(n * args.t) : (4 * uGrid[1][args.M - 1] - uGrid[1][args.M - 2] + 2 * args.h * Data.psi2(n * args.t)) / 3;

            double res = uGrid[0][m];
            res += args.t * Data.f((n - 1) * args.t, m * args.h);
            res += (args.t / (args.h * args.h)) * (uGrid[0][m + 1] - 2 * uGrid[0][m] + uGrid[0][m - 1]);

            return res;
        }
        else
        {
            if (n == 0)
                return Data.u(Data.T_START,m * args.h);

            if (m == 0)
                return args.conditions[0] == Data.FIRST ? Data.u(n * args.t,Data.X_START) : (4 * uGrid[1][1] - uGrid[1][2] - 2 * args.h * Data.ux(n * args.t,Data.X_START)) / 3;

            if (m == args.M)
                return args.conditions[1] == Data.FIRST ? Data.u(n * args.t,Data.X_END) : (4 * uGrid[1][args.M - 1] - uGrid[1][args.M - 2] + 2 * args.h * Data.ux(n * args.t,Data.X_END)) / 3;

            double res = uGrid[0][m];
            res += args.t * Data.ft((n - 1) * args.t, m * args.h);
            res += (args.t / (args.h * args.h)) * (uGrid[0][m + 1] - 2 * uGrid[0][m] + uGrid[0][m - 1]);

            return res;
        }
    }

    private double getImplicitU(int n, int m)
    {
        if (m == args.M)
            return (getMu2(n) + getHi2() * betaGrid[args.M]) / (1 - getHi2() * alphaGrid[args.M]);

        if (m == 0)
            return getHi1() * uGrid[1][m + 1] + getMu1(n);

        return alphaGrid[m + 1] * uGrid[1][m + 1] + betaGrid[m + 1];
    }

    private void printErrors()
    {
        System.out.println("Погрешности:");
        System.out.println("\tАбсолютная погрешность в норме Ch = " + getAbsCh());
        System.out.println("\tОтносительная погрешность в норме Ch = " + getRelCh());
        System.out.println("\tАбсолютная погрешность в норме l2,h = " + getAbsL2h());
        System.out.println("\tОтносительная погрешность в норме l2,h = " + getRelL2h());
    }

    private double getAbsCh()
    {
        double max = 0.0;

        for (int i = 0; i < args.M + 1; i++)
        {
            double tmp = Math.abs(Abs(i));
            if (tmp > max)
                max = tmp;
        }
        return max;
    }

    private double getRelCh()
    {
        double max = 0.0;

        for (int i = 0; i < args.M + 1; i++)
        {
            double tmp = Math.abs(Rel(i));
            if (tmp > max)
                max = tmp;
        }
        return getAbsCh() / max;
    }

    private double getAbsL2h()
    {
        double sum = 0.0;

        for (int i = 0; i < args.M + 1; i++)
            sum += Abs(i) * Abs(i);

        return Math.sqrt(args.h * sum);
    }

    private double getRelL2h()
    {
        double sum = 0.0;

        for (int i = 0; i < args.M + 1; i++)
            sum += Rel(i) * Rel(i);

        return getAbsL2h() / Math.sqrt(args.h * sum);
    }

    private double Abs(int m)
    {
        return uGrid[1][m] - Data.u(Data.T_END,args.h * m);
    }

    private double Rel(int m)
    {
        return Data.u(Data.T_END,args.h * m);
    }

    private void writeToFile(FileWriter file) throws IOException
    {
        for (int i = 0; i < args.M + 1; i++)
        {
            double x = i * args.h;
            String str = String.valueOf(x) + "\t" + String.valueOf(uGrid[1][i]) + "\n";
            if (uGrid[1][i] > uMax)
                uMax = uGrid[1][i];

            if (uGrid[1][i] < uMin)
                uMin = uGrid[1][i];

            file.write(str);
        }
        file.write("\n\n");
    }

    private void makePlt(String filename,int n0) throws IOException
    {
        FileWriter file = new FileWriter(filename);
        file.write("idx=0\n");
        file.write("set xrange [" + Data.X_START + ":" + Data.X_END + "]\n");
        file.write("set xlabel \"x\"\n");
        file.write("set yrange [" + uMin + ":" + uMax + "]\n");
        file.write("set ylabel \"U(t,x)\"\n");
        file.write("n0=" + n0 + "\n");
        file.write("load \"plt/plotter_for.plt\"\n");
        file.write("pause mouse any");
        file.close();
    }

    private void writeHelp() throws IOException
    {
        FileWriter help = new FileWriter("plt/help.txt");

        help.write(uMin + " " + uMax + " " + Data.X_START + " " + Data.X_END + " " + args.M);
        help.close();
    }
}
