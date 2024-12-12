package cn.natdon.onscripter;

import android.os.Bundle;
import android.os.Message;
import android.os.Handler;

import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.params.HttpConnectionParams;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;

import java.util.List;
import java.util.Iterator;
import java.util.Arrays;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipEntry;
import java.util.zip.CheckedInputStream;
import java.util.zip.CRC32;

import de.idyl.crypto.zip.impl.*;
import de.idyl.crypto.zip.AesZipFileDecrypter;

public class DataDownloader extends Thread
{
	private byte[] buf = null;

	public DataDownloader( String dir, String password, String ver, String url, Handler h )
	{
		dir_name = dir;
		this.password = password;
		download_version = ver;
		download_url = url;
		handler = h;
		buf = new byte[8192*2];

		this.start();
	}
	
	@Override
	public void run()
	{
		HttpResponse response = null;
		HttpGet request;
		int retry = 0;

		File file = new File(dir_name);
		try {
			file.mkdirs();
		}
		catch( SecurityException e ){
			sendMessage(-2, 0, "Failed to create root directory: " + e.toString());
			return;
		}

		BufferedOutputStream tmp_out = null;
		String tmp_path = dir_name + "/" + download_url.substring(download_url.lastIndexOf("/")+1);

		long downloaded = 0;
		long totalLen = 0;
		while(true){
			DefaultHttpClient client = null;
			request = new HttpGet(download_url);
			request.addHeader("Accept", "*/*");
			if (totalLen > 0) request.addHeader("Range", "bytes="+downloaded+"-"+totalLen);
			try {
				client = new DefaultHttpClient();
				client.getParams().setBooleanParameter("http.protocol.handle-redirects", true);
				HttpConnectionParams.setConnectionTimeout(client.getParams(), 10000);
				HttpConnectionParams.setSoTimeout(client.getParams(), 3000);
				response = client.execute(request);
			} catch (org.apache.http.conn.ConnectTimeoutException e) {
				retry++;
				continue;
			} catch (java.net.SocketException e) {
				retry++;
				continue;
			} catch (java.net.SocketTimeoutException e) {
				retry++;
				continue;
			} catch (IOException e) {
				sendMessage(-2, 0, "Failed to fetch zip file: " + e.toString());
				return;
			};

			if (response == null || 
				(response.getStatusLine().getStatusCode() != HttpStatus.SC_OK &&
				response.getStatusLine().getStatusCode() != HttpStatus.SC_PARTIAL_CONTENT)){
				response = null;
				sendMessage(-2, 0, "Failed to fetch zip file.");
				return;
			}

			totalLen = response.getEntity().getContentLength();
			if (response.getStatusLine().getStatusCode() == HttpStatus.SC_OK){
				downloaded = 0;
				try {
					if (tmp_out != null){
						tmp_out.flush();
						tmp_out.close();
						tmp_out = null;
					}
					tmp_out = new BufferedOutputStream(new FileOutputStream(tmp_path));
				} catch( Exception e ) {
					sendMessage(-2, 0, "Failed to create temporary file: " + e.toString());
					return;
				};
			}
			else{
				totalLen += downloaded;
			}

			BufferedInputStream stream = null;
			try {
				stream = new BufferedInputStream(response.getEntity().getContent());
			} catch( java.io.IOException e ) {
				client.getConnectionManager().shutdown();
				retry++;
				continue;
			} catch( java.lang.IllegalStateException e ) {
				client.getConnectionManager().shutdown();
				retry++;
				continue;
			}

			try {
				int len = stream.read(buf);
				while (len >= 0){
					if (len > 0) tmp_out.write(buf, 0, len);
					downloaded += len;
					sendMessage((int)downloaded, (int)totalLen, "Downloading archives from Internet: retry " + retry);
					len = stream.read(buf);
					try{
						Thread.sleep(1);
					} catch (InterruptedException e){
					}
				}
			} catch (java.net.SocketException e) {
			} catch( java.net.SocketTimeoutException e ) {
			} catch( java.io.IOException e ) {
				sendMessage(-2, 0, "Failed to write or download: " + e.toString());
				return;
			}

			try {
				stream.close();
				stream = null;
			} catch( java.io.IOException e ) {
			};

			client.getConnectionManager().shutdown();

			if (downloaded == totalLen) break;
			retry++;
		}

		try {
			tmp_out.flush();
			tmp_out.close();
			tmp_out = null;
		} catch( java.io.IOException e ) {
		};

		int ret = 0;
		if (password.equals("")) 
			ret = extractZip(tmp_path);
		else
			ret = extractEncryptedZip(tmp_path, password);
		if (ret != 0) return;

		file = new File(tmp_path);
		try {
			file.delete();
		}
		catch( SecurityException e ){
			sendMessage(-2, 0, "Failed to delete temporary file: " + e.toString());
			return;
		}

		file = new File(dir_name + "/" + download_version);
		try {
			file.createNewFile();
		} catch( Exception e ) {
			sendMessage(-2, 0, "Failed to create version file: " + e.toString());
			return;
		};

		sendMessage(-1, 0, null);
	}

