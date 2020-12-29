package util;

public class MyException extends Throwable
{
    public static final int MISSED_MODE = 1;
    public static final int MISSED_SCHEME = 2;
    public static final int UNKNOWN_SCHEME = 3;
    public static final int UNKNOWN_MODE = 4;
    public static final int MISSED_M = 5;
    public static final int MISSED_N = 6;
    public static final int WRONG_N = 7;
    public static final int WRONG_M = 8;

    private int errCode;

    public MyException(int errCode)
    {
        this.errCode = errCode;
    }

    public void handleException()
    {
        switch(errCode)
        {
            case MISSED_MODE:
                System.err.println("Пропещен режим работы: -mode test/visual");
                return;

            case MISSED_SCHEME:
                System.err.println("Пропещена разностная схема: -scheme explicit/implicit/cn");
                return;

            case UNKNOWN_SCHEME:
                System.err.println("Неизвестная схема");
                return;

            case UNKNOWN_MODE:
                System.err.println("Неизвестный режим работы");
                return;

            case MISSED_M:
                System.err.println("Пропущен параметр M");
                return;

            case MISSED_N:
                System.err.println("Пропущен параметр N");
                return;

            case WRONG_M:
                System.err.println("Неверный параметр M");
                return;

            case WRONG_N:
                System.err.println("Неверный параметр N");
                return;

            default:
                System.err.println("Unknown exception");
        }
    }
}
