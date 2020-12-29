import HeatEquation.HeatEquation;
import util.Arguments;
import util.MyException;

public class Main
{
    public static void main(String[] args) throws MyException
    {
        try
        {
            Arguments arguments = new Arguments();
            arguments.parse(args);
            HeatEquation equation = new HeatEquation(arguments);
            equation.solve();
        }
        catch (MyException exception)
        {
            exception.handleException();
        }
        catch (Exception e)
        {
            System.err.println("Возникло стандартное исключение");
            e.printStackTrace();
        }
    }
}
