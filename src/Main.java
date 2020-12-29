import Matrix.Matrix;
import HeatEquation.HeatEquation;
import util.*;

public class Main
{
    public static void main(String[] args) throws MyException
    {
        try
        {
            Arguments arguments = new Arguments();
            arguments.parse(args);

            if (arguments.scheme != Data.IMPLICIT)
            {
                System.err.println("Необходимо использовать неявную схему");
                return;
            }

            if (arguments.M != arguments.N)
            {
                System.err.println("M != N");
                return;
            }

            HeatEquation equation = new HeatEquation(arguments);
            equation.solve();
            Matrix matrix = new Matrix(equation.getMatrixA());

            if (!matrix.equals(matrix.transpose()))
            {
                System.err.println("Матрица несимметрична");
                return;
            }
            matrix.Jacobi(Data.precision,0);

            double[] values = matrix.getEigenValues();
            double[][] vectors = matrix.getEigenVectors();
            int idx = Data.findDominant(values);
            System.out.println("λ" + idx + "=" + values[idx] + " - доминирующее значение");
            double[] dominantVector = new double[vectors.length];
            for (int i = 0; i < vectors.length; i++)
                dominantVector[i] = vectors[i][idx];
            equation.plot(values[idx],dominantVector);
        }
        catch (MyException ex)
        {
            ex.handleException();
        }
        catch (Exception e)
        {
            System.err.println("Возникло стандартное исключение");
            e.printStackTrace();
        }
    }
}
