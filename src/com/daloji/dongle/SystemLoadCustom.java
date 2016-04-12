package com.daloji.dongle;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.activity.InvalidActivityException;

import com.daloji.dongle.InvalidConfigurationException;

/**
 *  Custom System.loadLibrary for loading  native shared library from current jar archive
 * 
 * @author Johan Boris IANTILA
 * @since USBDongle 1.0
 * @see SystemLoadCustom
 *
 */
public class SystemLoadCustom {

	
	private static final  String HOST_DIRECTORY = "/tmp/"; 
	
	private static final  String JAR_DIRECTORY = "/lib/"; 
	
	private static final String LINUX_EXTENSION =".so";
	
	
	
	/**
     * load shared library from jar 
     * @param shared library location inside jar archive file
     * @throws InvalidActivityException If temporary file creation or read/write operation fails or file 
     * 
     */
	public static void loadLibrary(String path) throws InvalidConfigurationException{
		if(path == null){
			throw new InvalidConfigurationException("library path name must be initialize");
		}
		
		String[] parts = path.split("/");
	    String filename = (parts.length > 1) ? parts[parts.length - 1] : null;
	    if(filename == null){
	    	throw new InvalidConfigurationException("error configuration shared library filename is null");
	    }
	     
		try {
		
			InputStream in =SystemLoadCustom.class.getResourceAsStream(JAR_DIRECTORY+filename+LINUX_EXTENSION);        
			File libinhost = new File(HOST_DIRECTORY+filename+LINUX_EXTENSION);
			if(!libinhost.exists()){
				OutputStream out = new FileOutputStream(libinhost);
				byte[] buf = new byte[4096];
				int len;
				while ((len = in.read(buf)) > 0){
					out.write(buf, 0, len);
				}
				in.close();
				out.close();
			}
			System.load(HOST_DIRECTORY+filename+LINUX_EXTENSION);
		}
		catch(FileNotFoundException ex){
			throw new InvalidConfigurationException(ex.getMessage() + " in the specified directory.");
			
		}
		catch(IOException e){
			throw new InvalidConfigurationException(e.getMessage());  
		}
		
	}
}