	private int extractEncryptedZip(String zip_path, String password)
	{
		AesZipFileDecrypter aes_zip = null;
		try{
			aes_zip = new AesZipFileDecrypter(new File(zip_path), new AESDecrypterBC());
		} catch( java.io.IOException e ) {
			sendMessage(-2, 0, "Failed to read from encrypted zip file: " + e.toString());
			return -1;
		};

		List<ExtZipEntry> entry_list = null;
		try{
			entry_list = aes_zip.getEntryList();
		} catch( java.io.IOException e ) {
			sendMessage(-2, 0, "Failed to get entry from encrypted zip file: " + e.toString());
			return -1;
		}

		Iterator ite = entry_list.iterator();
		int num_file = 0;
		while(ite.hasNext()){
			num_file++;
			ExtZipEntry aes_entry = (ExtZipEntry)ite.next();

			String path = dir_name + "/" + aes_entry.getName();
			if (aes_entry.isDirectory()){
				try {
					(new File( path )).mkdirs();
				} catch( SecurityException e ) {
					sendMessage(-2, 0, "Failed to create directory: " + e.toString());
					return -1;
				}
				continue;
			}

			try {
				(new File( path.substring(0, path.lastIndexOf("/") ))).mkdirs();
			} catch( SecurityException e ){
				sendMessage(-2, 0, "Failed to create directory: " + e.toString());
				return -1;
			};

			BufferedOutputStream out = null;
			try {
				out = new BufferedOutputStream(new FileOutputStream(path+"_tmp.zip"));
			} catch( Exception e ) {
				sendMessage(-2, 0, "Failed to create temporary file: " + e.toString());
				return -1;
			};

			try {
				aes_zip.extractEntry(aes_entry, out, password, handler, "Decrypting archives: " + num_file + "/" + entry_list.size());
			} catch( Exception e ) {
				sendMessage(-2, 0, e.toString());
				return -1;
			}

			ZipInputStream zip = null;
			try {
				zip = new ZipInputStream(new BufferedInputStream(new FileInputStream(path+"_tmp.zip")));
			} catch( java.io.FileNotFoundException e ) {
				sendMessage(-2, 0, "Failed to read from temporary file: " + e.toString());
				return -1;
			};
			
			ZipEntry entry = null;
			try {
				entry = zip.getNextEntry();
			} catch( java.io.IOException e ) {
				sendMessage(-2, 0, "Failed to get entry from zip file: " + e.toString());
				return -1;
			}

			if (extractZipEntry(path, zip, (int)entry.getSize(), "Extracting archives: " + num_file + "/" + entry_list.size()) != 0) return -1;

			File file = new File(path+"_tmp.zip");
			try {
				file.delete();
			}
			catch( SecurityException e ){
				sendMessage(-2, 0, "Failed to delete temporary file: " + e.toString());
				return -1;
			}
		}

		return 0;
	}
	
	private int extractZip(String zip_path)
	{
		ZipInputStream zip = null;
		try {
			zip = new ZipInputStream(new BufferedInputStream(new FileInputStream(zip_path)));
		} catch( java.io.FileNotFoundException e ) {
			sendMessage(-2, 0, "Failed to read from zip file: " + e.toString());
			return -1;
		};
			
		int num_file = 0;
		while(true){
			num_file++;
			ZipEntry entry = null;
			try {
				entry = zip.getNextEntry();
			} catch( java.io.IOException e ) {
				sendMessage(-2, 0, "Failed to get entry from zip file: " + e.toString());
				return -1;
			}
			if (entry == null) break;

			String path = dir_name + "/" + entry.getName();
			if (entry.isDirectory()){
				try {
					(new File( path )).mkdirs();
				} catch( SecurityException e ) {
					sendMessage(-2, 0, "Failed to create directory: " + e.toString());
					return -1;
				}
				continue;
			}

			try {
				(new File( path.substring(0, path.lastIndexOf("/") ))).mkdirs();
			} catch( SecurityException e ){
				sendMessage(-2, 0, "Failed to create directory: " + e.toString());
				return -1;
			};

			if (extractZipEntry(path, zip, (int)entry.getSize(), "Extracting archives: " + num_file) != 0) return -1;

			try {
				CheckedInputStream check = new CheckedInputStream( new FileInputStream(path), new CRC32() );
				while( check.read(buf) > 0 ) {};
				check.close();
				if (check.getChecksum().getValue() != entry.getCrc()){
					File ff = new File(path);
					ff.delete();
					throw new Exception();
				}
			} catch( Exception e ){
				sendMessage(-2, 0, "CRC check failed");
				return -1;
			}
		}

		return 0;
	}

	private int extractZipEntry(String out_path, ZipInputStream zip, int total_size, String mes)
	{
		BufferedOutputStream out = null;
		try {
			out = new BufferedOutputStream(new FileOutputStream( out_path ));
		} catch( Exception e ) {
			sendMessage(-2, 0, "Failed to create file: " + e.toString());
			return -1;
		};

		int total_read = 0;
		try {
			int len = zip.read(buf);
			while (len >= 0){
				if (len > 0) out.write(buf, 0, len);
				total_read += len;
				sendMessage(total_read, total_size, mes);
				len = zip.read(buf);
				try{
					Thread.sleep(1);
				} catch (InterruptedException e){
				}
			}
			out.flush();
			out.close();
		} catch( java.io.IOException e ) {
			sendMessage(-2, 0, "Failed to write: " + e.toString());
			return -1;
		}

		return 0;
	}
        
	public void sendMessage(int current, int total, String str){
		Message msg = handler.obtainMessage();
		Bundle b = new Bundle();
		b.putInt("total", total);
		b.putInt("current", current);
		b.putString("message", str);
		msg.setData(b);
		handler.sendMessage(msg);
	}

	private String dir_name = null;
	private String password = null;
	private String download_version = null;
	private String download_url = null;
	private Handler handler = null;
}
