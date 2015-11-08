/*
 * <p>SysLogger Utility Classes for logging information to syslogd servers 
 * instead of a log file on local  *disk.This SysLogger doesn't uses 
 * any Logging API of Merlin and hence can be used on any version of 
 * java platform. </p>
 * @author  Rajesh Ramchandani
 * @version 1.0
 */

import java.net.*;
import java.io.IOException;

public class SysLogger {

	private static String ipAddress="localhost";
	private String identification="";
	private String logOptions="";
	private String severity="LOG_INFO";
	private String message="";
        private boolean logOpened = false;
        private boolean sendOkay = false;
	private InetAddress in;

  /**
    * Contructor
    * @param	none
    * @return	void
    */
	public SysLogger() {
	}

  /**
    * Sets LocalHost Address
    * @param  none
    * @return  IP address as String
    */
	private String setLocalHostAddress() {
		try {
		in = InetAddress.getLocalHost();
		} catch(UnknownHostException uhe) {}
		String inp = in.getHostAddress();
		return inp;
	}

  /**
    * Checks if the Log is open
    * @param  none
    * @return  true or false
    */
	public boolean isOpen() {
		if(logOpened) {
			return true;
		}
		return false;
	}
  /**
    * Sets the IP address of the syslogd server
    * <p> If the client doesn't specify the syslogd's IP address,
      <tt>localhost</tt> is assumed to be the syslogd server.
    * @param  none
    * @return  void
    */
        public void setSysLogger() {
		ipAddress = setLocalHostAddress();
        }

  /**
    * Sets the IP address of the syslogd server
    * <p> If the client doesn't specify the syslogd's IP address,
      <tt>localhost</tt> is assumed to be the syslogd server.
    * @param  ip
	      IP address of the syslogd server
    * @return  void
    */
	public void setSysLogger(String ip) {
		ipAddress = ip;
	}
	
  /**
    * Shows the IP address of the syslogd server
    * <p> If the client doesn't specify the syslogd's IP address,
    * by calling setSysLogger(), localhost's IP address is returned </p>
    * @param  none
    * @return  syslogd server's IP address.
    */
	public String showSysLogger() {
		return ipAddress;
	}


  /**
    * Starts the logging process
    * <p> Just by calling <tt>openLog()</tt> with no arguments,
    * starts logging without identification prefix to the messages.
    * IP address of client machine is logged wherever supported.
    * "Java App" is prefixed to all the messages logged.</p>
    * @param  none
    * @return  none
    */
	public void openLog() {
		System.out.println("No identification used");
		logOptions="LOG_IP";
		identification = "Java App";
		logOpened = true;
        }

  /**
    * Starts the logging process 
    * <p> Logging process starts by calling <tt>openLog()</tt> and
    * <tt>String ident</tt> is used to prefix all the logged messages.
    * @param  ident
    *	      This is used as prefix to all the messages sent 
	      to the syslogd
    * @param  logopt
	      Specifies the logging options to be used.
    * @return  void
    */
 
	public void openLog(String ident, String logopt) {
		if(ident == null) {
			System.out.println("No identification used");
		}
		identification = ident;	
		System.out.println("Identification used will be " + identification);
		if(logopt == null) {
			logOptions="LOG_IP";
		}


		if(!(logopt == "LOG_IP") || (logopt == "LOG_CONS") || (logopt == "LOG_ODELAY") || (logopt == "LOG_NDELAY") ||(logopt == "LOG_NOWAIT")) {
			System.out.println("Invalid Logging Option" + logopt + ", " +
                        "Will use LOP_IP as default");
			logOptions="LOG_IP";
		}
	

		if(logopt == "LOG_IP") {
			System.out.println("Logging Option is " + logopt);
			logOptions="LOG_IP";
		}
		if(logopt == "LOG_CONS") {
			System.out.println("Logging Option is " + logopt);
			logOptions="LOG_CONS";
		}
		if(logopt == "LOG_ODELAY") {
			System.out.println("Logging Option is " + logopt);
			logOptions="LOG_ODELAY";
		}
		if(logopt == "LOG_NDELAY") {
			System.out.println("Logging Option is " + logopt);
			logOptions="LOG_NDELAY";
		}
		if(logopt == "LOG_NOWAIT") {
			System.out.println("Logging Option is " + logopt);
			logOptions="LOG_NOWAIT";
		}
		
		logOpened = true;
	}		
	
  /**
    * Stop the logging process
    * <p> No more logging will occur after log has been closed.</p>
    * @param  none
    * return  none
    */
    
        public void closeLog() {
                identification="";
                logOptions="";
		logOpened = false;
        }

  /**
    * Log the message provided to the specific syslogd server after opening the log
    * at the specified severity level.
    * <p> This method checks to see if the log has been previously opened, and then
    * sets the messages severity level as specified.</p>
    * @param  sev
    *	      Defines the severity of the message
    * @param  msg
    *	      Message to be logged
    * @throws UnOpenedLogException
    *         If log has not been previously openend.
    */

	public void sysLog(String sev, String msg) throws IOException, UnOpenedLogException {
		if(!logOpened){
			throw new UnOpenedLogException("Log file is not open");
		}

		if(msg == null) {
			message="";
			severity="LOG_INFO";
		}
		if(sev == "LOG_EMERG") {
			message = msg;
			severity="LOG_EMERG";
		}
		if(sev == "LOG_ALERT") {
			message = msg;
			severity="LOG_ALERT";
		}
		if(sev == "LOG_CRIT") {
			message = msg;
			severity="LOG_CRIT";
		}
		if(sev == "LOG_ERR") {
			message = msg;
			severity="LOG_ERR";
		}
		if(sev == "LOG_WARNING") {
			message = msg;
			severity="LOG_WARNING";
		}
		if(sev == "LOG_NOTICE") {
			message = msg;
			severity="LOG_NOTICE";
		}
		if(sev == "LOG_INFO") {
			message = msg;
			severity="LOG_INFO";
		}
		System.out.println("Severity and message is " + severity  +  message);
		sendOkay = true;
		sendMessage(identification,severity,message);
	}	


  /**
    *Sends message to the specified syslogd server
    *<p>Not directky called by the application developer</p>
    */
	private void sendMessage(String identity, String logSev, String logMsg) throws UnOpenedLogException {
		if(sendOkay = false) {
			throw new UnOpenedLogException("Log file is not open");
		}
		try {
			System.out.println("Inside sendMessage");
                        DatagramSocket soc = new DatagramSocket();
			String msgA = "<3>" + identity + ":  " + logSev + ":  " + logMsg;
			System.out.println("Message about to be logged is " + msgA);
			byte msg[] = msgA.getBytes();
			System.out.println("msg length is " + msg.length);
			System.out.println("ipAddress is " + ipAddress);
			DatagramPacket p = new DatagramPacket(msg,msg.length,InetAddress.getByName(ipAddress),514);
			soc.send(p);
			System.out.println("Send successful");
			soc.close();
		} catch(Exception e) {
			System.out.println("Error sending message to syslogd"); 
			e.getMessage();
			System.exit(1);
		}
	}
	
}

  /**
    * UnOpenedLogException class
    */
    class UnOpenedLogException extends IOException {

	public UnOpenedLogException() {}
	public UnOpenedLogException(String s) {
		super(s);
	}

}
	
