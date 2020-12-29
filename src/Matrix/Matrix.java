package Matrix;

import static java.lang.Math.*;

public class Matrix
{
    private double[][] mat;
    int[] index;
    private final int n;

    private double[][] eigenVectors;
    private double[] eigenValues;

    public Matrix(int size)
    {
        this.n = size;
        this.index = new int[n];
        this.mat = new double[this.n][this.n];
        for (int i = 0; i < this.n; i++)
        {
            for (int j = 0; j < this.n; j++)
                this.mat[i][j] = 0.0;
            index[i] = i;
        }
    }

    public Matrix(Matrix matrix)
    {
        this.n = matrix.n;
        this.mat = new double[this.n][this.n];

        for (int i = 0; i < this.n; i++)
        {
            for (int j = 0; j < this.n; j++)
                this.mat[i][j] = matrix.mat[i][j];
        }
    }


    public Matrix(double[][] m)
    {
        this.n = m.length;
        index = new int[n];
        this.mat = new double[this.n][this.n];
        for (int i = 0; i < this.n; i++)
        {
            for (int j = 0; j < this.n; j++)
                this.mat[i][j] = m[i][j];
            index[i] = i;
        }
    }

    public Matrix transpose()
    {
        Matrix res = new Matrix(n);

        for (int i = 0; i < this.n; i++)
        {
            for (int j = 0; j < this.n; j++)
                res.mat[j][i] = this.mat[i][j];
        }
        return res;
    }

    public int Jacobi(double precision, int mode)
    {
        double[] lines = getLineSums();
        double fault = countFault();

        Matrix vectors = new Matrix(n);
        for (int i = 0; i < n; i++)
            vectors.mat[i][i] = 1;

        int iterations = 0;
        while (fault > precision)
        {
            int[] ij = findElement(mode, iterations, lines);
            int I = ij[0], J = ij[1];

            for (int i = 0; i < n; i++)
            {
                if (i != I)
                    lines[i] -= mat[i][I] * mat[i][I];
                if (i != J)
                    lines[i] -= mat[i][J] * mat[i][J];
            }
            double a = mat[I][J];
            double cos = getCos(I,J), sin = getSin(I,J);

            mulLeft(cos,sin,I,J);
            mulRight(cos,sin,I,J);

            double b = mat[I][J];
            fault -= 2 * (a * a - b * b);

            for (int i = 0; i < n; i++)
            {
                if (i != I && i != J)
                    lines[i] += mat[i][I] * mat[i][I] + mat[i][J] * mat[i][J];
            }
            lines[I] = getLineSum(I);
            lines[J] = getLineSum(J);

            iterations++;
            vectors.mulRight(cos,sin,I,J);
        }

        eigenVectors = vectors.getMat();
        eigenValues = new double[n];
        for (int i = 0; i < n; i++)
            eigenValues[i] = mat[i][i];

        return iterations;
    }

