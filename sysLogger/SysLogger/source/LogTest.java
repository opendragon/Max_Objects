import SysLogger;
import java.io.*;

public class LogTest {

	public static void main(String args[]) {

	String msg = "";
	String filename="file.txt";
	SysLogger l = new SysLogger();
	l.setSysLogger("129.145.25.153"); //Choose the syslogd server
//	l.setSysLogger();
	System.out.println("syslog server used is : " + l.showSysLogger());

	l.openLog("LogTest", "LOG_CONS");
	
	boolean res = l.isOpen();
	System.out.println("Log file open is " + res );
	//Try to open a non-existing file to get exception

	try {
	FileReader in = new FileReader(filename);
	} catch (FileNotFoundException e) {
		msg = e.getMessage();
	}
	try {
	l.sysLog("LOG_ALERT",  msg); 
	l.sysLog("LOG_ALERT", "Message from LogTest App!");
	} catch(IOException e) {System.out.println("Exception is " + e.getMessage()); }
 	l.closeLog();	

	}
}
