import java.net.*;
import java.io.*;
import java.util.*;

public class EchoClient
{
	public static void main(String[] args)
	{
		try
		{
			//Make connection to server socket:
			String host = "127.0.0.1";
			int port = 6017;
			Socket sock = new Socket(host, port);

			//Create output stream to send messages to server:
			OutputStream out = sock.getOutputStream();
			BufferedOutputStream bout = new BufferedOutputStream(out);
			OutputStreamWriter outStreamWriter = new OutputStreamWriter(bout);
			BufferedWriter boutStreamWriter = new BufferedWriter(outStreamWriter);

			//Create input stream to get messages from server:
			InputStream in = sock.getInputStream();
			BufferedInputStream bin = new BufferedInputStream(in);
			InputStreamReader inStreamReader = new InputStreamReader(bin);
			BufferedReader binStreamReader = new BufferedReader(inStreamReader);



			String outputLine, inputLine;
			Scanner scan = new Scanner(System.in);

			/*
			 * Loop:
			 * (1) - Get input from user;
			 * (2) - Send that input to server;
			 * (3) - Get the answer from server and print if;
			 */
			while((outputLine = scan.nextLine()) != null) 
			{
				//Write to the server:
				boutStreamWriter.write(outputLine + "\n");
				boutStreamWriter.flush();

				//Read from the server and print it:
				inputLine = binStreamReader.readLine();
				System.out.println("Echo: " + inputLine);

			}

			//Close the socket connection:
			sock.close();
		}
		catch(IOException ioe)
		{
			System.err.println(ioe);
		}
	}
}