    private double countFault()
    {
        double res = 0.0;

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i != j)
                    res += mat[i][j] * mat[i][j];
            }
        }
        return res;
    }

    private double[] getLineSums()
    {
        double[] res = new double[this.n];
        for (int i = 0; i < this.n; i++)
            res[i] = getLineSum(i);
        return res;
    }

    private double getLineSum(int i)
    {
        double res = 0.0;
        for (int j = 0; j < this.n; j++)
        {
            if (j != i)
                res += this.mat[i][j] * this.mat[i][j];
        }
        return res;
    }

    private int[] findElement(int mode, int iter, double[] lines)
    {
        int[] res = new int[2];
        double max = 0.0;
        switch (mode)
        {
            case 0:
                for (int i = 0; i < n; i++)
                {
                    for (int j = i + 1; j < n; j++)
                    {
                        if (abs(mat[i][j]) > abs(max))
                        {
                            max = mat[i][j];
                            res[0] = i;
                            res[1] = j;
                        }
                    }
                }
                break;
            case 1:
                res[0] = findMax(lines);
                for (int j = 0; j < n; j++)
                {
                    if (abs(max) < abs(mat[res[0]][j]) && res[0] != j)
                    {
                        max = mat[res[0]][j];
                        res[1] = j;
                    }
                }
                break;
            case 2:
                if (iter == 0)
                {
                    res[0] = 0;
                    res[1] = 1;
                    return res;
                }
                while (iter >= 0)
                {
                    for (int i = 0; i < n - 1; i++)
                    {
                        for (int j = i + 1; j < n; j++)
                        {
                            iter--;
                            if (iter < 0)
                            {
                                max = mat[i][j];
                                res[0] = i;
                                res[1] = j;
                                return res;
                            }
                        }
                    }
                }
                break;
            default:
                break;
        }
        return res;
    }

    private int findMax(double[] lines)
    {
        double max = 0.0;
        int maxI = 0;
        for (int i = 0; i < n; i++)
        {
            if (lines[i] > max)
            {
                max = lines[i];
                maxI = i;
            }
        }
        return maxI;
    }

    private double getCos(int I, int J)
    {
        double x = -2.0 * mat[I][J], y = mat[I][I] - mat[J][J];
        if (y == 0)
            return 1.0 / sqrt(2.0);

        return sqrt(0.5 + abs(y) / (2 * sqrt(x * x + y * y)));
    }

    private double getSin(int I, int J)
    {
        double x = -2.0 * mat[I][J], y = mat[I][I] - mat[J][J];
        if (y == 0)
            return 1.0 / sqrt(2.0);

        double cos = getCos(I, J);
        return sign(x * y) * abs(x) / (2 * cos * sqrt(x * x + y * y));
    }

    private int sign(double x)
    {
        if (x < 0) return -1;
        if (x > 0) return 1;
        return 0;
    }

    public void mulLeft(double cos, double sin, int I, int J)
    {
        double[][] tmp = new double[2][n];
        int[] index = new int[2];
        index[0] = I;
        index[1] = J;

        for (int i = 0; i < index.length; i++)
        {
            for (int j = 0; j < n; j++)
            {
                tmp[i][j] = 0;
                tmp[i][j] += cos * mat[index[i]][j];
                if (i == 0)
                    tmp[i][j] += -sin * mat[index[1]][j];
                else
                    tmp[i][j] += sin * mat[index[0]][j];
            }
        }

        for (int i = 0; i < index.length; i++)
        {
            if (n >= 0) System.arraycopy(tmp[i], 0, mat[index[i]], 0, n);
        }
    }

    public void mulRight(double cos, double sin, int I, int J)
    {
        double[][] tmp = new double[n][2];
        int[] index = new int[2];
        index[0] = I;
        index[1] = J;

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < index.length; j++)
            {
                tmp[i][j] = 0;
                tmp[i][j] += mat[i][index[j]] * cos;
                if (j == 0)
                    tmp[i][j] += mat[i][index[1]] * -sin;
                else
                    tmp[i][j] += mat[i][index[0]] * sin;
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < index.length; j++)
                mat[i][index[j]] = tmp[i][j];
        }
    }

    public double[][] getMat()
    {
        return mat;
    }

    public double[][] getEigenVectors()
    {
        return eigenVectors;
    }

    public double[] getEigenValues()
    {
        return eigenValues;
    }

    public boolean equals(Matrix an)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (mat[i][j] != an.mat[i][j])
                    return false;
            }
        }
        return true;
    }

    public void print()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                System.out.print(mat[i][j] + " ");
            System.out.println();
        }
    }

    private int findMajor(int k)
    {
        double max = mat[index[k]][index[k]];
        int maxi = k;
        for (int i = k + 1; i < n; i++)
        {
            if (abs(max) < abs(mat[index[i]][index[k]]))
            {
                max = mat[index[i]][index[k]];
                maxi = i;
            }
        }
        return maxi;
    }

    private void swapLines(int k)
    {
        int maj = findMajor(k);
        int t = index[k];
        index[k] = index[maj];
        index[maj] = t;
    }

    private void calcL(int k)
    {
        double sum = 0.0;

        for (int i = k; i < n; i++)
        {
            sum = 0.0;
            for (int j = 0; j < k; j++)
                sum += mat[index[i]][j] * mat[index[j]][k];

            mat[index[i]][k] = mat[index[i]][k] - sum;
        }
    }

    private void calcU(int i)
    {
        double sum = 0.0;

        for (int k = i + 1; k < n; k++)
        {
            sum = 0.0;
            for (int j = 0; j < i; j++)
                sum += mat[index[i]][j] * mat[index[j]][k];

            mat[index[i]][k] = (mat[index[i]][k] - sum) / mat[index[i]][i];
        }
    }

    private void calcLU()
    {
        swapLines(0);

        for (int i = 1; i < n; i++)
            mat[index[0]][i] /=  mat[index[0]][0];

        for (int i = 1; i < n; i++)
        {
            calcL(i);
            swapLines(i);
            calcU(i);
        }

        for (int i = 0; i < n; i++)
        {
            if (mat[index[i]][index[i]] == 0)
            {
                System.err.println("Матрица вырожденная");
                System.exit(-1);
            }
        }
    }

    public double[] solve(double[] b)
    {
        calcLU();
        return solveU(solveL(b));
    }

    private double[] solveL(double[] b)
    {
        for (int i = 0; i < n; i++)
        {
            b[index[i]] /= mat[index[i]][i];

            for (int j = i + 1; j < n; j++)
                b[index[j]] -= mat[index[j]][i] * b[index[i]];

        }
        return b;
    }

    private double[] solveU(double[] y)
    {
        for (int i = n - 1; i >= 0; i--)
        {
            for (int j = 0; j < i; j++)
                y[index[j]] -= mat[index[j]][i] * y[index[i]];
        }
        double[] res = new double[n];
        for (int i = 0; i < n; i++)
            res[i] = y[index[i]];
        return res;
    }
}