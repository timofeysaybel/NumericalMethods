package HeatEquation;

import Matrix.Matrix;
import util.Arguments;
import util.Data;
import util.Vector;

import java.io.FileWriter;
import java.io.IOException;

public class HeatEquation
{
    private final double[][] Grid;
    private final double[] alphaGrid;
    private final double[] betaGrid;
    private final double A;
    private final double B;
    private final double C;
    private final Arguments args;

    public HeatEquation(Arguments args)
    {
        Grid = new double[args.N + 1][args.M + 1];
        alphaGrid = new double[args.M + 1];
        betaGrid = new double[args.M + 1];

        A = B = Data.A2 * args.t / (args.h * args.h);
        C = 1 + 2 * A;
        this.args = args;
    }

    public void solve() throws IOException
    {
        if (args.scheme == Data.IMPLICIT)
            solveImplicit();

//        if (args.mode == Data.TEST)
//            printErrors();
    }

    public void printGrid()
    {
        for (int i = 0; i < args.N + 1; i++)
        {
            for (int j = 0; j < args.M + 1; j++)
                System.out.print(Grid[i][j] + " ");
            System.out.println();
        }
    }

    private void solveImplicit()
    {
        for (int i = 1; i < args.M + 1; i++)
        {
            Grid[0][i] = getImplicitU(0,i);
            alphaGrid[i] = getAlpha(i);
        }
        for (int i = 0; i < args.N + 1; i++)
        {
            for (int j = 1; j < args.M + 1; j++)
                betaGrid[j] = getBeta(i,j);

            for (int j = args.M; j >= 0; j--)
                Grid[i][j] = getImplicitU(i,j);
        }
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
        if (args.mode == Data.VISUAL)
            return n == 0 ? Data.u0(m * args.h) + args.t * Data.f(args.t, m * args.h) : Grid[n - 1][m] + args.t * Data.f(n * args.t, m * args.h);
        else
            return n == 0 ? Data.u(Data.T_START, m * args.h) + args.t * Data.ft(args.t, m * args.h) : Grid[n - 1][m] + args.t * Data.ft(n * args.t, m * args.h);
    }

    private double getBeta(int n, int m)
    {
        return m == 1 ? getMu1(n) : (A * betaGrid[m - 1] + getF(n,m - 1)) / (C - alphaGrid[m - 1] * A);
    }

    private double getImplicitU(int n, int m)
    {
        if (n == 0)
        {
            if (args.mode == Data.VISUAL)
            {
                return Data.u0(m * args.h);
            }
        }

        if (m == args.M)
            return (getMu2(n) + getHi2() * betaGrid[args.M]) / (1 - getHi2() * alphaGrid[args.M]);

        if (m == 0)
            return getHi1() * Grid[n][m + 1] + getMu1(n);

        return alphaGrid[m + 1] * Grid[n][m + 1] + betaGrid[m + 1];
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
        return Grid[args.N][m] - Data.u(Data.T_END,args.h * m);
    }

    private double Rel(int m)
    {
        return Data.u(Data.T_END,args.h * m);
    }

    private double[] getVn(int n)
    {
        return Grid[n];
    }

    /*
    public double[][] getMatrixA()
    {
        double[][] res = new double[args.N][args.M];
        double[][] tmpMatrix = new double[args.M][args.M];

        for (int i = 0; i < args.N; i++)
            tmpMatrix[i] = getVn(i);

        double[][] cMatrix = new double[args.M][args.M];
        for (int i = 0; i < args.N; i++)
        {
            double[] v = Vector.mul(Vector.sub(getVn(i+1),getVn(i)),1.0/args.t);
            for (int j = 0; j < args.M; j++)
            {
                cMatrix[j][i] = v[j];
            }
        }
        Matrix b = new Matrix(tmpMatrix);
        b.print();
        for (int i = 0; i < args.M; i++)
            res[i] = b.solve(cMatrix[i]);

        return res;
    }*/

    public double[][] getMatrixA()
    {
        double[][] res = new double[args.N][args.M];

        for (int i = 0; i < args.N; i++)
        {
            for (int j = 1; j < args.M; j++)
                res[i][j] = (Grid[i+1][j+1] - 2 * Grid[i+1][j] - Grid[i+1][j-1]) / (args.h * args.h);
                //res[i][j] = (Grid[i+1][j] - Grid[i][j]) / (args.t * Grid[i+1][j]);
        }

        for (int i = 0; i < args.N; i++)
        {
            for (int j = i - 1; j >= 0; j--)
                res[i][j] = res[j][i];
        }

        return res;
    }

    public void plot(double value,double[] vector) throws IOException
    {
        FileWriter file = new FileWriter("plt/res.dat");
        for (int n = 0; n < args.N + 1; n++)
        {
            file.write(n* args.t + "\t" + discrepancy(n) + "\t" + Math.exp(value * n* args.t) + "\t" + discrepancy(n,vector) + "\n");
        }
        file.close();
        makePlt("plt/plotter.plt");
    }

    private double discrepancy(int n)
    {
        double sum = 0.0;
        for (int i = 0; i < args.M; i++)
            sum += (Data.u(args.t * n,args.h * i) - Grid[n][i]) * (Data.u(args.t * n,args.h * i) - Grid[n][i]);

        return Math.sqrt(sum);
    }

    private double discrepancy(int n,double[] vector)
    {
        double sum = 0.0;
        double[] w0 = Vector.mul(vector,findC(vector,n));
        for (int i = 0; i < args.M; i++)
        {
            if (i == 0)
                sum += (Data.u(args.t * n, args.h * i) - (Grid[n][i] - w0[i])) * (Data.u(args.t * n, args.h * i) - (Grid[n][i] - w0[i]));
            else
                sum += (Data.u(args.t * n, args.h * i) - Grid[n][i]) * (Data.u(args.t * n, args.h * i) - Grid[n][i]);
        }
        return Math.sqrt(sum);
    }

    private double findC(double[] vector,int n)
    {
        double[] v0 = new double[args.M];

        for (int i = 0; i < args.M; i++)
        {
            v0[i] = Grid[n][i];
        }

        return Vector.mul(v0,vector) / Vector.mul(vector,vector);
    }

    private void makePlt(String filename) throws IOException
    {
        FileWriter file = new FileWriter(filename);
        file.write("set xrange [" + Data.T_START + ":" + Data.T_END + "]\n");
        file.write("set xlabel \"time\"\n");
        file.write("set logscale y\n");
        file.write("plot \"plt/res.dat\" u 1:2 w l title \"Зависимость нормы решения от времени\", " +
                       "\"plt/res.dat\" u 1:3 w l title \"e(λt)\", " +
                       "\"plt/res.dat\" u 1:4 w l title \"Зависимость нормы мод. решения от времени\"\n");
        file.write("pause mouse any");
        file.close();
    }
}
