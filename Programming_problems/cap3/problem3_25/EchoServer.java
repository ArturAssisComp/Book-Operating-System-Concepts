//Server:

import java.net.*;
import java.io.*;
import java.util.*;

public class EchoServer
{
	public static void main(String[] args)
	{
		int server_port = 6017;
		System.out.println("Starting server: ");

		try
		{
			ServerSocket sock = new ServerSocket(server_port);

			//Listen for connections:
			while(true)
			{
				System.out.println("Listening for connections -> ");
				Socket client = sock.accept();
				System.out.println("- Connection established.");

				InputStream in = client.getInputStream();
				BufferedInputStream bin = new BufferedInputStream(in);

				OutputStream out = client.getOutputStream();
				BufferedOutputStream bout = new BufferedOutputStream(out);

				//Read the quote from the socket:
				int byteBuffer;
				while ((byteBuffer = bin.read()) != -1) 
				{
					bout.write(byteBuffer);
					if(byteBuffer == '\n') bout.flush();
				}

				//Close the socket:
				client.close();
				System.out.println("- Connection Closed.");
			}

		}
		catch(IOException ioe)
		{
			System.err.println(ioe);
		}
	}


}
