package util;

public class Arguments
{
    public byte scheme;
    public byte mode;
    public byte xStart = Data.X_START, xEnd = Data.X_END,
            tStart = Data.T_START, tEnd = Data.T_END;
    public byte[] conditions = Data.BOUNDARY_CONDITION;
    public int M;
    public int N;
    public double t,h;

    public void parse(String[] args) throws MyException
    {
        boolean[] ar = new boolean[]{false,false,false,false};
        for (int i = 0; i < args.length; i++)
        {
            switch (args[i])
            {

                case "-M":
                    if (i + 1 >= args.length)
                        throw new MyException(MyException.MISSED_M);

                    M = Integer.parseInt(args[i + 1]);
                    if (M <= 0)
                        throw new MyException(MyException.WRONG_M);
                    ar[2] = true;
                    break;

                case "-N":
                    if (i + 1 >= args.length)
                        throw new MyException(MyException.MISSED_N);

                    N = Integer.parseInt(args[i + 1]);
                    if (N <= 0)
                        throw new MyException(MyException.WRONG_N);
                    ar[3] = true;
                    break;

                default:
                    break;
            }
        }

        for (int i = 0; i < ar.length; i++)
        {
            if (!ar[i])
            {
                switch(i)
                {
                    case 2:
                        throw new MyException(MyException.WRONG_M);

                    case 3:
                        throw new MyException(MyException.WRONG_N);
                }
            }
        }
        scheme = Data.IMPLICIT;
        mode = Data.TEST;
        t = (double)(tEnd - tStart) / N;
        h = (double)(xEnd - xStart) / M;
    }
}