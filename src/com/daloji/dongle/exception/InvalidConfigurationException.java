package com.daloji.dongle.exception;



public class InvalidConfigurationException extends Exception
{

    /** The serialVersionUID. */
    private static final long serialVersionUID = -4642961908674605132L;

    /**
     * Constructor.
     */
    public InvalidConfigurationException()
    {
    }

    /**
     * Constructor.
     * 
     * @param arg0
     */
    public InvalidConfigurationException(String arg0)
    {
        super(arg0);
    }

    /**
     * Constructor.
     * 
     * @param cause
     */
    public InvalidConfigurationException(Throwable cause)
    {
        super(cause);
    }

    /**
     * Constructor.
     * 
     * @param message
     * @param cause
     */
    public InvalidConfigurationException(String message, Throwable cause)
    {
        super(message, cause);
    }

    /**
     * Constructor.
     * 
     * @param message
     * @param cause
     * @param enableSuppression
     * @param writableStackTrace
     */
    public InvalidConfigurationException(String message, Throwable cause,
                                         boolean enableSuppression,
                                         boolean writableStackTrace)
    {
        super(message, cause, enableSuppression, writableStackTrace);
    }

}